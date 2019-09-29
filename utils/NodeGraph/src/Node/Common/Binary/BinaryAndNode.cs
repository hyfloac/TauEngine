namespace NodeGraph.Node.Common.Binary
{
    public class SByteAndNode : BinaryOperatorNode<sbyte>
    {
        public SByteAndNode(string arbitraryName) : base("SByteAnd", arbitraryName) { }

        protected override sbyte ApplyMathFuncToValues(sbyte x, sbyte y) => (sbyte) (x & y);
    }

    public class ShortAndNode : BinaryOperatorNode<short>
    {
        public ShortAndNode(string arbitraryName) : base("ShortAnd", arbitraryName) { }

        protected override short ApplyMathFuncToValues(short x, short y) => (short) (x & y);
    }

    public class IntAndNode : BinaryOperatorNode<int>
    {
        public IntAndNode(string arbitraryName) : base("IntAnd", arbitraryName) { }

        protected override int ApplyMathFuncToValues(int x, int y) => x & y;
    }

    public class LongAndNode : BinaryOperatorNode<long>
    {
        public LongAndNode(string arbitraryName) : base("LongAnd", arbitraryName) { }

        protected override long ApplyMathFuncToValues(long x, long y) => x & y;
    }

    public class ByteAndNode : BinaryOperatorNode<byte>
    {
        public ByteAndNode(string arbitraryName) : base("ByteAnd", arbitraryName) { }

        protected override byte ApplyMathFuncToValues(byte x, byte y) => (byte) (x & y);
    }

    public class UShortAndNode : BinaryOperatorNode<ushort>
    {
        public UShortAndNode(string arbitraryName) : base("UShortAnd", arbitraryName) { }

        protected override ushort ApplyMathFuncToValues(ushort x, ushort y) => (ushort) (x & y);
    }

    public class UIntAndNode : BinaryOperatorNode<uint>
    {
        public UIntAndNode(string arbitraryName) : base("UIntAnd", arbitraryName) { }

        protected override uint ApplyMathFuncToValues(uint x, uint y) => x & y;
    }

    public class ULongAndNode : BinaryOperatorNode<ulong>
    {
        public ULongAndNode(string arbitraryName) : base("ULongAnd", arbitraryName) { }

        protected override ulong ApplyMathFuncToValues(ulong x, ulong y) => x & y;
    }
}
