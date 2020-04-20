#pragma once

#include "DLL.hpp"
#include "Layer.hpp"

class TAU_DLL ImGuiLayer final : public ILayer
{
private:
public:
    ~ImGuiLayer() noexcept override = default;

    LAYER_IMPL(ImGuiLayer)
};
