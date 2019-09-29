namespace NodeGraph.Node.Common.Binary
{
    public class SByteXnorNode : BinaryOperatorNode<sbyte>
    {
        public SByteXnorNode(string arbitraryName) : base("SByteXnor", arbitraryName) { }

        protected override sbyte ApplyMathFuncToValues(sbyte x, sbyte y) => (sbyte) ~(x ^ y);
    }

    public class ShortXnorNode : BinaryOperatorNode<short>
    {
        public ShortXnorNode(string arbitraryName) : base("ShortXnor", arbitraryName) { }

        protected override short ApplyMathFuncToValues(short x, short y) => (short) ~(x ^ y);
    }

    public class IntXnorNode : BinaryOperatorNode<int>
    {
        public IntXnorNode(string arbitraryName) : base("IntXnor", arbitraryName) { }

        protected override int ApplyMathFuncToValues(int x, int y) => ~(x ^ y);
    }

    public class LongXnorNode : BinaryOperatorNode<long>
    {
        public LongXnorNode(string arbitraryName) : base("LongXnor", arbitraryName) { }

        protected override long ApplyMathFuncToValues(long x, long y) => ~(x ^ y);
    }

    public class ByteXnorNode : BinaryOperatorNode<byte>
    {
        public ByteXnorNode(string arbitraryName) : base("ByteXnor", arbitraryName) { }

        protected override byte ApplyMathFuncToValues(byte x, byte y) => (byte) ~(x ^ y);
    }

    public class UShortXnorNode : BinaryOperatorNode<ushort>
    {
        public UShortXnorNode(string arbitraryName) : base("UShortXnor", arbitraryName) { }

        protected override ushort ApplyMathFuncToValues(ushort x, ushort y) => (ushort) ~(x ^ y);
    }

    public class UIntXnorNode : BinaryOperatorNode<uint>
    {
        public UIntXnorNode(string arbitraryName) : base("UIntXnor", arbitraryName) { }

        protected override uint ApplyMathFuncToValues(uint x, uint y) => ~(x ^ y);
    }

    public class ULongXnorNode : BinaryOperatorNode<ulong>
    {
        public ULongXnorNode(string arbitraryName) : base("ULongXnor", arbitraryName) { }

        protected override ulong ApplyMathFuncToValues(ulong x, ulong y) => ~(x ^ y);
    }
}
