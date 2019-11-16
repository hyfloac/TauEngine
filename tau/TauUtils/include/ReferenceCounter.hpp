#pragma once

#include <cstddef>
#include <new>

class ReferenceCounter final
{
private:
    std::size_t* _refCount;
public:
    inline ReferenceCounter() noexcept
        : _refCount(new(std::nothrow) std::size_t(1))
    { }

    inline ~ReferenceCounter() noexcept
    {
        if(--(*_refCount) == 0)
        { delete _refCount; }
    }

    ReferenceCounter(const ReferenceCounter& other) noexcept
        : _refCount(other._refCount)
    { ++(*_refCount); }

    ReferenceCounter(ReferenceCounter&& other) noexcept
        : _refCount(other._refCount)
    { ++(*_refCount); }

    ReferenceCounter& operator=(const ReferenceCounter& other) noexcept
    {
        if(this == &other)
        { return *this; }
        _refCount = other._refCount;
        ++(*_refCount);
        return *this;
    }

    ReferenceCounter& operator=(ReferenceCounter&& other) noexcept
    {
        if(this == &other)
        { return *this; }
        _refCount = other._refCount;
        ++(*_refCount);
        return *this;
    }

    [[nodiscard]] inline std::size_t refCount() const noexcept { return *_refCount; }

    [[nodiscard]] inline bool finalRef() const noexcept { return (*_refCount) <= 1; }
    [[nodiscard]] inline bool operator()() const noexcept { return (*_refCount) <= 1; }
    [[nodiscard]] inline operator bool() const noexcept { return (*_refCount) <= 1; }
};

typedef ReferenceCounter RefCounter;
