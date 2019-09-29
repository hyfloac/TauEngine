#include "Node.hpp"
#include <iostream>

struct ConstNode : public AbstractNode
{
    ConnectionOutValueHolder<int> _outValue;

    ConstNode(const char* nodeTypeName, const char* arbitraryName, int value)
        : AbstractNode(nodeTypeName, arbitraryName),
        _outValue("Result", *this, value)
    { }


    NodeValidationCode validateConnections() const noexcept override
    {
        return NodeValidationCode::Success;
    }

    ConnectionPointIn* findConnectionIn(const type_info& type, const char* targetConnectionName) noexcept override
    {
        return nullptr;
    }

    ConnectionPointOut* findConnectionOut(const type_info& type, const char* targetConnectionName) noexcept override
    {
        if(type == typeid(int))
        {
            return &_outValue;
        }

        return nullptr;
    }

    void compute() noexcept override
    {
        if(_outValue.connection() != nullptr)
        {
            static_cast<ConnectionInValueHolder<int>*>(_outValue.connection())->value() = _outValue.value();
        }
    }
};

struct PrintNode : public AbstractNode
{
    ConnectionInValueHolder<float> _inValue;

    PrintNode(const char* nodeTypeName, const char* arbitraryName)
        : AbstractNode(nodeTypeName, arbitraryName),
        _inValue("Result", *this, true)
    { }


    NodeValidationCode validateConnections() const noexcept override
    {
        return NodeValidationCode::Success;
    }

    ConnectionPointIn* findConnectionIn(const type_info& type, const char* targetConnectionName) noexcept override
    {
        if(type == typeid(float))
        {
            return &_inValue;
        }

        return nullptr;
    }

    ConnectionPointOut* findConnectionOut(const type_info& type, const char* targetConnectionName) noexcept override
    {
        return nullptr;
    }

    void compute() noexcept override
    {
        _inValue.connection()->node().compute();

        std::cout << _inValue.value() << std::endl;

    }
};

int main()
{
    BinaryOperatorNode<int, int, float> divNode("Div Node", "Test", [](int x, int y) { return (float) x / (float) y; });

    ConstNode x("X", "X", 13);
    ConstNode y("Y", "Y", 7);
    
    PrintNode printer("Print", "Print");

    divNode.connect(&x._outValue, typeid(int), "X");
    divNode.connect(&y._outValue, typeid(int), "Y");

    divNode.connect(&printer._inValue, typeid(float), "Result");

    printer.compute();

    std::getchar();

    return 0;
}
