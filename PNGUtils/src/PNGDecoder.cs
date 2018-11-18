using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PNGUtils
{
    public interface IChunk
    {

    }

    public enum PixelFormat : byte
    {
        GrayScale = 0,
        RGB = 2,
        Indexed = 3,
        GrayScaleAlpha = 4,
        RGBA = 6
    }

    public struct PNGHeader : IChunk
    {
        public uint width;
        public uint height;
        public byte bitsPerPixel;
        public PixelFormat colorType;
        public byte compression;
        public byte filter;
        public byte interlace;
    }

    public class PNGDecoder
    {
        private static readonly byte[] signature = {0x89, (byte) 'P', (byte) 'N', (byte) 'G', (byte) '\r', (byte) '\n', 0x1A, (byte) '\n' };
        private BinaryReader br;

        public PNGDecoder(Stream stream)
        {
            br = new BinaryReader(stream);
        }


    }
}
