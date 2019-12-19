#pragma once

#include "Objects.hpp"
#include "NumTypes.hpp"
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
using _RemoveReferenceT = typename _RemoveReference<_T>::type;

template<typename _T>
[[nodiscard]] constexpr inline _T&& _forward(_RemoveReferenceT<_T>& ref) noexcept
{
    return static_cast<_T&&>(ref);
}

    template<typename _T>
[[nodiscard]] constexpr inline _T&& _forward(_RemoveReferenceT<_T>&& ref) noexcept
{
    return static_cast<_T&&>(ref);
}

template<typename _T>
struct _ReferenceCountDataObject final
{
    DELETE_COPY(_ReferenceCountDataObject);
    DEFAULT_DESTRUCT(_ReferenceCountDataObject);
public:
    uSys _refCount;
    _T _obj;
public:
    template<typename... _Args>
    _ReferenceCountDataObject(_Args&&... args) noexcept
        : _refCount(1), _obj(_forward<_Args>(args)...)
    { }
};

template<typename _T>
struct _SWReferenceCount final
{
    DELETE_COPY(_SWReferenceCount);
    DEFAULT_DESTRUCT(_SWReferenceCount);
public:
    uSys _strongRefCount;
    uSys _weakRefCount;
    u8 _objRaw[sizeof(_T)];
public:
    template<typename... _Args>
    _SWReferenceCount(_Args&&... args) noexcept
        : _strongRefCount(1), _weakRefCount(0), _objRaw{}
    { (void) new(_objRaw) _T(_forward<_Args>(args)...); }

    [[nodiscard]] _T& obj() noexcept { return *reinterpret_cast<_T*>(_objRaw); }
    [[nodiscard]] const _T& obj() const noexcept { return *reinterpret_cast<_T*>(_objRaw); }

    [[nodiscard]] _T* objPtr() noexcept { return reinterpret_cast<_T*>(_objRaw); }
    [[nodiscard]] const _T* objPtr() const noexcept { return reinterpret_cast<_T*>(_objRaw); }

    void destroyObj() noexcept { reinterpret_cast<_T*>(_objRaw)->~_T(); }
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
        : _ptr(new(::std::nothrow) RCDO<_T>(args...))
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
        : _ptr(new(::std::nothrow) RCDO<_T>(args...))
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
class WeakReferenceCountingPointer;

template<typename _T>
class StrongReferenceCountingPointer final
{
private:
    template<typename _TT>
    using SWRC = _ReferenceCountingPointerUtils::_SWReferenceCount<_TT>;
private:
    SWRC<_T>* _ptr;
public:
    template<typename... _Args>
    inline explicit StrongReferenceCountingPointer(_Args&&... args) noexcept
        : _ptr(new(::std::nothrow) SWRC<_T>(args...))
    { }

    inline StrongReferenceCountingPointer(const WeakReferenceCountingPointer<_T>& ptr) noexcept;

    inline ~StrongReferenceCountingPointer() noexcept
    {
        if(--_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { delete _ptr; }
        }
    }

    inline StrongReferenceCountingPointer(const StrongReferenceCountingPointer& copy) noexcept
        : _ptr(copy._ptr)
    { ++_ptr->_strongRefCount; }

    inline StrongReferenceCountingPointer(StrongReferenceCountingPointer&& move) noexcept
        : _ptr(move._ptr)
    { ++_ptr->_strongRefCount; }

    inline StrongReferenceCountingPointer& operator=(const StrongReferenceCountingPointer& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(--_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { delete _ptr; }
        }

        _ptr = copy._ptr;
        ++_ptr->_strongRefCount;

        return *this;
    }

    inline StrongReferenceCountingPointer& operator=(StrongReferenceCountingPointer&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(--_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { delete _ptr; }
        }

        _ptr = move._ptr;
        ++_ptr->_strongRefCount;

        return *this;
    }

    [[nodiscard]] inline _T& operator *() noexcept { return _ptr->obj(); }
    [[nodiscard]] inline _T& operator *() const noexcept { return _ptr->obj(); }
    [[nodiscard]] inline _T* operator ->() noexcept { return _ptr->objPtr(); }
    [[nodiscard]] inline _T* operator ->() const noexcept { return _ptr->objPtr(); }

    [[nodiscard]] inline _T* get() noexcept { return _ptr->objPtr(); }
    [[nodiscard]] inline _T* get() const noexcept { return _ptr->objPtr(); }

    [[nodiscard]] inline ::std::size_t refCount() const noexcept { return _ptr->_strongRefCount; }
    [[nodiscard]] inline ::std::size_t strongRefCount() const noexcept { return _ptr->_strongRefCount; }
    [[nodiscard]] inline ::std::size_t weakRefCount() const noexcept { return _ptr->_weakRefCount; }

    [[nodiscard]] inline operator bool() const noexcept { return true; }
    [[nodiscard]] inline bool operator ==(const StrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const StrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const WeakReferenceCountingPointer<_T>& ptr) const noexcept;
    [[nodiscard]] inline bool operator !=(const WeakReferenceCountingPointer<_T>& ptr) const noexcept;
    [[nodiscard]] inline bool operator ==(const _T*& ptr) const noexcept { return _ptr->objPtr() == ptr; }
    [[nodiscard]] inline bool operator !=(const _T*& ptr) const noexcept { return _ptr->objPtr() != ptr; }
    [[nodiscard]] inline bool operator ==(const nullptr_t) const noexcept { return false; }
    [[nodiscard]] inline bool operator !=(const nullptr_t) const noexcept { return true; }
private:
    friend class WeakReferenceCountingPointer<_T>;
};

template<typename _T>
class WeakReferenceCountingPointer final
{
private:
    template<typename _TT>
    using SWRC = _ReferenceCountingPointerUtils::_SWReferenceCount<_TT>;
private:
    SWRC<_T>* _ptr;
public:
    inline WeakReferenceCountingPointer(const StrongReferenceCountingPointer<_T>& ptr) noexcept
        : _ptr(ptr._ptr)
    { ++_ptr->_weakRefCount; }

    inline ~WeakReferenceCountingPointer() noexcept
    {
        --_ptr->_weakRefCount;
        if(!_ptr->_stongRefCount)
        { delete _ptr; }
    }

    inline WeakReferenceCountingPointer(const WeakReferenceCountingPointer& copy) noexcept
        : _ptr(copy._ptr)
    { ++_ptr->_weakRefCount; }

    inline WeakReferenceCountingPointer(WeakReferenceCountingPointer&& move) noexcept
        : _ptr(move._ptr)
    { ++_ptr->_weakRefCount; }

    inline WeakReferenceCountingPointer& operator=(const WeakReferenceCountingPointer& copy) noexcept
    {
        if(this == &copy)
        { return *this; }
        
        --_ptr->_weakRefCount;
        if(!_ptr->_stongRefCount)
        { delete _ptr; }

        _ptr = copy._ptr;
        ++_ptr->_weakRefCount;

        return *this;
    }

    inline WeakReferenceCountingPointer& operator=(WeakReferenceCountingPointer&& move) noexcept
    {
        if(this == &move)
        { return *this; }
        
        --_ptr->_weakRefCount;
        if(!_ptr->_stongRefCount)
        { delete _ptr; }

        _ptr = move._ptr;
        ++_ptr->_weakRefCount;

        return *this;
    }

    [[nodiscard]] inline _T& operator *() { return _ptr->obj(); }
    [[nodiscard]] inline _T& operator *() const { return _ptr->obj(); }
    [[nodiscard]] inline _T* operator ->() noexcept { return _ptr->_strongRefCount ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] inline _T* operator ->() const noexcept { return _ptr->_strongRefCount ? _ptr->objPtr() : nullptr; }

    [[nodiscard]] inline _T* get() noexcept { return _ptr->_strongRefCount ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] inline _T* get() const noexcept { return _ptr->_strongRefCount ? _ptr->objPtr() : nullptr; }

    [[nodiscard]] inline ::std::size_t refCount() const noexcept { return _ptr->_weakRefCount; }
    [[nodiscard]] inline ::std::size_t strongRefCount() const noexcept { return _ptr->_strongRefCount; }
    [[nodiscard]] inline ::std::size_t weakRefCount() const noexcept { return _ptr->_weakRefCount; }

    [[nodiscard]] inline operator bool() const noexcept { return true; }
    [[nodiscard]] inline bool operator ==(const StrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const StrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const WeakReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const WeakReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const _T*& ptr) const noexcept { return _ptr->_strongRefCount && _ptr->objPtr() == ptr; }
    [[nodiscard]] inline bool operator !=(const _T*& ptr) const noexcept { return _ptr->_strongRefCount && _ptr->objPtr() != ptr; }
    [[nodiscard]] inline bool operator ==(const nullptr_t) const noexcept { return false; }
    [[nodiscard]] inline bool operator !=(const nullptr_t) const noexcept { return true; }
private:
    friend class StrongReferenceCountingPointer<_T>;
};

template<typename _T>
class NullableWeakReferenceCountingPointer;

template<typename _T>
class NullableStrongReferenceCountingPointer final
{
private:
    template<typename _TT>
    using SWRC = _ReferenceCountingPointerUtils::_SWReferenceCount<_TT>;
private:
    SWRC<_T>* _ptr;
public:
    template<typename... _Args>
    inline explicit NullableStrongReferenceCountingPointer(_Args&&... args) noexcept
        : _ptr(new(::std::nothrow) SWRC<_T>(args...))
    { }

    inline NullableStrongReferenceCountingPointer(const NullableWeakReferenceCountingPointer<_T>& ptr) noexcept;

    inline ~NullableStrongReferenceCountingPointer() noexcept
    {
        if(_ptr && --_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { delete _ptr; }
        }
    }

    inline NullableStrongReferenceCountingPointer(const NullableStrongReferenceCountingPointer& copy) noexcept
        : _ptr(copy._ptr)
    { ++_ptr->_strongRefCount; }

    inline NullableStrongReferenceCountingPointer(NullableStrongReferenceCountingPointer&& move) noexcept
        : _ptr(move._ptr)
    { ++_ptr->_strongRefCount; }

    inline NullableStrongReferenceCountingPointer& operator=(const NullableStrongReferenceCountingPointer& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(--_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { delete _ptr; }
        }

        _ptr = copy._ptr;
        ++_ptr->_strongRefCount;

        return *this;
    }

    inline NullableStrongReferenceCountingPointer& operator=(NullableStrongReferenceCountingPointer&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(--_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { delete _ptr; }
        }

        _ptr = move._ptr;
        ++_ptr->_strongRefCount;

        return *this;
    }

    inline NullableStrongReferenceCountingPointer<_T>& operator=(const nullptr_t) noexcept
    {
        if(_ptr && --_ptr->_strongRefCount == 0)
        { 
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { delete _ptr; }
        }

        _ptr = nullptr;

        return *this;
    }

    [[nodiscard]] inline _T& operator *() { return _ptr->obj(); }
    [[nodiscard]] inline _T& operator *() const { return _ptr->obj(); }
    [[nodiscard]] inline _T* operator ->() noexcept { return _ptr ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] inline _T* operator ->() const noexcept { return _ptr ? _ptr->objPtr() : nullptr; }

    [[nodiscard]] inline _T* get() noexcept { return _ptr ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] inline _T* get() const noexcept { return _ptr ? _ptr->objPtr() : nullptr; }

    [[nodiscard]] inline ::std::size_t refCount() const noexcept { return _ptr->_strongRefCount; }
    [[nodiscard]] inline ::std::size_t strongRefCount() const noexcept { return _ptr->_strongRefCount; }
    [[nodiscard]] inline ::std::size_t weakRefCount() const noexcept { return _ptr->_weakRefCount; }

    [[nodiscard]] inline operator bool() const noexcept { return true; }
    [[nodiscard]] inline bool operator ==(const NullableStrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const NullableStrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const NullableWeakReferenceCountingPointer<_T>& ptr) const noexcept;
    [[nodiscard]] inline bool operator !=(const NullableWeakReferenceCountingPointer<_T>& ptr) const noexcept;
    [[nodiscard]] inline bool operator ==(const _T*& ptr) const noexcept { return _ptr && _ptr->objPtr() == ptr; }
    [[nodiscard]] inline bool operator !=(const _T*& ptr) const noexcept { return _ptr && _ptr->objPtr() != ptr; }
    [[nodiscard]] inline bool operator ==(const nullptr_t) const noexcept { return !_ptr; }
    [[nodiscard]] inline bool operator !=(const nullptr_t) const noexcept { return _ptr; }
private:
    friend class NullableWeakReferenceCountingPointer<_T>;
};

template<typename _T>
class NullableWeakReferenceCountingPointer final
{
private:
    template<typename _TT>
    using SWRC = _ReferenceCountingPointerUtils::_SWReferenceCount<_TT>;
private:
    SWRC<_T>* _ptr;
public:
    inline NullableWeakReferenceCountingPointer(const NullableStrongReferenceCountingPointer<_T>& ptr) noexcept
        : _ptr(ptr._ptr)
    { ++_ptr->_weakRefCount; }

    inline ~NullableWeakReferenceCountingPointer() noexcept
    {
        if(_ptr)
        {
            --_ptr->_weakRefCount;
            if(!_ptr->_stongRefCount)
            { delete _ptr; }
        }
    }

    inline NullableWeakReferenceCountingPointer(const NullableWeakReferenceCountingPointer& copy) noexcept
        : _ptr(copy._ptr)
    { ++_ptr->_weakRefCount; }

    inline NullableWeakReferenceCountingPointer(NullableWeakReferenceCountingPointer&& move) noexcept
        : _ptr(move._ptr)
    { ++_ptr->_weakRefCount; }

    inline NullableWeakReferenceCountingPointer& operator=(const NullableWeakReferenceCountingPointer& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        --_ptr->_weakRefCount;
        if(!_ptr->_stongRefCount)
        { delete _ptr; }

        _ptr = copy._ptr;
        ++_ptr->_weakRefCount;

        return *this;
    }

    inline NullableWeakReferenceCountingPointer& operator=(NullableWeakReferenceCountingPointer&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        --_ptr->_weakRefCount;
        if(!_ptr->_stongRefCount)
        { delete _ptr; }

        _ptr = move._ptr;
        ++_ptr->_weakRefCount;

        return *this;
    }

    [[nodiscard]] inline _T& operator *() { return _ptr->obj(); }
    [[nodiscard]] inline _T& operator *() const { return _ptr->obj(); }
    [[nodiscard]] inline _T* operator ->() noexcept { return _ptr ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] inline _T* operator ->() const noexcept { return _ptr ? _ptr->objPtr() : nullptr; }

    [[nodiscard]] inline _T* get() noexcept { return _ptr ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] inline _T* get() const noexcept { return _ptr ? _ptr->objPtr() : nullptr; }

    [[nodiscard]] inline ::std::size_t refCount() const noexcept { return _ptr->_weakRefCount; }
    [[nodiscard]] inline ::std::size_t strongRefCount() const noexcept { return _ptr->_strongRefCount; }
    [[nodiscard]] inline ::std::size_t weakRefCount() const noexcept { return _ptr->_weakRefCount; }

    [[nodiscard]] inline operator bool() const noexcept { return true; }
    [[nodiscard]] inline bool operator ==(const NullableStrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const NullableStrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const NullableWeakReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const NullableWeakReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const _T*& ptr) const noexcept { return _ptr && _ptr->objPtr() == ptr; }
    [[nodiscard]] inline bool operator !=(const _T*& ptr) const noexcept { return _ptr && _ptr->objPtr() != ptr; }
    [[nodiscard]] inline bool operator ==(const nullptr_t) const noexcept { return !_ptr; }
    [[nodiscard]] inline bool operator !=(const nullptr_t) const noexcept { return _ptr; }
private:
    friend class NullableStrongReferenceCountingPointer<_T>;
};

template <typename _T>
StrongReferenceCountingPointer<_T>::StrongReferenceCountingPointer(const WeakReferenceCountingPointer<_T>& ptr) noexcept
    : _ptr(ptr._ptr)
{ ++_ptr->_strongRefCount; }

template <typename _T>
bool StrongReferenceCountingPointer<_T>::operator==(const WeakReferenceCountingPointer<_T>& ptr) const noexcept
{ return _ptr == ptr._ptr; }

template <typename _T>
bool StrongReferenceCountingPointer<_T>::operator!=(const WeakReferenceCountingPointer<_T>& ptr) const noexcept
{ return _ptr != ptr._ptr; }

template <typename _T>
NullableStrongReferenceCountingPointer<_T>::NullableStrongReferenceCountingPointer(const NullableWeakReferenceCountingPointer<_T>& ptr) noexcept
    : _ptr(ptr._ptr)
{ ++_ptr->_strongRefCount; }

template <typename _T>
bool NullableStrongReferenceCountingPointer<_T>::operator==(const NullableWeakReferenceCountingPointer<_T>& ptr) const noexcept
{ return _ptr == ptr._ptr; }

template <typename _T>
bool NullableStrongReferenceCountingPointer<_T>::operator!=(const NullableWeakReferenceCountingPointer<_T>& ptr) const noexcept
{ return _ptr != ptr._ptr; }
