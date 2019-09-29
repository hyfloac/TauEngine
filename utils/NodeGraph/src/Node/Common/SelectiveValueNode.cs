namespace NodeGraph.Node.Common
{
    public class SelectiveValue<T> : AbstractNode
    {
        public ConnectionPointIn<bool> Control { get; private set; }
        public ConnectionPointIn<T> InX { get; private set; }
        public ConnectionPointIn<T> InY { get; private set; }

        public ConnectionPointOut<T> OutValue { get; private set; }

        public SelectiveValue(string arbitraryName)
            : base("SelectiveValue", arbitraryName)
        {
            Control = new ConnectionPointIn<bool>("Control", this, true, null);

            InX = new ConnectionPointIn<T>("X", this, true, null);
            InY = new ConnectionPointIn<T>("Y", this, true, null);

            OutValue = new ConnectionPointOut<T>("Value", this, null);
        }

        public override NodeValidationCode ValidateConnections()
        {
            if(Control.Connection == null || InX.Connection == null || InY.Connection == null)
            {
                return NodeValidationCode.RequiredConnectionsNotFulfilled;
            }

            if(ReferenceEquals(OutValue.Connection.HolderNode, this))
            {
                return NodeValidationCode.RecursiveConnection;
            }

            return NodeValidationCode.Success;
        }

        protected override ConnectionPointOut<TT> FindConnectionOut<TT>(string targetConnectionName)
        {
            if(typeof(TT) != typeof(T))
            {
                return null;
            }

            switch(targetConnectionName)
            {
                case "Value": return (ConnectionPointOut<TT>) (object) OutValue;
                default: return null;
            }
        }

        protected override ConnectionPointIn<TT> FindConnectionIn<TT>(string targetConnectionName)
        {
            if(typeof(TT) == typeof(bool) && targetConnectionName == "Control")
            {
                return (ConnectionPointIn<TT>) (object) Control;
            }

            if(typeof(TT) == typeof(T))
            {
                switch(targetConnectionName)
                {
                    case "X": return (ConnectionPointIn<TT>) (object) InX;
                    case "Y": return (ConnectionPointIn<TT>) (object) InY;
                }
            }

            return null;
        }

        public override void Compute()
        {
            if(Control.Connection == null || InX.Connection == null || InY.Connection == null)
            {
                return;
            }

            Control.Connection.HolderNode.Compute();
            InX.Connection.HolderNode.Compute();
            InY.Connection.HolderNode.Compute();

            OutValue.Value = Control.Value ? InX.Value : InY.Value;
            if(OutValue.Connection != null)
            {
                OutValue.Connection.Value = OutValue.Value;
            }
        }
    }
}
