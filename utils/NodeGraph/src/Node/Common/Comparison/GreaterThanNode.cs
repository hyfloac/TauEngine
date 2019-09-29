namespace NodeGraph.Node.Common.Comparison
{
    public class SByteGreaterThanNode : BinaryOperatorNode<sbyte, bool>
    {
        public SByteGreaterThanNode(string arbitraryName) : base("SByteGreaterThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(sbyte x, sbyte y) => x > y;
    }

    public class ShortGreaterThanNode : BinaryOperatorNode<short, bool>
    {
        public ShortGreaterThanNode(string arbitraryName) : base("ShortGreaterThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(short x, short y) => x > y;
    }

    public class IntGreaterThanNode : BinaryOperatorNode<int, bool>
    {
        public IntGreaterThanNode(string arbitraryName) : base("IntGreaterThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(int x, int y) => x > y;
    }

    public class LongGreaterThanNode : BinaryOperatorNode<long, bool>
    {
        public LongGreaterThanNode(string arbitraryName) : base("LongGreaterThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(long x, long y) => x > y;
    }

    public class ByteGreaterThanNode : BinaryOperatorNode<byte, bool>
    {
        public ByteGreaterThanNode(string arbitraryName) : base("ByteGreaterThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(byte x, byte y) => x > y;
    }

    public class UShortGreaterThanNode : BinaryOperatorNode<ushort, bool>
    {
        public UShortGreaterThanNode(string arbitraryName) : base("UShortGreaterThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(ushort x, ushort y) => x > y;
    }

    public class UIntGreaterThanNode : BinaryOperatorNode<uint, bool>
    {
        public UIntGreaterThanNode(string arbitraryName) : base("UIntGreaterThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(uint x, uint y) => x > y;
    }

    public class ULongGreaterThanNode : BinaryOperatorNode<ulong, bool>
    {
        public ULongGreaterThanNode(string arbitraryName) : base("ULongGreaterThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(ulong x, ulong y) => x > y;
    }

    public class FloatGreaterThanNode : BinaryOperatorNode<float, bool>
    {
        public FloatGreaterThanNode(string arbitraryName) : base("FloatGreaterThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(float x, float y) => x > y;
    }

    public class DoubleGreaterThanNode : BinaryOperatorNode<double, bool>
    {
        public DoubleGreaterThanNode(string arbitraryName) : base("DoubleGreaterThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(double x, double y) => x > y;
    }
}
