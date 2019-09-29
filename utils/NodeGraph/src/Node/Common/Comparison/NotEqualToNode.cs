namespace NodeGraph.Node.Common.Comparison
{
    public class SByteNotEqualToNode : BinaryOperatorNode<sbyte, bool>
    {
        public SByteNotEqualToNode(string arbitraryName) : base("SByteNotEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(sbyte x, sbyte y) => x != y;
    }

    public class ShortNotEqualToNode : BinaryOperatorNode<short, bool>
    {
        public ShortNotEqualToNode(string arbitraryName) : base("ShortNotEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(short x, short y) => x != y;
    }

    public class IntNotEqualToNode : BinaryOperatorNode<int, bool>
    {
        public IntNotEqualToNode(string arbitraryName) : base("IntNotEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(int x, int y) => x != y;
    }

    public class LongNotEqualToNode : BinaryOperatorNode<long, bool>
    {
        public LongNotEqualToNode(string arbitraryName) : base("LongNotEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(long x, long y) => x != y;
    }

    public class ByteNotEqualToNode : BinaryOperatorNode<byte, bool>
    {
        public ByteNotEqualToNode(string arbitraryName) : base("ByteNotEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(byte x, byte y) => x != y;
    }

    public class UShortNotEqualToNode : BinaryOperatorNode<ushort, bool>
    {
        public UShortNotEqualToNode(string arbitraryName) : base("UShortNotEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(ushort x, ushort y) => x != y;
    }

    public class UIntNotEqualToNode : BinaryOperatorNode<uint, bool>
    {
        public UIntNotEqualToNode(string arbitraryName) : base("UIntNotEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(uint x, uint y) => x != y;
    }

    public class ULongNotEqualToNode : BinaryOperatorNode<ulong, bool>
    {
        public ULongNotEqualToNode(string arbitraryName) : base("ULongNotEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(ulong x, ulong y) => x != y;
    }

    public class FloatNotEqualToNode : BinaryOperatorNode<float, bool>
    {
        public FloatNotEqualToNode(string arbitraryName) : base("FloatNotEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(float x, float y) => x != y;
    }

    public class DoubleNotEqualToNode : BinaryOperatorNode<double, bool>
    {
        public DoubleNotEqualToNode(string arbitraryName) : base("DoubleNotEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(double x, double y) => x != y;
    }

    public class FloatEpsilonNotEqualToNode : TrinaryOperatorNode<float, bool>
    {
        public FloatEpsilonNotEqualToNode(string arbitraryName) : base("FloatEpsilonNotEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(float x, float y, float epsilon) => System.Math.Abs(x - y) > epsilon;
    }

    public class DoubleEpsilonNotEqualToNode : TrinaryOperatorNode<double, bool>
    {
        public DoubleEpsilonNotEqualToNode(string arbitraryName) : base("DoubleEpsilonNotEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(double x, double y, double epsilon) => System.Math.Abs(x - y) > epsilon;
    }
}
