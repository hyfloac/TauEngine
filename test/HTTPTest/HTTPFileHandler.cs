using System.Collections.Generic;
using System.IO;

namespace HTTPTest
{
    public struct HTTPFileData
    {
        public HTTPContentType Type { get; private set; }
        public byte[] Data { get; private set; }

        public HTTPFileData(HTTPContentType type, byte[] data)
        {
            Type = type;
            Data = data;
        }
    }

    public class HTTPFileHandler
    {
        private readonly Dictionary<string, HTTPFileData> files;

        public DirectoryInfo FileRoot { get; set; }

        public HTTPFileHandler(DirectoryInfo fileRoot)
        {
            this.files = new Dictionary<string, HTTPFileData>();
            FileRoot = fileRoot;
        }

        public HTTPFileData? GetFile(string filePath)
        {
            string fullPath = Path.Combine(FileRoot.FullName, filePath);
            if(this.files.ContainsKey(fullPath))
            {
                return this.files[fullPath];
            }

            if(!File.Exists(fullPath)) { return null; }

            HTTPFileData fileData = new HTTPFileData(HTTPContentTypeUtils.GetContentType(Path.GetExtension(fullPath)), File.ReadAllBytes(fullPath));

            this.files.Add(fullPath, fileData);

            return fileData;
        }
    }
}
