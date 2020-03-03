#pragma once

#include <layer/ILayer.hpp>
#include <system/Window.hpp>
#include <RenderingPipeline.hpp>
#include <shader/IShaderProgram.hpp>
#include <shader/Uniform.hpp>

class LineLayer final : public ILayer
{
    DEFAULT_DESTRUCT(LineLayer);
    LAYER_IMPL(LineLayer);
private:
    Window& _window;
    RenderingPipeline& _rp;
    const glm::mat4& _ortho;

    CPPRef<IVertexArray> _vao;

    CPPRef<IShaderProgram> _shader;
    // CPPRef<IUniform<const glm::mat4&>> _viewMatrixUni;
    // CPPRef<IUniform<float>> _lineThicknessUni;
    // CPPRef<IUniform<const glm::vec4&>> _viewportUni;
    // CPPRef<IUniform<float>> _miterLimitUni;
public:
    LineLayer(Window& window, RenderingPipeline& rp, const glm::mat4& ortho) noexcept;

    void onUpdate(float fixedDelta) noexcept override;

    void onRender(const DeltaTime& delta) noexcept override;

    void onEvent(Event& e) noexcept override;
};
