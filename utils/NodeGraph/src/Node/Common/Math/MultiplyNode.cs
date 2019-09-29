namespace NodeGraph.Node.Common.Math
{
    public class SByteMultiplyNode : BinaryOperatorNode<sbyte>
    {
        public SByteMultiplyNode(string arbitraryName) : base("SByteMultiply", arbitraryName) { }

        protected override sbyte ApplyMathFuncToValues(sbyte x, sbyte y) => (sbyte) (x * y);
    }

    public class ShortMultiplyNode : BinaryOperatorNode<short>
    {
        public ShortMultiplyNode(string arbitraryName) : base("ShortMultiply", arbitraryName) { }

        protected override short ApplyMathFuncToValues(short x, short y) => (short) (x * y);
    }

    public class IntMultiplyNode : BinaryOperatorNode<int>
    {
        public IntMultiplyNode(string arbitraryName) : base("IntMultiply", arbitraryName) { }

        protected override int ApplyMathFuncToValues(int x, int y) => x * y;
    }

    public class LongMultiplyNode : BinaryOperatorNode<long>
    {
        public LongMultiplyNode(string arbitraryName) : base("LongMultiply", arbitraryName) { }

        protected override long ApplyMathFuncToValues(long x, long y) => x * y;
    }

    public class ByteMultiplyNode : BinaryOperatorNode<byte>
    {
        public ByteMultiplyNode(string arbitraryName) : base("ByteMultiply", arbitraryName) { }

        protected override byte ApplyMathFuncToValues(byte x, byte y) => (byte) (x * y);
    }

    public class UShortMultiplyNode : BinaryOperatorNode<ushort>
    {
        public UShortMultiplyNode(string arbitraryName) : base("UShortMultiply", arbitraryName) { }

        protected override ushort ApplyMathFuncToValues(ushort x, ushort y) => (ushort) (x * y);
    }

    public class UIntMultiplyNode : BinaryOperatorNode<uint>
    {
        public UIntMultiplyNode(string arbitraryName) : base("UIntMultiply", arbitraryName) { }

        protected override uint ApplyMathFuncToValues(uint x, uint y) => x * y;
    }

    public class ULongMultiplyNode : BinaryOperatorNode<ulong>
    {
        public ULongMultiplyNode(string arbitraryName) : base("ULongMultiply", arbitraryName) { }

        protected override ulong ApplyMathFuncToValues(ulong x, ulong y) => x * y;
    }

    public class FloatMultiplyNode : BinaryOperatorNode<float>
    {
        public FloatMultiplyNode(string arbitraryName) : base("FloatMultiply", arbitraryName) { }

        protected override float ApplyMathFuncToValues(float x, float y) => x * y;
    }

    public class DoubleMultiplyNode : BinaryOperatorNode<double>
    {
        public DoubleMultiplyNode(string arbitraryName) : base("DoubleMultiply", arbitraryName) { }

        protected override double ApplyMathFuncToValues(double x, double y) => x * y;
    }
}
