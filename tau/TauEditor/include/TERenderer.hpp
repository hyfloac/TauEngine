#pragma once

#include <TextHandler.hpp>

class Window;

class TERenderer final
{
private:
    TextHandler _th;
    RenderingPipeline _rp;
    Matrix4x4f _ortho;
public:
    TERenderer(Window& window) noexcept;

    TextHandler& textHandler() noexcept { return _th; }

    RenderingPipeline& renderingPipeline() noexcept { return _rp; }

    const Matrix4x4f& ortho() const noexcept { return _ortho; }
    Matrix4x4f& ortho() noexcept { return _ortho; }
};
