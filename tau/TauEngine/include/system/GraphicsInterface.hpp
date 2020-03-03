#pragma once

#include <Objects.hpp>
#include "DLL.hpp"
#include "Utils.hpp"
#include "GraphicsAccelerator.hpp"
#include "DynArray.hpp"

class TAU_DLL NOVTABLE IGraphicsInterface
{
    DEFAULT_CONSTRUCT_PO(IGraphicsInterface);
    DEFAULT_DESTRUCT_VI(IGraphicsInterface);
    DELETE_COPY(IGraphicsInterface);
public:
    [[nodiscard]] virtual RefDynArray<CPPRef<IGraphicsAccelerator>> graphicsAccelerators() noexcept = 0;
};
