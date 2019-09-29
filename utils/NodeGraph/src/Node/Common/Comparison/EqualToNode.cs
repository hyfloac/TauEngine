namespace NodeGraph.Node.Common.Comparison
{
    public class SByteEqualToNode : BinaryOperatorNode<sbyte, bool>
    {
        public SByteEqualToNode(string arbitraryName) : base("SByteEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(sbyte x, sbyte y) => x == y;
    }

    public class ShortEqualToNode : BinaryOperatorNode<short, bool>
    {
        public ShortEqualToNode(string arbitraryName) : base("ShortEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(short x, short y) => x == y;
    }

    public class IntEqualToNode : BinaryOperatorNode<int, bool>
    {
        public IntEqualToNode(string arbitraryName) : base("IntEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(int x, int y) => x == y;
    }

    public class LongEqualToNode : BinaryOperatorNode<long, bool>
    {
        public LongEqualToNode(string arbitraryName) : base("LongEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(long x, long y) => x == y;
    }

    public class ByteEqualToNode : BinaryOperatorNode<byte, bool>
    {
        public ByteEqualToNode(string arbitraryName) : base("ByteEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(byte x, byte y) => x == y;
    }

    public class UShortEqualToNode : BinaryOperatorNode<ushort, bool>
    {
        public UShortEqualToNode(string arbitraryName) : base("UShortEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(ushort x, ushort y) => x == y;
    }

    public class UIntEqualToNode : BinaryOperatorNode<uint, bool>
    {
        public UIntEqualToNode(string arbitraryName) : base("UIntEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(uint x, uint y) => x == y;
    }

    public class ULongEqualToNode : BinaryOperatorNode<ulong, bool>
    {
        public ULongEqualToNode(string arbitraryName) : base("ULongEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(ulong x, ulong y) => x == y;
    }

    public class FloatEqualToNode : BinaryOperatorNode<float, bool>
    {
        public FloatEqualToNode(string arbitraryName) : base("FloatEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(float x, float y) => x == y;
    }

    public class DoubleEqualToNode : BinaryOperatorNode<double, bool>
    {
        public DoubleEqualToNode(string arbitraryName) : base("DoubleEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(double x, double y) => x == y;
    }

    public class FloatEpsilonEqualToNode : TrinaryOperatorNode<float, bool>
    {
        public FloatEpsilonEqualToNode(string arbitraryName) : base("FloatEpsilonNotEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(float x, float y, float epsilon) => System.Math.Abs(x - y) < epsilon;
    }

    public class DoubleEpsilonEqualToNode : TrinaryOperatorNode<double, bool>
    {
        public DoubleEpsilonEqualToNode(string arbitraryName) : base("DoubleEpsilonNotEqualTo", arbitraryName) { }

        protected override bool ApplyMathFuncToValues(double x, double y, double epsilon) => System.Math.Abs(x - y) < epsilon;
    }
}
