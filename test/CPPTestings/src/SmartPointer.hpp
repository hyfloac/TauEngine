#pragma once

typedef unsigned int u32;

template<typename _T>
class ScopedPointer
{
private:
    _T* _ptr;
public:
    ScopedPointer() noexcept = delete;

    ScopedPointer(_T* ptr) noexcept
        : _ptr(ptr)
    { }

    ~ScopedPointer() noexcept
    {
        delete _ptr;
    }

    ScopedPointer(const ScopedPointer<_T>& copy) noexcept = delete;
    ScopedPointer(ScopedPointer<_T>&& move) noexcept = delete;

    ScopedPointer<_T>& operator =(const ScopedPointer<_T>& copy) noexcept = delete;
    ScopedPointer<_T>& operator =(ScopedPointer<_T>&& move) noexcept = delete;

    inline const _T* ptr() const noexcept { return _ptr; }
    inline       _T* ptr()       noexcept { return _ptr; }

    inline const _T* operator ->() const noexcept { return _ptr; }
    inline       _T* operator ->()       noexcept { return _ptr; }
};

template<typename _T>
class ScopedPointer<_T[]>
{
private:
    _T* _ptr;
public:
    ScopedPointer() noexcept = delete;

    ScopedPointer(_T* ptr) noexcept
        : _ptr(ptr)
    { }

    ~ScopedPointer() noexcept
    {
        delete[] _ptr;
    }

    ScopedPointer(const ScopedPointer<_T[]>& copy) noexcept = delete;
    ScopedPointer(ScopedPointer<_T[]>&& move) noexcept = delete;

    ScopedPointer<_T[]>& operator =(const ScopedPointer<_T[]>& copy) noexcept = delete;
    ScopedPointer<_T[]>& operator =(ScopedPointer<_T[]>&& move) noexcept = delete;

    inline const _T* ptr() const noexcept { return _ptr; }
    inline       _T* ptr()       noexcept { return _ptr; }

    inline const _T* operator ->() const noexcept { return _ptr; }
    inline       _T* operator ->()       noexcept { return _ptr; }

    inline const _T* operator [](u32 index) const noexcept { return &_ptr[index]; }
    inline       _T* operator [](u32 index)       noexcept { return &_ptr[index]; }
};

template<typename _T>
class ReferencePointer
{
private:
    _T*  _ptr;
    u32* _refCount;
public:
    ReferencePointer() noexcept
        : _ptr(nullptr), _refCount(new u32)
    {
        *_refCount = 0;
    }

    ReferencePointer(_T* ptr) noexcept
        : _ptr(ptr), _refCount(new u32)
    {
        *_refCount = 1;
    }

    ~ReferencePointer() noexcept
    {
        if(--(*_refCount) <= 0)
        {
            delete _ptr;
            delete _refCount;
        }
    }

    ReferencePointer(const ReferencePointer<_T>& copy) noexcept
        : _ptr(copy._ptr), _refCount(copy._refCount)
    {
        ++(*_refCount);
    }

    ReferencePointer(ReferencePointer<_T>&& move) noexcept
        : _ptr(move._ptr), _refCount(move._refCount)
    {
        ++(*_refCount);
    }

    ReferencePointer& operator =(const ReferencePointer<_T>& copy) noexcept
    {
        if(this != &copy)
        {
            _ptr = copy._ptr;
            _refCount = copy._refCount;
            ++(*_refCount);
        }

        return *this;
    }

    ReferencePointer<_T>& operator =(ReferencePointer<_T>&& move) noexcept
    {
        if(this != &move)
        {
            _ptr = move._ptr;
            _refCount = move._refCount;
            ++(*_refCount);
        }

        return *this;
    }

    inline const _T* ptr() const noexcept { return _ptr; }
    inline       _T* ptr()       noexcept { return _ptr; }

    inline const _T* operator ->() const noexcept { return _ptr; }
    inline       _T* operator ->()       noexcept { return _ptr; }
};

template<typename _T>
class ReferencePointer<_T[]>
{
private:
    _T* _ptr;
    u32* _refCount;
public:
    ReferencePointer() noexcept
        : _ptr(nullptr), _refCount(new u32)
    {
        *_refCount = 0;
    }

    ReferencePointer(_T* ptr) noexcept
        : _ptr(ptr), _refCount(new u32)
    {
        *_refCount = 1;
    }

    ~ReferencePointer() noexcept
    {
        if (--(*_refCount) <= 0)
        {
            delete[] _ptr;
            delete _refCount;
        }
    }

    ReferencePointer(const ReferencePointer<_T[]>& copy) noexcept
        : _ptr(copy._ptr), _refCount(copy._refCount)
    {
        ++(*_refCount);
    }

    ReferencePointer(ReferencePointer<_T[]>&& move) noexcept
        : _ptr(move._ptr), _refCount(move._refCount)
    { }

    ReferencePointer<_T[]>& operator =(const ReferencePointer<_T[]>& copy) noexcept
    {
        if(this != &copy)
        {
            _ptr = copy._ptr;
            _refCount = copy._refCount;
            ++(*_refCount);
        }

        return *this;
    }

    ReferencePointer<_T[]>& operator =(ReferencePointer<_T[]>&& move) noexcept
    {
        if(this != &move)
        {
            _ptr = move._ptr;
            _refCount = move._refCount;
            ++(*_refCount);
        }

        return *this;
    }

    inline const _T* ptr() const noexcept { return _ptr; }
    inline       _T* ptr()       noexcept { return _ptr; }

    inline const _T* operator ->() const noexcept { return _ptr; }
    inline       _T* operator ->()       noexcept { return _ptr; }

    inline const _T* operator [](u32 index) const noexcept { return &_ptr[index]; }
    inline       _T* operator [](u32 index)       noexcept { return &_ptr[index]; }
};
