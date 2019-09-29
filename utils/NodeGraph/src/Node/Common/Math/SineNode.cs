namespace NodeGraph.Node.Common.Math
{
    public class SByteSineNode : UnaryOperatorNode<sbyte, double>
    {
        public SByteSineNode(string arbitraryName) : base("SByteSine", arbitraryName) { }

        protected override double ApplyMathFuncToValue(sbyte x) => System.Math.Sin(x);
    }

    public class ShortSineNode : UnaryOperatorNode<short, double>
    {
        public ShortSineNode(string arbitraryName) : base("ShortSine", arbitraryName) { }

        protected override double ApplyMathFuncToValue(short x) => System.Math.Sin(x);
    }

    public class IntSineNode : UnaryOperatorNode<int, double>
    {
        public IntSineNode(string arbitraryName) : base("IntSine", arbitraryName) { }

        protected override double ApplyMathFuncToValue(int x) => System.Math.Sin(x);
    }

    public class LongSineNode : UnaryOperatorNode<long, double>
    {
        public LongSineNode(string arbitraryName) : base("LongSine", arbitraryName) { }

        protected override double ApplyMathFuncToValue(long x) => System.Math.Sin(x);
    }

    public class ByteSineNode : UnaryOperatorNode<byte, double>
    {
        public ByteSineNode(string arbitraryName) : base("ByteSine", arbitraryName) { }

        protected override double ApplyMathFuncToValue(byte x) => System.Math.Sin(x);
    }

    public class UShortSineNode : UnaryOperatorNode<ushort, double>
    {
        public UShortSineNode(string arbitraryName) : base("UShortSine", arbitraryName) { }

        protected override double ApplyMathFuncToValue(ushort x) => System.Math.Sin(x);
    }

    public class UIntSineNode : UnaryOperatorNode<uint, double>
    {
        public UIntSineNode(string arbitraryName) : base("UIntSine", arbitraryName) { }

        protected override double ApplyMathFuncToValue(uint x) => System.Math.Sin(x);
    }

    public class ULongSineNode : UnaryOperatorNode<ulong, double>
    {
        public ULongSineNode(string arbitraryName) : base("ULongSine", arbitraryName) { }

        protected override double ApplyMathFuncToValue(ulong x) => System.Math.Sin(x);
    }

    public class FloatSineNode : UnaryOperatorNode<float, double>
    {
        public FloatSineNode(string arbitraryName) : base("FloatSine", arbitraryName) { }

        protected override double ApplyMathFuncToValue(float x) => System.Math.Sin(x);
    }

    public class DoubleSineNode : UnaryOperatorNode<double, double>
    {
        public DoubleSineNode(string arbitraryName) : base("DoubleSine", arbitraryName) { }

        protected override double ApplyMathFuncToValue(double x) => System.Math.Sin(x);
    }
}
