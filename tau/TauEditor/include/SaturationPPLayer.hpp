#pragma once

#include <layer/PostProcessLayer.hpp>
#include <shader/Uniform.hpp>
#include <system/GraphicsInterface.hpp>
#include "Globals.hpp"

class ControlEvent;
class IShaderProgram;
class IVertexArray;
class IFrameBuffer;
class ITextureSampler;
class ITextureUploader;
class WindowResizeEvent;

class SaturationPPLayer final : public IPostProcessLayer
{
    DEFAULT_DESTRUCT(SaturationPPLayer);
    PP_LAYER_IMPL(SaturationPPLayer);
private:
    Globals& _globals;
    float _saturation;

    CPPRef<IShaderProgram> _shader;
    NullableRef<IVertexArray> _va;
    NullableRef<IFrameBuffer> _frameBuffer;
    CPPRef<ITextureSampler> _textureSampler;
    NullableRef<ITextureUploader> _fbUploader;

    UniformBlockU<float> _saturationUniform;
public:
    SaturationPPLayer(Globals& globals, float saturation) noexcept;

    [[nodiscard]] float saturation() const noexcept { return _saturation; }
    void saturation(const float saturation) noexcept
    {
        _saturation = saturation;
        _saturationUniform.set(_globals.rc, _saturation);
    }

    bool init() noexcept;

    void onEvent(Event& e) noexcept override;

    void onFrameBufferBind() noexcept override;
    void onFrameBufferUnbind() noexcept override;

    void onRender() noexcept override;
private:
    bool onWindowResize(WindowResizeEvent& e) noexcept;
    bool onControlEvent(ControlEvent& e) noexcept;
};
