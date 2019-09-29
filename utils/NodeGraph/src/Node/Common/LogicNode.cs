namespace NodeGraph.Node.Common
{
    public class LogicalOr : BinaryOperatorNode<bool>
    {
        public LogicalOr(string arbitraryName) : base("LogicalOr", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(bool x, bool y) => x || y;
    }

    public class LogicalAnd : BinaryOperatorNode<bool>
    {
        public LogicalAnd(string arbitraryName) : base("LogicalAnd", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(bool x, bool y) => x && y;
    }

    public class LogicalXor : BinaryOperatorNode<bool>
    {
        public LogicalXor(string arbitraryName) : base("LogicalXor", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(bool x, bool y) => x ^ y;
    }

    public class LogicalNor : BinaryOperatorNode<bool>
    {
        public LogicalNor(string arbitraryName) : base("LogicalNor", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(bool x, bool y) => !(x || y);
    }

    public class LogicalNand : BinaryOperatorNode<bool>
    {
        public LogicalNand(string arbitraryName) : base("LogicalNand", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(bool x, bool y) => !(x && y);
    }

    public class LogicalXnor : BinaryOperatorNode<bool>
    {
        public LogicalXnor(string arbitraryName) : base("LogicalXnor", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(bool x, bool y) => !(x ^ y);
    }

    public class LogicalNot : UnaryOperatorNode<bool>
    {
        public LogicalNot(string arbitraryName) : base("LogicalNot", arbitraryName) { }

        protected override bool ApplyMathFuncToValue(bool x) => !x;
    }
}
