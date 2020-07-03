#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>

#include "DLL.hpp"

class ICommandList;

class TAU_DLL TAU_NOVTABLE ICommandQueue
{
    DEFAULT_CONSTRUCT_PO(ICommandQueue);
    DEFAULT_DESTRUCT_VI(ICommandQueue);
    DEFAULT_CM_PO(ICommandQueue);
public:
    virtual void executeCommandLists(uSys count, const ICommandList** lists) noexcept = 0;
};

struct CommandQueueArgs final
{
    
};

class TAU_DLL TAU_NOVTABLE ICommandQueueBuilder
{
    DEFAULT_CONSTRUCT_PO(ICommandQueueBuilder);
    DEFAULT_DESTRUCT_VI(ICommandQueueBuilder);
    DEFAULT_CM_PO(ICommandQueueBuilder);
public:
    enum class Error
    {
        NoError = 0,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        InternalError
    };
public:
    [[nodiscard]] virtual ICommandQueue* build(const CommandQueueArgs& args, [[tau::out]] Error* error) noexcept = 0;
    [[nodiscard]] virtual ICommandQueue* build(const CommandQueueArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept = 0;
    [[nodiscard]] virtual CPPRef<ICommandQueue> buildCPPRef(const CommandQueueArgs& args, [[tau::out]] Error* error) noexcept = 0;
    [[nodiscard]] virtual NullableRef<ICommandQueue> buildTauRef(const CommandQueueArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ICommandQueue> buildTauSRef(const CommandQueueArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept = 0;
};
