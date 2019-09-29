namespace NodeGraph.Node.Common
{
    public class ConstantNode<T> : AbstractNode
    {
        public ConnectionPointOut<T> OutValue { get; private set; }

        public ConstantNode(string arbitraryName, T value)
            : base("Constant", arbitraryName)
        {
            OutValue = new ConnectionPointOut<T>("Value", this, value, null);
        }

        public override NodeValidationCode ValidateConnections() => NodeValidationCode.Success;

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

        protected override ConnectionPointIn<TT> FindConnectionIn<TT>(string targetConnectionName) => null;

        public override void Compute()
        {
            if(OutValue.Connection != null)
            {
                OutValue.Connection.Value = OutValue.Value;
            }
        }
    }
}
