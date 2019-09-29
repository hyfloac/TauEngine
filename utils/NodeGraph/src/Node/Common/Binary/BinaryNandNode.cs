namespace NodeGraph.Node.Common.Binary
{
    public class SByteNandNode : BinaryOperatorNode<sbyte>
    {
        public SByteNandNode(string arbitraryName) : base("SByteNand", arbitraryName) { }

        protected override sbyte ApplyMathFuncToValues(sbyte x, sbyte y) => (sbyte) ~(x & y);
    }

    public class ShortNandNode : BinaryOperatorNode<short>
    {
        public ShortNandNode(string arbitraryName) : base("ShortNand", arbitraryName) { }

        protected override short ApplyMathFuncToValues(short x, short y) => (short) ~(x & y);
    }

    public class IntNandNode : BinaryOperatorNode<int>
    {
        public IntNandNode(string arbitraryName) : base("IntNand", arbitraryName) { }

        protected override int ApplyMathFuncToValues(int x, int y) => ~(x & y);
    }

    public class LongNandNode : BinaryOperatorNode<long>
    {
        public LongNandNode(string arbitraryName) : base("LongNand", arbitraryName) { }

        protected override long ApplyMathFuncToValues(long x, long y) => ~(x & y);
    }

    public class ByteNandNode : BinaryOperatorNode<byte>
    {
        public ByteNandNode(string arbitraryName) : base("ByteNand", arbitraryName) { }

        protected override byte ApplyMathFuncToValues(byte x, byte y) => (byte) ~(x & y);
    }

    public class UShortNandNode : BinaryOperatorNode<ushort>
    {
        public UShortNandNode(string arbitraryName) : base("UShortNand", arbitraryName) { }

        protected override ushort ApplyMathFuncToValues(ushort x, ushort y) => (ushort) ~(x & y);
    }

    public class UIntNandNode : BinaryOperatorNode<uint>
    {
        public UIntNandNode(string arbitraryName) : base("UIntNand", arbitraryName) { }

        protected override uint ApplyMathFuncToValues(uint x, uint y) => ~(x & y);
    }

    public class ULongNandNode : BinaryOperatorNode<ulong>
    {
        public ULongNandNode(string arbitraryName) : base("ULongNand", arbitraryName) { }

        protected override ulong ApplyMathFuncToValues(ulong x, ulong y) => ~(x & y);
    }
}
