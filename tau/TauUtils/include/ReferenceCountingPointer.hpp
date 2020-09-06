// ReSharper disable CppClangTidyBugproneUnhandledSelfAssignment
#pragma once

#include "Objects.hpp"
#include "NumTypes.hpp"
#include "allocator/TauAllocator.hpp"
#include <atomic>

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
    DELETE_CM(_ReferenceCountDataObject);
public:
    uSys _refCount;
    TauAllocator& _allocator;
    u8 _objRaw[sizeof(_T)];
public:
    template<typename... _Args>
    _ReferenceCountDataObject(TauAllocator& allocator, _Args&&... args) noexcept;

    ~_ReferenceCountDataObject() noexcept;

    [[nodiscard]]       _T& obj()       noexcept { return *reinterpret_cast<_T*>(_objRaw); }
    [[nodiscard]] const _T& obj() const noexcept { return *reinterpret_cast<_T*>(_objRaw); }

    [[nodiscard]]       _T* objPtr()       noexcept { return reinterpret_cast<_T*>(_objRaw); }
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
    DELETE_CM(_SWReferenceCount);
public:
    uSys _strongRefCount;
    uSys _weakRefCount;
    TauAllocator& _allocator;
    u8 _objRaw[sizeof(_T)];
public:
    template<typename... _Args>
    _SWReferenceCount(TauAllocator& allocator, _Args&&... args) noexcept;

    [[nodiscard]]       _T& obj()       noexcept { return *reinterpret_cast<_T*>(_objRaw); }
    [[nodiscard]] const _T& obj() const noexcept { return *reinterpret_cast<_T*>(_objRaw); }

    [[nodiscard]]       _T* objPtr()       noexcept { return reinterpret_cast<_T*>(_objRaw); }
    [[nodiscard]] const _T* objPtr() const noexcept { return reinterpret_cast<_T*>(_objRaw); }

    void destroyObj() noexcept { reinterpret_cast<_T*>(_objRaw)->~_T(); }
};

template<typename _ToT, typename _FromT>
const _ReferenceCountDataObject<_ToT>& RCDOCast(const _ReferenceCountDataObject<_FromT>& obj) noexcept;

template<typename _ToT, typename _FromT>
_ReferenceCountDataObject<_ToT>& RCDOCast(_ReferenceCountDataObject<_FromT>& obj) noexcept;

template<typename _ToT, typename _FromT>
_ReferenceCountDataObject<_ToT>&& RCDOCast(_ReferenceCountDataObject<_FromT>&& obj) noexcept;

template<typename _ToT, typename _FromT>
const _SWReferenceCount<_ToT>& SWRCCast(const _SWReferenceCount<_FromT>& obj) noexcept;

template<typename _ToT, typename _FromT>
_SWReferenceCount<_ToT>& SWRCCast(_SWReferenceCount<_FromT>& obj) noexcept;

template<typename _ToT, typename _FromT>
_SWReferenceCount<_ToT>&& SWRCCast(_SWReferenceCount<_FromT>&& obj) noexcept;
}

class ReferenceCountingPointerBase
{
    DEFAULT_CONSTRUCT_PO(ReferenceCountingPointerBase);
    DEFAULT_DESTRUCT_VI(ReferenceCountingPointerBase);
    DEFAULT_CM_PO(ReferenceCountingPointerBase);
public:
    [[nodiscard]] virtual uSys refCount() const noexcept = 0;

    [[nodiscard]] virtual operator bool() const noexcept = 0;

    [[nodiscard]] virtual bool operator ==(nullptr_t) const noexcept = 0;
    [[nodiscard]] virtual bool operator !=(nullptr_t) const noexcept = 0;
};

template<typename _T>
class TReferenceCountingPointerBase : public ReferenceCountingPointerBase
{
    DEFAULT_CONSTRUCT_PO(TReferenceCountingPointerBase);
    DEFAULT_DESTRUCT_VI(TReferenceCountingPointerBase);
    DEFAULT_CM_PO(TReferenceCountingPointerBase);
protected:
    template<typename _TT>
    TReferenceCountingPointerBase(const TReferenceCountingPointerBase<_TT>& copy) noexcept
    { }

    template<typename _TT>
    TReferenceCountingPointerBase(TReferenceCountingPointerBase<_TT>&& move) noexcept
    { }

    template<typename _TT>
    TReferenceCountingPointerBase<_T>& operator=(const TReferenceCountingPointerBase<_TT>& copy) noexcept
    { return *this; }

    template<typename _TT>
    TReferenceCountingPointerBase<_T>& operator=(TReferenceCountingPointerBase<_TT>&& move) noexcept
    { return *this; }
public:
    [[nodiscard]] virtual       _T& operator  *()                = 0;
    [[nodiscard]] virtual const _T& operator  *() const          = 0;
    [[nodiscard]] virtual       _T* operator ->()       noexcept = 0;
    [[nodiscard]] virtual const _T* operator ->() const noexcept = 0;

    [[nodiscard]] virtual       _T* get()       noexcept = 0;
    [[nodiscard]] virtual const _T* get() const noexcept = 0;

    [[nodiscard]] virtual bool operator ==(const _T* const& ptr) const noexcept = 0;
    [[nodiscard]] virtual bool operator !=(const _T* const& ptr) const noexcept = 0;
};

template<typename _T>
class ReferenceCountingPointer;

template<typename _T>
class StrongReferenceCountingPointer;

template<typename _T>
class WeakReferenceCountingPointer;

template<typename _T>
class ReferenceCountingPointer final : public TReferenceCountingPointerBase<_T>
{
public:
    template<typename _TT>
    using RCDO = _ReferenceCountingPointerUtils::_ReferenceCountDataObject<_TT>;

    [[nodiscard]] static uSys allocSize() noexcept;
private:
    RCDO<_T>* _rcdo;
    _T* _tPtr;
public:
    template<typename... _Args>
    explicit ReferenceCountingPointer(TauAllocator& allocator, _Args&&... args) noexcept;

    template<typename _TT>
    ReferenceCountingPointer(const ReferenceCountingPointer<_TT>& rcp) noexcept;

    template<typename _TT>
    ReferenceCountingPointer(ReferenceCountingPointer<_TT>&& rcp) noexcept;

    constexpr ReferenceCountingPointer(nullptr_t) noexcept;

    constexpr ReferenceCountingPointer() noexcept;

    ~ReferenceCountingPointer() noexcept override;

    ReferenceCountingPointer(const ReferenceCountingPointer<_T>& copy) noexcept;
    ReferenceCountingPointer(ReferenceCountingPointer<_T>&& move) noexcept;

    ReferenceCountingPointer<_T>& operator=(nullptr_t) noexcept;

    ReferenceCountingPointer<_T>& operator=(const ReferenceCountingPointer<_T>& copy) noexcept;
    ReferenceCountingPointer<_T>& operator=(ReferenceCountingPointer<_T>&& move) noexcept;

    template<typename _TT>
    ReferenceCountingPointer<_T>& operator=(const ReferenceCountingPointer<_TT>& copy) noexcept;

    template<typename _TT>
    ReferenceCountingPointer<_T>& operator=(ReferenceCountingPointer<_TT>&& move) noexcept;

    template<typename... _Args>
    void reset(TauAllocator& allocator, _Args&&... args) noexcept;

    [[nodiscard]]       _T& operator  *()                override { return *_tPtr; }
    [[nodiscard]] const _T& operator  *() const          override { return *_tPtr; }
    [[nodiscard]]       _T* operator ->()       noexcept override { return _tPtr; }
    [[nodiscard]] const _T* operator ->() const noexcept override { return _tPtr; }

    [[nodiscard]]       _T* get()       noexcept override { return _tPtr; }
    [[nodiscard]] const _T* get() const noexcept override { return _tPtr; }

    [[nodiscard]] uSys refCount() const noexcept override { return _rcdo ? _rcdo->_refCount : 0; }

    [[nodiscard]] RCDO<_T>*& _getBlock()       noexcept { return _rcdo; }
    [[nodiscard]] RCDO<_T>*  _getBlock() const noexcept { return _rcdo; }

    [[nodiscard]] operator bool() const noexcept override { return _rcdo; }

    [[nodiscard]] bool operator ==(const ReferenceCountingPointer<_T>& ptr) const noexcept { return _rcdo == ptr._rcdo; }
    [[nodiscard]] bool operator !=(const ReferenceCountingPointer<_T>& ptr) const noexcept { return _rcdo != ptr._rcdo; }

    [[nodiscard]] bool operator ==(const _T* const & ptr) const noexcept override;
    [[nodiscard]] bool operator !=(const _T* const & ptr) const noexcept override;

    [[nodiscard]] bool operator ==(nullptr_t) const noexcept override { return !_rcdo; }
    [[nodiscard]] bool operator !=(nullptr_t) const noexcept override { return  _rcdo; }
};

template<typename _T>
class StrongReferenceCountingPointer final : public TReferenceCountingPointerBase<_T>
{
public:
    template<typename _TT>
    using SWRC = _ReferenceCountingPointerUtils::_SWReferenceCount<_TT>;

    [[nodiscard]] static uSys allocSize() noexcept;
private:
    SWRC<_T>* _swrc;
    _T* _tPtr;
public:
    template<typename... _Args>
    explicit StrongReferenceCountingPointer(TauAllocator& allocator, _Args&&... args) noexcept;

    template<typename _TT>
    StrongReferenceCountingPointer(const StrongReferenceCountingPointer<_TT>& rcp) noexcept;

    template<typename _TT>
    StrongReferenceCountingPointer(StrongReferenceCountingPointer<_TT>&& rcp) noexcept;

    constexpr StrongReferenceCountingPointer(nullptr_t) noexcept;

    constexpr StrongReferenceCountingPointer() noexcept;

    ~StrongReferenceCountingPointer() noexcept override;

    StrongReferenceCountingPointer(const StrongReferenceCountingPointer<_T>& copy) noexcept;
    StrongReferenceCountingPointer(StrongReferenceCountingPointer<_T>&& move) noexcept;

    StrongReferenceCountingPointer<_T>& operator=(const StrongReferenceCountingPointer<_T>& copy) noexcept;
    StrongReferenceCountingPointer<_T>& operator=(StrongReferenceCountingPointer<_T>&& move) noexcept;

    StrongReferenceCountingPointer<_T>& operator=(const WeakReferenceCountingPointer<_T>& copy) noexcept;

    StrongReferenceCountingPointer<_T>& operator=(nullptr_t) noexcept;

    template<typename _TT>
    StrongReferenceCountingPointer<_T>& operator=(const StrongReferenceCountingPointer<_TT>& copy) noexcept;

    template<typename _TT>
    StrongReferenceCountingPointer<_T>& operator=(StrongReferenceCountingPointer<_TT>&& move) noexcept;

    template<typename _TT>
    StrongReferenceCountingPointer<_T>& operator=(const WeakReferenceCountingPointer<_TT>& copy) noexcept;

    template<typename... _Args>
    void reset(TauAllocator& allocator, _Args&&... args) noexcept;

    [[nodiscard]]       _T& operator  *()                override { return *_tPtr; }
    [[nodiscard]] const _T& operator  *() const          override { return *_tPtr; }
    [[nodiscard]]       _T* operator ->()       noexcept override { return _tPtr; }
    [[nodiscard]] const _T* operator ->() const noexcept override { return _tPtr; }

    [[nodiscard]]       _T* get()       noexcept override { return _tPtr; }
    [[nodiscard]] const _T* get() const noexcept override { return _tPtr; }

    [[nodiscard]] uSys refCount()       const noexcept override { return _swrc ? _swrc->_strongRefCount : 0; }
    [[nodiscard]] uSys strongRefCount() const noexcept          { return _swrc ? _swrc->_strongRefCount : 0; }
    [[nodiscard]] uSys weakRefCount()   const noexcept          { return _swrc ? _swrc->_weakRefCount   : 0; }

    [[nodiscard]] SWRC<_T>*& _getBlock()       noexcept { return _swrc; }
    [[nodiscard]] SWRC<_T>*  _getBlock() const noexcept { return _swrc; }

    [[nodiscard]] operator bool() const noexcept override { return _swrc; }

    [[nodiscard]] bool operator ==(const StrongReferenceCountingPointer<_T>& ptr) const noexcept;
    [[nodiscard]] bool operator !=(const StrongReferenceCountingPointer<_T>& ptr) const noexcept;

    [[nodiscard]] bool operator ==(const WeakReferenceCountingPointer<_T>& ptr) const noexcept;
    [[nodiscard]] bool operator !=(const WeakReferenceCountingPointer<_T>& ptr) const noexcept;

    [[nodiscard]] bool operator ==(const _T* const& ptr) const noexcept override;
    [[nodiscard]] bool operator !=(const _T* const& ptr) const noexcept override;

    [[nodiscard]] bool operator ==(nullptr_t) const noexcept override { return !_swrc; }
    [[nodiscard]] bool operator !=(nullptr_t) const noexcept override { return  _swrc; }
private:
    friend class WeakReferenceCountingPointer<_T>;
};

template<typename _T>
class WeakReferenceCountingPointer final : public TReferenceCountingPointerBase<_T>
{
public:
    template<typename _TT>
    using SWRC = _ReferenceCountingPointerUtils::_SWReferenceCount<_TT>;

    [[nodiscard]] static uSys allocSize() noexcept;
private:
    SWRC<_T>* _swrc;
    _T* _tPtr;
public:
    WeakReferenceCountingPointer(const StrongReferenceCountingPointer<_T>& ptr) noexcept;

    template<typename _TT>
    WeakReferenceCountingPointer(const StrongReferenceCountingPointer<_TT>& rcp) noexcept;

    template<typename _TT>
    WeakReferenceCountingPointer(const WeakReferenceCountingPointer<_TT>& rcp) noexcept;

    template<typename _TT>
    WeakReferenceCountingPointer(WeakReferenceCountingPointer<_TT>&& rcp) noexcept;

    constexpr WeakReferenceCountingPointer(nullptr_t) noexcept;

    constexpr WeakReferenceCountingPointer() noexcept;

    ~WeakReferenceCountingPointer() noexcept override;

    WeakReferenceCountingPointer(const WeakReferenceCountingPointer<_T>& copy) noexcept;
    WeakReferenceCountingPointer(WeakReferenceCountingPointer<_T>&& move) noexcept;

    WeakReferenceCountingPointer<_T>& operator=(const WeakReferenceCountingPointer<_T>& copy) noexcept;
    WeakReferenceCountingPointer<_T>& operator=(WeakReferenceCountingPointer<_T>&& move) noexcept;

    WeakReferenceCountingPointer<_T>& operator=(const StrongReferenceCountingPointer<_T>& copy) noexcept;

    template<typename _TT>
    WeakReferenceCountingPointer<_T>& operator=(const StrongReferenceCountingPointer<_TT>& copy) noexcept;

    template<typename _TT>
    WeakReferenceCountingPointer<_T>& operator=(const WeakReferenceCountingPointer<_TT>& copy) noexcept;

    template<typename _TT>
    WeakReferenceCountingPointer<_T>& operator=(WeakReferenceCountingPointer<_TT>&& move) noexcept;

    [[nodiscard]]       _T& operator  *()                override { return *_tPtr; }
    [[nodiscard]] const _T& operator  *() const          override { return *_tPtr; }
    [[nodiscard]]       _T* operator ->()       noexcept override { return _swrc && _swrc->_strongRefCount ? _tPtr : nullptr; }
    [[nodiscard]] const _T* operator ->() const noexcept override { return _swrc && _swrc->_strongRefCount ? _tPtr : nullptr; }

    [[nodiscard]]       _T* get()       noexcept override { return _swrc && _swrc->_strongRefCount ? _tPtr : nullptr; }
    [[nodiscard]] const _T* get() const noexcept override { return _swrc && _swrc->_strongRefCount ? _tPtr : nullptr; }

    [[nodiscard]] uSys refCount()       const noexcept override { return _swrc ? _swrc->_weakRefCount   : 0; }
    [[nodiscard]] uSys strongRefCount() const noexcept          { return _swrc ? _swrc->_strongRefCount : 0; }
    [[nodiscard]] uSys weakRefCount()   const noexcept          { return _swrc ? _swrc->_weakRefCount   : 0; }

    [[nodiscard]] SWRC<_T>*& _getBlock()       noexcept { return _swrc; }
    [[nodiscard]] SWRC<_T>*  _getBlock() const noexcept { return _swrc; }

    [[nodiscard]] operator bool() const noexcept override { return _swrc && _swrc->_strongRefCount; }

    [[nodiscard]] bool operator ==(const StrongReferenceCountingPointer<_T>& ptr) const noexcept { return _swrc == ptr._rcdo; }
    [[nodiscard]] bool operator !=(const StrongReferenceCountingPointer<_T>& ptr) const noexcept { return _swrc != ptr._rcdo; }

    [[nodiscard]] bool operator ==(const WeakReferenceCountingPointer<_T>& ptr) const noexcept { return _swrc == ptr._swrc; }
    [[nodiscard]] bool operator !=(const WeakReferenceCountingPointer<_T>& ptr) const noexcept { return _swrc != ptr._swrc; }

    [[nodiscard]] bool operator ==(const _T* const& ptr) const noexcept override;
    [[nodiscard]] bool operator !=(const _T* const& ptr) const noexcept override;

    [[nodiscard]] bool operator ==(const nullptr_t) const noexcept override { return !_swrc; }
    [[nodiscard]] bool operator !=(const nullptr_t) const noexcept override { return  _swrc; }
private:
    friend class StrongReferenceCountingPointer<_T>;
};

template<typename _ToT, typename _FromT>
ReferenceCountingPointer<_ToT> RCPStaticCast(const ReferenceCountingPointer<_FromT>& ptr) noexcept
{ return ReferenceCountingPointer<_ToT>(ptr); }

template<typename _ToT, typename _FromT>
ReferenceCountingPointer<_ToT> RCPStaticCast(ReferenceCountingPointer<_FromT>&& ptr) noexcept
{ return ReferenceCountingPointer<_ToT>(ptr); }

template<typename _ToT, typename _FromT>
StrongReferenceCountingPointer<_ToT> RCPStaticCast(const StrongReferenceCountingPointer<_FromT>& ptr) noexcept
{ return StrongReferenceCountingPointer<_ToT>(ptr); }

template<typename _ToT, typename _FromT>
StrongReferenceCountingPointer<_ToT> RCPStaticCast(StrongReferenceCountingPointer<_FromT>&& ptr) noexcept
{ return StrongReferenceCountingPointer<_ToT>(ptr); }

template<typename _ToT, typename _FromT>
WeakReferenceCountingPointer<_ToT> RCPStaticCast(const WeakReferenceCountingPointer<_FromT>& ptr) noexcept
{ return WeakReferenceCountingPointer<_ToT>(ptr); }

template<typename _ToT, typename _FromT>
WeakReferenceCountingPointer<_ToT> RCPStaticCast(WeakReferenceCountingPointer<_FromT>&& ptr) noexcept
{ return WeakReferenceCountingPointer<_ToT>(ptr); }

template<typename _ToT, typename _FromT>
ReferenceCountingPointer<_ToT> RCPReinterpretCast(const ReferenceCountingPointer<_FromT>& ptr) noexcept
{ return reinterpret_cast<const ReferenceCountingPointer<_ToT>&>(ptr); }

template<typename _ToT, typename _FromT>
ReferenceCountingPointer<_ToT> RCPReinterpretCast(ReferenceCountingPointer<_FromT>&& ptr) noexcept
{ return reinterpret_cast<ReferenceCountingPointer<_ToT>&&>(ptr); }

template<typename _ToT, typename _FromT>
StrongReferenceCountingPointer<_ToT> RCPReinterpretCast(const StrongReferenceCountingPointer<_FromT>& ptr) noexcept
{ return reinterpret_cast<const StrongReferenceCountingPointer<_ToT>&>(ptr); }

template<typename _ToT, typename _FromT>
StrongReferenceCountingPointer<_ToT> RCPReinterpretCast(StrongReferenceCountingPointer<_FromT>&& ptr) noexcept
{ return reinterpret_cast<StrongReferenceCountingPointer<_ToT>&&>(ptr); }

template<typename _ToT, typename _FromT>
WeakReferenceCountingPointer<_ToT> RCPReinterpretCast(const WeakReferenceCountingPointer<_FromT>& ptr) noexcept
{ return reinterpret_cast<const WeakReferenceCountingPointer<_ToT>&>(ptr); }

template<typename _ToT, typename _FromT>
WeakReferenceCountingPointer<_ToT> RCPReinterpretCast(WeakReferenceCountingPointer<_FromT>&& ptr) noexcept
{ return reinterpret_cast<WeakReferenceCountingPointer<_ToT>&&>(ptr); }

#include "ReferenceCountingPointer.inl"
