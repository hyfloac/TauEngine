using System.Collections.Generic;
using System.IO;
using System.Text;

namespace BinaryGL
{
    public class UniformVariable : IBinaryGLObject
    {
        public string Name { get; set; }
        public VariableType Type { get; set; }

        public UniformVariable(string name, VariableType type)
        {
            Name = name;
            Type = type;
        }

        public BinaryShaderOpcode GetOpcode() => BinaryShaderOpcode.DeclareUniform;

        public string GetCleanString()
        {
            string typeName = Type.GetGLSLName();

            StringBuilder sb = new StringBuilder(10 + typeName.Length + Name.Length);

            sb.Append("uniform ").Append(typeName).Append(' ').Append(Name).Append(';');

            return sb.ToString();
        }

        public string GetCompactString() => this.GetCleanString();

        public override string ToString() => GetCompactString();

        public void Write(BinaryWriter writer)
        {
            writer.Write(Name);
            writer.Write((byte) Type);
        }

        public void Read(BinaryReader reader)
        {
            Name = reader.ReadString();
            Type = (VariableType) reader.ReadByte();
        }
    }
}
