using System.IO;
using System.Text;

namespace BinaryGL
{
    public enum PassThroughType : byte
    {
        INPUT,
        OUTPUT
    }

    public class IOVariable : IBinaryGLObject
    {
        public PassThroughType PassThrough { get; set; }
        public VariableType Type { get; set; }
        public string Name { get; set; }
        public uint Index { get; set; }

        public IOVariable(PassThroughType passThrough, VariableType type, string name, uint index)
        {
            this.PassThrough = passThrough;
            this.Type = type;
            this.Name = name;
            this.Index = index;
        }

        public IOVariable(PassThroughType passThrough, VariableType type, uint index)
            : this(passThrough, type, $"_{index:X}", index)
        { }

        public BinaryShaderOpcode GetOpcode() => BinaryShaderOpcode.IOVariableDeclaration;

        public string GetCleanString()
        {
            string passThroughStr = PassThrough.GetGLSLName();
            string typeStr = Type.GetGLSLName();
            string layout = $"layout(location = {this.Index})";

            StringBuilder sb = new StringBuilder(4 + layout.Length + passThroughStr.Length + typeStr.Length + Name.Length);

            sb.Append(layout).Append(' ').Append(passThroughStr).Append(' ').Append(typeStr).Append(' ').Append(Name).Append(';');

            return sb.ToString();
        }

        public string GetCompactString()
        {
            string passThroughStr = PassThrough.GetGLSLName();
            string typeStr = Type.GetGLSLName();
            string compactName = $"_{this.Index:X}";
            string layout = $"layout(location={this.Index})";

            StringBuilder sb = new StringBuilder(4 + layout.Length + passThroughStr.Length + typeStr.Length + compactName.Length);

            sb.Append(layout).Append(' ').Append(passThroughStr).Append(' ').Append(typeStr).Append(' ').Append(compactName).Append(';');

            return sb.ToString();
        }

        public override string ToString() => GetCompactString();

        public void Write(BinaryWriter writer)
        {
            writer.Write((byte) PassThrough);
            writer.Write((byte) Type);
            writer.Write(Name);
            writer.Write(Index);
        }

        public void Read(BinaryReader reader)
        {
            PassThrough = (PassThroughType) reader.ReadByte();
            Type = (VariableType) reader.ReadByte();
            Name = reader.ReadString();
            Index = reader.ReadUInt32();
        }
    }

    public static class PassThroughTypeUtils
    {
        public static string GetGLSLName(this PassThroughType type)
        {
            switch(type)
            {
                case PassThroughType.INPUT:  return "in";
                case PassThroughType.OUTPUT: return "out";
                default: return "";
            }
        }
    }
}
