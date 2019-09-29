namespace NodeGraph.Node.Common.Binary
{
    public class SByteOrNode : BinaryOperatorNode<sbyte>
    {
        public SByteOrNode(string arbitraryName) : base("SByteOr", arbitraryName) { }

        protected override sbyte ApplyMathFuncToValues(sbyte x, sbyte y) => (sbyte) (x | y);
    }

    public class ShortOrNode : BinaryOperatorNode<short>
    {
        public ShortOrNode(string arbitraryName) : base("ShortOr", arbitraryName) { }

        protected override short ApplyMathFuncToValues(short x, short y) => (short) (x | y);
    }

    public class IntOrNode : BinaryOperatorNode<int>
    {
        public IntOrNode(string arbitraryName) : base("IntOr", arbitraryName) { }

        protected override int ApplyMathFuncToValues(int x, int y) => x | y;
    }

    public class LongOrNode : BinaryOperatorNode<long>
    {
        public LongOrNode(string arbitraryName) : base("LongOr", arbitraryName) { }

        protected override long ApplyMathFuncToValues(long x, long y) => x | y;
    }

    public class ByteOrNode : BinaryOperatorNode<byte>
    {
        public ByteOrNode(string arbitraryName) : base("ByteOr", arbitraryName) { }

        protected override byte ApplyMathFuncToValues(byte x, byte y) => (byte) (x | y);
    }

    public class UShortOrNode : BinaryOperatorNode<ushort>
    {
        public UShortOrNode(string arbitraryName) : base("UShortOr", arbitraryName) { }

        protected override ushort ApplyMathFuncToValues(ushort x, ushort y) => (ushort) (x | y);
    }

    public class UIntOrNode : BinaryOperatorNode<uint>
    {
        public UIntOrNode(string arbitraryName) : base("UIntOr", arbitraryName) { }

        protected override uint ApplyMathFuncToValues(uint x, uint y) => x | y;
    }

    public class ULongOrNode : BinaryOperatorNode<ulong>
    {
        public ULongOrNode(string arbitraryName) : base("ULongOr", arbitraryName) { }

        protected override ulong ApplyMathFuncToValues(ulong x, ulong y) => x | y;
    }
}
