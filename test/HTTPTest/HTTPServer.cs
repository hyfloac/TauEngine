using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace HTTPTest
{
    static class Utils
    {
        private const byte CR = 0x0D;
        private const byte LF = 0x0A;
        private const byte SemiColon = 0x3A;
        private const byte Space = 0x20;

        public static StringBuilder AddHeader(this StringBuilder sb, string name, string value) => sb.Append(name).Append(": ").Append(value).Append("\r\n");

        public static List<byte> AddBytes(this List<byte> bytes, byte[] data)
        {
            bytes.AddRange(data);
            return bytes;
        }

        public static List<byte> AddString(this List<byte> bytes, string str) => bytes.AddBytes(Encoding.ASCII.GetBytes(str));

        public static List<byte> AddCRLF(this List<byte> bytes)
        {
            bytes.Add(CR);
            bytes.Add(LF);
            return bytes;
        }

        public static List<byte> AddHeader(this List<byte> bytes, string name, string value)
        {
            bytes.AddString(name);
            bytes.Add(SemiColon);
            bytes.Add(Space);
            bytes.AddString(value);
            return bytes.AddCRLF();
        }

        public static Stream WriteBytes(this Stream stream, byte[] bytes)
        {
            stream.Write(bytes, 0, bytes.Length);
            return stream;
        }

        public static Stream WriteString(this Stream stream, string str) => stream.WriteBytes(Encoding.ASCII.GetBytes(str));

        public static Stream WriteCRLF(this Stream stream)
        {
            stream.WriteByte(CR);
            stream.WriteByte(LF);
            return stream;
        }

        public static Stream WriteHeader(this Stream stream, string name, string value)
        {
            stream.WriteString(name);
            stream.WriteByte(SemiColon);
            stream.WriteByte(Space);
            stream.WriteString(value);
            return stream.WriteCRLF();
        }
    }

    public class HTTPServer
    {
        public const string HeaderServer = "Server";
        public const string ContentType = "Content-Type";
        public const string ContentLength = "Content-Length";
        public const string CacheControl = "Cache-Control";

        private static readonly byte[] HTTPVersion = Encoding.ASCII.GetBytes("HTTP/1.1 ");

        public string ServerName { get; set; }

        public HTTPServer(string serverName = "")
        {
            ServerName = serverName;
        }



        public List<byte> FormResponse(HTTPResponseCode responseCode, HTTPFileData? page)
        {
            List<byte> bytes = new List<byte>();

            bytes.AddBytes(HTTPVersion).AddString(responseCode.GetStringResponse()).AddCRLF();

            if(!String.IsNullOrWhiteSpace(ServerName))
            {
                bytes.AddHeader(HeaderServer, ServerName);
            }

            bytes.AddHeader(CacheControl, "no-cache");
            
            if(page?.Data != null && page.Value.Data.Length > 0)
            {
                bytes.AddHeader(ContentType, page.Value.Type.GetMIMEType());
                bytes.AddHeader(ContentLength, page.Value.Data.Length.ToString());
                bytes.AddCRLF();
                bytes.AddBytes(page.Value.Data);
            }

            return bytes.AddCRLF();
        }

        public List<byte> FormResponse(HTTPResponseCode responseCode, string page)
        {
            List<byte> bytes = new List<byte>();

            bytes.AddBytes(HTTPVersion).AddString(responseCode.GetStringResponse()).AddCRLF();

            if(!String.IsNullOrWhiteSpace(ServerName))
            {
                bytes.AddHeader(HeaderServer, ServerName);
            }

            bytes.AddHeader(CacheControl, "no-cache");

            if(!String.IsNullOrWhiteSpace(page))
            {
                bytes.AddHeader(ContentType, HTTPContentType.HTML.GetMIMEType());
                bytes.AddHeader(ContentLength, page.Length.ToString());
                bytes.AddCRLF();
                bytes.AddString(page);
            }

            return bytes.AddCRLF();
        }

        public void FormResponse(HTTPResponseCode responseCode, HTTPFileData? page, Stream stream)
        {
            try
            { 
                stream.WriteBytes(HTTPVersion).WriteString(responseCode.GetStringResponse()).WriteCRLF();

                if(!String.IsNullOrWhiteSpace(ServerName))
                {
                    stream.WriteHeader(HeaderServer, ServerName);
                }

                stream.WriteHeader(CacheControl, "no-cache");

                if(page?.Data != null && page.Value.Data.Length > 0)
                {
                    stream.WriteHeader(ContentType, page.Value.Type.GetMIMEType());
                    stream.WriteHeader(ContentLength, page.Value.Data.Length.ToString());
                    stream.WriteCRLF();
                    stream.WriteBytes(page.Value.Data);
                }

                stream.WriteCRLF();
            }
            catch(Exception e)
            {
                Console.WriteLine(e);
            }
        }

        public void FormResponse(HTTPResponseCode responseCode, string page, Stream stream)
        {
            try
            {
                stream.WriteBytes(HTTPVersion).WriteString(responseCode.GetStringResponse()).WriteCRLF();

                if(!String.IsNullOrWhiteSpace(ServerName))
                {
                    stream.WriteHeader(HeaderServer, ServerName);
                }

                stream.WriteHeader(CacheControl, "no-cache");

                if(!String.IsNullOrWhiteSpace(page))
                {
                    stream.WriteHeader(ContentType, HTTPContentType.HTML.GetMIMEType());
                    stream.WriteHeader(ContentLength, page.Length.ToString());
                    stream.WriteCRLF();
                    stream.WriteString(page);
                }

                stream.WriteCRLF();
            }
            catch(Exception e)
            {
                Console.WriteLine(e);
                // throw;
            }
        }
    }
}
