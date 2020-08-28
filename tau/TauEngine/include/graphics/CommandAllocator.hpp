#pragma once

#include <Objects.hpp>
#include <RunTimeType.hpp>

#include "DLL.hpp"

#define COMMAND_ALLOCATOR_IMPL_BASE(_TYPE) \
    RTT_IMPL(_TYPE, ICommandAllocator)

#define COMMAND_ALLOCATOR_IMPL(_TYPE) COMMAND_ALLOCATOR_IMPL_BASE(_TYPE)

#ifndef TAU_CA_EXPOSE_MEM_STAT
  #if !defined(TAU_PRODUCTION)
    #define TAU_CA_EXPOSE_MEM_STAT 1
  #else
    #define TAU_CA_EXPOSE_MEM_STAT 0
  #endif
#endif

class TAU_DLL TAU_NOVTABLE ICommandAllocator
{
    DEFAULT_CONSTRUCT_PO(ICommandAllocator);
    DEFAULT_DESTRUCT_VI(ICommandAllocator);
    DEFAULT_CM_PO(ICommandAllocator);
public:
    virtual void reset(bool releasePages = false) noexcept = 0;

#if TAU_CA_EXPOSE_MEM_STAT
    /**
     * Gets the amount of memory reserved in bytes.
     *
     *   This may return a negative number if they API is unable to
     * determine exactly how much memory is allocated. This would
     * be indicative of an API which has builtin support for
     * command lists and allocators (such as DirectX12 or Vulkan).
     *
     * @return
     *      The amount of reserved memory in bytes.
     */
    [[nodiscard]] virtual iSys reservedMemory() const noexcept = 0;

    /**
     * Gets the amount of memory committed in bytes.
     *
     *   This may return a negative number if they API is unable to
     * determine exactly how much memory is allocated. This would
     * be indicative of an API which has builtin support for
     * command lists and allocators (such as DirectX12 or Vulkan).
     *
     * @return
     *      The amount of committed memory in bytes.
     */
    [[nodiscard]] virtual iSys committedMemory() const noexcept = 0;

    /**
     * Gets the amount of memory allocated in bytes.
     *
     *   This is the amount of memory that has actually been
     * allocated and used by a command list.
     *
     *   This may return a negative number if they API is unable to
     * determine exactly how much memory is allocated. This would
     * be indicative of an API which has builtin support for
     * command lists and allocators (such as DirectX12 or Vulkan).
     *
     * @return
     *      The amount of allocated memory in bytes.
     */
    [[nodiscard]] virtual iSys allocatedMemory() const noexcept = 0;
#endif
    
    RTT_BASE_IMPL(ICommandAllocator);
    RTT_BASE_CHECK(ICommandAllocator);
    RTT_BASE_CAST(ICommandAllocator);
};
