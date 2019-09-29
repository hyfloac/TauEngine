namespace NodeGraph.Node
{
    public abstract class AbstractConnection<T>
    {
        public string Name { get; set; }
        public INode HolderNode { get; private set; }
        public T Value { get; set; }

        protected AbstractConnection(string name, INode holder, T value)
        {
            Name = name;
            HolderNode = holder;
            Value = value;
        }

        protected AbstractConnection(string name, INode holder)
        {
            Name = name;
            HolderNode = holder;
        }

        protected bool Equals(AbstractConnection<T> other) => other != null && this.Name == other.Name && Equals(this.HolderNode, other.HolderNode);

        public override bool Equals(object obj) => ReferenceEquals(this, obj) || this.Equals(obj as AbstractConnection<T>);

        public override int GetHashCode()
        {
            unchecked
            {
                int hashCode = Name.GetHashCode();
                hashCode = (hashCode * 397) ^ HolderNode.GetHashCode();
                return hashCode;
            }
        }
    }

    public class ConnectionPointIn<T> : AbstractConnection<T>
    {
        public bool Required { get; set; }
        public ConnectionPointOut<T> Connection { get; set; }

        public ConnectionPointIn(string name, INode holder, T value, bool required, ConnectionPointOut<T> connection)
            : base(name, holder, value)
        {
            Required = required;
            Connection = connection;
        }

        public ConnectionPointIn(string name, INode holder, bool required, ConnectionPointOut<T> connection)
            : base(name, holder)
        {
            Required = required;
            Connection = connection;
        }

        public ConnectionPointIn(string name, INode holder, bool required)
            : base(name, holder)
        {
            Required = required;
            Connection = null;
        }

        public void Connect(ConnectionPointOut<T> conn)
        {
            Connection = conn;
            conn.Connection = this;
        }
    }

    public class ConnectionPointOut<T> : AbstractConnection<T>
    {
        public ConnectionPointIn<T> Connection { get; set; }

        public ConnectionPointOut(string name, INode holder, T value, ConnectionPointIn<T> connection)
            : base(name, holder, value)
        {
            Connection = connection;
        }

        public ConnectionPointOut(string name, INode holder, ConnectionPointIn<T> connection)
            : base(name, holder)
        {
            Connection = connection;
        }

        public ConnectionPointOut(string name, INode holder)
            : base(name, holder)
        {
            Connection = null;
        }

        public void Connect(ConnectionPointIn<T> conn)
        {
            Connection = conn;
            conn.Connection = this;
        }
    }
}
