#pragma once

#include <Objects.hpp>
#include <RunTimeType.hpp>

#include "DLL.hpp"

#define RENDER_TARGET_IMPL_BASE(_TYPE) \
    RTT_IMPL(_TYPE, IRenderTarget)

#define RENDER_TARGET_IMPL(_TYPE) RENDER_TARGET_IMPL_BASE(_TYPE)

class TAU_DLL TAU_NOVTABLE IRenderTarget
{
    DEFAULT_CONSTRUCT_PO(IRenderTarget);
    DEFAULT_DESTRUCT_VI(IRenderTarget);
    DEFAULT_CM_PO(IRenderTarget);
public:
    RTT_BASE_IMPL(IRenderTarget);
    RTT_BASE_CHECK(IRenderTarget);
    RTT_BASE_CAST(IRenderTarget);
};
