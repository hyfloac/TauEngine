using System.IO;

namespace BinaryGL
{
    public interface IBinaryGLObject
    {
        BinaryShaderOpcode GetOpcode();

        string GetCleanString();

        string GetCompactString();

        void Write(BinaryWriter writer);

        void Read(BinaryReader reader);
    }
}
