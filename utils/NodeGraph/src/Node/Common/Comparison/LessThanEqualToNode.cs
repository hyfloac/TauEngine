namespace NodeGraph.Node.Common.Comparison
{
    public class SByteLessThanEqualToNode : BinaryOperatorNode<sbyte, bool>
    {
        public SByteLessThanEqualToNode(string arbitraryName) : base("SByteLessThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(sbyte x, sbyte y) => x <= y;
    }

    public class ShortLessThanEqualToNode : BinaryOperatorNode<short, bool>
    {
        public ShortLessThanEqualToNode(string arbitraryName) : base("ShortLessThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(short x, short y) => x <= y;
    }

    public class IntLessThanEqualToNode : BinaryOperatorNode<int, bool>
    {
        public IntLessThanEqualToNode(string arbitraryName) : base("IntLessThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(int x, int y) => x <= y;
    }

    public class LongLessThanEqualToNode : BinaryOperatorNode<long, bool>
    {
        public LongLessThanEqualToNode(string arbitraryName) : base("LongLessThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(long x, long y) => x <= y;
    }

    public class ByteLessThanEqualToNode : BinaryOperatorNode<byte, bool>
    {
        public ByteLessThanEqualToNode(string arbitraryName) : base("ByteLessThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(byte x, byte y) => x <= y;
    }

    public class UShortLessThanEqualToNode : BinaryOperatorNode<ushort, bool>
    {
        public UShortLessThanEqualToNode(string arbitraryName) : base("UShortLessThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(ushort x, ushort y) => x <= y;
    }

    public class UIntLessThanEqualToNode : BinaryOperatorNode<uint, bool>
    {
        public UIntLessThanEqualToNode(string arbitraryName) : base("UIntLessThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(uint x, uint y) => x <= y;
    }

    public class ULongLessThanEqualToNode : BinaryOperatorNode<ulong, bool>
    {
        public ULongLessThanEqualToNode(string arbitraryName) : base("ULongLessThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(ulong x, ulong y) => x <= y;
    }

    public class FloatLessThanEqualToNode : BinaryOperatorNode<float, bool>
    {
        public FloatLessThanEqualToNode(string arbitraryName) : base("FloatLessThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(float x, float y) => x <= y;
    }

    public class DoubleLessThanEqualToNode : BinaryOperatorNode<double, bool>
    {
        public DoubleLessThanEqualToNode(string arbitraryName) : base("DoubleLessThanEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(double x, double y) => x <= y;
    }
}
