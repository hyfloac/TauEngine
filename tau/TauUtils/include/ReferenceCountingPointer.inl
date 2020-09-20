#pragma once

#pragma warning(push, 0)
#include <memory>
#pragma warning(pop)

#if defined(TAU_INDEV) || 0
#include "ReferenceCountingPointer.hpp"
#endif

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
    : _rcdo(allocator.allocateT<RCDO<_T>>(allocator, args...))
    , _tPtr(_rcdo->objPtr())
{ }

template<typename _T>
template<typename _TT>
inline ReferenceCountingPointer<_T>::ReferenceCountingPointer(const ReferenceCountingPointer<_TT>& rcp) noexcept
    : TReferenceCountingPointerBase<_T>(rcp)
    , _rcdo(reinterpret_cast<RCDO<_T>*>(rcp._getBlock()))
    , _tPtr(static_cast<_T*>(rcp._tPtr))
{
    if(_rcdo) 
    { _rcdo->addRef(); }
}

template<typename _T>
template<typename _TT>
inline ReferenceCountingPointer<_T>::ReferenceCountingPointer(ReferenceCountingPointer<_TT>&& rcp) noexcept
    : TReferenceCountingPointerBase<_T>(::std::move(rcp))
    , _rcdo(reinterpret_cast<RCDO<_T>*>(rcp._getBlock()))
    , _tPtr(static_cast<_T*>(rcp._tPtr))
{ rcp._getBlock() = nullptr; }

template<typename _T>
inline constexpr ReferenceCountingPointer<_T>::ReferenceCountingPointer(const nullptr_t) noexcept
    : _rcdo(nullptr)
    , _tPtr(nullptr)
{ }

template<typename _T>
inline constexpr ReferenceCountingPointer<_T>::ReferenceCountingPointer() noexcept
    : _rcdo(nullptr)
    , _tPtr(nullptr)
{ }

template<typename _T>
inline ReferenceCountingPointer<_T>::~ReferenceCountingPointer() noexcept
{
    if(_rcdo && _rcdo->release() == 0)
    { _rcdo->_allocator.deallocateT(_rcdo); }
}

template<typename _T>
inline ReferenceCountingPointer<_T>::ReferenceCountingPointer(const ReferenceCountingPointer<_T>& copy) noexcept
    : TReferenceCountingPointerBase<_T>(copy)
    , _rcdo(copy._rcdo)
    , _tPtr(copy._tPtr)
{
    if(_rcdo) 
    { _rcdo->addRef(); }
}

template<typename _T>
inline ReferenceCountingPointer<_T>::ReferenceCountingPointer(ReferenceCountingPointer<_T>&& move) noexcept
    : TReferenceCountingPointerBase<_T>(::std::move(move))
    , _rcdo(move._rcdo)
    , _tPtr(move._tPtr)
{ move._rcdo = nullptr; }

template<typename _T>
inline ReferenceCountingPointer<_T>& ReferenceCountingPointer<_T>::operator=(const nullptr_t) noexcept
{
    if(_rcdo && _rcdo->release() == 0)
    { _rcdo->_allocator.deallocateT(_rcdo); }

    _rcdo = nullptr;
    _tPtr = nullptr;

    return *this;
}

template<typename _T>
inline ReferenceCountingPointer<_T>& ReferenceCountingPointer<_T>::operator=(const ReferenceCountingPointer<_T>& copy) noexcept
{
    if(this == &copy)
    { return *this; }

    if(_rcdo && _rcdo->release() == 0)
    { _rcdo->_allocator.deallocateT(_rcdo); }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    _rcdo = copy._rcdo;
    _tPtr = copy._tPtr;

    if(_rcdo)
    { _rcdo->addRef(); }

    return *this;
}

template<typename _T>
inline ReferenceCountingPointer<_T>& ReferenceCountingPointer<_T>::operator=(ReferenceCountingPointer<_T>&& move) noexcept
{
    if(this == &move)
    { return *this; }

    if(_rcdo && _rcdo->release() == 0)
    { _rcdo->_allocator.deallocateT(_rcdo); }

    TReferenceCountingPointerBase<_T>::operator =(::std::move(move));

    _rcdo = move._rcdo;
    _tPtr = move._tPtr;

    move._rcdo = nullptr;

    return *this;
}

template<typename _T>
template<typename _TT>
inline ReferenceCountingPointer<_T>& ReferenceCountingPointer<_T>::operator=(const ReferenceCountingPointer<_TT>& copy) noexcept
{
    if(_rcdo && _rcdo->release() == 0)
    { _rcdo->_allocator.deallocateT(_rcdo); }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    _rcdo = reinterpret_cast<RCDO<_T>*>(copy._getBlock());
    _tPtr = static_cast<_T*>(copy._tPtr);

    if(_rcdo)
    { _rcdo->addRef(); }

    return *this;
}

template<typename _T>
template<typename _TT>
inline ReferenceCountingPointer<_T>& ReferenceCountingPointer<_T>::operator=(ReferenceCountingPointer<_TT>&& move) noexcept
{
    if(_rcdo && _rcdo->release() == 0)
    { _rcdo->_allocator.deallocateT(_rcdo); }

    TReferenceCountingPointerBase<_T>::operator =(::std::move(move));

    _rcdo = reinterpret_cast<RCDO<_T>*>(move._getBlock());
    _tPtr = static_cast<_T*>(move._tPtr);

    move._getBlock() = nullptr;

    return *this;
}

template<typename _T>
template<typename... _Args>
inline void ReferenceCountingPointer<_T>::reset(TauAllocator& allocator, _Args&&... args) noexcept
{
    if(_rcdo && _rcdo->release() == 0)
    { _rcdo->_allocator.deallocateT(_rcdo); }

    _rcdo = allocator.allocateT<RCDO<_T>>(allocator, args...);
    _tPtr = _rcdo->objPtr();
}

template<typename _T>
inline bool ReferenceCountingPointer<_T>::operator==(const _T* const& ptr) const noexcept
{
    if(_rcdo && ptr)
    { return _rcdo->objPtr() == ptr; }
    if(!_rcdo && !ptr)
    { return true; }
    return false;
}

template<typename _T>
inline bool ReferenceCountingPointer<_T>::operator!=(const _T* const& ptr) const noexcept
{
    if(_rcdo && ptr)
    { return _rcdo->objPtr() != ptr; }
    if(!_rcdo && !ptr)
    { return false; }
    return true;
}

template<typename _T>
inline uSys StrongReferenceCountingPointer<_T>::allocSize() noexcept
{ return sizeof(SWRC<_T>); }

template<typename _T>
template<typename... _Args>
inline StrongReferenceCountingPointer<_T>::StrongReferenceCountingPointer(TauAllocator& allocator, _Args&&... args) noexcept
    : _swrc(allocator.allocateT<SWRC<_T>>(allocator, args...))
    , _tPtr(_swrc->objPtr())
{ }

template<typename _T>
template<typename _TT>
inline StrongReferenceCountingPointer<_T>::StrongReferenceCountingPointer(const StrongReferenceCountingPointer<_TT>& rcp) noexcept
    : TReferenceCountingPointerBase<_T>(rcp)
    , _swrc(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    , _tPtr(static_cast<_T*>(rcp._tPtr))
{
    if(_swrc) 
    { _swrc->addRefStrong(); }
}

template<typename _T>
template<typename _TT>
inline StrongReferenceCountingPointer<_T>::StrongReferenceCountingPointer(StrongReferenceCountingPointer<_TT>&& rcp) noexcept
    : TReferenceCountingPointerBase<_T>(::std::move(rcp))
    , _swrc(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    , _tPtr(static_cast<_T*>(rcp._tPtr))
{ rcp._getBlock() = nullptr; }

template<typename _T>
inline constexpr StrongReferenceCountingPointer<_T>::StrongReferenceCountingPointer(const nullptr_t) noexcept
    : _swrc(nullptr)
    , _tPtr(nullptr)
{ }

template<typename _T>
inline constexpr StrongReferenceCountingPointer<_T>::StrongReferenceCountingPointer() noexcept
    : _swrc(nullptr)
    , _tPtr(nullptr)
{ }

template<typename _T>
inline StrongReferenceCountingPointer<_T>::~StrongReferenceCountingPointer() noexcept
{
    if(_swrc && _swrc->releaseStrong() == 0)
    {
        _swrc->destroyObj();
        if(!_swrc->_weakRefCount)
        { _swrc->_allocator.deallocateT(_swrc); }
    }
}

template<typename _T>
inline StrongReferenceCountingPointer<_T>::StrongReferenceCountingPointer(const StrongReferenceCountingPointer<_T>& copy) noexcept
    : TReferenceCountingPointerBase<_T>(copy)
    , _swrc(copy._swrc)
    , _tPtr(copy._tPtr)
{
    if(_swrc) 
    { _swrc->addRefStrong(); }
}

template<typename _T>
inline StrongReferenceCountingPointer<_T>::StrongReferenceCountingPointer(StrongReferenceCountingPointer<_T>&& move) noexcept
    : TReferenceCountingPointerBase<_T>(::std::move(move))
    , _swrc(move._swrc)
    , _tPtr(move._tPtr)
{ move._swrc = nullptr; }

template<typename _T>
inline StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(const StrongReferenceCountingPointer<_T>& copy) noexcept
{
    if(this == &copy)
    { return *this; }

    if(_swrc && _swrc->releaseStrong() == 0)
    {
        _swrc->destroyObj();
        if(!_swrc->_weakRefCount)
        { _swrc->_allocator.deallocateT(_swrc); }
    }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    _swrc = copy._swrc;
    _tPtr = copy._tPtr;

    if(_swrc) { _swrc->addRefStrong(); }

    return *this;
}

template<typename _T>
inline StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(StrongReferenceCountingPointer<_T>&& move) noexcept
{
    if(this == &move)
    { return *this; }

    if(_swrc && _swrc->releaseStrong() == 0)
    {
        _swrc->destroyObj();
        if(!_swrc->_weakRefCount)
        { _swrc->_allocator.deallocateT(_swrc); }
    }

    TReferenceCountingPointerBase<_T>::operator =(::std::move(move));

    _swrc = move._swrc;
    _tPtr = move._tPtr;

    move._swrc = nullptr;

    return *this;
}

template<typename _T>
inline StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(const nullptr_t) noexcept
{
    if(_swrc && _swrc->releaseStrong() == 0)
    {
        _swrc->destroyObj();
        if(!_swrc->_weakRefCount)
        { _swrc->_allocator.deallocateT(_swrc); }
    }

    _swrc = nullptr;
    _tPtr = nullptr;

    return *this;
}

template<typename _T>
template<typename _TT>
inline StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(const StrongReferenceCountingPointer<_TT>& copy) noexcept
{
    if(_swrc && _swrc->releaseStrong() == 0)
    {
        _swrc->destroyObj();
        if(!_swrc->_weakRefCount)
        { _swrc->_allocator.deallocateT(_swrc); }
    }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    _swrc = reinterpret_cast<SWRC<_T>*>(copy._getBlock());
    _tPtr = static_cast<_T*>(copy._tPtr);

    _swrc->addRefStrong();

    return *this;
}

template<typename _T>
template<typename _TT>
inline StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(StrongReferenceCountingPointer<_TT>&& move) noexcept
{
    if(_swrc && _swrc->releaseStrong() == 0)
    {
        _swrc->destroyObj();
        if(!_swrc->_weakRefCount)
        { _swrc->_allocator.deallocateT(_swrc); }
    }

    TReferenceCountingPointerBase<_T>::operator =(::std::move(move));

    _swrc = reinterpret_cast<SWRC<_T>*>(move._getBlock());
    _tPtr = static_cast<_T*>(move._tPtr);

    move._getBlock() = nullptr;

    return *this;
}

template<typename _T>
inline StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(const WeakReferenceCountingPointer<_T>& copy) noexcept
{
    if(_swrc && _swrc->releaseStrong() == 0)
    {
        _swrc->destroyObj();
        if(!_swrc->_weakRefCount)
        { _swrc->_allocator.deallocate(_swrc); }
    }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    if(copy)
    {
        _swrc = copy._rcdo;
        _tPtr = copy._tPtr;

        _swrc->addRefStrong();
    }
    else
    {
        _swrc = nullptr;
        _tPtr = nullptr;
    }

    return *this;
}

template <typename _T>
template <typename _TT>
inline StrongReferenceCountingPointer<_T>& StrongReferenceCountingPointer<_T>::operator=(const WeakReferenceCountingPointer<_TT>& copy) noexcept
{
    if(_swrc && _swrc->releaseStrong() == 0)
    {
        _swrc->destroyObj();
        if(!_swrc->_weakRefCount)
        { _swrc->_allocator.deallocate(_swrc); }
    }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    if(copy)
    {
        _swrc = reinterpret_cast<SWRC<_T>*>(copy._getBlock());
        _tPtr = static_cast<_T*>(copy._tPtr);

        _swrc->addRefStrong();
    }
    else
    {
        _swrc = nullptr;
        _tPtr = nullptr;
    }

    return *this;
}

template<typename _T>
template<typename... _Args>
inline void StrongReferenceCountingPointer<_T>::reset(TauAllocator& allocator, _Args&&... args) noexcept
{
    if(_swrc && _swrc->releaseStrong() == 0)
    {
        _swrc->destroyObj();
        if(!_swrc->_weakRefCount)
        { _swrc->_allocator.deallocateT(_swrc); }
    }

    _swrc = allocator.allocateT<SWRC<_T>>(allocator, args...);
    _tPtr = _swrc->objPtr();
}

template<typename _T>
inline bool StrongReferenceCountingPointer<_T>::operator==(const StrongReferenceCountingPointer<_T>& ptr) const noexcept
{ return _swrc == ptr._swrc; }

template<typename _T>
inline bool StrongReferenceCountingPointer<_T>::operator!=(const StrongReferenceCountingPointer<_T>& ptr) const noexcept
{ return _swrc != ptr._swrc; }

template<typename _T>
inline bool StrongReferenceCountingPointer<_T>::operator==(const WeakReferenceCountingPointer<_T>& ptr) const noexcept
{ return _swrc == ptr._rcdo; }

template<typename _T>
inline bool StrongReferenceCountingPointer<_T>::operator!=(const WeakReferenceCountingPointer<_T>& ptr) const noexcept
{ return _swrc != ptr._rcdo; }

template<typename _T>
inline bool StrongReferenceCountingPointer<_T>::operator==(const _T* const& ptr) const noexcept
{
    if(_swrc && ptr)
    { return _swrc->objPtr() == ptr; }
    if(!_swrc && !ptr)
    { return true; }
    return false;
}

template<typename _T>
inline bool StrongReferenceCountingPointer<_T>::operator!=(const _T* const& ptr) const noexcept
{
    if(_swrc && ptr)
    { return _swrc->objPtr() != ptr; }
    if(!_swrc && !ptr)
    { return false; }
    return true;
}

template<typename _T>
inline uSys WeakReferenceCountingPointer<_T>::allocSize() noexcept
{ return sizeof(SWRC<_T>); }

template<typename _T>
inline WeakReferenceCountingPointer<_T>::WeakReferenceCountingPointer(const StrongReferenceCountingPointer<_T>& ptr) noexcept
    : TReferenceCountingPointerBase<_T>(ptr)
    , _swrc(ptr._swrc)
    , _tPtr(ptr._tPtr)
{
    if(_swrc) 
    { _swrc->addRefWeak(); }
}

template<typename _T>
template<typename _TT>
inline WeakReferenceCountingPointer<_T>::WeakReferenceCountingPointer(const StrongReferenceCountingPointer<_TT>& rcp) noexcept
    : TReferenceCountingPointerBase<_T>(rcp)
    , _swrc(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    , _tPtr(static_cast<_T*>(rcp.get()))
{
    if(_swrc) 
    { _swrc->addRefWeak(); }
}

template<typename _T>
template<typename _TT>
inline WeakReferenceCountingPointer<_T>::WeakReferenceCountingPointer(const WeakReferenceCountingPointer<_TT>& rcp) noexcept
    : TReferenceCountingPointerBase<_T>(rcp)
    , _swrc(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    , _tPtr(static_cast<_T*>(rcp.get()))
{
    if(_swrc) 
    { _swrc->addRefWeak(); }
}

template<typename _T>
template<typename _TT>
inline WeakReferenceCountingPointer<_T>::WeakReferenceCountingPointer(WeakReferenceCountingPointer<_TT>&& rcp) noexcept
    : TReferenceCountingPointerBase<_T>(::std::move(rcp))
    , _swrc(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    , _tPtr(static_cast<_T*>(rcp.get()))
{ rcp._getBlock() = nullptr; }

template<typename _T>
inline constexpr WeakReferenceCountingPointer<_T>::WeakReferenceCountingPointer(const nullptr_t) noexcept
    : _swrc(nullptr)
    , _tPtr(nullptr)
{ }

template<typename _T>
inline constexpr WeakReferenceCountingPointer<_T>::WeakReferenceCountingPointer() noexcept
    : _swrc(nullptr)
    , _tPtr(nullptr)
{ }

template<typename _T>
inline WeakReferenceCountingPointer<_T>::~WeakReferenceCountingPointer() noexcept
{
    if(_swrc && _swrc->releaseWeak() == 0 && !_swrc->_strongRefCount)
    { _swrc->_allocator.deallocate(_swrc); }
}

template<typename _T>
inline WeakReferenceCountingPointer<_T>::WeakReferenceCountingPointer(const WeakReferenceCountingPointer<_T>& copy) noexcept
    : TReferenceCountingPointerBase<_T>(copy)
    , _swrc(copy._swrc)
    , _tPtr(copy._tPtr)
{
    if(_swrc) 
    { _swrc->addRefWeak(); }
}

template<typename _T>
inline WeakReferenceCountingPointer<_T>::WeakReferenceCountingPointer(WeakReferenceCountingPointer<_T>&& move) noexcept
    : TReferenceCountingPointerBase<_T>(::std::move(move))
    , _swrc(move._swrc)
    , _tPtr(move._tPtr)
{ move._swrc = nullptr; }

template<typename _T>
inline WeakReferenceCountingPointer<_T>& WeakReferenceCountingPointer<_T>::operator=(const WeakReferenceCountingPointer<_T>& copy) noexcept
{
    if(this == &copy)
    { return *this; }

    if(_swrc->releaseWeak() == 0 && !_swrc->_strongRefCount)
    { _swrc->_allocator.deallocate(_swrc); }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    _swrc = copy._swrc;
    _tPtr = copy._tPtr;

    if(_swrc) 
    { _swrc->addRefWeak(); }

    return *this;
}

template<typename _T>
inline WeakReferenceCountingPointer<_T>& WeakReferenceCountingPointer<_T>::operator=(WeakReferenceCountingPointer<_T>&& move) noexcept
{
    if(this == &move)
    { return *this; }

    if(_swrc->releaseWeak() == 0 && !_swrc->_strongRefCount)
    { _swrc->_allocator.deallocate(_swrc); }

    TReferenceCountingPointerBase<_T>::operator =(::std::move(move));

    _swrc = move._swrc;
    _tPtr = move._tPtr;

    move._swrc = nullptr;

    return *this;
}

template<typename _T>
inline WeakReferenceCountingPointer<_T>& WeakReferenceCountingPointer<_T>::operator=(const StrongReferenceCountingPointer<_T>& copy) noexcept
{
    if(_swrc->releaseWeak() == 0 && !_swrc->_strongRefCount)
    { _swrc->_allocator.deallocate(_swrc); }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    _swrc = copy._rcdo;
    _tPtr = copy._tPtr;

    if(_swrc) 
    { _swrc->addRefWeak(); }

    return *this;
}

template<typename _T>
template<typename _TT>
inline WeakReferenceCountingPointer<_T>& WeakReferenceCountingPointer<_T>::operator=(const StrongReferenceCountingPointer<_TT>& copy) noexcept
{
    if(_swrc->releaseWeak() == 0 && !_swrc->_strongRefCount)
    { _swrc->_allocator.deallocate(_swrc); }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    _swrc = reinterpret_cast<SWRC<_T>*>(copy._getBlock());
    _tPtr = static_cast<_T*>(copy._tPtr);

    if(_swrc) 
    { _swrc->addRefWeak(); }

    return *this;
}

template<typename _T>
template<typename _TT>
inline WeakReferenceCountingPointer<_T>& WeakReferenceCountingPointer<_T>::operator=(const WeakReferenceCountingPointer<_TT>& copy) noexcept
{
    if(_swrc->releaseWeak() == 0 && !_swrc->_strongRefCount)
    { _swrc->_allocator.deallocate(_swrc); }

    TReferenceCountingPointerBase<_T>::operator =(copy);

    _swrc = reinterpret_cast<SWRC<_T>*>(copy._getBlock());
    _tPtr = static_cast<_T*>(copy._tPtr);

    if(_swrc) 
    { _swrc->addRefWeak(); }

    return *this;
}

template<typename _T>
template<typename _TT>
inline WeakReferenceCountingPointer<_T>& WeakReferenceCountingPointer<_T>::operator=(WeakReferenceCountingPointer<_TT>&& move) noexcept
{
    if(_swrc->releaseWeak() == 0 && !_swrc->_strongRefCount)
    { _swrc->_allocator.deallocate(_swrc); }

    TReferenceCountingPointerBase<_T>::operator=(::std::move(move));

    _swrc = reinterpret_cast<SWRC<_T>*>(move._getBlock());
    _tPtr = static_cast<_T*>(move._tPtr);

    move._getBlock() = nullptr;

    return *this;
}

template<typename _T>
inline bool WeakReferenceCountingPointer<_T>::operator==(const _T* const& ptr) const noexcept
{
    if(_swrc && ptr)
    { return _swrc->objPtr() == ptr; }
    if(!_swrc && !ptr)
    { return true; }
    return false;
}

template<typename _T>
inline bool WeakReferenceCountingPointer<_T>::operator!=(const _T* const& ptr) const noexcept
{
    if(_swrc && ptr)
    { return _swrc->objPtr() != ptr; }
    if(!_swrc && !ptr)
    { return false; }
    return true;
}
