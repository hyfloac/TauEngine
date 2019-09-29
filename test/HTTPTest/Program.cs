using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Reflection;
using System.Reflection.Emit;

namespace HTTPTest
{
    static class Program
    {
        static void Main(string[] args)
        {
            IPAddress localAddress = IPAddress.Parse("127.0.0.1");
            TcpListener server = new TcpListener(localAddress, 80);

            HTTPServer httpServer = new HTTPServer("Test Server");
            HTTPFileHandler fileHandler = new HTTPFileHandler(new DirectoryInfo("../../"));

            try
            {
                server.Start();

                byte[] bytes = new byte[256];

                while(true)
                {
                    Console.Write("Waiting for a connection... ");

                    // Perform a blocking call to accept requests.
                    // You could also user server.AcceptSocket() here.
                    TcpClient client = server.AcceptTcpClient();
                    Console.WriteLine("Connected!");

                    NetworkStream nStream = client.GetStream();

                    BufferedStream bStream = new BufferedStream(nStream);

                    Stream stream = nStream;

                    HTTPRequest request = HTTPRequestParser.Parse(bStream);

                    // Console.WriteLine("Received: \n{0}", read);

                    if(request != null)
                    {
                        // string requestLine = read.Split(new[] { '\r' }, 2)[0];
                        // HTTPRequest request = HTTPRequest.Create(requestLine);

                        if(request.Type == RequestType.Head)
                        {
                            httpServer.FormResponse(HTTPResponseCode.Ok, "", stream);
                        }
                        else if(request.RequestPath == "secretPage")
                        {
                            if(!request.Params.ContainsKey("Secret"))
                            {
                                request.Params.Add("Secret", "No Secret Detected");
                            }
                            httpServer.FormResponse(HTTPResponseCode.Ok, $"<html><head></head><body>Secret Page!<br /><h1>{request.Params["Secret"]}</h1></body></html>", stream);
                        }
                        else if(request.RequestPath == "ajax_req")
                        {
                            httpServer.FormResponse(HTTPResponseCode.Ok, Guid.NewGuid().ToString(), stream);
                        }
                        else
                        {
                            HTTPFileData? file = fileHandler.GetFile(request.RequestPath);
                            if(file != null)
                            {
                                httpServer.FormResponse(HTTPResponseCode.Ok, file, stream);
                            }
                            else
                            {
#if DEBUG
                                string _path = Path.Combine(fileHandler.FileRoot.FullName, request.RequestPath);
                                httpServer.FormResponse(HTTPResponseCode.NotFound, $"<html><body><h1>Error 404 Page Not Found</h1><br/><h2>{_path}</h2></body></html>", stream);
#else
                                httpServer.FormResponse(HTTPResponseCode.NotFound, $"<html><body><h1>Error 404 Page Not Found</h1><br/><h2>{request.RequestPath}</h2></body></html>", stream);
#endif
                            }
                        }

                        stream.Flush();
                        // stream.Write(response.GetInternalArray(), 0, response.Count);
                        // Console.WriteLine("Sent: \n{0}", Encoding.ASCII.GetString(response.GetInternalArray()));
                    }

                    client.Close();
                }
            }
            catch(SocketException e)
            {
                Console.WriteLine("SocketException: {0}", e);
            }
            finally
            {
                server.Stop();
            }

            Console.WriteLine("\nHit enter to continue...");
            Console.Read();
        }
    }

    public static class ListExtensions
    {
        static class ArrayAccessor<T>
        {
            public static readonly Func<List<T>, T[]> Getter;

            static ArrayAccessor()
            {
                var dm = new DynamicMethod("get", MethodAttributes.Static | MethodAttributes.Public, CallingConventions.Standard, typeof(T[]), new Type[] { typeof(List<T>) }, typeof(ArrayAccessor<T>), true);
                var il = dm.GetILGenerator();
                il.Emit(OpCodes.Ldarg_0); // Load List<T> argument
                il.Emit(OpCodes.Ldfld, typeof(List<T>).GetField("_items", BindingFlags.NonPublic | BindingFlags.Instance)); // Replace argument by field
                il.Emit(OpCodes.Ret); // Return field
                Getter = (Func<List<T>, T[]>) dm.CreateDelegate(typeof(Func<List<T>, T[]>));
            }
        }

        public static T[] GetInternalArray<T>(this List<T> list)
        {
            return ArrayAccessor<T>.Getter(list);
        }
    }
}
