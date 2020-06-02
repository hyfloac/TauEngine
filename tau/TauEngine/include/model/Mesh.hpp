#pragma once

#include "DLL.hpp"
#include <Objects.hpp>
#include <NumTypes.hpp>

class IRenderingContext;

class TAU_DLL TAU_NOVTABLE IMesh
{
    DEFAULT_CONSTRUCT_PO(IMesh);
    DEFAULT_DESTRUCT_VI(IMesh);
    DELETE_COPY(IMesh);
public:
    virtual void bind(IRenderingContext& context) noexcept = 0;
    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual void draw(IRenderingContext& context) noexcept = 0;
    virtual void drawInstanced(IRenderingContext& context, uSys instanceCount) noexcept = 0;
};
