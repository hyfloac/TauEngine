#pragma once

#include <String.hpp>
#include <RunTimeType.hpp>
#include <Objects.hpp>
#include "DLL.hpp"

#ifndef EXCEPTION_GEN_NAMES
  #ifndef TAU_PRODUCTION
    #define EXCEPTION_GEN_NAMES 1
  #else
    #define EXCEPTION_GEN_NAMES 0
  #endif
#endif

#define EXCEPTION_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                                   public: \
                                       [[nodiscard]] static Exception::ExceptionType getStaticType() noexcept \
                                       { static Exception::ExceptionType type = Exception::ExceptionType::define(); \
                                         return type; } \
                                       [[nodiscard]] virtual Exception::ExceptionType getExceptionType() const noexcept override \
                                       { return _TYPE::getStaticType(); }

#if EXCEPTION_GEN_NAMES
  #define EXCEPTION_IMPL(_TYPE) EXCEPTION_IMPL_BASE(_TYPE); \
                                [[nodiscard]] virtual const char* getName() const noexcept override \
                                { return #_TYPE; }
  #define EXCEPTION_GET_NAME(_EVENT_PTR) (_EVENT_PTR)->getName()
#else
  #define EXCEPTION_IMPL(_TYPE) EXCEPTION_IMPL_BASE(_TYPE)
  #define EXCEPTION_GET_NAME(_EVENT_PTR) ""
#endif

class TAU_DLL Exception
{
    DEFAULT_CONSTRUCT_PO(Exception);
    DEFAULT_DESTRUCT_VI(Exception);
    DELETE_COPY(Exception);
public:
    using ExceptionType = RunTimeType<Exception>;
public:
    [[nodiscard]] virtual Exception::ExceptionType getExceptionType() const noexcept = 0;

#if EXCEPTION_GEN_NAMES
    [[nodiscard]] virtual const char* getName() const noexcept = 0;
    [[nodiscard]] virtual DynString toString() const noexcept { return DynString(getName()); }
#endif
    
    template<typename _T>
    [[nodiscard]] bool isExceptionType() const noexcept
    { return _T::getStaticType() == getExceptionType(); }
private:
    friend class ExceptionDispatcher;
};

class ExceptionDispatcher final
{
    DEFAULT_DESTRUCT(ExceptionDispatcher);
    DEFAULT_COPY(ExceptionDispatcher);
private:
    Exception& _ex;
    Exception::ExceptionType _cache;
public:
    inline ExceptionDispatcher(Exception& ex) noexcept
        : _ex(ex), _cache(ex.getExceptionType())
    { }

    template<typename _T, typename _F>
    inline bool dispatch(const _F& func) noexcept
    {
        if(_cache == _T::getStaticType())
        {
            func(reinterpret_cast<_T&>(_ex));
            return true;
        }
        return false;
    }

    template<typename _T, typename _C, typename _F>
    inline bool dispatch(_C* instance, const _F& func) noexcept
    {
        if(_cache == _T::getStaticType())
        {
            (instance->*func)(reinterpret_cast<_T&>(_ex));
            return true;
        }
        return false;
    }
};
