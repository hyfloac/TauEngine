namespace HTTPTest
{
    public enum HTTPResponseCode
    {
        // Information Processing
        Continue = 100,
        SwitchingProtocol = 101,
        Processing = 102,
        EarlyHints = 103,

        // Successful Responses
        Ok = 200,
        Created = 201,
        Accepted = 202,
        NonAuthoritativeInformation = 203,
        NoContent = 204,
        ResetContent = 205,
        PartialContent = 206,
        MultiStatus = 207,
        AlreadyReported = 208,
        IMUsed = 226,

        // Redirection Messages
        MultipleChoice = 300,
        MovedPermanently = 301,
        Found = 302,
        SeeOther = 303,
        NotModified = 304,
        UseProxy = 305,
        Unused = 306,
        TemporaryRedirect = 307,
        PermanentRedirect = 308,

        // Client Error Responses
        BadRequest = 400,
        Unauthorized = 401,
        PaymentRequired = 402,
        Forbidden = 403,
        NotFound = 404,
        MethodNotAllowed = 405,
        NotAcceptable = 406,
        ProxyAuthenticationRequired = 407,
        RequestTimeout = 408,
        Conflict = 409,
        Gone = 410,
        LengthRequired = 411,
        PreconditionFailed = 412,
        PayloadTooLarge = 413,
        URITooLong = 414,
        UnsupportedMediaType = 415,
        RequestRangeNotSatisfiable = 416,
        ExpectationFailed = 417,
        ImATeapot = 418,
        MisdirectedRequest = 421,
        UnprocessableEntity = 422,
        Locked = 423,
        FailedDependency = 424,
        TooEarly = 425,
        UpgradeRequired = 426,
        PreconditionRequired = 428,
        TooManyRequests = 429,
        RequestHeaderFieldsTooLarge = 431,
        UnavailableForLegalReasons = 451,

        // Server Error Responses
        InternalServerError = 500,
        NotImplemented = 501,
        BadGateway = 502,
        ServiceUnavailable = 503,
        GatewayTimeout = 504,
        HTTPVersionNotSupported = 505,
        VariantAlsoNegotiates = 506,
        InsufficientStorage = 507,
        LoopDetected = 508,
        NotExtended = 510,
        NetworkAuthenticationError = 511
    }

    public static class HTTPResponseCodeUtils
    {
        public static string GetStringResponse(this HTTPResponseCode code)
        {
            switch(code)
            {
                // Information Responses
                case HTTPResponseCode.Continue: return "100 Continue";
                case HTTPResponseCode.SwitchingProtocol: return "101 Switching Protocol";
                case HTTPResponseCode.Processing: return "102 Processing";
                case HTTPResponseCode.EarlyHints: return "103 Early Hints";

                // Successful Responses
                case HTTPResponseCode.Ok: return "200 OK";
                case HTTPResponseCode.Created: return "201 Created";
                case HTTPResponseCode.Accepted: return "202 Accepted";
                case HTTPResponseCode.NonAuthoritativeInformation: return "203 Non-Authoritative Information";
                case HTTPResponseCode.NoContent: return "204 No Content";
                case HTTPResponseCode.ResetContent: return "205 Reset Content";
                case HTTPResponseCode.PartialContent: return "206 Partial Content";
                case HTTPResponseCode.MultiStatus: return "207 Multi-Status";
                case HTTPResponseCode.AlreadyReported: return "208 Already Reported";
                case HTTPResponseCode.IMUsed: return "226 IM Used";

                // Redirection Messages
                case HTTPResponseCode.MultipleChoice: return "300 Multiple Choice";
                case HTTPResponseCode.MovedPermanently: return "301 Moved Permanently";
                case HTTPResponseCode.Found: return "302 Found";
                case HTTPResponseCode.SeeOther: return "303 See Other";
                case HTTPResponseCode.NotModified: return "304 Not Modified";
                case HTTPResponseCode.UseProxy: return "305 Use Proxy";
                case HTTPResponseCode.Unused: return "306 unused";
                case HTTPResponseCode.TemporaryRedirect: return "307 Temporary Redirect";
                case HTTPResponseCode.PermanentRedirect: return "308 Permanent Redirect";
                                                  
                // Client Error Responses
                case HTTPResponseCode.BadRequest: return "400 Bad Request";
                case HTTPResponseCode.Unauthorized: return "401 Unauthorized";
                case HTTPResponseCode.PaymentRequired: return "402 Payment Required";
                case HTTPResponseCode.Forbidden: return "403 Forbidden";
                case HTTPResponseCode.NotFound: return "404 Not Found";
                case HTTPResponseCode.MethodNotAllowed: return "405 Method Not Allowed";
                case HTTPResponseCode.NotAcceptable: return "406 Not Acceptable";
                case HTTPResponseCode.ProxyAuthenticationRequired: return "407 Proxy Authentication Required";
                case HTTPResponseCode.RequestTimeout: return "408 Request Timeout";
                case HTTPResponseCode.Conflict: return "409 Conflict";
                case HTTPResponseCode.Gone: return "410 Gone";
                case HTTPResponseCode.LengthRequired: return "411 Length Required";
                case HTTPResponseCode.PreconditionFailed: return "412 Precondition Failed";
                case HTTPResponseCode.PayloadTooLarge: return "413 Payload Too Large";
                case HTTPResponseCode.URITooLong: return "414 URI Too Long";
                case HTTPResponseCode.UnsupportedMediaType: return "415 Unsupported Media Type";
                case HTTPResponseCode.RequestRangeNotSatisfiable: return "416 Requested Range Not Satisfiable";
                case HTTPResponseCode.ExpectationFailed: return "417 Expectation Failed";
                case HTTPResponseCode.ImATeapot: return "418 I'm a teapot";
                case HTTPResponseCode.MisdirectedRequest: return "421 Misdirected Request";
                case HTTPResponseCode.UnprocessableEntity: return "422 Unprocessable Entity";
                case HTTPResponseCode.Locked: return "423 Locked";
                case HTTPResponseCode.FailedDependency: return "424 Failed Dependency";
                case HTTPResponseCode.TooEarly: return "425 Too Early";
                case HTTPResponseCode.UpgradeRequired: return "426 Upgrade Required";
                case HTTPResponseCode.PreconditionRequired: return "428 Precondition Required";
                case HTTPResponseCode.TooManyRequests: return "429 Too Many Requests";
                case HTTPResponseCode.RequestHeaderFieldsTooLarge: return "431 Request Header Fields Too Large";
                case HTTPResponseCode.UnavailableForLegalReasons: return "451 Unavailable For Legal Reasons";

                // Server Error Responses
                default:
                case HTTPResponseCode.InternalServerError: return "500 Internal Server Error";
                case HTTPResponseCode.NotImplemented: return "501 Not Implemented";
                case HTTPResponseCode.BadGateway: return "502 Bad Gateway";
                case HTTPResponseCode.ServiceUnavailable: return "503 Service Unavailable";
                case HTTPResponseCode.GatewayTimeout: return "504 Gateway Timeout";
                case HTTPResponseCode.HTTPVersionNotSupported: return "505 HTTP Version Not Supported";
                case HTTPResponseCode.VariantAlsoNegotiates: return "506 Variant Also Negotiates";
                case HTTPResponseCode.InsufficientStorage: return "507 Insufficient Storage";
                case HTTPResponseCode.LoopDetected: return "508 Loop Detected";
                case HTTPResponseCode.NotExtended: return "510 Not Extended";
                case HTTPResponseCode.NetworkAuthenticationError: return "511 Network Authentication Required";
            }
        }
    }
}
