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

#if TAU_RTTI_DEBUG
#define EXCEPTION_IMPL_BASE(_TYPE) \
    public: \
        [[nodiscard]] static Exception::ExceptionType getStaticType() noexcept \
        { static Exception::ExceptionType type("Exception::" TAU_RTTI_STRING(_TYPE), nullptr); \
          return type; } \
        [[nodiscard]] virtual Exception::ExceptionType getExceptionType() const noexcept override \
        { return _TYPE::getStaticType(); }
#else
#define EXCEPTION_IMPL_BASE(_TYPE) \
    public: \
        [[nodiscard]] static Exception::ExceptionType getStaticType() noexcept \
        { static Exception::ExceptionType type; \
          return type; } \
        [[nodiscard]] virtual Exception::ExceptionType getExceptionType() const noexcept override \
        { return _TYPE::getStaticType(); }
#endif


#if EXCEPTION_GEN_NAMES
  #define EXCEPTION_IMPL(_TYPE) \
      EXCEPTION_IMPL_BASE(_TYPE); \
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
    DEFAULT_CM_PO(Exception);
public:
    using ExceptionType = RunTimeType<Exception>;
public:
    [[nodiscard]] virtual ExceptionType getExceptionType() const noexcept = 0;

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
    DEFAULT_CM_PU(ExceptionDispatcher);
public:
    ExceptionDispatcher(Exception& ex) noexcept
        : m_Exception(&ex)
        , m_TypeCache(ex.getExceptionType())
    { }

    template<typename TException, typename TFunc>
    bool dispatch(const TFunc& func) noexcept
    {
        if(m_TypeCache == TException::getStaticType())
        {
            func(reinterpret_cast<TException&>(m_Exception));
            return true;
        }
        return false;
    }

    template<typename TException, typename TClass, typename TFunc>
    bool dispatch(TClass* instance, const TFunc& func) noexcept
    {
        if(m_TypeCache == TException::getStaticType())
        {
            (instance->*func)(reinterpret_cast<TException&>(m_Exception));
            return true;
        }
        return false;
    }
private:
    Exception* m_Exception;
    Exception::ExceptionType m_TypeCache;
};
