#pragma once

#include "Connection.hpp"
#include <cstring>

enum class NodeValidationCode
{
    Success,
    NullConnectionPoint,
    RecursiveConnection,
    DuplicateInput,
    NeitherSourceNorDestination,
    RequiredConnectionsNotFulfilled,
    InvalidDestination,
    InvalidSource
};

class AbstractNode
{
protected:
    const char* _nodeTypeName;
    const char* _arbitraryName;
protected:
    AbstractNode(const char* nodeTypeName, const char* arbitraryName) noexcept
        : _nodeTypeName(nodeTypeName), _arbitraryName(arbitraryName)
    { }
public:
    virtual ~AbstractNode() noexcept = default;

    AbstractNode(const AbstractNode& copy) noexcept = delete;
    AbstractNode(AbstractNode&& move) noexcept = delete;

    AbstractNode& operator =(const AbstractNode& copy) noexcept = delete;
    AbstractNode& operator =(AbstractNode&& move) noexcept = delete;

    bool connect(ConnectionPointOut* output, const type_info& type, const char* targetConnectionName) noexcept
    {
        if(output == nullptr || targetConnectionName == nullptr)
        {
            return false;
        }

        ConnectionPointIn* conn = findConnectionIn(type, targetConnectionName);

        if(conn == nullptr)
        {
            return false;
        }

        conn->connect(output);
        return true;
    }

    bool connect(ConnectionPointIn* input, const type_info& type, const char* targetConnectionName) noexcept
    {
        if(input == nullptr || targetConnectionName == nullptr)
        {
            return false;
        }

        ConnectionPointOut* conn = findConnectionOut(type, targetConnectionName);

        if(conn == nullptr)
        {
            return false;
        }

        conn->connect(input);
        return true;
    }

    virtual NodeValidationCode validateConnections() const noexcept = 0;

    virtual ConnectionPointIn* findConnectionIn(const type_info& type, const char* targetConnectionName) noexcept = 0;

    virtual ConnectionPointOut* findConnectionOut(const type_info& type, const char* targetConnectionName) noexcept = 0;

    virtual void compute() noexcept = 0;
};

template<typename _InputX, typename _InputY, typename _Output>
class BinaryOperatorNode : public AbstractNode
{
protected:
    ConnectionInValueHolder<_InputX> _inX;
    ConnectionInValueHolder<_InputY> _inY;

    ConnectionOutValueHolder<_Output> _outResult;

    using func_f = _Output(*)(_InputX x, _InputY);

    func_f _func;
public:
    BinaryOperatorNode(const char* nodeTypeName, const char* arbitraryName, func_f func)
        : AbstractNode(nodeTypeName, arbitraryName),
        _inX("X", *this, true),
        _inY("Y", *this, true),
        _outResult("Result", *this),
        _func(func)
    { }

    virtual ~BinaryOperatorNode() noexcept = default;

    BinaryOperatorNode(const BinaryOperatorNode& copy) noexcept = delete;
    BinaryOperatorNode(BinaryOperatorNode&& move) noexcept = delete;

    BinaryOperatorNode& operator =(const BinaryOperatorNode& copy) noexcept = delete;
    BinaryOperatorNode& operator =(BinaryOperatorNode&& move) noexcept = delete;

    NodeValidationCode validateConnections() const noexcept override
    {
        if(_inX.connection() == nullptr || _inY.connection() == nullptr)
        {
            return NodeValidationCode::RequiredConnectionsNotFulfilled;
        }

        if(this == &(_outResult.node()))
        {
            return NodeValidationCode::RecursiveConnection;
        }

        return NodeValidationCode::Success;
    }

    ConnectionPointIn* findConnectionIn(const type_info& type, const char* targetConnectionName) noexcept override
    {
        if(type == typeid(_InputX) && strcmp(targetConnectionName, "X") == 0)
        {
            return &_inX;
        }

        if(type == typeid(_InputY) && strcmp(targetConnectionName, "Y") == 0)
        {
            return &_inY;
        }

        return nullptr;
    }

    ConnectionPointOut* findConnectionOut(const type_info& type, const char* targetConnectionName) noexcept override
    {
        if(type == typeid(_Output) && strcmp(targetConnectionName, "Result") == 0)
        {
            return &_outResult;
        }

        return nullptr;
    }

    void compute() noexcept override
    {
        if(_inX.connection() == nullptr || _inY.connection() == nullptr)
        {
            return;
        }

        _inX.connection()->node().compute();
        _inY.connection()->node().compute();

        *_outResult.value() = _func(*_inX.value(), *_inY.value());
        if(_outResult.connection() != nullptr)
        {
            static_cast<ConnectionInValueHolder<_Output>*>(_outResult.connection())->value() = _outResult.value();
        }
    }
};
