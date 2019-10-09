#pragma once

#include <layer/ILayer.hpp>
#include <DLL.hpp>

class TAU_DLL ImGuiLayer final : public ILayer
{
private:
public:
    ~ImGuiLayer() noexcept override = default;

    LAYER_IMPL(ImGuiLayer)
};
