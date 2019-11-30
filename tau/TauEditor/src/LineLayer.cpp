#include "LineLayer.hpp"
#include <shader/IShader.hpp>
#include <model/IBuffer.hpp>
#include <model/IVertexArray.hpp>

LineLayer::LineLayer(Window& window, RenderingPipeline& rp, const glm::mat4& ortho) noexcept
    : ILayer(true), _window(window), _rp(rp), _ortho(ortho),
      _vao(window.renderingContext()->createVertexArray(2, DrawType::SeparatedTriangles)), _shader(IShaderProgram::create(*window.renderingContext()))
{
    Ref<IShader> vertexShader = IShader::create(*window.renderingContext(), IShader::Type::Vertex, "|TERes/line/LineVertex.glsl");
    Ref<IShader> geometryShader = IShader::create(*window.renderingContext(), IShader::Type::Geometry, "|TERes/line/LineGeometry.glsl");
    Ref<IShader> pixelShader = IShader::create(*window.renderingContext(), IShader::Type::Pixel, "|TERes/line/LinePixel.glsl");

    vertexShader->loadShader();
    geometryShader->loadShader();
    pixelShader->loadShader();

    _shader->setVertexShader(*window.renderingContext(), vertexShader);
    _shader->setGeometryShader(*window.renderingContext(), geometryShader);
    _shader->setPixelShader(*window.renderingContext(), pixelShader);

    _shader->link(*window.renderingContext());

    _viewMatrixUni = _shader->getUniformMatrix4x4Float("viewMatrix", false);
    _lineThicknessUni = _shader->getUniformFloat("lineThickness");
    _viewportUni = _shader->getUniformVector4Float("viewport");
    _miterLimitUni = _shader->getUniformFloat("miterLimit");

    Ref<IBuffer> lineData = window.renderingContext()->createBuffer(2, IBuffer::Type::ArrayBuffer);
    float bufferData[] = {
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
    };
    lineData->fillBuffer(*window.renderingContext(), sizeof(float) * 2 * 6, bufferData);
    lineData->descriptor().addDescriptor(ShaderDataType::Vector2Float);
    lineData->descriptor().addDescriptor(ShaderDataType::Vector4Float);

    _vao->addVertexBuffer(*window.renderingContext(), lineData);
    _vao->drawCount() = 2;
}

void LineLayer::onUpdate(float fixedDelta) noexcept
{
    UNUSED(fixedDelta);
}

void LineLayer::onRender(float delta) noexcept
{
    UNUSED(delta);
    TAU_RENDER_S(_rp, {
        self->_shader->bind(context);
        self->_viewMatrixUni->set(self->_ortho);
        self->_lineThicknessUni->set(1.0f);
        self->_viewportUni->set(glm::vec4(0.0f, 0.0f, window.width(), window.height()));
        self->_miterLimitUni->set(1.0f);

        self->_vao->bind(context);
        self->_vao->preDraw(context);
        self->_vao->draw(context);
        self->_vao->postDraw(context);
        self->_vao->unbind(context);
    });
}

void LineLayer::onEvent(Event& e) noexcept
{
    UNUSED(e);
}
