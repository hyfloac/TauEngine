namespace NodeGraph.Node.Common.Binary
{
    public class SByteNorNode : BinaryOperatorNode<sbyte>
    {
        public SByteNorNode(string arbitraryName) : base("SByteNor", arbitraryName) { }

        protected override sbyte ApplyMathFuncToValues(sbyte x, sbyte y) => (sbyte) ~(x | y);
    }

    public class ShortNorNode : BinaryOperatorNode<short>
    {
        public ShortNorNode(string arbitraryName) : base("ShortNor", arbitraryName) { }

        protected override short ApplyMathFuncToValues(short x, short y) => (short) ~(x | y);
    }

    public class IntNorNode : BinaryOperatorNode<int>
    {
        public IntNorNode(string arbitraryName) : base("IntNor", arbitraryName) { }

        protected override int ApplyMathFuncToValues(int x, int y) => ~(x | y);
    }

    public class LongNorNode : BinaryOperatorNode<long>
    {
        public LongNorNode(string arbitraryName) : base("LongNor", arbitraryName) { }

        protected override long ApplyMathFuncToValues(long x, long y) => ~(x | y);
    }

    public class ByteNorNode : BinaryOperatorNode<byte>
    {
        public ByteNorNode(string arbitraryName) : base("ByteNor", arbitraryName) { }

        protected override byte ApplyMathFuncToValues(byte x, byte y) => (byte) ~(x | y);
    }

    public class UShortNorNode : BinaryOperatorNode<ushort>
    {
        public UShortNorNode(string arbitraryName) : base("UShortNor", arbitraryName) { }

        protected override ushort ApplyMathFuncToValues(ushort x, ushort y) => (ushort) ~(x | y);
    }

    public class UIntNorNode : BinaryOperatorNode<uint>
    {
        public UIntNorNode(string arbitraryName) : base("UIntNor", arbitraryName) { }

        protected override uint ApplyMathFuncToValues(uint x, uint y) => ~(x | y);
    }

    public class ULongNorNode : BinaryOperatorNode<ulong>
    {
        public ULongNorNode(string arbitraryName) : base("ULongNor", arbitraryName) { }

        protected override ulong ApplyMathFuncToValues(ulong x, ulong y) => ~(x | y);
    }
}
