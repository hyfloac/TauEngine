namespace NodeGraph.Node.Common.Comparison
{
    public class SByteLessThanNode : BinaryOperatorNode<sbyte, bool>
    {
        public SByteLessThanNode(string arbitraryName) : base("SByteLessThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(sbyte x, sbyte y) => x < y;
    }

    public class ShortLessThanNode : BinaryOperatorNode<short, bool>
    {
        public ShortLessThanNode(string arbitraryName) : base("ShortLessThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(short x, short y) => x < y;
    }

    public class IntLessThanNode : BinaryOperatorNode<int, bool>
    {
        public IntLessThanNode(string arbitraryName) : base("IntLessThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(int x, int y) => x < y;
    }

    public class LongLessThanNode : BinaryOperatorNode<long, bool>
    {
        public LongLessThanNode(string arbitraryName) : base("LongLessThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(long x, long y) => x < y;
    }

    public class ByteLessThanNode : BinaryOperatorNode<byte, bool>
    {
        public ByteLessThanNode(string arbitraryName) : base("ByteLessThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(byte x, byte y) => x < y;
    }

    public class UShortLessThanNode : BinaryOperatorNode<ushort, bool>
    {
        public UShortLessThanNode(string arbitraryName) : base("UShortLessThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(ushort x, ushort y) => x < y;
    }

    public class UIntLessThanNode : BinaryOperatorNode<uint, bool>
    {
        public UIntLessThanNode(string arbitraryName) : base("UIntLessThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(uint x, uint y) => x < y;
    }

    public class ULongLessThanNode : BinaryOperatorNode<ulong, bool>
    {
        public ULongLessThanNode(string arbitraryName) : base("ULongLessThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(ulong x, ulong y) => x < y;
    }

    public class FloatLessThanNode : BinaryOperatorNode<float, bool>
    {
        public FloatLessThanNode(string arbitraryName) : base("FloatLessThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(float x, float y) => x < y;
    }

    public class DoubleLessThanNode : BinaryOperatorNode<double, bool>
    {
        public DoubleLessThanNode(string arbitraryName) : base("DoubleLessThan", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(double x, double y) => x < y;
    }
}
