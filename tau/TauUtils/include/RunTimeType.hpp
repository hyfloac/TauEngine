#pragma once

#include "Objects.hpp"
#include "NumTypes.hpp"
#include <functional>

template<typename _T>
class RunTimeType;

namespace std {
    template<typename _T>
    struct hash<RunTimeType<_T>> final
    {
        [[nodiscard]] inline ::std::size_t operator()(const RunTimeType<_T>& rtt) const noexcept;
    };
}

template<typename _T>
class RunTimeType final
{
    DEFAULT_DESTRUCT(RunTimeType);
    DEFAULT_CM(RunTimeType);
public:
    static RunTimeType<_T> define() noexcept
    {
        static u64 currentUID = 0;
        return RunTimeType<_T>(++currentUID);
    }
private:
    u64 _uid;
private:
    explicit inline RunTimeType(const u64 uid) noexcept
        : _uid(uid)
    { }
public:
    [[nodiscard]] inline bool operator ==(const RunTimeType<_T>& other) const noexcept { return _uid == other._uid; }
    [[nodiscard]] inline bool operator !=(const RunTimeType<_T>& other) const noexcept { return _uid != other._uid; }
private:
    friend struct std::hash<RunTimeType<_T>>;
};

template<typename _T>
[[nodiscard]] inline ::std::size_t std::hash<RunTimeType<_T>>::operator()(const RunTimeType<_T>& rtt) const noexcept
{ return static_cast<::std::size_t>(rtt._uid); }

#define RTT_BASE_IMPL(_TYPE) \
    public: \
        [[nodiscard]] virtual RunTimeType<_TYPE> _getRTType() const noexcept = 0;

#define RTTD_BASE_IMPL(_TYPE) \
    public: \
        [[nodiscard]] virtual RunTimeType<_TYPE> _getRTType_##_TYPE() const noexcept = 0;

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
        { return obj->_castRTType<_T>(); }

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
        { return obj->_castRTType_##_TYPE<_T>(); }

namespace _RTT_Utils
{
    template<typename _T> struct remove_reference       { typedef _T type; };
    template<typename _T> struct remove_reference<_T&>  { typedef _T type; };
    template<typename _T> struct remove_reference<_T&&> { typedef _T type; };

    template<typename _T>
    using remove_reference_t = typename remove_reference<_T>::type;

    template<typename _T> struct remove_pointer                     { typedef _T type; };
    template<typename _T> struct remove_pointer<_T*>                { typedef _T type; };
    template<typename _T> struct remove_pointer<_T* const>          { typedef _T type; };
    template<typename _T> struct remove_pointer<_T* volatile>       { typedef _T type; };
    template<typename _T> struct remove_pointer<_T* const volatile> { typedef _T type; };

    template<typename _T>
    using remove_pointer_t = typename remove_pointer<_T>::type;
}

#define RTT_CHECK(_VAR, _T) (_RTT_Utils::remove_pointer_t<_RTT_Utils::remove_reference_t<decltype(_VAR)>>::_isRTType<_T>(_VAR))
#define RTT_CAST(_VAR, _T) (_RTT_Utils::remove_pointer_t<_RTT_Utils::remove_reference_t<decltype(_VAR)>>::_castRTType<_T>(_VAR))

#define RTTD_CHECK(_VAR, _T, _BASE_TYPE) (_RTT_Utils::remove_pointer_t<_RTT_Utils::remove_reference_t<decltype(_VAR)>>::_isRTType_##_BASE_TYPE<_T>(_VAR))
#define RTTD_CAST(_VAR, _T, _BASE_TYPE) (_RTT_Utils::remove_pointer_t<_RTT_Utils::remove_reference_t<decltype(_VAR)>>::_castRTType_##_BASE_TYPE<_T>(_VAR))                   
