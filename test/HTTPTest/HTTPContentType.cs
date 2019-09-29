namespace HTTPTest
{
    public enum HTTPContentType
    {
        Ascii,
        CSS,
        CSV,
        HTML,
        Calendar,
        JavaScript,
        XML,

        Bitmap,
        GIF,
        Icon,
        JPEG,
        PNG,
        SVG,
        TIFF,

        WAV,
        MP3,

        MPEG,

        TrueTypeFont,

        MSWord,
        MSPowerPoint,
        MSVisio,
        MSExcel,
        MSWordOpenXML,
        MSPowerPointOpenXML,
        MSExcelOpenXML,

        Zip,
        LZMA_7Zip,
        BZip,
        BZip2,
        RAR,
        TAR,

        CShellScript,
        BourneShellScript,

        EPub,
        JSON,
        PDF,
        RTF,
        XHTML,
        XMLApplication,
    }

    public static class HTTPContentTypeUtils
    {
        public static string GetMIMEType(this HTTPContentType type)
        {
            switch(type)
            {
                default:
                case HTTPContentType.Ascii:      return "text/plain";
                case HTTPContentType.CSS:        return "text/css";
                case HTTPContentType.CSV:        return "text/csv";
                case HTTPContentType.HTML:       return "text/html";
                case HTTPContentType.Calendar:   return "text/calendar";
                case HTTPContentType.JavaScript: return "text/javascript";
                case HTTPContentType.XML:        return "text/xml";

                case HTTPContentType.Bitmap: return "image/bmp";
                case HTTPContentType.GIF:    return "image/gif";
                case HTTPContentType.Icon:   return "image/vnd.microsoft.icon";
                case HTTPContentType.JPEG:   return "image/jpeg";
                case HTTPContentType.PNG:    return "image/png";
                case HTTPContentType.SVG:    return "image/svg-xml";
                case HTTPContentType.TIFF:   return "image/tiff";

                case HTTPContentType.WAV: return "audio/wav";
                case HTTPContentType.MP3: return "audio/mpeg";

                case HTTPContentType.MPEG: return "video/mpeg";

                case HTTPContentType.TrueTypeFont: return "font/ttf";

                case HTTPContentType.MSWord:       return "application/msword";
                case HTTPContentType.MSPowerPoint: return "application/vnd.ms-powerpoint";
                case HTTPContentType.MSVisio:      return "application/vnd.visio";
                case HTTPContentType.MSExcel:      return "application/vns.ms-excel";
                case HTTPContentType.MSWordOpenXML:       return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
                case HTTPContentType.MSPowerPointOpenXML: return "application/vnd.openxmlformats-officedocument.presentationml.presentation";
                case HTTPContentType.MSExcelOpenXML:      return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";

                case HTTPContentType.Zip:       return "application/zip";
                case HTTPContentType.LZMA_7Zip: return "application/x-7z-compressed";
                case HTTPContentType.BZip:      return "application/x-bzip";
                case HTTPContentType.BZip2:     return "application/x-bzip2";
                case HTTPContentType.RAR:       return "application/x-rar-compressed";
                case HTTPContentType.TAR:       return "application/x-tar";

                case HTTPContentType.CShellScript:      return "application/x-csh";
                case HTTPContentType.BourneShellScript: return "application/x-sh";

                case HTTPContentType.EPub:           return "application/epub+zip";
                case HTTPContentType.JSON:           return "application/json";
                case HTTPContentType.PDF:            return "application/pdf";
                case HTTPContentType.RTF:            return "application/rtf";
                case HTTPContentType.XHTML:          return "application/xhtml+xml";
                case HTTPContentType.XMLApplication: return "application/xml";
            }
        }

        public static HTTPContentType GetContentType(string extension)
        {
            extension = extension.ToLowerInvariant();

            if(extension.StartsWith("."))
            {
                extension = extension.Substring(1);
            }

            switch(extension)
            {
                default: return HTTPContentType.Ascii;
                case "css": return HTTPContentType.CSS;
                case "csv": return HTTPContentType.CSV;
                case "htm":
                case "html": return HTTPContentType.HTML;
                case "ics": return HTTPContentType.Calendar;
                case "js":
                case "mjs": return HTTPContentType.JavaScript;
                case "xml": return HTTPContentType.XML;

                case "bmp": return HTTPContentType.Bitmap;
                case "gif": return HTTPContentType.GIF;
                case "ico": return HTTPContentType.Icon;
                case "jpg":
                case "jpeg": return HTTPContentType.JPEG;
                case "png": return HTTPContentType.PNG;
                case "svg": return HTTPContentType.SVG;
                case "tif":
                case "tiff": return HTTPContentType.TIFF;

                case "wav": return HTTPContentType.WAV;
                case "mp3": return HTTPContentType.MP3;

                case "mpeg": return HTTPContentType.MPEG;

                case "ttf": return HTTPContentType.TrueTypeFont;

                case "doc": return HTTPContentType.MSWord;
                case "ppt": return HTTPContentType.MSPowerPoint;
                case "vsd": return HTTPContentType.MSVisio;
                case "xls": return HTTPContentType.MSExcel;
                case "docx": return HTTPContentType.MSWordOpenXML;
                case "pptx": return HTTPContentType.MSPowerPointOpenXML;
                case "xlsx": return HTTPContentType.MSExcelOpenXML;

                case "7z": return HTTPContentType.LZMA_7Zip;
                case "bz": return HTTPContentType.BZip;
                case "bz2": return HTTPContentType.BZip2;
                case "rar": return HTTPContentType.RAR;
                case "tar": return HTTPContentType.TAR;

                case "csh": return HTTPContentType.CShellScript;
                case "sh": return HTTPContentType.BourneShellScript;

                case "epub": return HTTPContentType.EPub;
                case "json": return HTTPContentType.JSON;
                case "pdf": return HTTPContentType.PDF;
                case "rtf": return HTTPContentType.RTF;
                case "xhtml": return HTTPContentType.XHTML;
            }
        }
    }
}
