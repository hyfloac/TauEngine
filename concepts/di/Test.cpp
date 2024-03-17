#include <iostream>
#include "DependencyInjection.hpp"
#include <cstdlib>

class Printer final
{
private:
    ::std::ostream& _output;
    int _x;
public:
    Printer(::std::ostream& output, int x) noexcept
        : _output(output)
        , _x(x)
    { }

    Printer* print(const char* str)
    {
        _output << str;
        return this;
    }

    Printer* print(int x)
    {
        _output << x;
        return this;
    }

    Printer* nl()
    {
        _output << ::std::endl;
        return this;
    }

    Printer* printX()
    {
        _output << '[' << _x << ']';
        return this;
    }
};

struct RandStore final
{
    int val; 

    RandStore()
        : val(rand())
    { }
};

class A final
{
private:
    ::std::shared_ptr<Printer> _printer;
    ::std::shared_ptr<RandStore> _randStore;
public:
    A() noexcept
        : DI_Inject(_printer)
        , DI_Inject(_randStore)
    { }

    void test()
    {
        _printer->printX()->print(" ")->print(_randStore->val)->nl();
    }
};

int main(int argCount, char* args[])
{
    srand(time(nullptr));

    DependencyInjector<Printer>::RegisterStatic(::std::cout, 32);
    DependencyInjector<RandStore>::RegisterScoped();

    {
        A a;
        a.test();
        a.test();
    }
    
    {
        A a;
        a.test();
        a.test();
    }

    return 0;
}
