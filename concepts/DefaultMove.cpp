#include <cstddef>
#include <memory>

class A
{
private:
    int* _a;
    size_t* _refCount;
public:
    A(const int a)
        : _a(new int(a))
        , _refCount(new(std::nothrow) size_t(1))
    { }

    ~A()
    { 
        if(--(*_refCount) == 0)
        { 
            delete _a;
            delete _refCount; 
        }
    }

    A(const A& copy)
        : _a(copy._a)
        , _refCount(copy._refCount)
    { ++(*_refCount); }

    A(A&& move)
        : _a(move._a)
        , _refCount(move._refCount)
    { move._refCount = nullptr; }

    A& operator=(const A& copy)
    {
        if(this == &copy)
        { return *this; }
        
        if(--(*_refCount) == 0)
        { 
            delete _a;
            delete _refCount; 
        }

        _a = copy._a;
        _refCount = copy._refCount;

        ++(*_refCount);

        return *this;
    }

    A& operator=(A&& move)
    {
        if(this == &move)
        { return *this; }

        if(--(*_refCount) == 0)
        { 
            delete _a;
            delete _refCount; 
        }

        _a = move._a;
        _refCount = move._refCount;

        move._refCount = nullptr;

        return *this;
    }

    [[nodiscard]] int a() const { return *_a; }
    [[nodiscard]] int& a() { return *_a; }
};

class B
{
private:
    A _a;
public:
    B(const int a)
        : _a(a)
    { }

    B(const A& a)
        : _a(a)
    { }

    B(A&& a)
        : _a(std::move(a))
    { }

    ~B() = default;

    B(const B& copy) = default;
    B(B&& move) = default;

    B& operator=(const B& copy) = default;
    B& operator=(B&& move) = default;
    
    [[nodiscard]]       A& a()       { return _a; }
    [[nodiscard]] const A& a() const { return _a; }
};

int test0(B& b, int q)
{
    B _b = b;
    _b.a().a() += q;
    return b.a().a();
}

int test1(B& b, int q)
{
    b = B(q);
    return b.a().a();
}
