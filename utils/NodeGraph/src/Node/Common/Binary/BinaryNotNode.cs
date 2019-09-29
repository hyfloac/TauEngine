namespace NodeGraph.Node.Common.Binary
{
    public class SByteNotNode : UnaryOperatorNode<sbyte, sbyte>
    {
        public SByteNotNode(string arbitraryName) : base("SByteNot", arbitraryName) { }

        protected override sbyte ApplyMathFuncToValue(sbyte x) => (sbyte) ~x;
    }

    public class ShortNotNode : UnaryOperatorNode<short, short>
    {
        public ShortNotNode(string arbitraryName) : base("ShortNot", arbitraryName) { }

        protected override short ApplyMathFuncToValue(short x) => (short) ~x;
    }

    public class IntNotNode : UnaryOperatorNode<int, int>
    {
        public IntNotNode(string arbitraryName) : base("IntNot", arbitraryName) { }

        protected override int ApplyMathFuncToValue(int x) => ~x;
    }

    public class LongNotNode : UnaryOperatorNode<long, long>
    {
        public LongNotNode(string arbitraryName) : base("LongNot", arbitraryName) { }

        protected override long ApplyMathFuncToValue(long x) => ~x;
    }

    public class ByteNotNode : UnaryOperatorNode<byte, byte>
    {
        public ByteNotNode(string arbitraryName) : base("ByteNot", arbitraryName) { }

        protected override byte ApplyMathFuncToValue(byte x) => (byte) ~x;
    }

    public class UShortNotNode : UnaryOperatorNode<ushort, ushort>
    {
        public UShortNotNode(string arbitraryName) : base("UShortNot", arbitraryName) { }

        protected override ushort ApplyMathFuncToValue(ushort x) => (ushort) ~x;
    }

    public class UIntNotNode : UnaryOperatorNode<uint, uint>
    {
        public UIntNotNode(string arbitraryName) : base("UIntNot", arbitraryName) { }

        protected override uint ApplyMathFuncToValue(uint x) => ~x;
    }

    public class ULongNotNode : UnaryOperatorNode<ulong, ulong>
    {
        public ULongNotNode(string arbitraryName) : base("ULongNot", arbitraryName) { }

        protected override ulong ApplyMathFuncToValue(ulong x) => ~x;
    }
}
