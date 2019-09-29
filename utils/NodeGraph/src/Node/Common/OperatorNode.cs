namespace NodeGraph.Node.Common
{
    public abstract class UnaryOperatorNode<Input, Output> : AbstractNode
    {
        public ConnectionPointIn<Input> In { get; private set; }
        public ConnectionPointOut<Output> OutResult { get; private set; }

        protected UnaryOperatorNode(string nodeTypeName, string arbitraryName)
            : base(nodeTypeName, arbitraryName)
        {
            In = new ConnectionPointIn<Input>("In", this, true, null);
            OutResult = new ConnectionPointOut<Output>("Result", this, null);
        }

        public override NodeValidationCode ValidateConnections()
        {
            if(In.Connection == null)
            {
                return NodeValidationCode.RequiredConnectionsNotFulfilled;
            }

            if(ReferenceEquals(OutResult.Connection.HolderNode, this))
            {
                return NodeValidationCode.RecursiveConnection;
            }

            return NodeValidationCode.Success;
        }

        protected override ConnectionPointOut<TT> FindConnectionOut<TT>(string targetConnectionName)
        {
            if(typeof(TT) != typeof(Input))
            {
                return null;
            }

            switch(targetConnectionName)
            {
                case "Result": return (ConnectionPointOut<TT>) (object) OutResult;
                default: return null;
            }
        }

        protected override ConnectionPointIn<TT> FindConnectionIn<TT>(string targetConnectionName)
        {
            if(typeof(TT) != typeof(Input))
            {
                return null;
            }

            switch(targetConnectionName)
            {
                case "In": return (ConnectionPointIn<TT>) (object) In;
                default: return null;
            }
        }

        public override void Compute()
        {
            if(In.Connection == null) { return; }

            In.Connection.HolderNode.Compute();

            OutResult.Value = ApplyMathFuncToValue(In.Value);
            if(OutResult.Connection != null)
            {
                OutResult.Connection.Value = OutResult.Value;
            }
        }

        protected abstract Output ApplyMathFuncToValue(Input x);
    }

    public abstract class UnaryOperatorNode<T> : UnaryOperatorNode<T, T>
    {
        protected UnaryOperatorNode(string nodeTypeName, string arbitraryName) : base(nodeTypeName, arbitraryName) { }
    }

    public abstract class BinaryOperatorNode<InputX, InputY, Output> : AbstractNode
    {
        public ConnectionPointIn<InputX> InX { get; private set; }
        public ConnectionPointIn<InputY> InY { get; private set; }

        public ConnectionPointOut<Output> OutResult { get; private set; }

        protected BinaryOperatorNode(string nodeTypeName, string arbitraryName)
            : base(nodeTypeName, arbitraryName)
        {
            InX = new ConnectionPointIn<InputX>("X", this, true, null);
            InY = new ConnectionPointIn<InputY>("Y", this, true, null);

            OutResult = new ConnectionPointOut<Output>("Result", this, null);
        }

        public override NodeValidationCode ValidateConnections()
        {
            if(InX.Connection == null || InY.Connection == null)
            {
                return NodeValidationCode.RequiredConnectionsNotFulfilled;
            }

            if(ReferenceEquals(OutResult.Connection.HolderNode, this))
            {
                return NodeValidationCode.RecursiveConnection;
            }

            return NodeValidationCode.Success;
        }

        protected override ConnectionPointOut<T> FindConnectionOut<T>(string targetConnectionName)
        {
            if(typeof(T) != typeof(Output))
            {
                return null;
            }

            switch(targetConnectionName)
            {
                case "Result": return (ConnectionPointOut<T>) (object) OutResult;
                default: return null;
            }
        }

        protected override ConnectionPointIn<T> FindConnectionIn<T>(string targetConnectionName)
        {
            if(typeof(T) == typeof(InputX) && targetConnectionName == "X")
            {
                return (ConnectionPointIn<T>) (object) InX;
            }

            if(typeof(T) == typeof(InputY) && targetConnectionName == "Y")
            {
                return (ConnectionPointIn<T>) (object) InY;
            }

            return null;
        }

        public override void Compute()
        {
            if(InX.Connection == null || InY.Connection == null)
            {
                return;
            }

            InX.Connection.HolderNode.Compute();
            InY.Connection.HolderNode.Compute();

            OutResult.Value = ApplyMathFuncToValues(InX.Value, InY.Value);
            if(OutResult.Connection != null)
            {
                OutResult.Connection.Value = OutResult.Value;
            }
        }

        protected abstract Output ApplyMathFuncToValues(InputX x, InputY y);
    }

    public abstract class BinaryOperatorNode<Input, Output> : BinaryOperatorNode<Input, Input, Output>
    {
        protected BinaryOperatorNode(string nodeTypeName, string arbitraryName) : base(nodeTypeName, arbitraryName) { }
    }

    public abstract class BinaryOperatorNode<T> : BinaryOperatorNode<T, T>
    {
        protected BinaryOperatorNode(string nodeTypeName, string arbitraryName) : base(nodeTypeName, arbitraryName) { }
    }

    public abstract class TrinaryOperatorNode<InputX, InputY, InputZ, Output> : AbstractNode
    {
        public ConnectionPointIn<InputX> InX { get; private set; }
        public ConnectionPointIn<InputY> InY { get; private set; }
        public ConnectionPointIn<InputZ> InZ { get; private set; }

        public ConnectionPointOut<Output> OutResult { get; private set; }

        protected TrinaryOperatorNode(string nodeTypeName, string arbitraryName)
            : base(nodeTypeName, arbitraryName)
        {
            InX = new ConnectionPointIn<InputX>("X", this, true, null);
            InY = new ConnectionPointIn<InputY>("Y", this, true, null);
            InZ = new ConnectionPointIn<InputZ>("Z", this, true, null);

            OutResult = new ConnectionPointOut<Output>("Result", this, null);
        }

        public override NodeValidationCode ValidateConnections()
        {
            if(InX.Connection == null || InY.Connection == null || InZ.Connection == null)
            {
                return NodeValidationCode.RequiredConnectionsNotFulfilled;
            }

            if(ReferenceEquals(OutResult.Connection.HolderNode, this))
            {
                return NodeValidationCode.RecursiveConnection;
            }

            return NodeValidationCode.Success;
        }

        protected override ConnectionPointOut<T> FindConnectionOut<T>(string targetConnectionName)
        {
            if(typeof(T) != typeof(Output))
            {
                return null;
            }

            switch(targetConnectionName)
            {
                case "Result": return (ConnectionPointOut<T>) (object) OutResult;
                default: return null;
            }
        }

        protected override ConnectionPointIn<T> FindConnectionIn<T>(string targetConnectionName)
        {
            if(typeof(T) == typeof(InputX) && targetConnectionName == "X")
            {
                return (ConnectionPointIn<T>) (object) InX;
            }

            if(typeof(T) == typeof(InputY) && targetConnectionName == "Y")
            {
                return (ConnectionPointIn<T>) (object) InY;
            }

            if(typeof(T) == typeof(InputZ) && targetConnectionName == "Z")
            {
                return (ConnectionPointIn<T>) (object) InZ;
            }

            return null;
        }

        public override void Compute()
        {
            if(InX.Connection == null || InY.Connection == null)
            {
                return;
            }

            InX.Connection.HolderNode.Compute();
            InY.Connection.HolderNode.Compute();

            OutResult.Value = ApplyMathFuncToValues(InX.Value, InY.Value, InZ.Value);
            if(OutResult.Connection != null)
            {
                OutResult.Connection.Value = OutResult.Value;
            }
        }

        protected abstract Output ApplyMathFuncToValues(InputX x, InputY y, InputZ z);
    }

    public abstract class TrinaryOperatorNode<InputX, InputY, Output> : TrinaryOperatorNode<InputX, InputY, InputY, Output>
    {
        protected TrinaryOperatorNode(string nodeTypeName, string arbitraryName) : base(nodeTypeName, arbitraryName) { }
    }

    public abstract class TrinaryOperatorNode<Input, Output> : TrinaryOperatorNode<Input, Input, Input, Output>
    {
        protected TrinaryOperatorNode(string nodeTypeName, string arbitraryName) : base(nodeTypeName, arbitraryName) { }
    }

    public abstract class TrinaryOperatorNode<T> : TrinaryOperatorNode<T, T, T, T>
    {
        protected TrinaryOperatorNode(string nodeTypeName, string arbitraryName) : base(nodeTypeName, arbitraryName) { }
    }
}
