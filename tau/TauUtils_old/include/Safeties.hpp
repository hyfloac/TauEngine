#pragma once

#pragma warning(push, 0)
#include <cstdio>
#include <memory>
#pragma warning(pop)

#include "NumTypes.hpp"
#include "ReferenceCountingPointer.hpp"
#include "Utils.hpp"

#define TAU_SAFETIES_str0(_X) #_X
#define TAU_SAFETIES_str(_X) TAU_SAFETIES_str0(_X)

#define TauAssert(_STATE) \
    do { \
        if(!(_STATE)) { \
            fprintf(stderr, "`" #_STATE "` Evaluated to false at line " TAU_SAFETIES_str(__LINE__) " in file \"" TAU_SAFETIES_str(__FILE__) "\".\n"); \
            DEBUG_BREAK; \
        } \
    } while(0)

#define RUNTIME_ERROR(_TEXT) \
    do { \
        fprintf(stderr, __TEXT "\nOccured at line " TAU_SAFETIES_str(__LINE__) " in file \"" TAU_SAFETIES_str(__FILE__) "\".\n"); \
        DEBUG_BREAK; \
    } while(0)

#define ERROR_CODE(_ERR)         do { if(error) { *error = _ERR; } return;         } while(0)
#define ERROR_CODE_V(_ERR, _VAL) do { if(error) { *error = _ERR; } return _VAL;    } while(0)
#define ERROR_CODE_N(_ERR)       do { if(error) { *error = _ERR; } return nullptr; } while(0)
#define ERROR_CODE_T(_ERR)       do { if(error) { *error = _ERR; } return true;    } while(0)
#define ERROR_CODE_F(_ERR)       do { if(error) { *error = _ERR; } return false;   } while(0)
#define ERROR_CODE_0(_ERR)       do { if(error) { *error = _ERR; } return 0;       } while(0)
#define ERROR_CODE_1(_ERR)       do { if(error) { *error = _ERR; } return 1;       } while(0)
#define ERROR_CODE_COND(_COND, _ERR)         do { if((_COND)) { if(error) { *error = _ERR; } return;         } } while(0)
#define ERROR_CODE_COND_V(_COND, _ERR, _VAL) do { if((_COND)) { if(error) { *error = _ERR; } return _VAL;    } } while(0)
#define ERROR_CODE_COND_N(_COND, _ERR)       do { if((_COND)) { if(error) { *error = _ERR; } return nullptr; } } while(0)
#define ERROR_CODE_COND_T(_COND, _ERR)       do { if((_COND)) { if(error) { *error = _ERR; } return true;    } } while(0)
#define ERROR_CODE_COND_F(_COND, _ERR)       do { if((_COND)) { if(error) { *error = _ERR; } return false;   } } while(0)
#define ERROR_CODE_COND_0(_COND, _ERR)       do { if((_COND)) { if(error) { *error = _ERR; } return 0;       } } while(0)
#define ERROR_CODE_COND_1(_COND, _ERR)       do { if((_COND)) { if(error) { *error = _ERR; } return 1;       } } while(0)

#ifdef _DEBUG
  #define Ensure(__STATE) TauAssert(__STATE)
#else
  #define Ensure(__STATE)
#endif

#ifndef TAU_NO_NULLABILITY

#define NonNull  /* Guaranteed to not be null. */
#define Nullable /* Potentially could be null. */

#endif

/**
 *   A wrapper to print an error and trigger a breakpoint if
 * a value is null.
 */
template<typename _T>
class NotNull final
{
private:
    _T* _t;
public:
    constexpr inline NotNull(_T* t) noexcept : _t(t)
    {
        Ensure(t != nullptr);
    }

    constexpr inline NotNull(const NotNull<_T>& copy) noexcept : _t(copy._t) { }
    constexpr inline NotNull(NotNull<_T>&& move) noexcept : _t(move._t) { }

    inline ~NotNull() noexcept = default;

    constexpr inline NotNull<_T>& operator =(const NotNull<_T>& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        this->_t = copy._t;
        return *this;
    }

    constexpr inline NotNull<_T>& operator =(NotNull<_T>&& move) noexcept
    {
        if(this == &move)
        { return *this; }

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
using CPPRef = std::shared_ptr<_T>;

template<typename _T>
using CPPStrongRef = std::shared_ptr<_T>;

template<typename _T>
using CPPWeakRef = std::weak_ptr<_T>;

template<typename _T>
using Ref = ReferenceCountingPointer<_T>;

template<typename _T>
using NullableRef = Ref<_T>;

template<typename _T>
using StrongRef = StrongReferenceCountingPointer<_T>;

template<typename _T>
using WeakRef = WeakReferenceCountingPointer<_T>;

template<typename _T>
using NullableStrongRef = StrongRef<_T>;

template<typename _T>
using NullableWeakRef = WeakRef<_T>;

template<typename _Out, typename _In>
[[nodiscard]] static inline CPPRef<_Out> RefStaticCast(const CPPRef<_In>& in) noexcept
{ return std::static_pointer_cast<_Out>(in); }

template<typename _Out, typename _In>
[[nodiscard]] static inline Ref<_Out> RefStaticCast(const Ref<_In>& in) noexcept
{ return RCPStaticCast<_Out>(in); }

template<typename _Out, typename _In>
[[nodiscard]] static inline StrongRef<_Out> RefStaticCast(const StrongRef<_In>& in) noexcept
{ return RCPStaticCast<_Out>(in); }

template<typename _Out, typename _In>
[[nodiscard]] static inline WeakRef<_Out> RefStaticCast(const WeakRef<_In>& in) noexcept
{ return RCPStaticCast<_Out>(in); }

template<typename _Out, typename _In>
[[nodiscard]] static inline CPPRef<_Out> RefReinterpretCast(const CPPRef<_In>& in) noexcept
{ return std::reinterpret_pointer_cast<_Out>(in); }

template<typename _Out, typename _In>
[[nodiscard]] static inline Ref<_Out> RefReinterpretCast(const Ref<_In>& in) noexcept
{ return RCPReinterpretCast<_Out>(in); }

template<typename _Out, typename _In>
[[nodiscard]] static inline StrongRef<_Out> RefReinterpretCast(const StrongRef<_In>& in) noexcept
{ return RCPReinterpretCast<_Out>(in); }

template<typename _Out, typename _In>
[[nodiscard]] static inline WeakRef<_Out> RefReinterpretCast(const WeakRef<_In>& in) noexcept
{ return RCPReinterpretCast<_Out>(in); }

template<typename _Out, typename _In>
[[nodiscard]] static inline CPPRef<_Out> RefCast(const CPPRef<_In>& in) noexcept
{ return RefStaticCast<_Out>(in); }

template<typename _Out, typename _In>
[[nodiscard]] static inline Ref<_Out> RefCast(const Ref<_In>& in) noexcept
{ return RefStaticCast<_Out>(in); }

template<typename _Out, typename _In>
[[nodiscard]] static inline StrongRef<_Out> RefCast(const StrongRef<_In>& in) noexcept
{ return RefStaticCast<_Out>(in); }

template<typename _Out, typename _In>
[[nodiscard]] static inline WeakRef<_Out> RefCast(const WeakRef<_In>& in) noexcept
{ return RefStaticCast<_Out>(in); }
