namespace NodeGraph.Node.Common.Math
{
    public class SByteSqrtNode : UnaryOperatorNode<sbyte, double>
    {
        public SByteSqrtNode(string arbitraryName) : base("SByteSqrt", arbitraryName) { }

        protected override double ApplyMathFuncToValue(sbyte x) => System.Math.Sqrt(x);
    }

    public class ShortSqrtNode : UnaryOperatorNode<short, double>
    {
        public ShortSqrtNode(string arbitraryName) : base("ShortSqrt", arbitraryName) { }

        protected override double ApplyMathFuncToValue(short x) => System.Math.Sqrt(x);
    }

    public class IntSqrtNode : UnaryOperatorNode<int, double>
    {
        public IntSqrtNode(string arbitraryName) : base("IntSqrt", arbitraryName) { }

        protected override double ApplyMathFuncToValue(int x) => System.Math.Sqrt(x);
    }

    public class LongSqrtNode : UnaryOperatorNode<long, double>
    {
        public LongSqrtNode(string arbitraryName) : base("LongSqrt", arbitraryName) { }

        protected override double ApplyMathFuncToValue(long x) => System.Math.Sqrt(x);
    }

    public class ByteSqrtNode : UnaryOperatorNode<byte, double>
    {
        public ByteSqrtNode(string arbitraryName) : base("ByteSqrt", arbitraryName) { }

        protected override double ApplyMathFuncToValue(byte x) => System.Math.Sqrt(x);
    }

    public class UShortSqrtNode : UnaryOperatorNode<ushort, double>
    {
        public UShortSqrtNode(string arbitraryName) : base("UShortSqrt", arbitraryName) { }

        protected override double ApplyMathFuncToValue(ushort x) => System.Math.Sqrt(x);
    }

    public class UIntSqrtNode : UnaryOperatorNode<uint, double>
    {
        public UIntSqrtNode(string arbitraryName) : base("UIntSqrt", arbitraryName) { }

        protected override double ApplyMathFuncToValue(uint x) => System.Math.Sqrt(x);
    }

    public class ULongSqrtNode : UnaryOperatorNode<ulong, double>
    {
        public ULongSqrtNode(string arbitraryName) : base("ULongSqrt", arbitraryName) { }

        protected override double ApplyMathFuncToValue(ulong x) => System.Math.Sqrt(x);
    }

    public class FloatSqrtNode : UnaryOperatorNode<float, double>
    {
        public FloatSqrtNode(string arbitraryName) : base("FloatSqrt", arbitraryName) { }

        protected override double ApplyMathFuncToValue(float x) => System.Math.Sqrt(x);
    }

    public class DoubleSqrtNode : UnaryOperatorNode<double, double>
    {
        public DoubleSqrtNode(string arbitraryName) : base("DoubleSqrt", arbitraryName) { }

        protected override double ApplyMathFuncToValue(double x) => System.Math.Sqrt(x);
    }
}
