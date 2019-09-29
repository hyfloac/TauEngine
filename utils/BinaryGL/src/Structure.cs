using System.Text;

namespace BinaryGL
{
    public class Structure : IBinaryGLObject
    {
        public string Name { get; set; }
        public StructureMember[] Members { get; }

        public Structure(string name, uint memberCount)
        {
            Name = name;
            Members = new StructureMember[memberCount];
        }

        public string GetCleanString()
        {
            StringBuilder sb = new StringBuilder(12 + 25 * Members.Length);

            sb.Append("struct ").Append(Name).Append("\n{\n");

            foreach(StructureMember sm in Members)
            {
                sb.Append("    ").Append(sm).Append('\n');
            }

            sb.Append("};");

            return sb.ToString();
        }

        public string GetCompactString()
        {
            StringBuilder sb = new StringBuilder(10 + 20 * Members.Length);

            sb.Append("struct ").Append(Name).Append('{');

            foreach(StructureMember sm in Members)
            {
                sb.Append(sm);
            }

            sb.Append("};");

            return sb.ToString();
        }

        public override string ToString() => GetCompactString();
    }

    public class StructureMember
    {
        public VariableTypes Type { get; set; }
        public uint[] ArraySizes { get; set; }
        public string Name { get; set; }

        public override string ToString()
        {
            string typeString = Type.GetGLSLName();

            int arrLen = 0;

            if(ArraySizes != null)
            {
                arrLen = ArraySizes.Length * 12;
            }

            StringBuilder sb = new StringBuilder(typeString.Length + arrLen + 1 + Name.Length);

            sb.Append(typeString).Append(' ').Append(Name);
            if(ArraySizes != null)
            {
                foreach(uint size in ArraySizes)
                {
                    sb.Append('[').Append(size).Append(']');
                }
            }

            sb.Append(';');

            return sb.ToString();
        }
    }
}
