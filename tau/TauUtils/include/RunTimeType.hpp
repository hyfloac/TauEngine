#pragma once

#include "NumTypes.hpp"

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
};
