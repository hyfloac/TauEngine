namespace NodeGraph.Node.Common.Math
{
    public class SByteSubtractNode : BinaryOperatorNode<sbyte>
    {
        public SByteSubtractNode(string arbitraryName) : base("SByteSubtract", arbitraryName) { }

        protected override sbyte ApplyMathFuncToValues(sbyte x, sbyte y) => (sbyte) (x - y);
    }

    public class ShortSubtractNode : BinaryOperatorNode<short>
    {
        public ShortSubtractNode(string arbitraryName) : base("ShortSubtract", arbitraryName) { }

        protected override short ApplyMathFuncToValues(short x, short y) => (short) (x - y);
    }

    public class IntSubtractNode : BinaryOperatorNode<int>
    {
        public IntSubtractNode(string arbitraryName) : base("IntSubtract", arbitraryName) { }

        protected override int ApplyMathFuncToValues(int x, int y) => x - y;
    }

    public class LongSubtractNode : BinaryOperatorNode<long>
    {
        public LongSubtractNode(string arbitraryName) : base("LongSubtract", arbitraryName) { }

        protected override long ApplyMathFuncToValues(long x, long y) => x - y;
    }

    public class ByteSubtractNode : BinaryOperatorNode<byte>
    {
        public ByteSubtractNode(string arbitraryName) : base("ByteSubtract", arbitraryName) { }

        protected override byte ApplyMathFuncToValues(byte x, byte y) => (byte) (x - y);
    }

    public class UShortSubtractNode : BinaryOperatorNode<ushort>
    {
        public UShortSubtractNode(string arbitraryName) : base("UShortSubtract", arbitraryName) { }

        protected override ushort ApplyMathFuncToValues(ushort x, ushort y) => (ushort) (x - y);
    }

    public class UIntSubtractNode : BinaryOperatorNode<uint>
    {
        public UIntSubtractNode(string arbitraryName) : base("UIntSubtract", arbitraryName) { }

        protected override uint ApplyMathFuncToValues(uint x, uint y) => x - y;
    }

    public class ULongSubtractNode : BinaryOperatorNode<ulong>
    {
        public ULongSubtractNode(string arbitraryName) : base("ULongSubtract", arbitraryName) { }

        protected override ulong ApplyMathFuncToValues(ulong x, ulong y) => x - y;
    }

    public class FloatSubtractNode : BinaryOperatorNode<float>
    {
        public FloatSubtractNode(string arbitraryName) : base("FloatSubtract", arbitraryName) { }

        protected override float ApplyMathFuncToValues(float x, float y) => x - y;
    }

    public class DoubleSubtractNode : BinaryOperatorNode<double>
    {
        public DoubleSubtractNode(string arbitraryName) : base("DoubleSubtract", arbitraryName) { }

        protected override double ApplyMathFuncToValues(double x, double y) => x - y;
    }
}
