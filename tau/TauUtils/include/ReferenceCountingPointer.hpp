#pragma once

#include <cstddef>
#include <Objects.hpp>
#include <memory>

namespace _ReferenceCountingPointerUtils {

template<typename _T>
struct _RemoveReference final
{
    using type = _T;
};

template<typename _T>
struct _RemoveReference<_T&> final
{
    using type = _T;
};

template<typename _T>
struct _RemoveReference<_T&&> final
{
    using type = _T;
};

template<typename _T>
[[nodiscard]] constexpr inline _T&& _forward(_RemoveReference<_T>& ref) noexcept
{
    return static_cast<_T&&>(ref);
}

template<typename _T>
struct _ReferenceCountDataObject final
{
    DELETE_COPY(_ReferenceCountDataObject);
    DEFAULT_DESTRUCT(_ReferenceCountDataObject);
public:
    ::std::size_t _refCount;
    _T _obj;
public:
    template<typename... _Args>
    _ReferenceCountDataObject(_Args&&... args) noexcept
        : _refCount(1), _obj(_forward(args)...)
    { }
};

struct _SWReferenceCount final
{
    DELETE_COPY(_SWReferenceCount);
    DEFAULT_DESTRUCT(_SWReferenceCount);
public:
    ::std::size_t _strongRefCount;
    ::std::size_t _weakRefCount;
public:
    _SWReferenceCount() noexcept
        : _strongRefCount(1), _weakRefCount(0)
    { }
};

}

template<typename _T>
class ReferenceCountingPointer final
{
private:
    template<typename _TT>
    using RCDO = _ReferenceCountingPointerUtils::_ReferenceCountDataObject<_TT>;
private:
    RCDO<_T>* _ptr;
public:
    template<typename... _Args>
    inline explicit ReferenceCountingPointer(_Args&&... args) noexcept
        : _ptr(new(::std::nothrow) RCDO<_T>(args))
    { }

    inline ~ReferenceCountingPointer() noexcept
    {
        if(--_ptr->_refCount == 0)
        { delete _ptr; }
    }

    inline ReferenceCountingPointer(const ReferenceCountingPointer<_T>& copy) noexcept
        : _ptr(copy._ptr)
    { ++_ptr->_refCount; }
    
    inline ReferenceCountingPointer(ReferenceCountingPointer<_T>&& move) noexcept
        : _ptr(move._ptr)
    { ++_ptr->_refCount; }

    inline ReferenceCountingPointer<_T>& operator=(const ReferenceCountingPointer<_T>& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(--_ptr->_refCount == 0)
        { delete _ptr; }

        _ptr = copy._ptr;
        ++_ptr->_refCount;

        return *this;
    }

    inline ReferenceCountingPointer<_T>& operator=(ReferenceCountingPointer<_T>&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(--_ptr->_refCount == 0)
        { delete _ptr; }

        _ptr = move._ptr;
        ++_ptr->_refCount;

        return *this;
    }

    [[nodiscard]] inline _T& operator *() noexcept { return _ptr->_obj; }
    [[nodiscard]] inline const _T& operator *() const noexcept { return _ptr->_obj; }
    [[nodiscard]] inline _T* operator ->() noexcept { return &_ptr->_obj; }
    [[nodiscard]] inline const _T* operator ->() const noexcept { return &_ptr->_obj; }

    [[nodiscard]] inline _T* get() noexcept { return &_ptr->_obj; }
    [[nodiscard]] inline const _T* get() const noexcept { return &_ptr->_obj; }

    [[nodiscard]] inline ::std::size_t refCount() const noexcept { return _ptr->_refCount; }

    [[nodiscard]] inline operator bool() const noexcept { return true; }
    [[nodiscard]] inline bool operator ==(const ReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const ReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const _T*& ptr) const noexcept { return &_ptr->_obj == ptr; }
    [[nodiscard]] inline bool operator !=(const _T*& ptr) const noexcept { return &_ptr->_obj != ptr; }
    [[nodiscard]] inline bool operator ==(const nullptr_t) const noexcept { return false; }
    [[nodiscard]] inline bool operator !=(const nullptr_t) const noexcept { return true; }
};

template<typename _T>
class NullableReferenceCountingPointer final
{
private:
    template<typename _TT>
    using RCDO = _ReferenceCountingPointerUtils::_ReferenceCountDataObject<_TT>;
private:
    RCDO<_T>* _ptr;
public:
    template<typename... _Args>
    inline explicit NullableReferenceCountingPointer(_Args&&... args) noexcept
        : _ptr(new(::std::nothrow) RCDO<_T>(args))
    { }

    inline NullableReferenceCountingPointer(const nullptr_t) noexcept
        : _ptr(nullptr)
    { }

    inline ~NullableReferenceCountingPointer() noexcept
    {
        if(_ptr && --_ptr->_refCount == 0)
        { delete _ptr; }
    }

    inline NullableReferenceCountingPointer(const NullableReferenceCountingPointer<_T>& copy) noexcept
        : _ptr(copy._ptr)
    {
        if(_ptr)
        { ++_ptr->_refCount; }
    }

    inline NullableReferenceCountingPointer(NullableReferenceCountingPointer<_T>&& move) noexcept
        : _ptr(move._ptr)
    {
        if(_ptr)
        { ++_ptr->_refCount; }
    }

    inline NullableReferenceCountingPointer<_T>& operator=(const nullptr_t) noexcept
    {
        if(_ptr && --_ptr->_refCount == 0)
        { delete _ptr; }

        _ptr = nullptr;

        return *this;
    }

    inline NullableReferenceCountingPointer<_T>& operator=(const NullableReferenceCountingPointer<_T>& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(_ptr && --_ptr->_refCount == 0)
        { delete _ptr; }

        _ptr = copy._ptr;
        if(_ptr)
        { ++_ptr->_refCount; }

        return *this;
    }

    inline NullableReferenceCountingPointer<_T>& operator=(NullableReferenceCountingPointer<_T>&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(_ptr && --_ptr->_refCount == 0)
        { delete _ptr; }

        _ptr = move._ptr;
        if(_ptr)
        { ++_ptr->_refCount; }

        return *this;
    }

    [[nodiscard]] inline _T& operator *() { return _ptr->_obj; }
    [[nodiscard]] inline const _T& operator *() const { return _ptr->_obj; }
    [[nodiscard]] inline _T* operator ->() noexcept { return _ptr ? &_ptr->_obj : nullptr; }
    [[nodiscard]] inline const _T* operator ->() const noexcept { return _ptr ? &_ptr->_obj : nullptr; }

    [[nodiscard]] inline _T* get() noexcept { return _ptr ? &_ptr->_obj : nullptr; }
    [[nodiscard]] inline const _T* get() const noexcept { return _ptr ? &_ptr->_obj : nullptr; }

    [[nodiscard]] inline ::std::size_t refCount() const noexcept { return _ptr ? _ptr->_refCount : 0; }

    [[nodiscard]] inline operator bool() const noexcept { return _ptr; }

    [[nodiscard]] inline bool operator ==(const ReferenceCountingPointer<_T>& ptr) const noexcept
    {
        if(_ptr && ptr._ptr)
        { return _ptr == ptr._ptr; }
        if(!_ptr && !ptr._ptr) 
        { return true; }
        return false;
    }

    [[nodiscard]] inline bool operator !=(const ReferenceCountingPointer<_T>& ptr) const noexcept
    {
        if(_ptr && ptr._ptr)
        { return _ptr != ptr._ptr; }
        if(!_ptr && !ptr._ptr) 
        { return false; }
        return true;
    }

    [[nodiscard]] inline bool operator ==(const _T*& ptr) const noexcept
    {
        if(_ptr && ptr)
        { return &_ptr->_obj == ptr; }
        if(!_ptr && !ptr) 
        { return true; }
        return false;
    }

    [[nodiscard]] inline bool operator !=(const _T*& ptr) const noexcept
    {
        if(_ptr && ptr)
        { return &_ptr->_obj != ptr; }
        if(!_ptr && !ptr) 
        { return false; }
        return true;
    }

    [[nodiscard]] inline bool operator ==(const nullptr_t ptr) const noexcept { return _ptr == ptr; }
    [[nodiscard]] inline bool operator !=(const nullptr_t ptr) const noexcept { return _ptr != ptr; }
};

template<typename _T>
class StrongReferenceCountingPointer
{
private:
    template<typename _TT>
    using SWRC = _ReferenceCountingPointerUtils::_SWReferenceCount;
private:
    SWRC* _refCount;
    _T* _ptr;
public:
    template<typename... _Args>
    inline explicit StrongReferenceCountingPointer(_Args&&... args) noexcept
        : _refCount(new(::std::nothrow) SWRC), _ptr(new(::std::nothrow) _T(_ReferenceCountingPointerUtils::_forward(args)))
    { }

    inline ~StrongReferenceCountingPointer() noexcept
    {
        if(--_ptr->_stongRefCount == 0)
        {
            if(!_refCount->_weakRefCount)
            { delete _refCount; }
            delete _ptr;
        }
    }

    inline StrongReferenceCountingPointer(const StrongReferenceCountingPointer& copy) noexcept
        : _refCount(copy._refCount), _ptr(copy._ptr)
    { ++_refCount->_strongRefCount; }

    inline StrongReferenceCountingPointer(StrongReferenceCountingPointer&& move) noexcept
        : _refCount(move._refCount), _ptr(move._ptr)
    { ++_refCount->_strongRefCount; }

    inline StrongReferenceCountingPointer& operator=(const StrongReferenceCountingPointer& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(--_refCount->_strongRefCount == 0)
        {
            if(!_refCount->_weakRefCount)
            { delete _refCount; }
            delete _ptr;
        }

        _refCount = copy._refCount;
        _ptr = copy._ptr;
        ++_refCount->_strongRefCount;

        return *this;
    }

    inline StrongReferenceCountingPointer& operator=(StrongReferenceCountingPointer&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(--_ptr->_refCount == 0)
        {
            if(!_refCount->_weakRefCount)
            { delete _refCount; }
            delete _ptr;
        }

        _refCount = move._refCount;
        _ptr = move._ptr;
        ++_refCount->_strongRefCount;

        return *this;
    }

    [[nodiscard]] inline _T& operator *() noexcept { return _ptr; }
    [[nodiscard]] inline _T& operator *() const noexcept { return _ptr; }
    [[nodiscard]] inline _T* operator ->() noexcept { return _ptr; }
    [[nodiscard]] inline _T* operator ->() const noexcept { return _ptr; }

    [[nodiscard]] inline _T* get() noexcept { return _ptr; }
    [[nodiscard]] inline _T* get() const noexcept { return _ptr; }

    [[nodiscard]] inline ::std::size_t refCount() const noexcept { return _refCount->_strongRefCount; }
    [[nodiscard]] inline ::std::size_t strongRefCount() const noexcept { return _refCount->_strongRefCount; }
    [[nodiscard]] inline ::std::size_t weakRefCount() const noexcept { return _refCount->_weakRefCount; }

    [[nodiscard]] inline operator bool() const noexcept { return _ptr; }
    [[nodiscard]] inline bool operator ==(const StrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const StrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const _T*& ptr) const noexcept { return &_ptr == ptr; }
    [[nodiscard]] inline bool operator !=(const _T*& ptr) const noexcept { return &_ptr != ptr; }
    [[nodiscard]] inline bool operator ==(const nullptr_t) const noexcept { return false; }
    [[nodiscard]] inline bool operator !=(const nullptr_t) const noexcept { return true; }
};