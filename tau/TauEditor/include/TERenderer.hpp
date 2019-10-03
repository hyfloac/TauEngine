#pragma once

#include <TextHandler.hpp>
#include "PhysWord.hpp"
#include <vector>

class Window;
class Vector2f;

class TERenderer final
{
private:
    static constexpr float textScaleConverter = 2.8571428571428571428571428571429f;

    TextHandler _th;
    RenderingPipeline _rp;
    Matrix4x4f _ortho;
    Window& _window;

    std::vector<PhysWord> _physWords;
public:
    TERenderer(Window& window) noexcept;

    [[nodiscard]] TextHandler& textHandler() noexcept { return _th; }

    [[nodiscard]] RenderingPipeline& renderingPipeline() noexcept { return _rp; }

    [[nodiscard]] const Matrix4x4f& ortho() const noexcept { return _ortho; }
    [[nodiscard]] Matrix4x4f& ortho() noexcept { return _ortho; }

    void render(const float delta) noexcept;
    void update(const float fixedDelta) noexcept;
};
