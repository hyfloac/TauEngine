#include <cstddef>
#include <cstdint>
#include <cstdio>

#if 0
  #define PRINTF(...) PRINTF(__VA_ARGS__)
#else
  #define PRINTF(...)
#endif

typedef ::std::size_t uSys;
typedef ::std::uint8_t u8;

namespace _TauAllocatorUtils {
template<typename _T>
struct _RemoveReference final
{ using type = _T; };

template<typename _T>
struct _RemoveReference<_T&> final
{ using type = _T; };

template<typename _T>
struct _RemoveReference<_T&&> final
{ using type = _T; };

template<typename _T>
using _RemoveReferenceT = typename _RemoveReference<_T>::type;

template<typename _T>
[[nodiscard]] constexpr inline _T&& _forward(_RemoveReferenceT<_T>& ref) noexcept
{ return static_cast<_T&&>(ref); }

template<typename _T>
[[nodiscard]] constexpr inline _T&& _forward(_RemoveReferenceT<_T>&& ref) noexcept
{ return static_cast<_T&&>(ref); }
}

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
public:
    uSys _refCount;
    u8 _objRaw[sizeof(_T)];
public:
    template<typename... _Args>
    _ReferenceCountDataObject(_Args&&... args) noexcept
        : _refCount(1), _objRaw{}
    { (void) new(_objRaw) _T(_TauAllocatorUtils::_forward<_Args>(args)...); }

    ~_ReferenceCountDataObject() noexcept
    { reinterpret_cast<_T*>(_objRaw)->~_T(); }

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
public:
    uSys _strongRefCount;
    uSys _weakRefCount;
    u8 _objRaw[sizeof(_T)];
public:
    template<typename... _Args>
    _SWReferenceCount(_Args&&... args) noexcept
        : _strongRefCount(1), _weakRefCount(0), _objRaw{}
    { (void) new(_objRaw) _T(_TauAllocatorUtils::_forward<_Args>(args)...); }

    [[nodiscard]]       _T& obj()       noexcept { return *reinterpret_cast<_T*>(_objRaw); }
    [[nodiscard]] const _T& obj() const noexcept { return *reinterpret_cast<_T*>(_objRaw); }

    [[nodiscard]]       _T* objPtr()       noexcept { return reinterpret_cast<_T*>(_objRaw); }
    [[nodiscard]] const _T* objPtr() const noexcept { return reinterpret_cast<_T*>(_objRaw); }

    void destroyObj() noexcept { reinterpret_cast<_T*>(_objRaw)->~_T(); }
};

template<typename _ToT, typename _FromT>
const _ReferenceCountDataObject<_ToT>& RCDOCast(const _ReferenceCountDataObject<_FromT>& obj) noexcept
{ return reinterpret_cast<const _ReferenceCountDataObject<_ToT>&>(obj); }

template<typename _ToT, typename _FromT>
_ReferenceCountDataObject<_ToT>& RCDOCast(_ReferenceCountDataObject<_FromT>& obj) noexcept
{ return reinterpret_cast<_ReferenceCountDataObject<_ToT>&>(obj); }

template<typename _ToT, typename _FromT>
_ReferenceCountDataObject<_ToT>&& RCDOCast(_ReferenceCountDataObject<_FromT>&& obj) noexcept
{ return reinterpret_cast<_ReferenceCountDataObject<_ToT>&&>(obj); }

template<typename _ToT, typename _FromT>
const _SWReferenceCount<_ToT>& SWRCCast(const _SWReferenceCount<_FromT>& obj) noexcept
{ return reinterpret_cast<const _SWReferenceCount<_ToT>&>(obj); }

template<typename _ToT, typename _FromT>
_SWReferenceCount<_ToT>& SWRCCast(_SWReferenceCount<_FromT>& obj) noexcept
{ return reinterpret_cast<_SWReferenceCount<_ToT>&>(obj); }

template<typename _ToT, typename _FromT>
_SWReferenceCount<_ToT>&& SWRCCast(_SWReferenceCount<_FromT>&& obj) noexcept
{ return reinterpret_cast<_SWReferenceCount<_ToT>&&>(obj); }

}

class PtrBase
{
protected:
    PtrBase() noexcept = default;

    PtrBase(const PtrBase& copy) noexcept = default;
    PtrBase(PtrBase&& move) noexcept = default;

    PtrBase& operator =(const PtrBase& copy) noexcept = default;
    PtrBase& operator =(PtrBase&& move) noexcept = default;
public:
    virtual ~PtrBase() noexcept = default;

    [[nodiscard]] virtual uSys refCount() const noexcept = 0;

    [[nodiscard]] virtual operator bool() const noexcept = 0;

    [[nodiscard]] virtual bool operator ==(nullptr_t ptr) const noexcept = 0;
    [[nodiscard]] virtual bool operator !=(nullptr_t ptr) const noexcept = 0;
};

template<typename _T>
class TPtrBase : public PtrBase
{
protected:
    TPtrBase() noexcept = default;

    TPtrBase(const TPtrBase<_T>& copy) noexcept = default;
    TPtrBase(TPtrBase<_T>&& move) noexcept = default;

    template<typename _TT>
    TPtrBase(const TPtrBase<_TT>& copy) noexcept { }
    
    template<typename _TT>
    TPtrBase(TPtrBase<_TT>&& move) noexcept { }

    TPtrBase<_T>& operator =(const TPtrBase<_T>& copy) noexcept = default;
    TPtrBase<_T>& operator =(TPtrBase<_T>&& move) noexcept = default;

    template<typename _TT>
    TPtrBase<_T>& operator =(const TPtrBase<_TT>& copy) noexcept { }

    template<typename _TT>
    TPtrBase<_T>& operator =(TPtrBase<_TT>&& move) noexcept { }
public:
    virtual ~TPtrBase() noexcept = default;

    [[nodiscard]] virtual       _T& operator  *()                 = 0;
    [[nodiscard]] virtual const _T& operator  *() const           = 0;

    [[nodiscard]] virtual       _T* operator ->()       noexcept = 0;
    [[nodiscard]] virtual const _T* operator ->() const noexcept = 0;

    [[nodiscard]] virtual       _T* get()       noexcept = 0;
    [[nodiscard]] virtual const _T* get() const noexcept = 0;

    [[nodiscard]] virtual bool operator ==(const _T*& ptr) const noexcept = 0;
    [[nodiscard]] virtual bool operator !=(const _T*& ptr) const noexcept = 0;
};

template<typename _T>
class RcPtr final : public TPtrBase<_T>
{
public:
    template<typename _TT>
    using RCDO = _ReferenceCountingPointerUtils::_ReferenceCountDataObject<_TT>;

    [[nodiscard]] static uSys allocSize() noexcept
    { return sizeof(RCDO<_T>); }
private:
    RCDO<_T>* _ptr;
public:
    template<typename... _Args>
    inline explicit RcPtr(_Args&&... args) noexcept
        : _ptr(new(::std::nothrow) RCDO<_T>(args...))
    { }

    template<typename _TT>
    inline RcPtr(const RcPtr<_TT>& rcp) noexcept
        : _ptr(reinterpret_cast<RCDO<_T>*>(rcp._getBlock()))
    {
        if(_ptr) { ++_ptr->_refCount; }
    }

    template<typename _TT>
    inline RcPtr(RcPtr<_TT>&& rcp) noexcept
        : _ptr(reinterpret_cast<RCDO<_T>*>(rcp._getBlock()))
    { rcp._getBlock() = nullptr; }

    inline ~RcPtr() noexcept
    {
        if(_ptr && --_ptr->_refCount == 0)
        { delete _ptr; }
    }

    template<typename... _Args>
    inline void reset(_Args&&... args) noexcept
    {
        if(_ptr && --_ptr->_refCount == 0)
        { delete _ptr; }

        _ptr = new(::std::nothrow) RCDO<_T>(allocator, args...);
    }

    inline RcPtr<_T>& operator=(const RcPtr<_T>& copy) noexcept
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

    inline RcPtr<_T>& operator=(RcPtr<_T>&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(_ptr && --_ptr->_refCount == 0)
        { delete _ptr; }

        _ptr = move._ptr;
        move._ptr = nullptr;

        return *this;
    }

    template<typename _TT>
    inline RcPtr<_T>& operator=(const RcPtr<_TT>& copy) noexcept
    {
        if(_ptr && --_ptr->_refCount == 0)
        { delete _ptr; }

        _ptr = reinterpret_cast<RCDO<_T>*>(copy._getBlock());
        if(_ptr)
        { ++_ptr->_refCount; }

        return *this;
    }

    template<typename _TT>
    inline RcPtr<_T>& operator=(RcPtr<_TT>&& move) noexcept
    {
        if(_ptr && --_ptr->_refCount == 0)
        { delete _ptr; }

        _ptr = reinterpret_cast<RCDO<_T>*>(move._getBlock());
        move._getBlock() = nullptr;

        return *this;
    }

    [[nodiscard]]       _T& operator  *()                override { return _ptr->obj(); }
    [[nodiscard]] const _T& operator  *() const          override { return _ptr->obj(); }
    [[nodiscard]]       _T* operator ->()       noexcept override { return _ptr ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] const _T* operator ->() const noexcept override { return _ptr ? _ptr->objPtr() : nullptr; }

    [[nodiscard]]       _T* get()       noexcept override { return _ptr ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] const _T* get() const noexcept override { return _ptr ? _ptr->objPtr() : nullptr; }

    [[nodiscard]] inline uSys refCount() const noexcept override { return _ptr ? _ptr->_refCount : 0; }

    [[nodiscard]] inline RCDO<_T>*& _getBlock()       noexcept { return _ptr; }
    [[nodiscard]] inline RCDO<_T>*  _getBlock() const noexcept { return _ptr; }

    [[nodiscard]] inline operator bool() const noexcept override { return _ptr; }

    [[nodiscard]] inline bool operator ==(const RcPtr<_T>& ptr) const noexcept
    {
        if(_ptr && ptr._ptr)
        { return _ptr == ptr._ptr; }
        if(!_ptr && !ptr._ptr)
        { return true; }
        return false;
    }

    [[nodiscard]] inline bool operator !=(const RcPtr<_T>& ptr) const noexcept
    {
        if(_ptr && ptr._ptr)
        { return _ptr != ptr._ptr; }
        if(!_ptr && !ptr._ptr)
        { return false; }
        return true;
    }

    [[nodiscard]] inline bool operator ==(const _T*& ptr) const noexcept override
    {
        if(_ptr && ptr)
        { return _ptr->objPtr() == ptr; }
        if(!_ptr && !ptr) 
        { return true; }
        return false;
    }

    [[nodiscard]] inline bool operator !=(const _T*& ptr) const noexcept override
    {
        if(_ptr && ptr)
        { return _ptr->objPtr() != ptr; }
        if(!_ptr && !ptr) 
        { return false; }
        return true;
    }

    [[nodiscard]] inline bool operator ==(const nullptr_t ptr) const noexcept override { return _ptr == ptr; }
    [[nodiscard]] inline bool operator !=(const nullptr_t ptr) const noexcept override { return _ptr != ptr; }

};

template<typename _T>
class WeakRcPtr;

template<typename _T>
class StrongRcPtr final : public TPtrBase<_T>
{
public:
    template<typename _TT>
    using SWRC = _ReferenceCountingPointerUtils::_SWReferenceCount<_TT>;

    [[nodiscard]] static uSys allocSize() noexcept
    { return sizeof(SWRC<_T>); }
private:
    SWRC<_T>* _ptr;
public:
    template<typename... _Args>
    inline explicit StrongRcPtr(_Args&&... args) noexcept
        : _ptr(new(::std::nothrow) SWRC<_T>(args...))
    {
        PRINTF("Strong create\n");
    }
    
    inline StrongRcPtr(const StrongRcPtr<_T>& rcp) noexcept
        : _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    {
        if(_ptr) { ++_ptr->_strongRefCount; }
        PRINTF("Strong copy\n");
    }

    inline StrongRcPtr(StrongRcPtr<_T>&& rcp) noexcept
        : _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    { 
        rcp._getBlock() = nullptr; 
        PRINTF("Strong move\n");
    }

    template<typename _TT>
    inline StrongRcPtr(const StrongRcPtr<_TT>& rcp) noexcept
        : _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    {
        if(_ptr) { ++_ptr->_strongRefCount; }
        PRINTF("Strong copy typed\n");
    }

    template<typename _TT>
    inline StrongRcPtr(StrongRcPtr<_TT>&& rcp) noexcept
        : _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    { 
        rcp._getBlock() = nullptr; 
        PRINTF("Strong move typed\n");
    }

    inline ~StrongRcPtr() noexcept
    {
        if(_ptr && --_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { delete _ptr; }
        }
        PRINTF("Strong destruct \n");
    }

    template<typename... _Args>
    inline void reset(_Args&&... args) noexcept
    {
        if(_ptr && --_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { delete _ptr; }
        }

        _ptr = new(::std::nothrow) SWRC<_T>(allocator, args...);

        PRINTF("Strong reset\n");
    }

    inline StrongRcPtr<_T>& operator=(const StrongRcPtr<_T>& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(_ptr && --_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { delete _ptr; }
        }

        _ptr = copy._ptr;
        if(_ptr)
        { ++_ptr->_strongRefCount; }

        PRINTF("Strong copy\n");

        return *this;
    }

    inline StrongRcPtr<_T>& operator=(StrongRcPtr<_T>&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(_ptr && --_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { delete _ptr; }
        }

        _ptr = move._ptr;
        move._ptr = nullptr;

        PRINTF("Strong move\n");

        return *this;
    }

    template<typename _TT>
    inline StrongRcPtr<_T>& operator=(const StrongRcPtr<_TT>& copy) noexcept
    {
        if(_ptr && --_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { delete _ptr; }
        }

        _ptr = reinterpret_cast<SWRC<_T>*>(copy._getBlock());
        if(_ptr)
        { ++_ptr->_strongRefCount; }

        PRINTF("Strong copy typed\n");

        return *this;
    }

    template<typename _TT>
    inline StrongRcPtr<_T>& operator=(StrongRcPtr<_TT>&& move) noexcept
    {
        if(_ptr && --_ptr->_strongRefCount == 0)
        {
            _ptr->destroyObj();
            if(!_ptr->_weakRefCount)
            { delete _ptr; }
        }

        _ptr = reinterpret_cast<SWRC<_T>*>(move._getBlock());
        move._getBlock() = nullptr;

        PRINTF("Strong move typed\n");

        return *this;
    }

    [[nodiscard]]       _T& operator  *()                override { return _ptr->obj(); }
    [[nodiscard]] const _T& operator  *() const          override { return _ptr->obj(); }
    [[nodiscard]]       _T* operator ->()       noexcept override { return _ptr ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] const _T* operator ->() const noexcept override { return _ptr ? _ptr->objPtr() : nullptr; }

    [[nodiscard]]       _T* get()       noexcept override { return _ptr ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] const _T* get() const noexcept override { return _ptr ? _ptr->objPtr() : nullptr; }

    [[nodiscard]] inline uSys refCount() const noexcept override { return _ptr ? _ptr->_strongRefCount : 0; }
    [[nodiscard]] inline uSys strongRefCount() const noexcept { return _ptr ? _ptr->_strongRefCount : 0; }
    [[nodiscard]] inline uSys weakRefCount() const noexcept { return _ptr ? _ptr->_weakRefCount : 0; }

    [[nodiscard]] inline SWRC<_T>*& _getBlock()       noexcept { return _ptr; }
    [[nodiscard]] inline SWRC<_T>*  _getBlock() const noexcept { return _ptr; }

    [[nodiscard]] inline operator bool() const noexcept override { return _ptr; }

    [[nodiscard]] inline bool operator ==(const StrongRcPtr<_T>& ptr) const noexcept
    {
        if(_ptr && ptr._ptr)
        { return _ptr == ptr._ptr; }
        if(!_ptr && !ptr._ptr)
        { return true; }
        return false;
    }

    [[nodiscard]] inline bool operator !=(const StrongRcPtr<_T>& ptr) const noexcept
    {
        if(_ptr && ptr._ptr)
        { return _ptr != ptr._ptr; }
        if(!_ptr && !ptr._ptr)
        { return false; }
        return true;
    }

    [[nodiscard]] inline bool operator ==(const _T*& ptr) const noexcept override
    {
        if(_ptr && ptr)
        { return _ptr->objPtr() == ptr; }
        if(!_ptr && !ptr) 
        { return true; }
        return false;
    }

    [[nodiscard]] inline bool operator !=(const _T*& ptr) const noexcept override
    {
        if(_ptr && ptr)
        { return _ptr->objPtr() != ptr; }
        if(!_ptr && !ptr) 
        { return false; }
        return true;
    }

    [[nodiscard]] inline bool operator ==(const nullptr_t ptr) const noexcept override { return _ptr == ptr; }
    [[nodiscard]] inline bool operator !=(const nullptr_t ptr) const noexcept override { return _ptr != ptr; }
private:
    friend class WeakRcPtr<_T>;
};

template<typename _T>
class WeakRcPtr final : public TPtrBase<_T>
{
public:
    template<typename _TT>
    using SWRC = _ReferenceCountingPointerUtils::_SWReferenceCount<_TT>;

    [[nodiscard]] static uSys allocSize() noexcept
    { return sizeof(SWRC<_T>); }
private:
    SWRC<_T>* _ptr;
public:
    inline WeakRcPtr(const StrongRcPtr<_T>& ptr) noexcept
        : _ptr(ptr._ptr)
    { 
        if(_ptr)
        { ++_ptr->_weakRefCount; }
        PRINTF("Weak create\n");
    }

    template<typename _TT>
    inline WeakRcPtr(const WeakRcPtr<_TT>& rcp) noexcept
        : _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    {
        if(_ptr) { ++_ptr->_weakRefCount; }
        PRINTF("Weak create copy\n");
    }

    template<typename _TT>
    inline WeakRcPtr(WeakRcPtr<_TT>&& rcp) noexcept
        : _ptr(reinterpret_cast<SWRC<_T>*>(rcp._getBlock()))
    { 
        rcp._getBlock() = nullptr;
        PRINTF("Weak create move\n");
    }

    inline ~WeakRcPtr() noexcept
    {
        if(_ptr && --_ptr->_weakRefCount == 0 && !_ptr->_strongRefCount)
        {
            delete _ptr;
        }
        PRINTF("Weak destruct\n");
    }

    inline WeakRcPtr<_T>& operator=(const WeakRcPtr<_T>& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(_ptr && --_ptr->_weakRefCount == 0 && !_ptr->_strongRefCount)
        {
            delete _ptr;
        }

        _ptr = copy._ptr;
        if(_ptr)
        { ++_ptr->_weakRefCount; }

        PRINTF("Weak copy\n");

        return *this;
    }

    inline WeakRcPtr<_T>& operator=(WeakRcPtr<_T>&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(_ptr && --_ptr->_weakRefCount == 0 && !_ptr->_strongRefCount)
        {
            delete _ptr;
        }

        _ptr = move._ptr;
        move._ptr = nullptr;

        PRINTF("Weak move\n");

        return *this;
    }

    template<typename _TT>
    inline WeakRcPtr<_T>& operator=(const WeakRcPtr<_TT>& copy) noexcept
    {
        if(_ptr && --_ptr->_weakRefCount == 0 && !_ptr->_strongRefCount)
        {
            delete _ptr;
        }

        _ptr = reinterpret_cast<SWRC<_T>*>(copy._getBlock());
        if(_ptr)
        { ++_ptr->_weakRefCount; }

        PRINTF("Weak copy typed\n");

        return *this;
    }

    template<typename _TT>
    inline WeakRcPtr<_T>& operator=(WeakRcPtr<_TT>&& move) noexcept
    {
        if(_ptr && --_ptr->_weakRefCount == 0 && !_ptr->_strongRefCount)
        {
            delete _ptr;
        }

        _ptr = reinterpret_cast<SWRC<_T>*>(move._getBlock());
        move._getBlock() = nullptr;

        PRINTF("Weak move typed\n");

        return *this;
    }

    inline WeakRcPtr<_T>& operator=(const StrongRcPtr<_T>& copy) noexcept
    {
        if(_ptr && --_ptr->_weakRefCount == 0 && !_ptr->_strongRefCount)
        {
            delete _ptr;
        }

        _ptr = copy._ptr;
        if(_ptr)
        { ++_ptr->_weakRefCount; }

        PRINTF("Weak reinstantiate\n");

        return *this;
    }

    template<typename _TT>
    inline WeakRcPtr<_T>& operator=(const StrongRcPtr<_TT>& copy) noexcept
    {
        if(_ptr && --_ptr->_weakRefCount == 0 && !_ptr->_strongRefCount)
        {
            delete _ptr;
        }

        _ptr = reinterpret_cast<SWRC<_T>*>(copy._getBlock());
        if(_ptr)
        { ++_ptr->_weakRefCount; }

        PRINTF("Weak reinstantiate typed\n");

        return *this;
    }

    [[nodiscard]]       _T& operator  *()                override { return _ptr->obj(); }
    [[nodiscard]] const _T& operator  *() const          override { return _ptr->obj(); }
    [[nodiscard]]       _T* operator ->()       noexcept override { return _ptr ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] const _T* operator ->() const noexcept override { return _ptr ? _ptr->objPtr() : nullptr; }

    [[nodiscard]]       _T* get()       noexcept override { return _ptr ? _ptr->objPtr() : nullptr; }
    [[nodiscard]] const _T* get() const noexcept override { return _ptr ? _ptr->objPtr() : nullptr; }

    [[nodiscard]] inline uSys refCount() const noexcept override { return _ptr ? _ptr->_strongRefCount : 0; }
    [[nodiscard]] inline uSys strongRefCount() const noexcept { return _ptr ? _ptr->_strongRefCount : 0; }
    [[nodiscard]] inline uSys weakRefCount() const noexcept { return _ptr ? _ptr->_weakRefCount : 0; }

    [[nodiscard]] inline SWRC<_T>*& _getBlock()       noexcept { return _ptr; }
    [[nodiscard]] inline SWRC<_T>*  _getBlock() const noexcept { return _ptr; }

    [[nodiscard]] inline operator bool() const noexcept override { return _ptr; }

    [[nodiscard]] inline bool operator ==(const StrongRcPtr<_T>& ptr) const noexcept
    {
        if(_ptr && ptr._ptr)
        { return _ptr == ptr._ptr; }
        if(!_ptr && !ptr._ptr)
        { return true; }
        return false;
    }

    [[nodiscard]] inline bool operator !=(const StrongRcPtr<_T>& ptr) const noexcept
    {
        if(_ptr && ptr._ptr)
        { return _ptr != ptr._ptr; }
        if(!_ptr && !ptr._ptr)
        { return false; }
        return true;
    }

    [[nodiscard]] inline bool operator ==(const WeakRcPtr<_T>& ptr) const noexcept
    {
        if(_ptr && ptr._ptr)
        { return _ptr == ptr._ptr; }
        if(!_ptr && !ptr._ptr)
        { return true; }
        return false;
    }

    [[nodiscard]] inline bool operator !=(const WeakRcPtr<_T>& ptr) const noexcept
    {
        if(_ptr && ptr._ptr)
        { return _ptr != ptr._ptr; }
        if(!_ptr && !ptr._ptr)
        { return false; }
        return true;
    }

    [[nodiscard]] inline bool operator ==(const _T*& ptr) const noexcept override
    {
        if(_ptr && ptr)
        { return _ptr->objPtr() == ptr; }
        if(!_ptr && !ptr) 
        { return true; }
        return false;
    }

    [[nodiscard]] inline bool operator !=(const _T*& ptr) const noexcept override
    {
        if(_ptr && ptr)
        { return _ptr->objPtr() != ptr; }
        if(!_ptr && !ptr) 
        { return false; }
        return true;
    }

    [[nodiscard]] inline bool operator ==(const nullptr_t ptr) const noexcept override { return _ptr == ptr; }
    [[nodiscard]] inline bool operator !=(const nullptr_t ptr) const noexcept override { return _ptr != ptr; }
private:
    friend class StrongRcPtr<_T>;
};

struct Test
{
    int x;

    Test() noexcept
        : x(0)
    { printf("Creating Default: %d\n", x); }

    Test(const int _x) noexcept
        : x(_x)
    { printf("Creating: %d\n", x); }

    ~Test() noexcept
    { printf("Destroying: %d\n", x); }
};

RcPtr<Test> test() noexcept
{
    return RcPtr<Test>(14);
}

int main(int argCount, char* args[])
{
    test();

    StrongRcPtr<Test> strong0(42);
    WeakRcPtr<Test> weak = strong0;

    {
        StrongRcPtr<Test> strong1(32);
        weak = strong1;
    }

    return 0;
}
