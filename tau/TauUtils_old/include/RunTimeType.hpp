#pragma once

#include "Objects.hpp"
#include "NumTypes.hpp"
#include "Safeties.hpp"
#include "AtomicIntrinsics.hpp"
#include <functional>

#ifndef TAU_RTTI_DEBUG
  #if defined(TAU_PRODUCTION)
    #define TAU_RTTI_DEBUG 0
  #else
    #define TAU_RTTI_DEBUG 1
  #endif
#endif

/**
 * A lightweight wrapper for RTTI.
 *
 *   Depending on whether debug mode is enabled this either
 * stores an incremental integer or a pointer to itself.
 *
 *   When debug mode is enabled this stores a pointer to itself
 * as well as the types name, and the parent RTTI structure. We
 * store the pointer because it will be unique, and consistent.
 * This potentially can make certain parts easier to debug.
 *
 *   When debug mode is not enabled this stores an incremental
 * integer. This integer is incremented at runtime and is
 * dependent on the order in which RTTI is checked. This is
 * because the integer is generated when the class is lazily
 * instantiated. The RTTI structure is stored as a static
 * member within a specialized function. Static function
 * variables are instantiated on the first call to the
 * function. Thus if the order in which types are checked
 * changes between runs, the underlying RTTI value will change,
 * making everything significantly more obfuscated, while still
 * being able to uniquely identify each type. This integer is
 * also incremented using atomics, so it is safe to instantiate
 * the type from multiple threads simultaneously. There is also
 * a template type, this is a type safety restriction that
 * ensures that two entirely different types with the same
 * underlying RTTI value aren't identified as the same by
 * accident.
 */
template<typename _T>
class RunTimeType;

namespace std {
    template<typename _T>
    struct hash<RunTimeType<_T>> final
    {
        [[nodiscard]] inline ::std::size_t operator()(const RunTimeType<_T>& rtt) const noexcept;
    };
}

#if TAU_RTTI_DEBUG
template<typename _T>
class RunTimeType final
{
    DEFAULT_DESTRUCT(RunTimeType);
    DEFAULT_CM_PU(RunTimeType);
private:
    void* _uid;
    const char* _name;
    const RunTimeType<_T>* _parent;
public:
    RunTimeType(const char* const name, const RunTimeType<_T>* const parent = nullptr) noexcept
        : _uid(this)
        , _name(name)
        , _parent(parent)
    { }

    [[nodiscard]] const char* name() const noexcept { return _name; }
    [[nodiscard]] const RunTimeType<_T>* parent() const noexcept { return _parent; }

    [[nodiscard]] bool operator ==(const RunTimeType<_T>& other) const noexcept { return _uid == other._uid; }
    [[nodiscard]] bool operator !=(const RunTimeType<_T>& other) const noexcept { return _uid != other._uid; }
private:
    friend struct std::hash<RunTimeType<_T>>;
};
#else
template<typename _T>
class RunTimeType final
{
    DEFAULT_DESTRUCT(RunTimeType);
    DEFAULT_CM_PU(RunTimeType);
public:
    static RunTimeType<_T> define() noexcept
    {
        static volatile u64 currentUID = 0;
        const u64 ret = atomicIncrement(&currentUID);
        return RunTimeType<_T>(ret);
    }
private:
    u64 _uid;
private:
    explicit RunTimeType(const u64 uid) noexcept
        : _uid(uid)
    { }
public:
    [[nodiscard]] const char* name() const noexcept { return nullptr; }
    [[nodiscard]] const RunTimeType<_T>* parent() const noexcept { return nullptr; }

    [[nodiscard]] bool operator ==(const RunTimeType<_T>& other) const noexcept { return _uid == other._uid; }
    [[nodiscard]] bool operator !=(const RunTimeType<_T>& other) const noexcept { return _uid != other._uid; }
private:
    friend struct std::hash<RunTimeType<_T>>;
};
#endif

template<typename _T>
[[nodiscard]] inline ::std::size_t std::hash<RunTimeType<_T>>::operator()(const RunTimeType<_T>& rtt) const noexcept
{ return static_cast<::std::size_t>(rtt._uid); }

#define TAU_RTTI_STRING0(_X) #_X
#define TAU_RTTI_STRING(_X) TAU_RTTI_STRING0(_X)

#if TAU_RTTI_DEBUG
  #define RTT_BASE_IMPL(_TYPE) \
    public: \
        [[nodiscard]] static RunTimeType<_TYPE> _getStaticRTType() noexcept\
        { static RunTimeType<_TYPE> type(TAU_RTTI_STRING(_TYPE)); \
          return type; } \
        [[nodiscard]] virtual RunTimeType<_TYPE> _getRTType() const noexcept = 0; \

  #define RTTD_BASE_IMPL(_TYPE) \
    public: \
        [[nodiscard]] static RunTimeType<_TYPE> _getStaticRTType_##_TYPE() noexcept\
        { static RunTimeType<_TYPE> type(TAU_RTTI_STRING(_TYPE)); \
          return type; } \
        [[nodiscard]] virtual RunTimeType<_TYPE> _getRTType_##_TYPE() const noexcept = 0;
#else
  #define RTT_BASE_IMPL(_TYPE) \
    public: \
        [[nodiscard]] virtual RunTimeType<_TYPE> _getRTType() const noexcept = 0;

  #define RTTD_BASE_IMPL(_TYPE) \
    public: \
        [[nodiscard]] virtual RunTimeType<_TYPE> _getRTType_##_TYPE() const noexcept = 0;
#endif

#if TAU_RTTI_DEBUG
  #define RTT_IMPL(_TYPE, _BASE_TYPE) \
    public: \
        [[nodiscard]] static RunTimeType<_BASE_TYPE> _getStaticRTType() noexcept \
        { static RunTimeType<_BASE_TYPE> type(TAU_RTTI_STRING(_TYPE), &_BASE_TYPE::_getStaticRTType()); \
          return type; } \
        [[nodiscard]] virtual RunTimeType<_BASE_TYPE> _getRTType() const noexcept override \
        { return _TYPE::_getStaticRTType(); }

  #define RTTD_IMPL(_TYPE, _BASE_TYPE) \
    public: \
        [[nodiscard]] static RunTimeType<_BASE_TYPE> _getStaticRTType_##_BASE_TYPE() noexcept \
        { static RunTimeType<_BASE_TYPE> type(TAU_RTTI_STRING(_TYPE), &_BASE_TYPE::_getStaticRTType_##_BASE_TYPE()); \
          return type; } \
        [[nodiscard]] virtual RunTimeType<_BASE_TYPE> _getRTType_##_BASE_TYPE() const noexcept override \
        { return _TYPE::_getStaticRTType_##_BASE_TYPE(); }
#else
  #define RTT_IMPL(_TYPE, _BASE_TYPE) \
    public: \
        [[nodiscard]] static RunTimeType<_BASE_TYPE> _getStaticRTType() noexcept \
        { static RunTimeType<_BASE_TYPE> type = RunTimeType<_BASE_TYPE>::define(); \
          return type; } \
        [[nodiscard]] virtual RunTimeType<_BASE_TYPE> _getRTType() const noexcept override \
        { return _TYPE::_getStaticRTType(); }

  #define RTTD_IMPL(_TYPE, _BASE_TYPE) \
    public: \
        [[nodiscard]] static RunTimeType<_BASE_TYPE> _getStaticRTType_##_BASE_TYPE() noexcept \
        { static RunTimeType<_BASE_TYPE> type = RunTimeType<_BASE_TYPE>::define(); \
          return type; } \
        [[nodiscard]] virtual RunTimeType<_BASE_TYPE> _getRTType_##_BASE_TYPE() const noexcept override \
        { return _TYPE::_getStaticRTType_##_BASE_TYPE(); }
#endif

#define RTT_BASE_CHECK(_TYPE) \
    public: \
        template<typename _T> \
        [[nodiscard]] bool _isRTType() const noexcept \
        { return _T::_getStaticRTType() == _getRTType(); } \
        template<typename _T> \
        [[nodiscard]] static bool _isRTType(const _TYPE& obj) noexcept \
        { return obj._isRTType<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static bool _isRTType(const _TYPE* const obj) noexcept \
        { return obj->_isRTType<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static bool _isRTType(const CPPRef<_TYPE>& obj) noexcept \
        { return obj->_isRTType<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static bool _isRTType(const CPPWeakRef<_TYPE>& obj) noexcept \
        { return obj->_isRTType<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static bool _isRTType(const Ref<_TYPE>& obj) noexcept \
        { return obj->_isRTType<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static bool _isRTType(const StrongRef<_TYPE>& obj) noexcept \
        { return obj->_isRTType<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static bool _isRTType(const WeakRef<_TYPE>& obj) noexcept \
        { return obj->_isRTType<_T>(); }

#define RTTD_BASE_CHECK(_TYPE) \
    public: \
        template<typename _T> \
        [[nodiscard]] bool _isRTType_##_TYPE() const noexcept \
        { return _T::_getStaticRTType_##_TYPE() == _getRTType_##_TYPE(); } \
        template<typename _T> \
        [[nodiscard]] static bool _isRTType_##_TYPE(const _TYPE& obj) noexcept \
        { return obj._isRTType_##_TYPE<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static bool _isRTType_##_TYPE(const _TYPE* const obj) noexcept \
        { return obj->_isRTType_##_TYPE<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static bool _isRTType_##_TYPE(const CPPRef<_TYPE>& obj) noexcept \
        { return obj->_isRTType_##_TYPE<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static bool _isRTType_##_TYPE(const CPPWeakRef<_TYPE>& obj) noexcept \
        { return obj->_isRTType_##_TYPE<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static bool _isRTType_##_TYPE(const Ref<_TYPE>& obj) noexcept \
        { return obj->_isRTType_##_TYPE<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static bool _isRTType_##_TYPE(const StrongRef<_TYPE>& obj) noexcept \
        { return obj->_isRTType_##_TYPE<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static bool _isRTType_##_TYPE(const WeakRef<_TYPE>& obj) noexcept \
        { return obj->_isRTType_##_TYPE<_T>(); }

#define RTT_BASE_CAST(_TYPE) \
    public: \
        template<typename _T> \
        [[nodiscard]] _T* _castRTType() noexcept \
        { return _isRTType<_T>() ? static_cast<_T*>(this) : nullptr; } \
        template<typename _T> \
        [[nodiscard]] const _T* _castRTType() const noexcept \
        { return _isRTType<_T>() ? static_cast<const _T*>(this) : nullptr; } \
        template<typename _T> \
        [[nodiscard]] static _T* _castRTType(_TYPE& obj) noexcept \
        { return obj._castRTType<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static const _T* _castRTType(const _TYPE& obj) noexcept \
        { return obj._castRTType<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static _T* _castRTType(_TYPE* const obj) noexcept \
        { return obj->_castRTType<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static const _T* _castRTType(const _TYPE* const obj) noexcept \
        { return obj->_castRTType<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static const _T* _castRTType(const CPPRef<_TYPE>& obj) noexcept \
        { return obj->_isRTType<_T>() ? RefStaticCast<_T>(obj) : nullptr; } \
        template<typename _T> \
        [[nodiscard]] static const _T* _castRTType(const Ref<_TYPE>& obj) noexcept \
        { return obj->_isRTType<_T>() ? RefStaticCast<_T>(obj) : nullptr; } \
        template<typename _T> \
        [[nodiscard]] static const _T* _castRTType(const StrongRef<_TYPE>& obj) noexcept \
        { return obj->_isRTType<_T>() ? RefStaticCast<_T>(obj) : nullptr; } \
        template<typename _T> \
        [[nodiscard]] static const _T* _castRTType(const WeakRef<_TYPE>& obj) noexcept \
        { return obj->_isRTType<_T>() ? RefStaticCast<_T>(obj) : nullptr; }

#define RTTD_BASE_CAST(_TYPE) \
    public: \
        template<typename _T> \
        [[nodiscard]] _T* _castRTType_##_TYPE() noexcept \
        { return _isRTType_##_TYPE<_T>() ? static_cast<_T*>(this) : nullptr; } \
        template<typename _T> \
        [[nodiscard]] const _T* _castRTType_##_TYPE() const noexcept \
        { return _isRTType_##_TYPE<_T>() ? static_cast<const _T*>(this) : nullptr; } \
        template<typename _T> \
        [[nodiscard]] static _T* _castRTType_##_TYPE(_TYPE& obj) noexcept \
        { return obj._castRTType_##_TYPE<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static const _T* _castRTType_##_TYPE(const _TYPE& obj) noexcept \
        { return obj._castRTType_##_TYPE<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static _T* _castRTType_##_TYPE(_TYPE* const obj) noexcept \
        { return obj->_castRTType_##_TYPE<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static const _T* _castRTType_##_TYPE(const _TYPE* const obj) noexcept \
        { return obj->_castRTType_##_TYPE<_T>(); } \
        template<typename _T> \
        [[nodiscard]] static const _T* _castRTType_##_TYPE(const CPPRef<_TYPE>& obj) noexcept \
        { return obj->_isRTType_##_TYPE<_T>() ? RefStaticCast<_T>(obj) : nullptr; } \
        template<typename _T> \
        [[nodiscard]] static const _T* _castRTType_##_TYPE(const Ref<_TYPE>& obj) noexcept \
        { return obj->_isRTType_##_TYPE<_T>() ? RefStaticCast<_T>(obj) : nullptr; } \
        template<typename _T> \
        [[nodiscard]] static const _T* _castRTType_##_TYPE(const StrongRef<_TYPE>& obj) noexcept \
        { return obj->_isRTType_##_TYPE<_T>() ? RefStaticCast<_T>(obj) : nullptr; } \
        template<typename _T> \
        [[nodiscard]] static const _T* _castRTType_##_TYPE(const WeakRef<_TYPE>& obj) noexcept \
        { return obj->_isRTType_##_TYPE<_T>() ? RefStaticCast<_T>(obj) : nullptr; }

namespace _RTT_Utils
{
    template<typename _T> struct remove_reference       { typedef _T type; };
    template<typename _T> struct remove_reference<_T&>  { typedef _T type; };
    template<typename _T> struct remove_reference<_T&&> { typedef _T type; };

    template<typename _T>
    using remove_reference_t = typename remove_reference<_T>::type;

    template<typename _T> struct remove_pointer                       { typedef _T type; };
    template<typename _T> struct remove_pointer<_T*>                  { typedef _T type; };
    template<typename _T> struct remove_pointer<_T* const>            { typedef _T type; };
    template<typename _T> struct remove_pointer<_T* volatile>         { typedef _T type; };
    template<typename _T> struct remove_pointer<_T* const volatile>   { typedef _T type; };
    template<typename _T> struct remove_pointer<CPPRef<_T>>           { typedef _T type; };
    template<typename _T> struct remove_pointer<CPPWeakRef<_T>>       { typedef _T type; };
    template<typename _T> struct remove_pointer<Ref<_T>>              { typedef _T type; };
    template<typename _T> struct remove_pointer<StrongRef<_T>>        { typedef _T type; };
    template<typename _T> struct remove_pointer<WeakRef<_T>>          { typedef _T type; };
    template<typename _T> struct remove_pointer<const CPPRef<_T>>     { typedef _T type; };
    template<typename _T> struct remove_pointer<const CPPWeakRef<_T>> { typedef _T type; };
    template<typename _T> struct remove_pointer<const Ref<_T>>        { typedef _T type; };
    template<typename _T> struct remove_pointer<const StrongRef<_T>>  { typedef _T type; };
    template<typename _T> struct remove_pointer<const WeakRef<_T>>    { typedef _T type; };

    template<typename _T>
    using remove_pointer_t = typename remove_pointer<_T>::type;
}

template<typename _TargetType, typename _InputType, ::std::enable_if_t<::std::is_base_of_v<_RTT_Utils::remove_pointer_t<_RTT_Utils::remove_reference_t<_InputType>>, _TargetType>, int> = 0>
[[nodiscard]] bool rtt_check(const _InputType& in) noexcept
{ return _RTT_Utils::remove_pointer_t<_RTT_Utils::remove_reference_t<_InputType>>::template _isRTType<_TargetType>(in); }

template<typename _TargetType, typename _InputType, ::std::enable_if_t<::std::is_base_of_v<_RTT_Utils::remove_pointer_t<_RTT_Utils::remove_reference_t<_InputType>>, _TargetType>, int> = 0>
[[nodiscard]] _TargetType* rtt_cast(const _InputType& in) noexcept
{ return _RTT_Utils::remove_pointer_t<_RTT_Utils::remove_reference_t<_InputType>>::_castRTType(in); }

#define RTT_CHECK(_VAR, _T) (_RTT_Utils::remove_pointer_t<_RTT_Utils::remove_reference_t<decltype(_VAR)>>::_isRTType<_T>(_VAR))
#define RTT_CAST(_VAR, _T) (_RTT_Utils::remove_pointer_t<_RTT_Utils::remove_reference_t<decltype(_VAR)>>::_castRTType<_T>(_VAR))

#define RTTD_CHECK(_VAR, _T, _BASE_TYPE) (_RTT_Utils::remove_pointer_t<_RTT_Utils::remove_reference_t<decltype(_VAR)>>::_isRTType_##_BASE_TYPE<_T>(_VAR))
#define RTTD_CAST(_VAR, _T, _BASE_TYPE) (_RTT_Utils::remove_pointer_t<_RTT_Utils::remove_reference_t<decltype(_VAR)>>::_castRTType_##_BASE_TYPE<_T>(_VAR))                   
