namespace NodeGraph.Node.Common.Binary
{
    public class SByteXorNode : BinaryOperatorNode<sbyte>
    {
        public SByteXorNode(string arbitraryName) : base("SByteXor", arbitraryName) { }

        protected override sbyte ApplyMathFuncToValues(sbyte x, sbyte y) => (sbyte) (x ^ y);
    }

    public class ShortXorNode : BinaryOperatorNode<short>
    {
        public ShortXorNode(string arbitraryName) : base("ShortXor", arbitraryName) { }

        protected override short ApplyMathFuncToValues(short x, short y) => (short) (x ^ y);
    }

    public class IntXorNode : BinaryOperatorNode<int>
    {
        public IntXorNode(string arbitraryName) : base("IntXor", arbitraryName) { }

        protected override int ApplyMathFuncToValues(int x, int y) => x ^ y;
    }

    public class LongXorNode : BinaryOperatorNode<long>
    {
        public LongXorNode(string arbitraryName) : base("LongXor", arbitraryName) { }

        protected override long ApplyMathFuncToValues(long x, long y) => x ^ y;
    }

    public class ByteXorNode : BinaryOperatorNode<byte>
    {
        public ByteXorNode(string arbitraryName) : base("ByteXor", arbitraryName) { }

        protected override byte ApplyMathFuncToValues(byte x, byte y) => (byte) (x ^ y);
    }

    public class UShortXorNode : BinaryOperatorNode<ushort>
    {
        public UShortXorNode(string arbitraryName) : base("UShortXor", arbitraryName) { }

        protected override ushort ApplyMathFuncToValues(ushort x, ushort y) => (ushort) (x ^ y);
    }

    public class UIntXorNode : BinaryOperatorNode<uint>
    {
        public UIntXorNode(string arbitraryName) : base("UIntXor", arbitraryName) { }

        protected override uint ApplyMathFuncToValues(uint x, uint y) => x ^ y;
    }

    public class ULongXorNode : BinaryOperatorNode<ulong>
    {
        public ULongXorNode(string arbitraryName) : base("ULongXor", arbitraryName) { }

        protected override ulong ApplyMathFuncToValues(ulong x, ulong y) => x ^ y;
    }
}
