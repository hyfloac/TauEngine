#pragma once

#include "Objects.hpp"
#include "NumTypes.hpp"
#include <memory>
#include "allocator/TauAllocator.hpp"

namespace _ReferenceCountingPointerUtils {

/**
 *   This does not store a direct _T because we may want to
 * cast it to a non-concrete interface, in which case the
 * compiler complains. By using a raw buffer we can convince
 * the compiler that it is still a reference to a concrete
 * type.
 */
template<typename _T>
struct _ReferenceCountDataObject final
{
    DELETE_COPY(_ReferenceCountDataObject);
public:
    uSys _refCount;
    TauAllocator& _allocator;
    u8 _objRaw[sizeof(_T)];
public:
    template<typename... _Args>
    _ReferenceCountDataObject(TauAllocator& allocator, _Args&&... args) noexcept
        : _refCount(1), _allocator(allocator), _objRaw{}
    { (void) new(_objRaw) _T(_TauAllocatorUtils::_forward<_Args>(args)...); }

    ~_ReferenceCountDataObject() noexcept
    { reinterpret_cast<_T*>(_objRaw)->~_T(); }

    [[nodiscard]] _T& obj() noexcept { return *reinterpret_cast<_T*>(_objRaw); }
    [[nodiscard]] const _T& obj() const noexcept { return *reinterpret_cast<_T*>(_objRaw); }

    [[nodiscard]] _T* objPtr() noexcept { return reinterpret_cast<_T*>(_objRaw); }
    [[nodiscard]] const _T* objPtr() const noexcept { return reinterpret_cast<_T*>(_objRaw); }
};

/**
 *   This does not store a direct _T because we may have to
 * destruct the value before the entire object is destroyed.
 */
template<typename _T>
struct _SWReferenceCount final
{
    DEFAULT_DESTRUCT(_SWReferenceCount);
    DELETE_COPY(_SWReferenceCount);
public:
    uSys _strongRefCount;
    uSys _weakRefCount;
    TauAllocator& _allocator;
    u8 _objRaw[sizeof(_T)];
public:
    template<typename... _Args>
    _SWReferenceCount(TauAllocator& allocator, _Args&&... args) noexcept
        : _strongRefCount(1), _weakRefCount(0), _allocator(allocator), _objRaw{}
    { (void) new(_objRaw) _T(_TauAllocatorUtils::_forward<_Args>(args)...); }

    [[nodiscard]] _T& obj() noexcept { return *reinterpret_cast<_T*>(_objRaw); }
    [[nodiscard]] const _T& obj() const noexcept { return *reinterpret_cast<_T*>(_objRaw); }

    [[nodiscard]] _T* objPtr() noexcept { return reinterpret_cast<_T*>(_objRaw); }
    [[nodiscard]] const _T* objPtr() const noexcept { return reinterpret_cast<_T*>(_objRaw); }

    void destroyObj() noexcept { reinterpret_cast<_T*>(_objRaw)->~_T(); }
};

template<typename _ToT, typename _FromT>
const _ReferenceCountDataObject<_ToT>& RCDOCast(const _ReferenceCountDataObject<_FromT>& obj) noexcept
{ return reinterpret_cast<const _ReferenceCountDataObject<_ToT>&>(obj); }

template<typename _ToT, typename _FromT>
_ReferenceCountDataObject<_ToT>& RCDOCast(_ReferenceCountDataObject<_FromT>& obj) noexcept
{ return reinterpret_cast<const _ReferenceCountDataObject<_ToT>&>(obj); }

template<typename _ToT, typename _FromT>
_ReferenceCountDataObject<_ToT>&& RCDOCast(_ReferenceCountDataObject<_FromT>&& obj) noexcept
{ return reinterpret_cast<const _ReferenceCountDataObject<_ToT>&&>(obj); }

template<typename _ToT, typename _FromT>
const _SWReferenceCount<_ToT>& SWRCCast(const _SWReferenceCount<_FromT>& obj) noexcept
{ return reinterpret_cast<const _SWReferenceCount<_ToT>&>(obj); }

template<typename _ToT, typename _FromT>
_SWReferenceCount<_ToT>& SWRCCast(_SWReferenceCount<_FromT>& obj) noexcept
{ return reinterpret_cast<const _SWReferenceCount<_ToT>&>(obj); }

template<typename _ToT, typename _FromT>
_SWReferenceCount<_ToT>&& SWRCCast(_SWReferenceCount<_FromT>&& obj) noexcept
{ return reinterpret_cast<const _SWReferenceCount<_ToT>&&>(obj); }

}

template<typename _T>
class NullableReferenceCountingPointer;

template<typename _T>
class ReferenceCountingPointer final
{
public:
    template<typename _TT>
    using RCDO = _ReferenceCountingPointerUtils::_ReferenceCountDataObject<_TT>;
private:
    RCDO<_T>* _ptr;
public:
    template<typename... _Args>
    inline explicit ReferenceCountingPointer(TauAllocator& allocator, _Args&&... args) noexcept
        : _ptr(allocator.allocateT<RCDO<_T>>(allocator, args...))
    { }

    template<typename _TT>
    inline ReferenceCountingPointer(const ReferenceCountingPointer<_TT>& rcp) noexcept
        : _ptr(reinterpret_cast<RCDO<_T>*>(rcp.get()))
    { ++_ptr->_refCount; }

    inline ~ReferenceCountingPointer() noexcept
    {
        if(--_ptr->_refCount == 0)
        { _ptr->_allocator.deallocateT(_ptr); }
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
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = copy._ptr;
        ++_ptr->_refCount;

        return *this;
    }

    inline ReferenceCountingPointer<_T>& operator=(ReferenceCountingPointer<_T>&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(--_ptr->_refCount == 0)
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = move._ptr;
        ++_ptr->_refCount;

        return *this;
    }

    inline ReferenceCountingPointer<_T>& operator=(const NullableReferenceCountingPointer<_T>& copy) noexcept;
    inline ReferenceCountingPointer<_T>& operator=(NullableReferenceCountingPointer<_T>&& move) noexcept;

    template<typename... _Args>
    inline void reset(TauAllocator& allocator, _Args&&... args) noexcept
    {
        if(--_ptr->_refCount == 0)
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = allocator.allocateT<RCDO<_T>>(allocator, args...);
    }

    [[nodiscard]] inline       _T& operator  *()       noexcept { return _ptr->obj();    }
    [[nodiscard]] inline const _T& operator  *() const noexcept { return _ptr->obj();    }
    [[nodiscard]] inline       _T* operator ->()       noexcept { return _ptr->objPtr(); }
    [[nodiscard]] inline const _T* operator ->() const noexcept { return _ptr->objPtr(); }

    [[nodiscard]] inline       _T* get()       noexcept { return _ptr->objPtr(); }
    [[nodiscard]] inline const _T* get() const noexcept { return _ptr->objPtr(); }

    [[nodiscard]] inline ::std::size_t refCount() const noexcept { return _ptr->_refCount; }

    [[nodiscard]] inline RCDO<_T>* _getBlock() const noexcept { return _ptr; }

    [[nodiscard]] inline operator bool() const noexcept { return true; }
    [[nodiscard]] inline bool operator ==(const ReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const ReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const NullableReferenceCountingPointer<_T>& ptr) const noexcept;
    [[nodiscard]] inline bool operator !=(const NullableReferenceCountingPointer<_T>& ptr) const noexcept;
    [[nodiscard]] inline bool operator ==(const _T*& ptr)  const noexcept { return _ptr->objPtr() == ptr; }
    [[nodiscard]] inline bool operator !=(const _T*& ptr)  const noexcept { return _ptr->objPtr() != ptr; }
    [[nodiscard]] inline bool operator ==(const nullptr_t) const noexcept { return false; }
    [[nodiscard]] inline bool operator !=(const nullptr_t) const noexcept { return true;  }
private:
    friend class NullableReferenceCountingPointer<_T>;
};

template<typename _T>
class NullableReferenceCountingPointer final
{
public:
    template<typename _TT>
    using RCDO = _ReferenceCountingPointerUtils::_ReferenceCountDataObject<_TT>;
private:
    RCDO<_T>* _ptr;
public:
    template<typename... _Args>
    inline explicit NullableReferenceCountingPointer(TauAllocator& allocator, _Args&&... args) noexcept
        : _ptr(allocator.allocateT<RCDO<_T>>(allocator, args...))
    { }

    template<typename _TT>
    inline NullableReferenceCountingPointer(const NullableReferenceCountingPointer<_TT> & rcp) noexcept
        : _ptr(reinterpret_cast<RCDO<_T>*>(rcp._getBlock()))
    {
        if(_ptr) { ++_ptr->_refCount; }
    }

    template<typename _TT>
    inline NullableReferenceCountingPointer(const ReferenceCountingPointer<_TT> & rcp) noexcept
        : _ptr(reinterpret_cast<RCDO<_T>*>(rcp._getBlock()))
    { ++_ptr->_refCount; }

    inline NullableReferenceCountingPointer(const nullptr_t) noexcept
        : _ptr(nullptr)
    { }

    inline NullableReferenceCountingPointer() noexcept
        : _ptr(nullptr)
    { }

    inline ~NullableReferenceCountingPointer() noexcept
    {
        if(_ptr && --_ptr->_refCount == 0)
        { _ptr->_allocator.deallocateT(_ptr); }
    }

    inline NullableReferenceCountingPointer(const NullableReferenceCountingPointer<_T>& copy) noexcept
        : _ptr(copy._ptr)
    {
        if(_ptr) { ++_ptr->_refCount; }
    }

    inline NullableReferenceCountingPointer(NullableReferenceCountingPointer<_T>&& move) noexcept
        : _ptr(move._ptr)
    {
        if(_ptr) { ++_ptr->_refCount; }
    }

    inline NullableReferenceCountingPointer(const ReferenceCountingPointer<_T>& copy) noexcept
        : _ptr(copy._ptr)
    { ++_ptr->_refCount; }

    inline NullableReferenceCountingPointer(ReferenceCountingPointer<_T>&& move) noexcept
        : _ptr(move._ptr)
    { ++_ptr->_refCount; }

    inline NullableReferenceCountingPointer<_T>& operator=(const nullptr_t) noexcept
    {
        if(_ptr && --_ptr->_refCount == 0)
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = nullptr;

        return *this;
    }

    inline NullableReferenceCountingPointer<_T>& operator=(const NullableReferenceCountingPointer<_T>& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(_ptr && --_ptr->_refCount == 0)
        { _ptr->_allocator.deallocateT(_ptr); }

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
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = move._ptr;
        if(_ptr)
        { ++_ptr->_refCount; }

        return *this;
    }

    inline NullableReferenceCountingPointer<_T>& operator=(const ReferenceCountingPointer<_T>& copy) noexcept
    {
        if(_ptr && --_ptr->_refCount == 0)
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = copy._ptr;
        ++_ptr->_refCount;

        return *this;
    }

    inline NullableReferenceCountingPointer<_T>& operator=(ReferenceCountingPointer<_T>&& move) noexcept
    {
        if(_ptr && --_ptr->_refCount == 0)
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = move._ptr;
        ++_ptr->_refCount;

        return *this;
    }


    template<typename... _Args>
    inline void reset(TauAllocator& allocator, _Args&&... args) noexcept
    {
        if(_ptr && --_ptr->_refCount == 0)
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = allocator.allocateT<RCDO<_T>>(allocator, args...);
    }

    [[nodiscard]] inline       _T& operator  *()                { return _ptr->obj(); }
    [[nodiscard]] inline const _T& operator  *() const          { return _ptr->obj(); }
    [[nodiscard]] inline       _T* operator ->()       noexcept { return _ptr ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] inline const _T* operator ->() const noexcept { return _ptr ? _ptr->objPtr() : nullptr; }

    [[nodiscard]] inline       _T* get()       noexcept { return _ptr ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] inline const _T* get() const noexcept { return _ptr ? _ptr->objPtr() : nullptr; }

    [[nodiscard]] inline ::std::size_t refCount() const noexcept { return _ptr ? _ptr->_refCount : 0; }

    [[nodiscard]] inline RCDO<_T>* _getBlock() const noexcept { return _ptr; }

    [[nodiscard]] inline operator bool() const noexcept { return _ptr; }

    [[nodiscard]] inline bool operator ==(const ReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const ReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr != ptr._ptr; }

    [[nodiscard]] inline bool operator ==(const NullableReferenceCountingPointer<_T>& ptr) const noexcept
    {
        if(_ptr && ptr._ptr)
        { return _ptr == ptr._ptr; }
        if(!_ptr && !ptr._ptr)
        { return true; }
        return false;
    }

    [[nodiscard]] inline bool operator !=(const NullableReferenceCountingPointer<_T>& ptr) const noexcept
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
        { return _ptr->objPtr() == ptr; }
        if(!_ptr && !ptr) 
        { return true; }
        return false;
    }

    [[nodiscard]] inline bool operator !=(const _T*& ptr) const noexcept
    {
        if(_ptr && ptr)
        { return _ptr->objPtr() != ptr; }
        if(!_ptr && !ptr) 
        { return false; }
        return true;
    }

    [[nodiscard]] inline bool operator ==(const nullptr_t ptr) const noexcept { return _ptr == ptr; }
    [[nodiscard]] inline bool operator !=(const nullptr_t ptr) const noexcept { return _ptr != ptr; }
private:
    friend class ReferenceCountingPointer<_T>;
};

template<typename _T>
class WeakReferenceCountingPointer;

template<typename _T>
class NullableStrongReferenceCountingPointer;

template<typename _T>
class NullableWeakReferenceCountingPointer;

template<typename _T>
class StrongReferenceCountingPointer final
{
public:
    template<typename _TT>
    using SWRC = _ReferenceCountingPointerUtils::_SWReferenceCount<_TT>;
private:
    SWRC<_T>* _ptr;
public:
    template<typename... _Args>
    inline explicit StrongReferenceCountingPointer(TauAllocator& allocator, _Args&&... args) noexcept
        : _ptr(allocator.allocateT<SWRC<_T>>(allocator, args...))
    { }

    template<typename _TT>
    inline StrongReferenceCountingPointer(const StrongReferenceCountingPointer<_TT> & rcp) noexcept
        : _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    { ++_ptr->_strongRefCount; }

    inline ~StrongReferenceCountingPointer() noexcept
    {
        if(--_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { _ptr->_allocator.deallocateT(_ptr); }
        }
    }

    inline StrongReferenceCountingPointer(const StrongReferenceCountingPointer<_T>& copy) noexcept
        : _ptr(copy._ptr)
    { ++_ptr->_strongRefCount; }

    inline StrongReferenceCountingPointer(StrongReferenceCountingPointer<_T>&& move) noexcept
        : _ptr(move._ptr)
    { ++_ptr->_strongRefCount; }

    inline StrongReferenceCountingPointer<_T>& operator=(const StrongReferenceCountingPointer<_T>& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(--_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { _ptr->_allocator.deallocateT(_ptr); }
        }

        _ptr = copy._ptr;
        ++_ptr->_strongRefCount;

        return *this;
    }

    inline StrongReferenceCountingPointer<_T>& operator=(StrongReferenceCountingPointer<_T>&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(--_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { _ptr->_allocator.deallocateT(_ptr); }
        }

        _ptr = move._ptr;
        ++_ptr->_strongRefCount;

        return *this;
    }

    inline StrongReferenceCountingPointer<_T>& operator=(const WeakReferenceCountingPointer<_T>& copy) noexcept;
    inline StrongReferenceCountingPointer<_T>& operator=(WeakReferenceCountingPointer<_T>&& move) noexcept;
    inline StrongReferenceCountingPointer<_T>& operator=(const NullableStrongReferenceCountingPointer<_T>& copy) noexcept;
    inline StrongReferenceCountingPointer<_T>& operator=(NullableStrongReferenceCountingPointer<_T>&& move) noexcept;
    inline StrongReferenceCountingPointer<_T>& operator=(const NullableWeakReferenceCountingPointer<_T>& copy) noexcept;
    inline StrongReferenceCountingPointer<_T>& operator=(NullableWeakReferenceCountingPointer<_T>&& move) noexcept;

    template<typename... _Args>
    inline void reset(TauAllocator& allocator, _Args&&... args) noexcept
    {
        if(--_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { _ptr->_allocator.deallocateT(_ptr); }
        }

        _ptr = allocator.allocateT<SWRC<_T>>(allocator, args...);
    }

    [[nodiscard]] inline       _T& operator  *()       noexcept { return _ptr->obj();    }
    [[nodiscard]] inline const _T& operator  *() const noexcept { return _ptr->obj();    }
    [[nodiscard]] inline       _T* operator ->()       noexcept { return _ptr->objPtr(); }
    [[nodiscard]] inline const _T* operator ->() const noexcept { return _ptr->objPtr(); }

    [[nodiscard]] inline       _T* get()       noexcept { return _ptr->objPtr(); }
    [[nodiscard]] inline const _T* get() const noexcept { return _ptr->objPtr(); }

    [[nodiscard]] inline ::std::size_t refCount()       const noexcept { return _ptr->_strongRefCount; }
    [[nodiscard]] inline ::std::size_t strongRefCount() const noexcept { return _ptr->_strongRefCount; }
    [[nodiscard]] inline ::std::size_t weakRefCount()   const noexcept { return _ptr->_weakRefCount;   }

    [[nodiscard]] inline SWRC<_T>* _getBlock() const noexcept { return _ptr; }

    [[nodiscard]] inline operator bool() const noexcept { return true; }
    [[nodiscard]] inline bool operator ==(const StrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const StrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const WeakReferenceCountingPointer<_T>& ptr) const noexcept;
    [[nodiscard]] inline bool operator !=(const WeakReferenceCountingPointer<_T>& ptr) const noexcept;
    [[nodiscard]] inline bool operator ==(const NullableStrongReferenceCountingPointer<_T>& ptr) const noexcept;
    [[nodiscard]] inline bool operator !=(const NullableStrongReferenceCountingPointer<_T>& ptr) const noexcept;
    [[nodiscard]] inline bool operator ==(const NullableWeakReferenceCountingPointer<_T>& ptr) const noexcept;
    [[nodiscard]] inline bool operator !=(const NullableWeakReferenceCountingPointer<_T>& ptr) const noexcept;
    [[nodiscard]] inline bool operator ==(const _T*& ptr) const noexcept { return _ptr->objPtr() == ptr; }
    [[nodiscard]] inline bool operator !=(const _T*& ptr) const noexcept { return _ptr->objPtr() != ptr; }
    [[nodiscard]] inline bool operator ==(const nullptr_t) const noexcept { return false; }
    [[nodiscard]] inline bool operator !=(const nullptr_t) const noexcept { return true; }
private:
    friend class WeakReferenceCountingPointer<_T>;
    friend class NullableStrongReferenceCountingPointer<_T>;
    friend class NullableWeakReferenceCountingPointer<_T>;
};

template<typename _T>
class WeakReferenceCountingPointer final
{
public:
    template<typename _TT>
    using SWRC = _ReferenceCountingPointerUtils::_SWReferenceCount<_TT>;
private:
    SWRC<_T>* _ptr;
public:
    inline WeakReferenceCountingPointer(const StrongReferenceCountingPointer<_T>& ptr) noexcept
        : _ptr(ptr._ptr)
    { ++_ptr->_weakRefCount; }

    template<typename _TT>
    inline WeakReferenceCountingPointer(const StrongReferenceCountingPointer<_TT> & rcp) noexcept
        : _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    { ++_ptr->_weakRefCount; }

    template<typename _TT>
    inline WeakReferenceCountingPointer(const WeakReferenceCountingPointer<_TT> & rcp) noexcept
        : _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    { ++_ptr->_weakRefCount; }

    inline ~WeakReferenceCountingPointer() noexcept
    {
        if(--_ptr->_weakRefCount == 0 && !_ptr->_stongRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }
    }

    inline WeakReferenceCountingPointer(const WeakReferenceCountingPointer<_T>& copy) noexcept
        : _ptr(copy._ptr)
    { ++_ptr->_weakRefCount; }

    inline WeakReferenceCountingPointer(WeakReferenceCountingPointer<_T>&& move) noexcept
        : _ptr(move._ptr)
    { ++_ptr->_weakRefCount; }

    inline WeakReferenceCountingPointer<_T>& operator=(const WeakReferenceCountingPointer<_T>& copy) noexcept
    {
        if(this == &copy)
        { return *this; }
        
        if(--_ptr->_weakRefCount == 0 && !_ptr->_stongRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = copy._ptr;
        ++_ptr->_weakRefCount;

        return *this;
    }

    inline WeakReferenceCountingPointer<_T>& operator=(WeakReferenceCountingPointer<_T>&& move) noexcept
    {
        if(this == &move)
        { return *this; }
        
        if(--_ptr->_weakRefCount == 0 && !_ptr->_stongRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = move._ptr;
        ++_ptr->_weakRefCount;

        return *this;
    }

    inline WeakReferenceCountingPointer<_T>& operator=(const StrongReferenceCountingPointer<_T>& copy) noexcept
    {
        if(--_ptr->_weakRefCount == 0 && !_ptr->_stongRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = copy._ptr;
        ++_ptr->_weakRefCount;

        return *this;
    }

    inline WeakReferenceCountingPointer<_T>& operator=(StrongReferenceCountingPointer<_T>&& move) noexcept
    {
        if(--_ptr->_weakRefCount == 0 && !_ptr->_stongRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = move._ptr;
        ++_ptr->_weakRefCount;

        return *this;
    }

    inline WeakReferenceCountingPointer<_T>& operator=(const NullableStrongReferenceCountingPointer<_T>& copy) noexcept;
    inline WeakReferenceCountingPointer<_T>& operator=(NullableStrongReferenceCountingPointer<_T>&& move) noexcept;
    inline WeakReferenceCountingPointer<_T>& operator=(const NullableWeakReferenceCountingPointer<_T>& copy) noexcept;
    inline WeakReferenceCountingPointer<_T>& operator=(NullableWeakReferenceCountingPointer<_T>&& move) noexcept;

    [[nodiscard]] inline       _T& operator  *()                { return _ptr->obj(); }
    [[nodiscard]] inline const _T& operator  *() const          { return _ptr->obj(); }
    [[nodiscard]] inline       _T* operator ->()       noexcept { return _ptr->_strongRefCount ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] inline const _T* operator ->() const noexcept { return _ptr->_strongRefCount ? _ptr->objPtr() : nullptr; }

    [[nodiscard]] inline       _T* get()       noexcept { return _ptr->_strongRefCount ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] inline const _T* get() const noexcept { return _ptr->_strongRefCount ? _ptr->objPtr() : nullptr; }

    [[nodiscard]] inline ::std::size_t refCount()       const noexcept { return _ptr ? _ptr->_weakRefCount   : 0; }
    [[nodiscard]] inline ::std::size_t strongRefCount() const noexcept { return _ptr ? _ptr->_strongRefCount : 0; }
    [[nodiscard]] inline ::std::size_t weakRefCount()   const noexcept { return _ptr ? _ptr->_weakRefCount   : 0; }

    [[nodiscard]] inline SWRC<_T>* _getBlock() const noexcept { return _ptr; }

    [[nodiscard]] inline operator bool() const noexcept { return _ptr->_strongRefCount; }
    [[nodiscard]] inline bool operator ==(const StrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const StrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const WeakReferenceCountingPointer<_T>& ptr)   const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const WeakReferenceCountingPointer<_T>& ptr)   const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const NullableStrongReferenceCountingPointer<_T>& ptr) const noexcept;
    [[nodiscard]] inline bool operator !=(const NullableStrongReferenceCountingPointer<_T>& ptr) const noexcept;
    [[nodiscard]] inline bool operator ==(const NullableWeakReferenceCountingPointer<_T>& ptr) const noexcept;
    [[nodiscard]] inline bool operator !=(const NullableWeakReferenceCountingPointer<_T>& ptr) const noexcept;
    [[nodiscard]] inline bool operator ==(const _T*& ptr) const noexcept { return _ptr->_strongRefCount && _ptr->objPtr() == ptr; }
    [[nodiscard]] inline bool operator !=(const _T*& ptr) const noexcept { return _ptr->_strongRefCount && _ptr->objPtr() != ptr; }
    [[nodiscard]] inline bool operator ==(const nullptr_t) const noexcept { return !_ptr->_strongRefCount; }
    [[nodiscard]] inline bool operator !=(const nullptr_t) const noexcept { return  _ptr->_strongRefCount; }
private:
    friend class StrongReferenceCountingPointer<_T>;
    friend class NullableStrongReferenceCountingPointer<_T>;
    friend class NullableWeakReferenceCountingPointer<_T>;
};

template<typename _T>
class NullableStrongReferenceCountingPointer final
{
public:
    template<typename _TT>
    using SWRC = _ReferenceCountingPointerUtils::_SWReferenceCount<_TT>;
private:
    SWRC<_T>* _ptr;
public:
    template<typename... _Args>
    inline explicit NullableStrongReferenceCountingPointer(TauAllocator& allocator, _Args&&... args) noexcept
        : _ptr(allocator.allocateT<SWRC<_T>>(allocator, args...))
    { }

    template<typename _TT>
    inline NullableStrongReferenceCountingPointer(const StrongReferenceCountingPointer<_TT> & rcp) noexcept
        : _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    { ++_ptr->_strongRefCount; }

    template<typename _TT>
    inline NullableStrongReferenceCountingPointer(const NullableStrongReferenceCountingPointer<_TT> & rcp) noexcept
        : _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    {
        if(_ptr) { ++_ptr->_strongRefCount; }
    }

    inline NullableStrongReferenceCountingPointer(const nullptr_t) noexcept
        : _ptr(nullptr)
    { }

    inline NullableStrongReferenceCountingPointer() noexcept
        : _ptr(nullptr)
    { }

    inline ~NullableStrongReferenceCountingPointer() noexcept
    {
        if(_ptr && --_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { _ptr->_allocator.deallocateT(_ptr); }
        }
    }

    inline NullableStrongReferenceCountingPointer(const NullableStrongReferenceCountingPointer<_T>& copy) noexcept
        : _ptr(copy._ptr)
    {
        if(_ptr) { ++_ptr->_strongRefCount; }
    }

    inline NullableStrongReferenceCountingPointer(NullableStrongReferenceCountingPointer<_T>&& move) noexcept
        : _ptr(move._ptr)
    {
        if(_ptr) { ++_ptr->_strongRefCount; }
    }

    inline NullableStrongReferenceCountingPointer(const StrongReferenceCountingPointer<_T>& copy) noexcept
        : _ptr(copy._ptr)
    { ++_ptr->_strongRefCount; }

    inline NullableStrongReferenceCountingPointer(StrongReferenceCountingPointer<_T>&& move) noexcept
        : _ptr(move._ptr)
    { ++_ptr->_strongRefCount; }

    inline NullableStrongReferenceCountingPointer<_T>& operator=(const NullableStrongReferenceCountingPointer<_T>& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(_ptr && --_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { _ptr->_allocator.deallocateT(_ptr); }
        }

        _ptr = copy._ptr;
        if(_ptr) { ++_ptr->_strongRefCount; }

        return *this;
    }

    inline NullableStrongReferenceCountingPointer<_T>& operator=(NullableStrongReferenceCountingPointer<_T>&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(_ptr && --_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { _ptr->_allocator.deallocateT(_ptr); }
        }

        _ptr = move._ptr;
        if(_ptr) { ++_ptr->_strongRefCount; }

        return *this;
    }
    
    inline NullableStrongReferenceCountingPointer<_T>& operator=(const StrongReferenceCountingPointer<_T>& copy) noexcept
    {
        if(_ptr && --_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { _ptr->_allocator.deallocateT(_ptr); }
        }

        _ptr = copy._ptr;
        ++_ptr->_strongRefCount;

        return *this;
    }

    inline NullableStrongReferenceCountingPointer<_T>& operator=(StrongReferenceCountingPointer<_T>&& move) noexcept
    {
        if(_ptr && --_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { _ptr->_allocator.deallocateT(_ptr); }
        }

        _ptr = move._ptr;
        ++_ptr->_strongRefCount;

        return *this;
    }

    inline NullableStrongReferenceCountingPointer<_T>& operator=(const WeakReferenceCountingPointer<_T>& copy) noexcept
    {
        if(_ptr && --_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { _ptr->_allocator.deallocateT(_ptr); }
        }

        if(copy)
        {
            _ptr = copy._ptr;
            ++_ptr->_strongRefCount;
        }
        else
        { _ptr = nullptr; }

        return *this;
    }

    inline NullableStrongReferenceCountingPointer<_T>& operator=(WeakReferenceCountingPointer<_T>&& move) noexcept
    {
        if(_ptr && --_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { _ptr->_allocator.deallocateT(_ptr); }
        }

        if(move)
        {
            _ptr = move._ptr;
            ++_ptr->_strongRefCount;
        }
        else
        { _ptr = nullptr; }

        return *this;
    }

    inline NullableStrongReferenceCountingPointer<_T>& operator=(const NullableWeakReferenceCountingPointer<_T>& copy) noexcept;
    inline NullableStrongReferenceCountingPointer<_T>& operator=(NullableWeakReferenceCountingPointer<_T>&& move) noexcept;

    inline NullableStrongReferenceCountingPointer<_T>& operator=(const nullptr_t) noexcept
    {
        if(_ptr && --_ptr->_strongRefCount == 0)
        { 
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { _ptr->_allocator.deallocateT(_ptr); }
        }

        _ptr = nullptr;

        return *this;
    }

    template<typename... _Args>
    inline void reset(TauAllocator& allocator, _Args&&... args) noexcept
    {
        if(_ptr && --_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { _ptr->_allocator.deallocateT(_ptr); }
        }

        _ptr = allocator.allocateT<SWRC<_T>>(allocator, args...);
    }

    [[nodiscard]] inline       _T& operator  *()                { return _ptr->obj(); }
    [[nodiscard]] inline const _T& operator  *() const          { return _ptr->obj(); }
    [[nodiscard]] inline       _T* operator ->()       noexcept { return _ptr ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] inline const _T* operator ->() const noexcept { return _ptr ? _ptr->objPtr() : nullptr; }

    [[nodiscard]] inline       _T* get()       noexcept { return _ptr ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] inline const _T* get() const noexcept { return _ptr ? _ptr->objPtr() : nullptr; }

    [[nodiscard]] inline ::std::size_t refCount()       const noexcept { return _ptr ? _ptr->_strongRefCount : 0; }
    [[nodiscard]] inline ::std::size_t strongRefCount() const noexcept { return _ptr ? _ptr->_strongRefCount : 0; }
    [[nodiscard]] inline ::std::size_t weakRefCount()   const noexcept { return _ptr ? _ptr->_weakRefCount   : 0; }

    [[nodiscard]] inline SWRC<_T>* _getBlock() const noexcept { return _ptr; }

    [[nodiscard]] inline operator bool() const noexcept { return _ptr; }
    [[nodiscard]] inline bool operator ==(const StrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const StrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const WeakReferenceCountingPointer<_T>& ptr)   const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const WeakReferenceCountingPointer<_T>& ptr)   const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const NullableStrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const NullableStrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const NullableWeakReferenceCountingPointer<_T>& ptr)   const noexcept;
    [[nodiscard]] inline bool operator !=(const NullableWeakReferenceCountingPointer<_T>& ptr)   const noexcept;
    [[nodiscard]] inline bool operator ==(const _T*& ptr) const noexcept { return _ptr && _ptr->objPtr() == ptr; }
    [[nodiscard]] inline bool operator !=(const _T*& ptr) const noexcept { return _ptr && _ptr->objPtr() != ptr; }
    [[nodiscard]] inline bool operator ==(const nullptr_t) const noexcept { return !_ptr; }
    [[nodiscard]] inline bool operator !=(const nullptr_t) const noexcept { return  _ptr; }
private:
    friend class StrongReferenceCountingPointer<_T>;
    friend class WeakReferenceCountingPointer<_T>;
    friend class NullableWeakReferenceCountingPointer<_T>;
};

template<typename _T>
class NullableWeakReferenceCountingPointer final
{
public:
    template<typename _TT>
    using SWRC = _ReferenceCountingPointerUtils::_SWReferenceCount<_TT>;
private:
    SWRC<_T>* _ptr;
public:
    inline NullableWeakReferenceCountingPointer(const NullableStrongReferenceCountingPointer<_T>& ptr) noexcept
        : _ptr(ptr._ptr)
    {
        if(_ptr) { ++_ptr->_weakRefCount; }
    }

    inline NullableWeakReferenceCountingPointer(const StrongReferenceCountingPointer<_T>& ptr) noexcept
        : _ptr(ptr._ptr)
    { ++_ptr->_weakRefCount; }

    template<typename _TT>
    inline NullableWeakReferenceCountingPointer(const NullableStrongReferenceCountingPointer<_TT> & rcp) noexcept
        : _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    {
        if(_ptr) { ++_ptr->_weakRefCount; }
    }

    template<typename _TT>
    inline NullableWeakReferenceCountingPointer(const StrongReferenceCountingPointer<_TT> & rcp) noexcept
        : _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    { ++_ptr->_weakRefCount; }

    template<typename _TT>
    inline NullableWeakReferenceCountingPointer(const NullableWeakReferenceCountingPointer<_TT> & rcp) noexcept
        : _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    {
        if(_ptr) { ++_ptr->_weakRefCount; }
    }

    template<typename _TT>
    inline NullableWeakReferenceCountingPointer(const WeakReferenceCountingPointer<_TT> & rcp) noexcept
        : _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    { ++_ptr->_weakRefCount; }

    inline NullableWeakReferenceCountingPointer(const nullptr_t) noexcept
        : _ptr(nullptr)
    { }

    inline NullableWeakReferenceCountingPointer() noexcept
        : _ptr(nullptr)
    { }

    inline ~NullableWeakReferenceCountingPointer() noexcept
    {
        if(_ptr && --_ptr->_weakRefCount == 0 && !_ptr->_stongRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }
    }

    inline NullableWeakReferenceCountingPointer(const NullableWeakReferenceCountingPointer<_T>& copy) noexcept
        : _ptr(copy._ptr)
    {
        if(_ptr) { ++_ptr->_weakRefCount; }
    }

    inline NullableWeakReferenceCountingPointer(NullableWeakReferenceCountingPointer<_T>&& move) noexcept
        : _ptr(move._ptr)
    {
        if(_ptr) { ++_ptr->_weakRefCount; }
    }

    inline NullableWeakReferenceCountingPointer(const WeakReferenceCountingPointer<_T>& copy) noexcept
        : _ptr(copy._ptr)
    { ++_ptr->_weakRefCount; }

    inline NullableWeakReferenceCountingPointer(WeakReferenceCountingPointer<_T>&& move) noexcept
        : _ptr(move._ptr)
    { ++_ptr->_weakRefCount; }

    inline NullableWeakReferenceCountingPointer<_T>& operator=(const NullableWeakReferenceCountingPointer<_T>& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(--_ptr->_weakRefCount == 0 && !_ptr->_stongRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = copy._ptr;

        if(_ptr) { ++_ptr->_weakRefCount; }

        return *this;
    }

    inline NullableWeakReferenceCountingPointer<_T>& operator=(NullableWeakReferenceCountingPointer<_T>&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(--_ptr->_weakRefCount == 0 && !_ptr->_stongRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = move._ptr;
        if(_ptr) { ++_ptr->_weakRefCount; }

        return *this;
    }

    inline NullableWeakReferenceCountingPointer<_T>& operator=(const StrongReferenceCountingPointer<_T>& copy) noexcept
    {
        if(--_ptr->_weakRefCount == 0 && !_ptr->_stongRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = copy._ptr;
        ++_ptr->_weakRefCount;

        return *this;
    }

    inline NullableWeakReferenceCountingPointer<_T>& operator=(StrongReferenceCountingPointer<_T>&& move) noexcept
    {
        if(--_ptr->_weakRefCount == 0 && !_ptr->_stongRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = move._ptr;
        ++_ptr->_weakRefCount;

        return *this;
    }

    inline NullableWeakReferenceCountingPointer<_T>& operator=(const WeakReferenceCountingPointer<_T>& copy) noexcept
    {
        if(--_ptr->_weakRefCount == 0 && !_ptr->_stongRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = copy._ptr;
        ++_ptr->_weakRefCount;

        return *this;
    }

    inline NullableWeakReferenceCountingPointer<_T>& operator=(WeakReferenceCountingPointer<_T>&& move) noexcept
    {
        if(--_ptr->_weakRefCount == 0 && !_ptr->_stongRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = move._ptr;
        ++_ptr->_weakRefCount;

        return *this;
    }

    inline NullableWeakReferenceCountingPointer<_T>& operator=(const NullableStrongReferenceCountingPointer<_T>& copy) noexcept
    {
        if(--_ptr->_weakRefCount == 0 && !_ptr->_stongRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = copy._ptr;
        if(_ptr) { ++_ptr->_weakRefCount; }

        return *this;
    }

    inline NullableWeakReferenceCountingPointer<_T>& operator=(NullableStrongReferenceCountingPointer<_T>&& move) noexcept
    {
        if(--_ptr->_weakRefCount == 0 && !_ptr->_stongRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }

        _ptr = move._ptr;
        if(_ptr) { ++_ptr->_weakRefCount; }

        return *this;
    }

    [[nodiscard]] inline       _T& operator  *()                { return _ptr->obj(); }
    [[nodiscard]] inline const _T& operator  *() const          { return _ptr->obj(); }
    [[nodiscard]] inline       _T* operator ->()       noexcept { return _ptr && _ptr->_strongRefCount ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] inline const _T* operator ->() const noexcept { return _ptr && _ptr->_strongRefCount ? _ptr->objPtr() : nullptr; }

    [[nodiscard]] inline       _T* get()       noexcept { return _ptr && _ptr->_strongRefCount ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] inline const _T* get() const noexcept { return _ptr && _ptr->_strongRefCount ? _ptr->objPtr() : nullptr; }

    [[nodiscard]] inline ::std::size_t refCount()       const noexcept { return _ptr ? _ptr->_weakRefCount   : 0; }
    [[nodiscard]] inline ::std::size_t strongRefCount() const noexcept { return _ptr ? _ptr->_strongRefCount : 0; }
    [[nodiscard]] inline ::std::size_t weakRefCount()   const noexcept { return _ptr ? _ptr->_weakRefCount   : 0; }

    [[nodiscard]] inline SWRC<_T>* _getBlock() const noexcept { return _ptr; }

    [[nodiscard]] inline operator bool() const noexcept { return _ptr && _ptr->_strongRefCount; }
    [[nodiscard]] inline bool operator ==(const StrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const StrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const WeakReferenceCountingPointer<_T>& ptr)   const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const WeakReferenceCountingPointer<_T>& ptr)   const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const NullableStrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const NullableStrongReferenceCountingPointer<_T>& ptr) const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const NullableWeakReferenceCountingPointer<_T>& ptr)   const noexcept { return _ptr == ptr._ptr; }
    [[nodiscard]] inline bool operator !=(const NullableWeakReferenceCountingPointer<_T>& ptr)   const noexcept { return _ptr != ptr._ptr; }
    [[nodiscard]] inline bool operator ==(const _T*& ptr) const noexcept { return _ptr && _ptr->objPtr() == ptr; }
    [[nodiscard]] inline bool operator !=(const _T*& ptr) const noexcept { return _ptr && _ptr->objPtr() != ptr; }
    [[nodiscard]] inline bool operator ==(const nullptr_t) const noexcept { return !_ptr; }
    [[nodiscard]] inline bool operator !=(const nullptr_t) const noexcept { return  _ptr; }
private:
    friend class NullableStrongReferenceCountingPointer<_T>;
};

template<typename _T>
ReferenceCountingPointer<_T>& ReferenceCountingPointer<_T>::operator=(const NullableReferenceCountingPointer<_T>& copy) noexcept
{
    if(!copy)
    { return *this; }

    if(--_ptr->_refCount == 0)
    { _ptr->_allocator.deallocateT(_ptr); }

    _ptr = copy._ptr;
    ++_ptr->_refCount;

    return *this;
}

template<typename _T>
ReferenceCountingPointer<_T>& ReferenceCountingPointer<_T>::operator=( NullableReferenceCountingPointer<_T>&& move) noexcept
{
    if(!move)
    { return *this; }

    if(--_ptr->_refCount == 0)
    { _ptr->_allocator.deallocateT(_ptr); }

    _ptr = move._ptr;
    ++_ptr->_refCount;

    return *this;
}

template<typename _T>
bool ReferenceCountingPointer<_T>::operator==(const NullableReferenceCountingPointer<_T>& ptr) const noexcept
{ return _ptr == ptr._ptr; }

template<typename _T>
bool ReferenceCountingPointer<_T>::operator!=(const NullableReferenceCountingPointer<_T>& ptr) const noexcept
{ return _ptr != ptr._ptr; }

template<typename _T>
StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(const WeakReferenceCountingPointer<_T>& copy) noexcept
{
    if(!copy)
    { return *this; }

    if(--_ptr->_strongRefCount == 0)
    {
        _ptr->destroyObj();
        if(!_ptr->_weakRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }
    }

    _ptr = copy._ptr;
    ++_ptr->_strongRefCount;

    return *this;
}

template<typename _T>
StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(WeakReferenceCountingPointer<_T>&& move) noexcept
{
    if(!move)
    { return *this; }

    if(--_ptr->_strongRefCount == 0)
    {
        _ptr->destroyObj();
        if(!_ptr->_weakRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }
    }

    _ptr = move._ptr;
    ++_ptr->_strongRefCount;

    return *this;
}

template<typename _T>
StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(const NullableStrongReferenceCountingPointer<_T>& copy) noexcept
{
    if(!copy)
    { return *this; }

    if(--_ptr->_strongRefCount == 0)
    {
        _ptr->destroyObj();
        if(!_ptr->_weakRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }
    }

    _ptr = copy._ptr;
    ++_ptr->_strongRefCount;

    return *this;
}

template<typename _T>
StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(NullableStrongReferenceCountingPointer<_T>&& move) noexcept
{
    if(!move)
    { return *this; }

    if(--_ptr->_strongRefCount == 0)
    {
        _ptr->destroyObj();
        if(!_ptr->_weakRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }
    }

    _ptr = move._ptr;
    ++_ptr->_strongRefCount;

    return *this;
}

template<typename _T>
StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(const NullableWeakReferenceCountingPointer<_T>& copy) noexcept
{
    if(!copy)
    { return *this; }

    if(--_ptr->_strongRefCount == 0)
    {
        _ptr->destroyObj();
        if(!_ptr->_weakRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }
    }

    _ptr = copy._ptr;
    ++_ptr->_strongRefCount;

    return *this;
}

template<typename _T>
StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(NullableWeakReferenceCountingPointer<_T>&& move) noexcept
{
    if(!move)
    { return *this; }

    if(--_ptr->_strongRefCount == 0)
    {
        _ptr->destroyObj();
        if(!_ptr->_weakRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }
    }

    _ptr = move._ptr;
    ++_ptr->_strongRefCount;

    return *this;
}

template<typename _T>
bool StrongReferenceCountingPointer<_T>::operator==(const WeakReferenceCountingPointer<_T>& ptr) const noexcept
{ return _ptr == ptr._ptr; }

template<typename _T>
bool StrongReferenceCountingPointer<_T>::operator!=(const WeakReferenceCountingPointer<_T>& ptr) const noexcept
{ return _ptr != ptr._ptr; }

template<typename _T>
bool StrongReferenceCountingPointer<_T>::operator==(const NullableStrongReferenceCountingPointer<_T>& ptr) const noexcept
{ return ptr ? _ptr == ptr._ptr : false; }

template<typename _T>
bool StrongReferenceCountingPointer<_T>::operator!=(const NullableStrongReferenceCountingPointer<_T>& ptr) const noexcept
{ return ptr ? _ptr != ptr._ptr : true; }

template<typename _T>
bool StrongReferenceCountingPointer<_T>::operator==(const NullableWeakReferenceCountingPointer<_T>& ptr) const noexcept
{ return ptr ? _ptr == ptr._ptr : false; }

template<typename _T>
bool StrongReferenceCountingPointer<_T>::operator!=(const NullableWeakReferenceCountingPointer<_T>& ptr) const noexcept
{ return ptr ? _ptr != ptr._ptr : true; }

template<typename _T>
WeakReferenceCountingPointer<_T>& WeakReferenceCountingPointer<_T>::operator=(const NullableStrongReferenceCountingPointer<_T>& copy) noexcept
{
    if(!copy)
    { return *this; }
    
    if(--_ptr->_weakRefCount == 0 && !_ptr->_stongRefCount)
    { _ptr->_allocator.deallocateT(_ptr); }

    _ptr = copy._ptr;
    ++_ptr->_weakRefCount;

    return *this;
}

template<typename _T>
WeakReferenceCountingPointer<_T>& WeakReferenceCountingPointer<_T>::operator=(NullableStrongReferenceCountingPointer<_T>&& move) noexcept
{
    if(!move)
    { return *this; }
    
    if(--_ptr->_weakRefCount == 0 && !_ptr->_stongRefCount)
    { _ptr->_allocator.deallocateT(_ptr); }

    _ptr = move._ptr;
    ++_ptr->_weakRefCount;

    return *this;
}

template<typename _T>
WeakReferenceCountingPointer<_T>& WeakReferenceCountingPointer<_T>::operator=(const NullableWeakReferenceCountingPointer<_T>& copy) noexcept
{
    if(!copy)
    { return *this; }
    
    if(--_ptr->_weakRefCount == 0 && !_ptr->_stongRefCount)
    { _ptr->_allocator.deallocateT(_ptr); }

    _ptr = copy._ptr;
    ++_ptr->_weakRefCount;

    return *this;
}

template<typename _T>
WeakReferenceCountingPointer<_T>& WeakReferenceCountingPointer<_T>::operator=(NullableWeakReferenceCountingPointer<_T>&& move) noexcept
{
    if(!move)
    { return *this; }
    
    if(--_ptr->_weakRefCount == 0 && !_ptr->_stongRefCount)
    { _ptr->_allocator.deallocateT(_ptr); }

    _ptr = move._ptr;
    ++_ptr->_weakRefCount;

    return *this;
}

template<typename _T>
bool WeakReferenceCountingPointer<_T>::operator==(const NullableStrongReferenceCountingPointer<_T>& ptr) const noexcept
{ return ptr ? _ptr == ptr._ptr : false; }

template<typename _T>
bool WeakReferenceCountingPointer<_T>::operator!=(const NullableStrongReferenceCountingPointer<_T>& ptr) const noexcept
{ return ptr ? _ptr != ptr._ptr : true; }

template<typename _T>
bool WeakReferenceCountingPointer<_T>::operator==(const NullableWeakReferenceCountingPointer<_T>& ptr) const noexcept
{ return ptr ? _ptr == ptr._ptr : false; }

template<typename _T>
bool WeakReferenceCountingPointer<_T>::operator!=(const NullableWeakReferenceCountingPointer<_T>& ptr) const noexcept
{ return ptr ? _ptr != ptr._ptr : true; }

template<typename _T>
NullableStrongReferenceCountingPointer<_T>& NullableStrongReferenceCountingPointer<_T>::operator=(const NullableWeakReferenceCountingPointer<_T>& copy) noexcept
{
    if(_ptr && --_ptr->_strongRefCount == 0)
    {
        _ptr->destroyObj();
        if(!_ptr->_weakRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }
    }
    
    _ptr = copy._ptr;
    if(_ptr) { ++_ptr->_strongRefCount; }
    
    return *this;
}

template<typename _T>
NullableStrongReferenceCountingPointer<_T>& NullableStrongReferenceCountingPointer<_T>::operator=(NullableWeakReferenceCountingPointer<_T>&& move) noexcept
{
    if(_ptr && --_ptr->_strongRefCount == 0)
    {
        _ptr->destroyObj();
        if(!_ptr->_weakRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }
    }
    
    _ptr = move._ptr;
    if(_ptr) { ++_ptr->_strongRefCount; }
    
    return *this;
}

template<typename _T>
bool NullableStrongReferenceCountingPointer<_T>::operator==(const NullableWeakReferenceCountingPointer<_T>& ptr) const noexcept
{ return _ptr == ptr._ptr; }

template<typename _T>
bool NullableStrongReferenceCountingPointer<_T>::operator!=(const NullableWeakReferenceCountingPointer<_T>& ptr) const noexcept
{ return _ptr != ptr._ptr; }

template<typename _ToT, typename _FromT>
ReferenceCountingPointer<_ToT> RCPCast(const ReferenceCountingPointer<_FromT>& ptr) noexcept
{ return reinterpret_cast<const ReferenceCountingPointer<_ToT>&>(ptr); }

template<typename _ToT, typename _FromT>
ReferenceCountingPointer<_ToT> RCPCast(ReferenceCountingPointer<_FromT>&& ptr) noexcept
{ return reinterpret_cast<ReferenceCountingPointer<_ToT>&&>(ptr); }

template<typename _ToT, typename _FromT>
NullableReferenceCountingPointer<_ToT> RCPCast(const NullableReferenceCountingPointer<_FromT>& ptr) noexcept
{ return reinterpret_cast<const NullableReferenceCountingPointer<_ToT>&>(ptr); }

template<typename _ToT, typename _FromT>
NullableReferenceCountingPointer<_ToT> RCPCast(NullableReferenceCountingPointer<_FromT>&& ptr) noexcept
{ return reinterpret_cast<NullableReferenceCountingPointer<_ToT>&&>(ptr); }

template<typename _ToT, typename _FromT>
StrongReferenceCountingPointer<_ToT> RCPCast(const StrongReferenceCountingPointer<_FromT>& ptr) noexcept
{ return reinterpret_cast<const StrongReferenceCountingPointer<_ToT>&>(ptr); }

template<typename _ToT, typename _FromT>
StrongReferenceCountingPointer<_ToT> RCPCast(StrongReferenceCountingPointer<_FromT>&& ptr) noexcept
{ return reinterpret_cast<StrongReferenceCountingPointer<_ToT>&&>(ptr); }

template<typename _ToT, typename _FromT>
NullableStrongReferenceCountingPointer<_ToT> RCPCast(const NullableStrongReferenceCountingPointer<_FromT>& ptr) noexcept
{ return reinterpret_cast<const NullableStrongReferenceCountingPointer<_ToT>&>(ptr); }

template<typename _ToT, typename _FromT>
NullableStrongReferenceCountingPointer<_ToT> RCPCast(NullableStrongReferenceCountingPointer<_FromT>&& ptr) noexcept
{ return reinterpret_cast<NullableStrongReferenceCountingPointer<_ToT>&&>(ptr); }

template<typename _ToT, typename _FromT>
WeakReferenceCountingPointer<_ToT> RCPCast(const WeakReferenceCountingPointer<_FromT>& ptr) noexcept
{ return reinterpret_cast<const WeakReferenceCountingPointer<_ToT>&>(ptr); }

template<typename _ToT, typename _FromT>
WeakReferenceCountingPointer<_ToT> RCPCast(WeakReferenceCountingPointer<_FromT>&& ptr) noexcept
{ return reinterpret_cast<WeakReferenceCountingPointer<_ToT>&&>(ptr); }

template<typename _ToT, typename _FromT>
NullableWeakReferenceCountingPointer<_ToT> RCPCast(const NullableWeakReferenceCountingPointer<_FromT>& ptr) noexcept
{ return reinterpret_cast<const NullableWeakReferenceCountingPointer<_ToT>&>(ptr); }

template<typename _ToT, typename _FromT>
NullableWeakReferenceCountingPointer<_ToT> RCPCast(NullableWeakReferenceCountingPointer<_FromT>&& ptr) noexcept
{ return reinterpret_cast<NullableWeakReferenceCountingPointer<_ToT>&&>(ptr); }

