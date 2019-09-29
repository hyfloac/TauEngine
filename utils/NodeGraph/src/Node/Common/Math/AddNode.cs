namespace NodeGraph.Node.Common.Math
{
    public class SByteAddNode : BinaryOperatorNode<sbyte>
    {
        public SByteAddNode(string arbitraryName) : base("SByteAdd", arbitraryName) { }

        protected override sbyte ApplyMathFuncToValues(sbyte x, sbyte y) => (sbyte) (x + y);
    }

    public class ShortAddNode : BinaryOperatorNode<short>
    {
        public ShortAddNode(string arbitraryName) : base("ShortAdd", arbitraryName) { }

        protected override short ApplyMathFuncToValues(short x, short y) => (short) (x + y);
    }

    public class IntAddNode : BinaryOperatorNode<int>
    {
        public IntAddNode(string arbitraryName) : base("IntAdd", arbitraryName) { }

        protected override int ApplyMathFuncToValues(int x, int y) => x + y;
    }

    public class LongAddNode : BinaryOperatorNode<long>
    {
        public LongAddNode(string arbitraryName) : base("LongAdd", arbitraryName) { }

        protected override long ApplyMathFuncToValues(long x, long y) => x + y;
    }

    public class ByteAddNode : BinaryOperatorNode<byte>
    {
        public ByteAddNode(string arbitraryName) : base("ByteAdd", arbitraryName) { }

        protected override byte ApplyMathFuncToValues(byte x, byte y) => (byte) (x + y);
    }

    public class UShortAddNode : BinaryOperatorNode<ushort>
    {
        public UShortAddNode(string arbitraryName) : base("UShortAdd", arbitraryName) { }

        protected override ushort ApplyMathFuncToValues(ushort x, ushort y) => (ushort) (x + y);
    }

    public class UIntAddNode : BinaryOperatorNode<uint>
    {
        public UIntAddNode(string arbitraryName) : base("UIntAdd", arbitraryName) { }

        protected override uint ApplyMathFuncToValues(uint x, uint y) => x + y;
    }

    public class ULongAddNode : BinaryOperatorNode<ulong>
    {
        public ULongAddNode(string arbitraryName) : base("ULongAdd", arbitraryName) { }

        protected override ulong ApplyMathFuncToValues(ulong x, ulong y) => x + y;
    }

    public class FloatAddNode : BinaryOperatorNode<float>
    {
        public FloatAddNode(string arbitraryName) : base("FloatAdd", arbitraryName) { }

        protected override float ApplyMathFuncToValues(float x, float y) => x + y;
    }

    public class DoubleAddNode : BinaryOperatorNode<double>
    {
        public DoubleAddNode(string arbitraryName) : base("DoubleAdd", arbitraryName) { }

        protected override double ApplyMathFuncToValues(double x, double y) => x + y;
    }
}
