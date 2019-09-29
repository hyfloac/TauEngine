using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace HTTPTest
{
    public enum RequestType : byte
    {
        Get,
        Head,
        Post,
        Put,
        Delete,
        Connect,
        Options,
        Trace,
        Patch,
        Unknown
    }

    public static class RequestTypeUtils
    {
        public static RequestType FromString(string str)
        {
            if(String.IsNullOrWhiteSpace(str))
            {
                return RequestType.Unknown;
            }

            switch(str.ToLowerInvariant())
            {
                case "get":     return RequestType.Get;
                case "head":    return RequestType.Head;
                case "post":    return RequestType.Post;
                case "put":     return RequestType.Put;
                case "delete":  return RequestType.Delete;
                case "connect": return RequestType.Connect;
                case "options": return RequestType.Options;
                case "trace":   return RequestType.Trace;
                case "patch":   return RequestType.Patch;
                default:        return RequestType.Unknown;
            }
        }
    }

    public class HTTPRequest
    {
        public RequestType Type { get; private set; }
        public string RequestPath { get; private set; }
        public Dictionary<string, string> Params { get; private set; }
        public string Host { get; private set; }
        public string Browser { get; private set; }
        public string AcceptableFileTypes { get; private set; }
        public string AcceptableLanguages { get; private set; }
        public string AcceptableEncodings { get; private set; }
        public string Cookie { get; private set; }

        public HTTPRequest(RequestType type, string requestPath, Dictionary<string, string> @params, string host, string browser, string acceptableFileTypes, string acceptableLanguages, string acceptableEncodings, string cookie)
        {
            this.Type = type;
            this.RequestPath = requestPath;
            this.Params = @params;
            this.Host = host;
            this.Browser = browser;
            this.AcceptableFileTypes = acceptableFileTypes;
            this.AcceptableLanguages = acceptableLanguages;
            this.AcceptableEncodings = acceptableEncodings;
            this.Cookie = cookie;
        }

        // public static HTTPRequest Create(string request)
        // {
        //     if(String.IsNullOrWhiteSpace(request))
        //     { return null; }
        //
        //     string[] split = request.Split(new [] { ' ' }, 3);
        //
        //     if(split.Length != 3)
        //     { return null; }
        //
        //     RequestType rt = RequestTypeUtils.FromString(split[0]);
        //
        //     if(rt == RequestType.Unknown)
        //     { return null; }
        //
        //     split[1] = split[1].Substring(1).Replace('/', '\\');
        //
        //     if(split[1].Contains("?"))
        //     {
        //         string[] pathSplit = split[1].Split(new[] {'?'}, 2);
        //
        //         if(pathSplit.Length != 2)
        //         {
        //             return null;
        //         }
        //
        //         Dictionary<string, string> _params = new Dictionary<string, string>();
        //
        //         string[] paramsSplit = pathSplit[1].Split('&');
        //
        //         foreach(string param in paramsSplit)
        //         {
        //             string[] paramSplit = param.Split(new[] {'='}, 2);
        //
        //             if(paramSplit.Length != 2)
        //             {
        //                 return null;
        //             }
        //
        //             _params[paramSplit[0]] = paramSplit[1];
        //         }
        //
        //         // if(pathSplit[0].EndsWith("\\"))
        //         // {
        //         //     pathSplit[0] = pathSplit[0].Substring(0, pathSplit[0].Length - 1);
        //         // }
        //
        //         return new HTTPRequest(rt, pathSplit[0], _params);
        //     }
        //     else
        //     {
        //         // if(split[1].EndsWith("\\"))
        //         // {
        //         //     split[1] = split[1].Substring(0, split[1].Length - 1);
        //         // }
        //
        //         return new HTTPRequest(rt, split[1], new Dictionary<string, string>());
        //     }
        // }

        public class Builder
        {
            public RequestType type;
            public string requestPath;
            public Dictionary<string, string> @params;
            public string host;
            public string browser;
            public string acceptableFileTypes;
            public string acceptableLanguages;
            public string acceptableEncodings;
            public string cookie;

            public Builder()
            {
                this.type = RequestType.Unknown;
                this.requestPath = String.Empty;
                this.@params = null;
                this.host = String.Empty;
                this.browser = String.Empty;
                this.acceptableFileTypes = String.Empty;
                this.acceptableLanguages = String.Empty;
                this.acceptableEncodings = String.Empty;
                this.cookie = String.Empty;
            }

            public HTTPRequest Build()
            {
                if(this.type == RequestType.Unknown)
                {
                    return null;
                }

                if(this.@params == null)
                {
                    this.@params = new Dictionary<string, string>();
                }

                return new HTTPRequest(this.type, this.requestPath, this.@params, this.host, this.browser, this.acceptableFileTypes, this.acceptableLanguages, this.acceptableEncodings, this.cookie);
            }

            public Builder Type(RequestType _type)
            {
                this.type = _type;
                return this;
            }

            public Builder RequestPath(string _requestPath)
            {
                this.requestPath = _requestPath;
                return this;
            }

            public Builder Params(Dictionary<string, string> _params)
            {
                this.@params = _params;
                return this;
            }

            public Builder Host(string _host)
            {
                this.host = _host;
                return this;
            }

            public Builder Browser(string _browser)
            {
                this.browser = _browser;
                return this;
            }

            public Builder AcceptableFileTypes(string _acceptableFileTypes)
            {
                this.acceptableFileTypes = _acceptableFileTypes;
                return this;
            }

            public Builder AcceptableLanguages(string _acceptableLanguages)
            {
                this.acceptableLanguages = _acceptableLanguages;
                return this;
            }

            public Builder AcceptableEncodings(string _acceptableEncodings)
            {
                this.acceptableEncodings = _acceptableEncodings;
                return this;
            }

            public Builder Cookie(string _cookie)
            {
                this.cookie = _cookie;
                return this;
            }
        }
    }

    public static class HTTPRequestParser
    {
        private static int SkipWhitespace(Stream stream)
        {
            while(true)
            {
                int _read = stream.ReadByte();
                if(_read < 0) { return -1; }

                char c = (char) _read;
                if(!Char.IsWhiteSpace(c) || c == '\r' || c == '\n') { return _read; }
            }
        }

        public static HTTPRequest Parse(Stream stream)
        {
            if(stream == null || !stream.CanRead) { return null; }

            StringBuilder sb = new StringBuilder();

            int _read = SkipWhitespace(stream);
            if(_read < 0) { return null; }

            sb.Append((char) _read);

            while(true)
            {
                _read = stream.ReadByte();
                if(_read < 0) { return null; }

                char c = (char) _read;
                if(Char.IsWhiteSpace(c)) { break; }

                sb.Append(c);
            }

            RequestType reqType = RequestTypeUtils.FromString(sb.ToString());

            if(reqType == RequestType.Unknown)
            {
                return null;
            }

            _read = SkipWhitespace(stream);
            if(_read < 0) { return null; }

            if(((char) _read) != '/') { return null; }

            sb.Clear();

            while(true)
            {
                _read = stream.ReadByte();
                if(_read < 0) { return null; }

                char c = (char) _read;
                if(Char.IsWhiteSpace(c) || c == '?') { break; }

                sb.Append(c);
            }

            string path = sb.ToString();

            sb.Clear();

            HTTPRequest.Builder builder = new HTTPRequest.Builder().Type(reqType).RequestPath(path);

            if(((char) _read) == '?')
            {
                Dictionary<string, string> @params = new Dictionary<string, string>();

                while(true)
                {
                    sb.Clear();

                    while(true)
                    {
                        _read = stream.ReadByte();
                        if(_read < 0) { return null; }

                        char c = (char) _read;
                        if(c == '=') { break; }

                        sb.Append(c);
                    }

                    string paramKey = sb.ToString();

                    sb.Clear();

                    while(true)
                    {
                        _read = stream.ReadByte();
                        if(_read < 0) { return null; }

                        char c = (char) _read;
                        if(c == '&' || Char.IsWhiteSpace(c)) { break; }

                        sb.Append(c);
                    }

                    string paramValue = sb.ToString();

                    @params.Add(paramKey, paramValue);

                    if(Char.IsWhiteSpace((char) _read))
                    {
                        break;
                    }
                }

                builder.Params(@params);
            }

            _read = SkipWhitespace(stream);
            if(_read < 0) { return null; }

            sb.Clear();

            sb.Append((char) _read);

            while(true)
            {
                _read = stream.ReadByte();
                if(_read < 0) { return null; }

                char c = (char) _read;
                if(Char.IsWhiteSpace(c) || c == '\r' || c == '\n') { break; }

                sb.Append(c);
            }

            if(!sb.ToString().Equals("HTTP/1.1")) { return null; }


            stream.Flush();
            // stream.Seek(0, SeekOrigin.End);
            // byte[] buffer = new byte[256];
            // int i;
            // while((i = stream.Read(buffer, 0, buffer.Length)) != 0)
            // {
            //     if(i != buffer.Length) { break; }
            // }

            return builder.Build();
        }
    }
}
