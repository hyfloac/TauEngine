using System;

namespace NodeGraph.Node.Common
{
    public class TypeCastNode<OldType, NewType> : UnaryOperatorNode<OldType, NewType> 
        where OldType : struct 
        where NewType : struct
    {
        public TypeCastNode(string arbitraryName) : base("TypeCastNode", arbitraryName) { }

        protected override NewType ApplyMathFuncToValue(OldType x) => (NewType) Convert.ChangeType(x, typeof(NewType));
    }
}
