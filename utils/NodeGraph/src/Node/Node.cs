using System;

namespace NodeGraph.Node
{
    public enum NodeValidationCode
    {
        Success,
        NullConnectionPoint,
        RecursiveConnection,
        DuplicateInput,
        NeitherSourceNorDestination,
        RequiredConnectionsNotFulfilled,
        InvalidDestination,
        InvalidSource
    }

    public interface INode
    {
        string NodeTypeName { get; }
        string ArbitraryName { get; set; }

        NodeValidationCode ValidateConnections();

        bool Connect<T>(ConnectionPointOut<T> output, string targetConnectionName);

        bool Connect<T>(ConnectionPointIn<T> input, string targetConnectionName);

        void Compute();
    }

    public abstract class AbstractNode : INode
    {
        public string NodeTypeName { get; }
        public string ArbitraryName { get; set; }
        public Guid UniqueIdentifier { get; set; }

        protected AbstractNode(string nodeTypeName, string arbitraryName)
        {
            NodeTypeName = nodeTypeName;
            ArbitraryName = arbitraryName;
        }

        public abstract NodeValidationCode ValidateConnections();

        public bool Connect<T>(ConnectionPointOut<T> output, string targetConnectionName)
        {
            if(output == null || targetConnectionName == null)
            {
                return false;
            }

            ConnectionPointIn<T> conn = FindConnectionIn<T>(targetConnectionName);

            if(conn == null)
            {
                return false;
            }

            conn.Connect(output);
            return true;
        }

        public bool Connect<T>(ConnectionPointIn<T> input, string targetConnectionName)
        {
            if(input == null || targetConnectionName == null)
            {
                return false;
            }

            ConnectionPointOut<T> conn = FindConnectionOut<T>(targetConnectionName);

            if(conn == null)
            {
                return false;
            }

            conn.Connect(input);
            return true;
        }

        protected abstract ConnectionPointOut<T> FindConnectionOut<T>(string targetConnectionName);

        protected abstract ConnectionPointIn<T> FindConnectionIn<T>(string targetConnectionName);

        public abstract void Compute();

        protected bool Equals(AbstractNode other) => other != null && Equals(this.UniqueIdentifier, other.UniqueIdentifier);

        public override bool Equals(object obj) => ReferenceEquals(this, obj) || this.Equals(obj as AbstractNode);

        public override int GetHashCode() => UniqueIdentifier.GetHashCode();
    }
}
