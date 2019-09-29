namespace NodeGraph.Node.Common.Math
{
    public class SByteDivideNode : BinaryOperatorNode<sbyte>
    {
        public SByteDivideNode(string arbitraryName) : base("SByteDivide", arbitraryName) { }

        protected override sbyte ApplyMathFuncToValues(sbyte x, sbyte y) => (sbyte) (x / y);
    }

    public class ShortDivideNode : BinaryOperatorNode<short>
    {
        public ShortDivideNode(string arbitraryName) : base("ShortDivide", arbitraryName) { }

        protected override short ApplyMathFuncToValues(short x, short y) => (short) (x / y);
    }

    public class IntDivideNode : BinaryOperatorNode<int>
    {
        public IntDivideNode(string arbitraryName) : base("IntDivide", arbitraryName) { }

        protected override int ApplyMathFuncToValues(int x, int y) => x / y;
    }

    public class LongDivideNode : BinaryOperatorNode<long>
    {
        public LongDivideNode(string arbitraryName) : base("LongDivide", arbitraryName) { }

        protected override long ApplyMathFuncToValues(long x, long y) => x / y;
    }

    public class ByteDivideNode : BinaryOperatorNode<byte>
    {
        public ByteDivideNode(string arbitraryName) : base("ByteDivide", arbitraryName) { }

        protected override byte ApplyMathFuncToValues(byte x, byte y) => (byte) (x / y);
    }

    public class UShortDivideNode : BinaryOperatorNode<ushort>
    {
        public UShortDivideNode(string arbitraryName) : base("UShortDivide", arbitraryName) { }

        protected override ushort ApplyMathFuncToValues(ushort x, ushort y) => (ushort) (x / y);
    }

    public class UIntDivideNode : BinaryOperatorNode<uint>
    {
        public UIntDivideNode(string arbitraryName) : base("UIntDivide", arbitraryName) { }

        protected override uint ApplyMathFuncToValues(uint x, uint y) => x / y;
    }

    public class ULongDivideNode : BinaryOperatorNode<ulong>
    {
        public ULongDivideNode(string arbitraryName) : base("ULongDivide", arbitraryName) { }

        protected override ulong ApplyMathFuncToValues(ulong x, ulong y) => x / y;
    }

    public class FloatDivideNode : BinaryOperatorNode<float>
    {
        public FloatDivideNode(string arbitraryName) : base("FloatDivide", arbitraryName) { }

        protected override float ApplyMathFuncToValues(float x, float y) => x / y;
    }

    public class DoubleDivideNode : BinaryOperatorNode<double>
    {
        public DoubleDivideNode(string arbitraryName) : base("DoubleDivide", arbitraryName) { }

        protected override double ApplyMathFuncToValues(double x, double y) => x / y;
    }
}
