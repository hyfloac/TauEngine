#pragma once

#include <NumTypes.hpp>
#include <Objects.hpp>
#include <DynArray.hpp>
#include <RunTimeType.hpp>

#include "DLL.hpp"

#define GRAPHICS_DISPLAY_IMPL(_TYPE) RTT_IMPL(_TYPE, IGraphicsDisplay)

class TAU_DLL TAU_NOVTABLE IGraphicsDisplay
{
    DEFAULT_CONSTRUCT_PO(IGraphicsDisplay);
    DEFAULT_DESTRUCT_VI(IGraphicsDisplay);
    DEFAULT_CM_PO(IGraphicsDisplay);
public:
    struct GraphicsDisplayMode final
    {
        uSys width;
        uSys height;
        uSys refreshRateNumerator;
        uSys refreshRateDenominator;
    };
public:
    [[nodiscard]] virtual const RefDynArray<GraphicsDisplayMode>& displayModes() const noexcept = 0;

    RTT_BASE_CAST(IGraphicsDisplay);
    RTT_BASE_CHECK(IGraphicsDisplay);
    RTT_BASE_IMPL(IGraphicsDisplay);
};
