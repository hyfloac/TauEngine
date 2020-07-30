#pragma once

#pragma warning(push, 0)
#include <memory>
#pragma warning(pop)

namespace _ReferenceCountingPointerUtils {
template<typename _T>
template<typename... _Args>
inline _ReferenceCountDataObject<_T>::_ReferenceCountDataObject(TauAllocator& allocator, _Args&&... args) noexcept
    : _refCount(1)
    , _allocator(allocator)
    , _objRaw { }
{ (void) new(_objRaw) _T(_TauAllocatorUtils::_forward<_Args>(args)...); }

template<typename _T>
inline _ReferenceCountDataObject<_T>::~_ReferenceCountDataObject() noexcept
{ reinterpret_cast<_T*>(_objRaw)->~_T(); }

template<typename _T>
template<typename... _Args>
inline _SWReferenceCount<_T>::_SWReferenceCount(TauAllocator& allocator, _Args&&... args) noexcept
    : _strongRefCount(1)
    , _weakRefCount(0)
    , _allocator(allocator)
    , _objRaw { }
{ (void) new(_objRaw) _T(_TauAllocatorUtils::_forward<_Args>(args)...); }

template<typename _ToT, typename _FromT>
inline const _ReferenceCountDataObject<_ToT>& RCDOCast(const _ReferenceCountDataObject<_FromT>& obj) noexcept
{ return reinterpret_cast<const _ReferenceCountDataObject<_ToT>&>(obj); }

template<typename _ToT, typename _FromT>
inline _ReferenceCountDataObject<_ToT>& RCDOCast(_ReferenceCountDataObject<_FromT>& obj) noexcept
{ return reinterpret_cast<_ReferenceCountDataObject<_ToT>&>(obj); }

template<typename _ToT, typename _FromT>
inline _ReferenceCountDataObject<_ToT>&& RCDOCast(_ReferenceCountDataObject<_FromT>&& obj) noexcept
{ return reinterpret_cast<_ReferenceCountDataObject<_ToT>&&>(obj); }

template<typename _ToT, typename _FromT>
inline const _SWReferenceCount<_ToT>& SWRCCast(const _SWReferenceCount<_FromT>& obj) noexcept
{ return reinterpret_cast<const _SWReferenceCount<_ToT>&>(obj); }

template<typename _ToT, typename _FromT>
inline _SWReferenceCount<_ToT>& SWRCCast(_SWReferenceCount<_FromT>& obj) noexcept
{ return reinterpret_cast<_SWReferenceCount<_ToT>&>(obj); }

template<typename _ToT, typename _FromT>
inline _SWReferenceCount<_ToT>&& SWRCCast(_SWReferenceCount<_FromT>&& obj) noexcept
{ return reinterpret_cast<_SWReferenceCount<_ToT>&&>(obj); }
}

template<typename _T>
inline uSys ReferenceCountingPointer<_T>::allocSize() noexcept
{ return sizeof(RCDO<_T>); }

template<typename _T>
template<typename... _Args>
inline ReferenceCountingPointer<_T>::ReferenceCountingPointer(TauAllocator& allocator, _Args&&... args) noexcept
    : _ptr(allocator.allocateT<RCDO<_T>>(allocator, args...))
{ }

template<typename _T>
template<typename _TT>
inline ReferenceCountingPointer<_T>::ReferenceCountingPointer(const ReferenceCountingPointer<_TT>& rcp) noexcept
    : TReferenceCountingPointerBase<_T>(rcp)
    , _ptr(reinterpret_cast<RCDO<_T>*>(rcp._getBlock()))
{
    if(_ptr) 
    { ++_ptr->_refCount; }
}

template<typename _T>
template<typename _TT>
inline ReferenceCountingPointer<_T>::ReferenceCountingPointer(ReferenceCountingPointer<_TT>&& rcp) noexcept
    : TReferenceCountingPointerBase<_T>(::std::move(move))
    , _ptr(reinterpret_cast<RCDO<_T>*>(rcp._getBlock()))
{ rcp._getBlock() = nullptr; }

template<typename _T>
inline ReferenceCountingPointer<_T>::ReferenceCountingPointer(const nullptr_t) noexcept
    : _ptr(nullptr)
{ }

template<typename _T>
inline ReferenceCountingPointer<_T>::ReferenceCountingPointer() noexcept
    : _ptr(nullptr)
{ }

template<typename _T>
inline ReferenceCountingPointer<_T>::~ReferenceCountingPointer() noexcept
{
    if(_ptr && --_ptr->_refCount == 0)
    { _ptr->_allocator.deallocateT(_ptr); }
}

template<typename _T>
inline ReferenceCountingPointer<_T>::ReferenceCountingPointer(const ReferenceCountingPointer<_T>& copy) noexcept
    : TReferenceCountingPointerBase<_T>(copy)
    , _ptr(copy._ptr)
{
    if(_ptr) 
    { ++_ptr->_refCount; }
}

template<typename _T>
inline ReferenceCountingPointer<_T>::ReferenceCountingPointer(ReferenceCountingPointer<_T>&& move) noexcept
    : TReferenceCountingPointerBase<_T>(::std::move(move))
    , _ptr(move._ptr)
{ move._ptr = nullptr; }

template<typename _T>
inline ReferenceCountingPointer<_T>& ReferenceCountingPointer<_T>::operator=(const nullptr_t) noexcept
{
    if(_ptr && --_ptr->_refCount == 0)
    { _ptr->_allocator.deallocateT(_ptr); }

    _ptr = nullptr;

    return *this;
}

template<typename _T>
inline ReferenceCountingPointer<_T>& ReferenceCountingPointer<_T>::operator=(const ReferenceCountingPointer<_T>& copy) noexcept
{
    if(this == &copy)
    { return *this; }

    if(_ptr && --_ptr->_refCount == 0)
    { _ptr->_allocator.deallocateT(_ptr); }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    _ptr = copy._ptr;
    if(_ptr)
    { ++_ptr->_refCount; }

    return *this;
}

template<typename _T>
inline ReferenceCountingPointer<_T>& ReferenceCountingPointer<_T>::operator=(ReferenceCountingPointer<_T>&& move) noexcept
{
    if(this == &move)
    { return *this; }

    if(_ptr && --_ptr->_refCount == 0)
    { _ptr->_allocator.deallocateT(_ptr); }

    TReferenceCountingPointerBase<_T>::operator =(::std::move(move));

    _ptr = move._ptr;
    move._ptr = nullptr;

    return *this;
}

template<typename _T>
template<typename _TT>
inline ReferenceCountingPointer<_T>& ReferenceCountingPointer<_T>::operator=(const ReferenceCountingPointer<_TT>& copy) noexcept
{
    if(_ptr && --_ptr->_refCount == 0)
    { _ptr->_allocator.deallocateT(_ptr); }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    _ptr = reinterpret_cast<RCDO<_T>*>(copy._getBlock());
    if(_ptr)
    { ++_ptr->_refCount; }

    return *this;
}

template<typename _T>
template<typename _TT>
inline ReferenceCountingPointer<_T>& ReferenceCountingPointer<_T>::operator=(ReferenceCountingPointer<_TT>&& move) noexcept
{
    if(_ptr && --_ptr->_refCount == 0)
    { _ptr->_allocator.deallocateT(_ptr); }

    TReferenceCountingPointerBase<_T>::operator =(::std::move(move));

    _ptr = reinterpret_cast<RCDO<_T>*>(move._getBlock());
    move._getBlock() = nullptr;

    return *this;
}

template<typename _T>
template<typename... _Args>
inline void ReferenceCountingPointer<_T>::reset(TauAllocator& allocator, _Args&&... args) noexcept
{
    if(_ptr && --_ptr->_refCount == 0)
    { _ptr->_allocator.deallocateT(_ptr); }

    _ptr = allocator.allocateT<RCDO<_T>>(allocator, args...);
}

template<typename _T>
inline bool ReferenceCountingPointer<_T>::operator==(const _T* const& ptr) const noexcept
{
    if(_ptr && ptr)
    { return _ptr->objPtr() == ptr; }
    if(!_ptr && !ptr)
    { return true; }
    return false;
}

template<typename _T>
inline bool ReferenceCountingPointer<_T>::operator!=(const _T* const& ptr) const noexcept
{
    if(_ptr && ptr)
    { return _ptr->objPtr() != ptr; }
    if(!_ptr && !ptr)
    { return false; }
    return true;
}

template<typename _T>
inline uSys StrongReferenceCountingPointer<_T>::allocSize() noexcept
{ return sizeof(SWRC<_T>); }

template<typename _T>
template<typename... _Args>
inline StrongReferenceCountingPointer<_T>::StrongReferenceCountingPointer(TauAllocator& allocator, _Args&&... args) noexcept
    : _ptr(allocator.allocateT<SWRC<_T>>(allocator, args...))
{ }

template<typename _T>
template<typename _TT>
inline StrongReferenceCountingPointer<_T>::StrongReferenceCountingPointer(const StrongReferenceCountingPointer<_TT>& rcp) noexcept
    : TReferenceCountingPointerBase<_T>(rcp)
    , _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
{
    if(_ptr) 
    { ++_ptr->_strongRefCount; }
}

template<typename _T>
template<typename _TT>
inline StrongReferenceCountingPointer<_T>::StrongReferenceCountingPointer(StrongReferenceCountingPointer<_TT>&& rcp) noexcept
    : TReferenceCountingPointerBase<_T>(::std::move(rcp))
    , _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
{ rcp._getBlock() = nullptr; }

template<typename _T>
inline StrongReferenceCountingPointer<_T>::StrongReferenceCountingPointer(const nullptr_t) noexcept
    : _ptr(nullptr)
{ }

template<typename _T>
inline StrongReferenceCountingPointer<_T>::StrongReferenceCountingPointer() noexcept
    : _ptr(nullptr)
{ }

template<typename _T>
inline StrongReferenceCountingPointer<_T>::~StrongReferenceCountingPointer() noexcept
{
    if(_ptr && --_ptr->_strongRefCount == 0)
    {
        _ptr->destroyObj();
        if(!_ptr->_weakRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }
    }
}

template<typename _T>
inline StrongReferenceCountingPointer<_T>::StrongReferenceCountingPointer(const StrongReferenceCountingPointer<_T>& copy) noexcept
    : TReferenceCountingPointerBase<_T>(copy)
    , _ptr(copy._ptr)
{
    if(_ptr) 
    { ++_ptr->_strongRefCount; }
}

template<typename _T>
inline StrongReferenceCountingPointer<_T>::StrongReferenceCountingPointer(StrongReferenceCountingPointer<_T>&& move) noexcept
    : TReferenceCountingPointerBase<_T>(::std::move(move))
    , _ptr(move._ptr)
{ move._ptr = nullptr; }

template<typename _T>
inline StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(const StrongReferenceCountingPointer<_T>& copy) noexcept
{
    if(this == &copy)
    { return *this; }

    if(_ptr && --_ptr->_strongRefCount == 0)
    {
        _ptr->destroyObj();
        if(!_ptr->_weakRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }
    }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    _ptr = copy._ptr;
    if(_ptr) { ++_ptr->_strongRefCount; }

    return *this;
}

template<typename _T>
inline StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(StrongReferenceCountingPointer<_T>&& move) noexcept
{
    if(this == &move)
    { return *this; }

    if(_ptr && --_ptr->_strongRefCount == 0)
    {
        _ptr->destroyObj();
        if(!_ptr->_weakRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }
    }

    TReferenceCountingPointerBase<_T>::operator =(::std::move(move));

    _ptr = move._ptr;
    move._ptr = nullptr;

    return *this;
}

template<typename _T>
inline StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(const nullptr_t) noexcept
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

template<typename _T>
template<typename _TT>
inline StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(const StrongReferenceCountingPointer<_TT>& copy) noexcept
{
    if(_ptr && --_ptr->_strongRefCount == 0)
    {
        _ptr->destroyObj();
        if(!_ptr->_weakRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }
    }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    _ptr = reinterpret_cast<SWRC<_T>*>(copy._getBlock());
    ++_ptr->_strongRefCount;

    return *this;
}

template<typename _T>
template<typename _TT>
inline StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(StrongReferenceCountingPointer<_TT>&& move) noexcept
{
    if(_ptr && --_ptr->_strongRefCount == 0)
    {
        _ptr->destroyObj();
        if(!_ptr->_weakRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }
    }

    TReferenceCountingPointerBase<_T>::operator =(::std::move(move));

    _ptr = reinterpret_cast<SWRC<_T>*>(move._getBlock());
    move._getBlock() = nullptr;

    return *this;
}

template<typename _T>
inline StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(const WeakReferenceCountingPointer<_T>& copy) noexcept
{
    if(_ptr && --_ptr->_strongRefCount == 0)
    {
        _ptr->destroyObj();
        if(!_ptr->_weakRefCount)
        { _ptr->_allocator.deallocate(_ptr); }
    }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    if(copy)
    {
        _ptr = copy._ptr;
        ++_ptr->_strongRefCount;
    }
    else
    { _ptr = nullptr; }

    return *this;
}

template <typename _T>
template <typename _TT>
inline StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(const WeakReferenceCountingPointer<_TT>& copy) noexcept
{
    if(_ptr && --_ptr->_strongRefCount == 0)
    {
        _ptr->destroyObj();
        if(!_ptr->_weakRefCount)
        { _ptr->_allocator.deallocate(_ptr); }
    }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    if(copy)
    {
        _ptr = reinterpret_cast<SWRC<_T>*>(copy._getBlock());
        ++_ptr->_strongRefCount;
    }
    else
    { _ptr = nullptr; }

    return *this;
}

template<typename _T>
template<typename... _Args>
inline void StrongReferenceCountingPointer<_T>::reset(TauAllocator& allocator, _Args&&... args) noexcept
{
    if(_ptr && --_ptr->_strongRefCount == 0)
    {
        _ptr->destroyObj();
        if(!_ptr->_weakRefCount)
        { _ptr->_allocator.deallocateT(_ptr); }
    }

    _ptr = allocator.allocateT<SWRC<_T>>(allocator, args...);
}

template<typename _T>
inline bool StrongReferenceCountingPointer<_T>::operator==(const StrongReferenceCountingPointer<_T>& ptr) const noexcept
{ return _ptr == ptr._ptr; }

template<typename _T>
inline bool StrongReferenceCountingPointer<_T>::operator!=(const StrongReferenceCountingPointer<_T>& ptr) const noexcept
{ return _ptr != ptr._ptr; }

template<typename _T>
inline bool StrongReferenceCountingPointer<_T>::operator==(const WeakReferenceCountingPointer<_T>& ptr) const noexcept
{ return _ptr == ptr._ptr; }

template<typename _T>
inline bool StrongReferenceCountingPointer<_T>::operator!=(const WeakReferenceCountingPointer<_T>& ptr) const noexcept
{ return _ptr != ptr._ptr; }

template<typename _T>
inline bool StrongReferenceCountingPointer<_T>::operator==(const _T* const& ptr) const noexcept
{
    if(_ptr && ptr)
    { return _ptr->objPtr() == ptr; }
    if(!_ptr && !ptr)
    { return true; }
    return false;
}

template<typename _T>
inline bool StrongReferenceCountingPointer<_T>::operator!=(const _T* const& ptr) const noexcept
{
    if(_ptr && ptr)
    { return _ptr->objPtr() != ptr; }
    if(!_ptr && !ptr)
    { return false; }
    return true;
}

template<typename _T>
inline uSys WeakReferenceCountingPointer<_T>::allocSize() noexcept
{ return sizeof(SWRC<_T>); }

template<typename _T>
inline WeakReferenceCountingPointer<_T>::WeakReferenceCountingPointer(const StrongReferenceCountingPointer<_T>& ptr) noexcept
    : TReferenceCountingPointerBase<_T>(ptr)
    , _ptr(ptr._ptr)
{
    if(_ptr) 
    { ++_ptr->_weakRefCount; }
}

template<typename _T>
template<typename _TT>
inline WeakReferenceCountingPointer<_T>::WeakReferenceCountingPointer(const StrongReferenceCountingPointer<_TT>& rcp) noexcept
    : TReferenceCountingPointerBase<_T>(rcp)
    , _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
{
    if(_ptr) 
    { ++_ptr->_weakRefCount; }
}

template<typename _T>
template<typename _TT>
inline WeakReferenceCountingPointer<_T>::WeakReferenceCountingPointer(const WeakReferenceCountingPointer<_TT>& rcp) noexcept
    : TReferenceCountingPointerBase<_T>(rcp)
    , _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
{
    if(_ptr) 
    { ++_ptr->_weakRefCount; }
}

template<typename _T>
template<typename _TT>
inline WeakReferenceCountingPointer<_T>::WeakReferenceCountingPointer(WeakReferenceCountingPointer<_TT>&& rcp) noexcept
    : TReferenceCountingPointerBase<_T>(::std::move(rcp))
    , _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
{ rcp._getBlock() = nullptr; }

template<typename _T>
inline WeakReferenceCountingPointer<_T>::WeakReferenceCountingPointer(const nullptr_t) noexcept
    : _ptr(nullptr)
{ }

template<typename _T>
inline WeakReferenceCountingPointer<_T>::WeakReferenceCountingPointer() noexcept
    : _ptr(nullptr)
{ }

template<typename _T>
inline WeakReferenceCountingPointer<_T>::~WeakReferenceCountingPointer() noexcept
{
    if(_ptr && --_ptr->_weakRefCount == 0 && !_ptr->_strongRefCount)
    { _ptr->_allocator.deallocate(_ptr); }
}

template<typename _T>
inline WeakReferenceCountingPointer<_T>::WeakReferenceCountingPointer(const WeakReferenceCountingPointer<_T>& copy) noexcept
    : TReferenceCountingPointerBase<_T>(copy)
    , _ptr(copy._ptr)
{
    if(_ptr) 
    { ++_ptr->_weakRefCount; }
}

template<typename _T>
inline WeakReferenceCountingPointer<_T>::WeakReferenceCountingPointer(WeakReferenceCountingPointer<_T>&& move) noexcept
    : TReferenceCountingPointerBase<_T>(::std::move(move))
    , _ptr(move._ptr)
{ move._ptr = nullptr; }

template<typename _T>
inline WeakReferenceCountingPointer<_T>& WeakReferenceCountingPointer<_T>::operator=(const WeakReferenceCountingPointer<_T>& copy) noexcept
{
    if(this == &copy)
    { return *this; }

    if(--_ptr->_weakRefCount == 0 && !_ptr->_strongRefCount)
    { _ptr->_allocator.deallocate(_ptr); }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    _ptr = copy._ptr;
    if(_ptr) 
    { ++_ptr->_weakRefCount; }

    return *this;
}

template<typename _T>
inline WeakReferenceCountingPointer<_T>& WeakReferenceCountingPointer<_T>::operator=(WeakReferenceCountingPointer<_T>&& move) noexcept
{
    if(this == &move)
    { return *this; }

    if(--_ptr->_weakRefCount == 0 && !_ptr->_strongRefCount)
    { _ptr->_allocator.deallocate(_ptr); }

    TReferenceCountingPointerBase<_T>::operator =(::std::move(move));

    _ptr = move._ptr;
    move._ptr = nullptr;

    return *this;
}

template<typename _T>
inline WeakReferenceCountingPointer<_T>& WeakReferenceCountingPointer<_T>::operator=(const StrongReferenceCountingPointer<_T>& copy) noexcept
{
    if(--_ptr->_weakRefCount == 0 && !_ptr->_strongRefCount)
    { _ptr->_allocator.deallocate(_ptr); }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    _ptr = copy._ptr;
    if(_ptr) 
    { ++_ptr->_weakRefCount; }

    return *this;
}

template<typename _T>
template<typename _TT>
inline WeakReferenceCountingPointer<_T>& WeakReferenceCountingPointer<_T>::operator=(const StrongReferenceCountingPointer<_TT>& copy) noexcept
{
    if(--_ptr->_weakRefCount == 0 && !_ptr->_strongRefCount)
    { _ptr->_allocator.deallocate(_ptr); }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    _ptr = reinterpret_cast<SWRC<_T>*>(copy._getBlock());
    if(_ptr) 
    { ++_ptr->_weakRefCount; }

    return *this;
}

template<typename _T>
template<typename _TT>
inline WeakReferenceCountingPointer<_T>& WeakReferenceCountingPointer<_T>::operator=(const WeakReferenceCountingPointer<_TT>& copy) noexcept
{
    if(--_ptr->_weakRefCount == 0 && !_ptr->_strongRefCount)
    { _ptr->_allocator.deallocate(_ptr); }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    _ptr = reinterpret_cast<SWRC<_T>*>(copy._getBlock());
    if(_ptr) 
    { ++_ptr->_weakRefCount; }

    return *this;
}

template<typename _T>
template<typename _TT>
inline WeakReferenceCountingPointer<_T>& WeakReferenceCountingPointer<_T>::operator=(WeakReferenceCountingPointer<_TT>&& move) noexcept
{
    if(--_ptr->_weakRefCount == 0 && !_ptr->_strongRefCount)
    { _ptr->_allocator.deallocate(_ptr); }

    TReferenceCountingPointerBase<_T>::operator =(::std::move(move));

    _ptr = reinterpret_cast<SWRC<_T>*>(move._getBlock());
    move._getBlock() = nullptr;

    return *this;
}

template<typename _T>
inline bool WeakReferenceCountingPointer<_T>::operator==(const _T* const& ptr) const noexcept
{
    if(_ptr && ptr)
    { return _ptr->objPtr() == ptr; }
    if(!_ptr && !ptr)
    { return true; }
    return false;
}

template<typename _T>
inline bool WeakReferenceCountingPointer<_T>::operator!=(const _T* const& ptr) const noexcept
{
    if(_ptr && ptr)
    { return _ptr->objPtr() != ptr; }
    if(!_ptr && !ptr)
    { return false; }
    return true;
}
