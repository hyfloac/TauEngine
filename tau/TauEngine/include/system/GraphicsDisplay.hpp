#pragma once

#include "DLL.hpp"
#include <NumTypes.hpp>
#include <Objects.hpp>
#include <DynArray.hpp>

class TAU_DLL TAU_NOVTABLE IGraphicsDisplay
{
    DEFAULT_CONSTRUCT_PO(IGraphicsDisplay);
    DEFAULT_DESTRUCT_VI(IGraphicsDisplay);
    DELETE_COPY(IGraphicsDisplay);
public:
    struct GraphicsDisplayMode final
    {
        u32 width;
        u32 height;
        u32 refreshRateNumerator;
        u32 refreshRateDenominator;
    };
public:
    [[nodiscard]] virtual RefDynArray<GraphicsDisplayMode> displayModes() noexcept = 0;
};
