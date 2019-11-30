#pragma once

#pragma warning(push, 0)
#include <cstdio>
#include <memory>
#pragma warning(pop)

#include <NumTypes.hpp>
#include <Utils.hpp>

#define ____str(__X) #__X
#define ___str(__X) ____str(__X)

#define Assert(__STATE) if(!(__STATE)) { fprintf(stderr, "`" #__STATE "` Evaluated to false at line " ___str(__LINE__) " in file `" ___str(__FILE__) ".\n"); DEBUG_BREAK; }

#define RUNTIME_ERROR(__TEXT) do { fprintf(stderr, __TEXT "\nOccured at line " ___str(__LINE__) " in file `" ___str(__FILE__) ".\n"); DEBUG_BREAK; } while(0)

#ifdef _DEBUG
  #define Ensure(__STATE) Assert(__STATE)
#else
  #define Ensure(__STATE)
#endif

#define NonNull  /* Guaranteed to not be null. */
#define Nullable /* Potentially could be null. */

/**
 *   A wrapper to print an error and trigger a breakpoint if
 * a value is null.
 */
template<typename _T>
class NotNull final
{
public:
    _T* _t;
public:
    constexpr inline NotNull(_T* t) noexcept : _t(t)
    {
        Ensure(t != nullptr);
    }
    constexpr inline NotNull(const NotNull& copy) noexcept : _t(copy._t) { }
    constexpr inline NotNull(NotNull&& move) noexcept : _t(move._t) { }

    inline ~NotNull() noexcept = default;

    constexpr inline NotNull& operator =(const NotNull& copy) noexcept
    {
        this->_t = copy._t;
        return *this;
    }

    constexpr inline NotNull& operator =(NotNull&& move) noexcept
    {
        this->_t = move._t;
        return *this;
    }

    constexpr inline _T& operator* ()       noexcept { return *_t; }
    constexpr inline _T& operator* () const noexcept { return *_t; }
    constexpr inline _T* operator->()       noexcept { return  _t; }
    constexpr inline _T* operator->() const noexcept { return  _t; }

    constexpr inline _T* operator()()       noexcept { return  _t; }
    constexpr inline _T* operator()() const noexcept { return  _t; }

    constexpr inline operator _T*()         noexcept { return  _t; }
    constexpr inline operator _T*()   const noexcept { return  _t; }
};

template<typename _T, _T _ErroneousValue, typename _Error, _Error _NonErroneousError>
class Expected final
{
    static_assert(!std::is_same_v<_T, _Error>, "Expected Type and Error type are the same, this will cause a compile conflict with the cast operators.");
private:
    _Error _error;
    _T _value;
public:
    inline Expected(_Error error) noexcept
        : _error(error), _value(_ErroneousValue)
    { 
        Ensure(error != _NonErroneousError);
    }

    inline Expected(_T value) noexcept
        : _error(_NonErroneousError), _value(value)
    {
        Ensure(value != _ErroneousValue);
    }

    inline bool   hasValue() const noexcept { return _value != _ErroneousValue; }
    inline _Error    error() const noexcept { return _error; }
    inline _T        value() const noexcept { return _value; }

    inline operator _Error() const noexcept { return _error; }
    inline operator     _T() const noexcept { return _value; }
    inline _T  operator ()() const noexcept { return _value; }
};

template<typename _T, _T _ErroneousValue>
class Expected<_T, _ErroneousValue, const char*, nullptr> final
{
    static_assert(!std::is_same_v<_T, const char*>, "Expected Type and Error type are the same, this will cause a compile conflict with the cast operators.");
private:
    const char* _error;
    _T _value;
public:
    inline Expected(NotNull<const char> error) noexcept
        : _error(error), _value(_ErroneousValue)
    { }

    inline Expected(_T value) noexcept
        : _error(nullptr), _value(value)
    { 
        Ensure(value != _ErroneousValue);
    }

    inline bool     hasValue() const noexcept { return _value != _ErroneousValue; }
    inline const char* error() const noexcept { return _error; }

    inline _T value() const noexcept
    {
        if(hasValue()) { return _value; }
        fprintf(stderr, "Value not retrieved. %s\n", _error);
        return _ErroneousValue;
    }

    inline operator const char*() const noexcept { return _error;  }
    inline operator          _T() const noexcept { return value(); }
    inline _T       operator ()() const noexcept { return value(); }
};

template<typename _T, _T _ErroneousValue, int _NonErroneousError>
class Expected<_T, _ErroneousValue, int, _NonErroneousError> final
{
    static_assert(!std::is_same_v<_T, int>, "Expected Type and Error type are the same, this will cause a compile conflict with the cast operators.");
private:
    int _error;
    _T _value;
public:
    Expected(i32 error) noexcept
        : _error(error), _value(nullptr)
    {
        Ensure(error != _NonErroneousError);
    }

    Expected(_T value) noexcept
        : _error(0), _value(value)
    {
        Ensure(value != _ErroneousValue);
    }

    inline bool hasValue() const noexcept { return _value != _ErroneousValue; }
    inline int     error() const noexcept { return _error; }

    inline _T value() const noexcept
    {
        if(hasValue()) { return _value; }
        fprintf(stderr, "Value not retrieved. Error Code: %d (0x%X)\n", _error, _error);
        return _ErroneousValue;
    }

    inline operator   int() const noexcept { return _error;  }
    inline operator    _T() const noexcept { return value(); }
    inline _T operator ()() const noexcept { return value(); }
};

template<typename _T, _T _ErroneousValue>
class Expected<_T, _ErroneousValue, int, 0> final
{
    static_assert(!std::is_same_v<_T, int>, "Expected Type and Error type are the same, this will cause a compile conflict with the cast operators.");
private:
    int _error;
    _T _value;
public:
    Expected(int error) noexcept
        : _error(error), _value(nullptr)
    {
        Ensure(error != 0);
    }

    Expected(_T value) noexcept
        : _error(0), _value(value)
    { 
        Ensure(value != _ErroneousValue);
    }

    inline bool hasValue() const noexcept { return _value != _ErroneousValue; }
    inline i32     error() const noexcept { return _error; }

    inline _T value() const noexcept
    {
        if(hasValue()) { return _value; }
        fprintf(stderr, "Value not retrieved. Error Code: %d (0x%X)\n", _error, _error);
        return _ErroneousValue;
    }

    inline operator   int() const noexcept { return _error;  }
    inline operator    _T() const noexcept { return value(); }
    inline _T operator ()() const noexcept { return value(); }
};

template<typename _T, typename _Error, _Error _NonErroneousError>
class ExpectedPtr final
{
    static_assert(!std::is_same_v<_T*, _Error>, "Expected Type and Error type are the same, this will cause a compile conflict with the cast operators.");
private:
    _Error _error;
    _T* _value;
public:
    inline ExpectedPtr(_Error error) noexcept
        : _error(error), _value(nullptr)
    {
        Ensure(error != _NonErroneousError);
    }

    inline ExpectedPtr(NotNull<_T> value) noexcept
        : _error(_NonErroneousError), _value(value)
    { }

    inline bool   hasValue() const noexcept { return _value; }
    inline _Error    error() const noexcept { return _error; }
    inline _T        value() const noexcept { return _value; }

    inline operator _Error() const noexcept { return _error; }
    inline operator     _T() const noexcept { return _value; }
    inline _T  operator ()() const noexcept { return _value; }
};

template<typename _T>
class ExpectedPtr<_T, const char*, nullptr> final
{
    static_assert(!std::is_same_v<_T*, const char*>, "Expected Type and Error type are the same, this will cause a compile conflict with the cast operators.");
private:
    const char* _error;
    _T* _value;
public:
    inline ExpectedPtr(NotNull<const char> error) noexcept
        : _error(error), _value(nullptr)
    { }

    inline ExpectedPtr(NotNull<_T> value) noexcept
        : _error(nullptr), _value(value)
    { }

    inline bool     hasValue() const noexcept { return _value; }
    inline const char* error() const noexcept { return _error; }

    inline _T value() const noexcept
    {
        if(hasValue()) { return _value; }
        fprintf(stderr, "Value not retrieved. %s\n", _error);
        return nullptr;
    }

    inline operator const char*() const noexcept { return _error;  }
    inline operator          _T() const noexcept { return value(); }
    inline _T  operator      ()() const noexcept { return value(); }
};

template<typename _T, int  _NonErroneousError>
class ExpectedPtr<_T, int, _NonErroneousError> final
{
private:
    int _error;
    _T* _value;
public:
    inline ExpectedPtr(i32 error) noexcept
        : _error(error), _value(nullptr)
    { 
        Ensure(error != _NonErroneousError)
    }

    inline ExpectedPtr(NotNull<_T> value) noexcept
        : _error(_NonErroneousError), _value(value)
    { }

    inline bool hasValue() const noexcept { return _value; }
    inline int     error() const noexcept { return _error; }

    inline _T* value() const noexcept 
    {
        if(hasValue()) { return _value; }
        fprintf(stderr, "Value not retrieved. Error Code: %d (0x%X)\n", _error, _error);
        return nullptr;
    }

    inline operator   int() const noexcept { return _error;  }
    inline operator    _T() const noexcept { return value(); }
    inline _T operator ()() const noexcept { return value(); }
};

template<typename _T>
class ExpectedPtr<_T, int, 0> final
{
private:
    int _error;
    _T* _value;
public:
    inline ExpectedPtr(i32 error) noexcept
        : _error(error), _value(nullptr)
    {
        Ensure(error != 0)
    }

    inline ExpectedPtr(NotNull<_T> value) noexcept
        : _error(0), _value(value)
    { }

    inline bool hasValue() const noexcept { return _value; }
    inline int     error() const noexcept { return _error; }

    inline _T* value() const noexcept
    {
        if(hasValue()) { return _value; }
        fprintf(stderr, "Value not retrieved. Error Code: %d (0x%X)\n", _error, _error);
        return nullptr;
    }

    inline operator   int() const noexcept { return _error;  }
    inline operator    _T() const noexcept { return value(); }
    inline _T operator ()() const noexcept { return value(); }
};


template<typename _T, _T _ErroneousValue>
using ExpectedS = Expected<_T, _ErroneousValue, const char*, nullptr>;

template<typename _T, _T _ErroneousValue>
using ExpectedI = Expected<_T, _ErroneousValue, int, 0>;

template<typename _T, _T _ErroneousValue, int _NonErroneousError>
using ExpectedIv = Expected<_T, _ErroneousValue, int, _NonErroneousError>;

template<typename _T>
using ExpectedPtrS = ExpectedPtr<_T, const char*, nullptr>;

template<typename _T>
using ExpectedPtrI = ExpectedPtr<_T, int, 0>;

template<typename _T, int _NonErroneousError>
using ExpectedPtrIv = ExpectedPtr<_T, int, _NonErroneousError>;

template<typename _T, typename _D = std::default_delete<_T>>
using Scoped = std::unique_ptr<_T, _D>;

template<typename _T>
using Ref = std::shared_ptr<_T>;

template<typename _T>
using StrongRef = std::shared_ptr<_T>;

template<typename _T>
using WeakRef = std::weak_ptr<_T>;

template<typename _Out, typename _In>
[[nodiscard]] static inline Ref<_Out> RefCast(const Ref<_In>& in) noexcept
{ return std::static_pointer_cast<_Out>(in); }


