#pragma once

#include <layer/ILayer.hpp>
#include <system/Window.hpp>
#include <RenderingPipeline.hpp>
#include <shader/IShaderProgram.hpp>
#include <shader/IUniform.hpp>

class LineLayer final : public ILayer
{
    DEFAULT_DESTRUCT(LineLayer);
    LAYER_IMPL(LineLayer);
private:
    Window& _window;
    RenderingPipeline& _rp;
    const glm::mat4& _ortho;

    Ref<IVertexArray> _vao;

    Ref<IShaderProgram> _shader;
    Ref<IUniform<const glm::mat4&>> _viewMatrixUni;
    Ref<IUniform<float>> _lineThicknessUni;
    Ref<IUniform<const glm::vec4&>> _viewportUni;
    Ref<IUniform<float>> _miterLimitUni;
public:
    LineLayer(Window& window, RenderingPipeline& rp, const glm::mat4& ortho) noexcept;

    void onUpdate(float fixedDelta) noexcept override;

    void onRender(float delta) noexcept override;

    void onEvent(Event& e) noexcept override;
};
