#include <memory>
#include <type_traits>

struct Foo
{
    int bar;
    float qux;

    Foo(const int _bar, const float _qux) 
        : bar(_bar)
        , qux(_qux)
    { }
};

struct Allocator
{
    virtual void* allocate(int size);
};

struct NewAllocator : public Allocator
{ 
    static NewAllocator& Instance() 
    {
        static NewAllocator instance;
        return instance;
    }

    void* allocate(int size) override
    { return operator new(size); }
};

struct MallocAllocator : public Allocator
{ 
    static MallocAllocator& Instance() 
    {
        static MallocAllocator instance;
        return instance;
    }

    void* allocate(int size) override
    { return malloc(size); }
};

template<typename _T>
class Wrapper
{
public:
    Allocator& alloc;
    _T t;
public:
    template<typename _TT, typename... _Args, typename std::enable_if_t<std::is_base_of_v<Allocator, _TT>, int> = 0>
    Wrapper(_TT& allocator, _Args&&... args)
        : alloc(allocator)
        , t(std::forward<_Args>(args)...)
    { }

    template<typename _TT, typename... _Args, typename std::enable_if_t<!std::is_base_of_v<Allocator, _TT>, int> = 0>
    Wrapper(_TT&& t, _Args&&... args)
        : alloc(NewAllocator::Instance())
        , t(std::forward<_TT>(t), std::forward<_Args>(args)...)
    { }
public:
    // template<typename... _Args>
    // Wrapper(_Args&&... args)
    //     : Wrapper(std::forward<_Args>(args)...)
    // { }
};

Wrapper<Foo> test0()
{
    return Wrapper<Foo>(32, 3.141592f);
}

Wrapper<Foo> test1()
{
    return Wrapper<Foo>(MallocAllocator::Instance(), 64, 2.71828f);
}
