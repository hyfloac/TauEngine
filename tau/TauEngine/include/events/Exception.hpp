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
    #define EXCEPTION_INTERNAL_TYPE_DECL(TYPE) ("Exception::" TAU_RTTI_STRING(TYPE), nullptr);
#else
    #define EXCEPTION_INTERNAL_TYPE_DECL(TYPE)
#endif

#define EXCEPTION_IMPL_BASE(TYPE) \
    public: \
        [[nodiscard]] static Exception::ExceptionType GetStaticType() noexcept \
        { static Exception::ExceptionType type EXCEPTION_INTERNAL_TYPE_DECL(TYPE); \
          return type; } \
        [[nodiscard]] virtual Exception::ExceptionType GetExceptionType() const noexcept override \
        { return TYPE::GetStaticType(); }

#if EXCEPTION_GEN_NAMES
  #define EXCEPTION_IMPL(TYPE) \
      EXCEPTION_IMPL_BASE(TYPE); \
      [[nodiscard]] virtual const c8* GetName() const noexcept override \
      { return u8 ## #TYPE; }
  #define EXCEPTION_GET_NAME(EVENT_PTR) (EVENT_PTR)->GetName()
#else
  #define EXCEPTION_IMPL(TYPE) EXCEPTION_IMPL_BASE(TYPE)
  #define EXCEPTION_GET_NAME(EVENT_PTR) u8""
#endif

class TAU_DLL Exception
{
    DEFAULT_CONSTRUCT_PO(Exception);
    DEFAULT_DESTRUCT_VI(Exception);
    DEFAULT_CM_PO(Exception);
public:
    using ExceptionType = RunTimeType<Exception>;
public:
    [[nodiscard]] virtual ExceptionType GetExceptionType() const noexcept = 0;

#if EXCEPTION_GEN_NAMES
    [[nodiscard]] virtual const c8* GetName() const noexcept = 0;
    [[nodiscard]] virtual C8DynString ToString() const noexcept { return C8DynString(GetName()); }
#endif
    
    template<typename TException>
    [[nodiscard]] bool IsExceptionType() const noexcept
    {
        return TException::GetStaticType() == GetExceptionType();
    }
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
        , m_TypeCache(ex.GetExceptionType())
    { }

    template<typename TException, typename TFunc>
    bool Dispatch(const TFunc& func) noexcept
    {
        if(m_TypeCache == TException::getStaticType())
        {
            func(reinterpret_cast<TException&>(m_Exception));
            return true;
        }

        return false;
    }

    template<typename TException, typename TClass, typename TFunc>
    bool Dispatch(TClass* instance, const TFunc& func) noexcept
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
