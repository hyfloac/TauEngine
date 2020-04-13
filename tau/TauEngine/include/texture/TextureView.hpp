#pragma once

#include <Objects.hpp>
#include <RunTimeType.hpp>

#include "DLL.hpp"

class IRenderingContext;

#define TEXTURE_VIEW_IMPL_BASE(_TYPE) \
    DELETE_COPY(_TYPE); \
    RTT_IMPL(_TYPE, ITextureView)

#define TEXTURE_VIEW_IMPL(_TYPE) TEXTURE_VIEW_IMPL_BASE(_TYPE)

class TAU_DLL TAU_NOVTABLE ITextureView
{
    DEFAULT_CONSTRUCT_PO(ITextureView);
    DEFAULT_DESTRUCT_VI(ITextureView);
    DELETE_COPY(ITextureView);
public:
    virtual void generateMipmaps(IRenderingContext& context) noexcept = 0;

    RTT_BASE_IMPL(ITextureView);
    RTT_BASE_CHECK(ITextureView);
    RTT_BASE_CAST(ITextureView);
};
