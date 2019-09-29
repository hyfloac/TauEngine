namespace NodeGraph.Node.Common.Comparison
{
    public class SByteGreaterThanEqualToNode : BinaryOperatorNode<sbyte, bool>
    {
        public SByteGreaterThanEqualToNode(string arbitraryName) : base("SByteGreaterThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(sbyte x, sbyte y) => x >= y;
    }

    public class ShortGreaterThanEqualToNode : BinaryOperatorNode<short, bool>
    {
        public ShortGreaterThanEqualToNode(string arbitraryName) : base("ShortGreaterThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(short x, short y) => x >= y;
    }

    public class IntGreaterThanEqualToNode : BinaryOperatorNode<int, bool>
    {
        public IntGreaterThanEqualToNode(string arbitraryName) : base("IntGreaterThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(int x, int y) => x >= y;
    }

    public class LongGreaterThanEqualToNode : BinaryOperatorNode<long, bool>
    {
        public LongGreaterThanEqualToNode(string arbitraryName) : base("LongGreaterThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(long x, long y) => x >= y;
    }

    public class ByteGreaterThanEqualToNode : BinaryOperatorNode<byte, bool>
    {
        public ByteGreaterThanEqualToNode(string arbitraryName) : base("ByteGreaterThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(byte x, byte y) => x >= y;
    }

    public class UShortGreaterThanEqualToNode : BinaryOperatorNode<ushort, bool>
    {
        public UShortGreaterThanEqualToNode(string arbitraryName) : base("UShortGreaterThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(ushort x, ushort y) => x >= y;
    }

    public class UIntGreaterThanEqualToNode : BinaryOperatorNode<uint, bool>
    {
        public UIntGreaterThanEqualToNode(string arbitraryName) : base("UIntGreaterThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(uint x, uint y) => x >= y;
    }

    public class ULongGreaterThanEqualToNode : BinaryOperatorNode<ulong, bool>
    {
        public ULongGreaterThanEqualToNode(string arbitraryName) : base("ULongGreaterThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(ulong x, ulong y) => x >= y;
    }

    public class FloatGreaterThanEqualToNode : BinaryOperatorNode<float, bool>
    {
        public FloatGreaterThanEqualToNode(string arbitraryName) : base("FloatGreaterThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(float x, float y) => x >= y;
    }

    public class DoubleGreaterThanEqualToNode : BinaryOperatorNode<double, bool>
    {
        public DoubleGreaterThanEqualToNode(string arbitraryName) : base("DoubleGreaterThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(double x, double y) => x >= y;
    }
}
