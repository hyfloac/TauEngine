using System.Text;

namespace BinaryGL
{
    public class FunctionPrototype : IBinaryGLObject
    {
        public VariableTypes ReturnType { get; set; }
        public string Name { get; set; }
        public uint ID { get; set; }
        public VariableTypes[] Parameters { get; set; }

        public FunctionPrototype(VariableTypes returnType, string name, uint id, params VariableTypes[] parameters)
        {
            this.ReturnType = returnType;
            this.Name = name;
            this.ID = id;
            this.Parameters = parameters;
        }

        public FunctionPrototype(VariableTypes returnType, uint id, params VariableTypes[] parameters)
            : this(returnType, $"_{id:X}", id, parameters)
        { }

        public string GetCleanString()
        {
            string returnTypeStr = ReturnType.GetGLSLName();
            string[] parametersStr = new string[Parameters.Length];
            uint parameterLen = 0;

            for(uint i = 0; i < parametersStr.Length; ++i)
            {
                parametersStr[i] = Parameters[i].GetGLSLName();
                parameterLen += (uint) parametersStr[i].Length + 8;
            }

            StringBuilder sb = new StringBuilder(3 + returnTypeStr.Length + Name.Length + (int) parameterLen);

            sb.Append(returnTypeStr).Append(' ').Append(Name).Append('(');

            if(parametersStr.Length >= 1)
            {
                sb.Append(parametersStr[0]).Append(" arg0");

                for(uint i = 1; i < parametersStr.Length; ++i)
                {
                    sb.Append(", ").Append(parametersStr[i]).Append($" arg{i:X}");
                }
            }

            sb.Append(')');

            return sb.ToString();
        }

        public string GetCompactString()
        {
            string returnTypeStr = ReturnType.GetGLSLName();
            string[] parametersStr = new string[Parameters.Length];
            uint parameterLen = 0;

            for(uint i = 0; i < parametersStr.Length; ++i)
            {
                parametersStr[i] = Parameters[i].GetGLSLName();
                parameterLen += (uint) parametersStr[i].Length + 5;
            }

            StringBuilder sb = new StringBuilder(3 + returnTypeStr.Length + Name.Length + (int) parameterLen);

            sb.Append(returnTypeStr).Append(' ').Append(Name).Append('(');

            if(parametersStr.Length >= 1)
            {
                sb.Append(parametersStr[0]).Append(" a0");

                for(uint i = 1; i < parametersStr.Length; ++i)
                {
                    sb.Append(',').Append(parametersStr[i]).Append($" a{i:X}");
                }
            }

            sb.Append(')');

            return sb.ToString();
        }

        public override string ToString() => GetCompactString();
    }
}
