#pragma once

#include <typeinfo>

#include <memory>

class AbstractNode;

class AbstractConnection
{
protected:
    const char* _name;
    AbstractNode& _holder;
    const type_info& _type;
protected:
    AbstractConnection(const char* name, AbstractNode& holder, const type_info& type) noexcept
        : _name(name), _holder(holder), _type(type)
    { }
public:
    const char* name() const noexcept { return _name; }
    const char*& name() noexcept { return _name; }
    AbstractNode& node() const noexcept { return _holder; }
    //AbstractNode&& node() noexcept { return _holder; }
    const type_info& type() const noexcept { return _type; }
};

class ConnectionPointOut;

class ConnectionPointIn : public AbstractConnection
{
protected:
    bool _required;
    ConnectionPointOut* _connection;
    friend ConnectionPointOut;
protected:
    ConnectionPointIn(const char* name, AbstractNode& holder, const type_info& type, bool required, ConnectionPointOut* connection) noexcept
        : AbstractConnection(name, holder, type), _required(required), _connection(connection)
    { }

    ConnectionPointIn(const char* name, AbstractNode& holder, const type_info& type, bool required) noexcept
        : AbstractConnection(name, holder, type), _required(required), _connection(nullptr)
    { }
public:
    virtual void connect(ConnectionPointOut* connection) noexcept;

    bool required() const noexcept { return _required; }

    ConnectionPointOut* connection() const noexcept { return _connection; }
    ConnectionPointOut*& connection() noexcept { return _connection; }
};

class ConnectionPointOut : public AbstractConnection
{
protected:
    ConnectionPointIn* _connection;
    friend ConnectionPointIn;
protected:
    ConnectionPointOut(const char* name, AbstractNode& holder, const type_info& type, ConnectionPointIn* connection) noexcept
        : AbstractConnection(name, holder, type), _connection(connection)
    { }

    ConnectionPointOut(const char* name, AbstractNode& holder, const type_info& type) noexcept
        : AbstractConnection(name, holder, type), _connection(nullptr)
    { }
public:
    virtual void connect(ConnectionPointIn* connection) noexcept;

    ConnectionPointIn* connection() const noexcept { return _connection; }
    ConnectionPointIn*& connection() noexcept { return _connection; }
};

inline void ConnectionPointIn::connect(ConnectionPointOut* connection) noexcept
{
    _connection = connection;
    connection->_connection = this;
}

inline void ConnectionPointOut::connect(ConnectionPointIn* connection) noexcept
{
    _connection = connection;
    connection->_connection = this;
}

template<typename _T>
class ConnectionOutValueHolder;

template<typename _T>
class ConnectionInValueHolder final : public ConnectionPointIn
{
private:
    std::shared_ptr<std::shared_ptr<_T>> _value;
    friend ConnectionOutValueHolder<_T>;
public:
    // ConnectionInValueHolder(const char* name, AbstractNode& holder, bool required, _T* value) noexcept
    //     : ConnectionPointIn(name, holder, typeid(_T), required), _value(value)
    // { }

    ConnectionInValueHolder(const char* name, AbstractNode& holder, bool required, _T value) noexcept
        : ConnectionPointIn(name, holder, typeid(_T), required), _value(new _T*(new _T(value)))
    {}

    ConnectionInValueHolder(const char* name, AbstractNode& holder, bool required) noexcept
        : ConnectionPointIn(name, holder, typeid(_T), required), _value(new _T*(new _T(nullptr)))
    { }

    void connect(ConnectionPointOut* connection) noexcept override;

    _T* value() const noexcept { return _value; }
    _T*& value() noexcept { return _value; }
};

template<typename _T>
class ConnectionOutValueHolder final : public ConnectionPointOut
{
private:
    std::shared_ptr<std::shared_ptr<_T>> _value;
    friend ConnectionInValueHolder<_T>;
public:
    // ConnectionOutValueHolder(const char* name, AbstractNode& holder, _T* value) noexcept
    //     : ConnectionPointOut(name, holder, typeid(_T)), _value(value)
    // { }

    ConnectionOutValueHolder(const char* name, AbstractNode& holder, _T value) noexcept
        : ConnectionPointOut(name, holder, typeid(_T)), _value(new _T*(new _T(value)))
    { }

    ConnectionOutValueHolder(const char* name, AbstractNode& holder) noexcept
        : ConnectionPointOut(name, holder, typeid(_T)), _value(new _T*(nullptr))
    { }

    void connect(ConnectionPointIn* connection) noexcept override;

    _T* value() const noexcept { return _value; }
    _T*& value() noexcept { return _value; }
};

template <typename _T>
void ConnectionInValueHolder<_T>::connect(ConnectionPointOut* connection) noexcept 
{ 
    ConnectionPointIn::connect(connection);

    _value = static_cast<ConnectionOutValueHolder<_T>*>(connection)->_value;
}

template <typename _T>
void ConnectionOutValueHolder<_T>::connect(ConnectionPointIn* connection) noexcept 
{
    ConnectionPointOut::connect(connection);

    static_cast<ConnectionOutValueHolder<_T>*>(connection)->_value = _value;
}
