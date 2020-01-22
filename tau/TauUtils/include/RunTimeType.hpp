#pragma once

#include "NumTypes.hpp"
#include <functional>

template<typename _T>
class RunTimeType;

namespace std
{
    template<typename _T>
    struct hash<RunTimeType<_T>> final
    {
        [[nodiscard]] inline size_t operator()(const RunTimeType<_T>& rtt) const noexcept;
    };
}

template<typename _T>
class RunTimeType final
{
public:
    static RunTimeType define() noexcept
    {
        static u64 currentUID = 0;
        return RunTimeType(++currentUID);
    }
private:
    u64 _uid;

    inline RunTimeType(u64 uid) noexcept
        : _uid(uid)
    { }
public:
    inline ~RunTimeType() noexcept = default;

    inline RunTimeType(const RunTimeType& copy) noexcept = default;
    inline RunTimeType(RunTimeType&& move) noexcept = default;

    inline RunTimeType& operator=(const RunTimeType& copy) noexcept = default;
    inline RunTimeType& operator=(RunTimeType&& move) noexcept = default;

    inline bool operator ==(const RunTimeType<_T>& other) const noexcept { return _uid == other._uid; }
    inline bool operator !=(const RunTimeType<_T>& other) const noexcept { return _uid != other._uid; }
private:
    friend struct std::hash<RunTimeType<_T>>;
};

namespace std
{
    template<typename _T>
    [[nodiscard]] inline size_t hash<RunTimeType<_T>>::operator()(const RunTimeType<_T>& rtt) const noexcept
    {
        return static_cast<size_t>(rtt._uid);
    }
}

#define RTT_BASE_IMPL(_TYPE) public: \
                                 [[nodiscard]] virtual RunTimeType<_TYPE> _getRTType() const noexcept = 0;

#define RTT_IMPL(_TYPE, _BASE_TYPE) public: \
                                        [[nodiscard]] static RunTimeType<_BASE_TYPE> _getStaticRTType() noexcept \
                                        { static RunTimeType<_BASE_TYPE> type = RunTimeType<_BASE_TYPE>::define(); \
                                          return type; } \
                                        [[nodiscard]] virtual RunTimeType<_BASE_TYPE> _getRTType() const noexcept override \
                                        { return _TYPE::_getStaticRTType(); }

#define RTT_BASE_CHECK(_TYPE) public: \
                                  template<typename _T> \
                                  [[nodiscard]] bool _isRTType() const noexcept \
                                  { return _T::_getStaticRTType() == _getRTType(); } \
                                  template<typename _T> \
                                  [[nodiscard]] static bool _isRTType(const _TYPE& obj) noexcept \
                                  { return obj._isRTType<_T>(); } \
                                  template<typename _T> \
                                  [[nodiscard]] static bool _isRTType(const _TYPE* const obj) noexcept \
                                  { return obj->_isRTType<_T>(); }

#define RTT_BASE_CAST(_TYPE) public: \
                                 template<typename _T> \
                                 [[nodiscard]] _T* _castRTType() noexcept \
                                 { return _isRTType<_T>() ? reinterpret_cast<_T>(this) : nullptr; } \
                                 template<typename _T> \
                                 [[nodiscard]] const _T* _castRTType() const noexcept \
                                 { return _isRTType<_T>() ? reinterpret_cast<_T>(this) : nullptr; } \
                                 template<typename _T> \
                                 [[nodiscard]] static _T* _castRTType(_TYPE& obj) noexcept \
                                 { return obj._castRTType<_T>(); } \
                                 template<typename _T> \
                                 [[nodiscard]] static const _T* _castRTType(const _TYPE& obj) noexcept \
                                 { return obj._castRTType<_T>(); } \
                                 template<typename _T> \
                                 [[nodiscard]] static _T* _castRTType(_TYPE* const obj) noexcept \
                                 { return obj._castRTType<_T>(); } \
                                 template<typename _T> \
                                 [[nodiscard]] static const _T* _castRTType(const _TYPE* const obj) noexcept \
                                 { return obj._castRTType<_T>(); }
namespace _RTT_Utils
{
    template<typename T> struct remove_reference { typedef T type; };
    template<typename T> struct remove_reference<T&> { typedef T type; };
    template<typename T> struct remove_reference<T&&> { typedef T type; };

    template<typename T> struct remove_pointer { typedef T type; };
    template<typename T> struct remove_pointer<T*> { typedef T type; };
    template<typename T> struct remove_pointer<T* const> { typedef T type; };
    template<typename T> struct remove_pointer<T* volatile> { typedef T type; };
    template<typename T> struct remove_pointer<T* const volatile> { typedef T type; };
}

#define RTT_CHECK(_VAR, _T) (_RTT_Utils::remove_reference<_RTT_Utils::remove_pointer<decltype(_VAR)>::type>::type::_isRTType<_T>(_VAR))
#define RTT_CAST(_VAR, _T) (_RTT_Utils::remove_reference<_RTT_Utils::remove_pointer<decltype(_VAR)>::type>::type::_castRTType<_T>(_VAR))
                             
