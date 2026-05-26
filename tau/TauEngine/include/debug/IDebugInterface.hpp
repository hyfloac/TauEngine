#pragma once

#include <DynArray.hpp>
#include <Objects.hpp>
#include <TauCOM.hpp>
#include <functional>
#include <EnumBitFields.hpp>
#include <Safeties.hpp>
#include <String.hpp>
#include "DLL.hpp"

#ifdef SendMessage
#define SendMessage_Tau_Store SendMessage
#undef SendMessage
#endif

namespace tau {

enum class DebugMessageSeverity : u32
{
    Verbose = 0x00000001,
    Info = 0x00000002,
    Warning = 0x00000004,
    Error = 0x00000008
};

enum class DebugMessageType : u32
{
    General = 0x00000001,
    Validation = 0x00000002,
    Performance = 0x00000004
};

}

ENUM_FLAGS(tau::DebugMessageSeverity);
ENUM_FLAGS(tau::DebugMessageType);

namespace tau {

struct DebugMessageChain
{
    DEFAULT_CONSTRUCT_PU(DebugMessageChain);
    DEFAULT_CM_PU(DebugMessageChain);
    DEFAULT_DESTRUCT_VI(DebugMessageChain);
public:
    com::UUID Iid;
    const DebugMessageChain* pNext;
};

struct DebugMessageObjectName final : DebugMessageChain
{
    DEFAULT_CONSTRUCT_PU(DebugMessageObjectName);
    DEFAULT_CM_PU(DebugMessageObjectName);
    DEFAULT_DESTRUCT_VI(DebugMessageObjectName);
public:
    void* ObjectHandle;
    C8DynString ObjectName;
};

struct BaseDebugMessage final : DebugMessageChain
{
    DEFAULT_CONSTRUCT_PU(BaseDebugMessage);
    DEFAULT_CM_PU(BaseDebugMessage);
    DEFAULT_DESTRUCT_VI(BaseDebugMessage);
public:
    u32 Flags;
    C8DynString MessageIdName;
    u32 MessageId;
    C8DynString Message;
    RefDynArray<DebugMessageObjectName> Objects;
};

class TAU_DLL TAU_NOVTABLE IDebugInterface : public com::IUnknown
{
    DEFAULT_CONSTRUCT_PO(IDebugInterface);
    DEFAULT_CM_PO(IDebugInterface);
    DEFAULT_DESTRUCT_VI(IDebugInterface);
public:
    using DebugCallback_f = ::std::function<void(DebugMessageSeverity severity, DebugMessageType types, const BaseDebugMessage* pMessage, void* pUserData)>;

    struct ConstructionInfo final : com::BaseConstructionInfo
    {
        DEFAULT_CONSTRUCT_PU(ConstructionInfo);
        DEFAULT_CM_PU(ConstructionInfo);
        DEFAULT_DESTRUCT_VI(ConstructionInfo);
    public:
        DebugMessageSeverity SeverityFilter;
        DebugMessageType TypesFilter;
        DebugCallback_f Callback;
        void* pUserData;
    };

    virtual void SendMessage(const DebugMessageSeverity severity, const DebugMessageType types, const BaseDebugMessage* pMessage) const noexcept = 0;

    [[nodiscard]] virtual DebugMessageSeverity GetSeverityFilter() const noexcept = 0;
    virtual void SetSeverityFilter(const DebugMessageSeverity filter) noexcept = 0;

    [[nodiscard]] virtual DebugMessageType GetTypesFilter() const noexcept = 0;
    virtual void SetTypesFilter(const DebugMessageType filter) noexcept = 0;

    [[nodiscard]] virtual DebugCallback_f GetCallback() const noexcept = 0;
    virtual void SetCallback(const DebugCallback_f callback) noexcept = 0;

    [[nodiscard]] virtual void* GetUserData() const noexcept = 0;
    virtual void SetUserData(void* pUserData) noexcept = 0;

    template<typename T>
    [[nodiscard]] T* GetUserData() const noexcept
    {
        return static_cast<T*>(GetUserData());
    }
};


namespace internal {

void RegisterDebugInterface(com::IComManager* comManager) noexcept;

}

}

TAU_DECL_UUID(IDebugInterface, 0x982FB68913C8423Dull, 0x8F7DF3EB20833223ull);

#ifdef SendMessage_Tau_Store
#define SendMessage SendMessage_Tau_Store
#undef SendMessage_Tau_Store
#endif
