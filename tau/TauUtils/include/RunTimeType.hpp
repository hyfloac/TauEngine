#pragma once

#include "NumTypes.hpp"
#include <unordered_map>

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
