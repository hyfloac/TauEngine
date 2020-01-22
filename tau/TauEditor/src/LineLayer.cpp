#include "LineLayer.hpp"
#include <shader/IShader.hpp>
#include <model/Buffer.hpp>
#include <model/VertexArray.hpp>
#include "VFS.hpp"

LineLayer::LineLayer(Window& window, RenderingPipeline& rp, const glm::mat4& ortho) noexcept
    : ILayer(true), _window(window), _rp(rp), _ortho(ortho),
      _vao(null), _shader(IShaderProgram::create(*window.renderingContext()))
{
    Ref<IShaderBuilder> shaderBuilder = window.renderingContext()->createShader();

    shaderBuilder->type(IShader::Type::Vertex);
    shaderBuilder->file(VFS::Instance().openFile("|TERes/line/LineVertex.glsl", FileProps::Read));
    Ref<IShader> vertexShader = Ref<IShader>(shaderBuilder->build());

    shaderBuilder->type(IShader::Type::Geometry);
    shaderBuilder->file(VFS::Instance().openFile("|TERes/line/LineGeometry.glsl", FileProps::Read));
    Ref<IShader> geometryShader = Ref<IShader>(shaderBuilder->build());

    shaderBuilder->type(IShader::Type::Pixel);
    shaderBuilder->file(VFS::Instance().openFile("|TERes/line/LinePixel.glsl", FileProps::Read));
    Ref<IShader> pixelShader = Ref<IShader>(shaderBuilder->build());

    // Ref<IShader> vertexShader = IShader::create(*window.renderingContext(), IShader::Type::Vertex, "|TERes/line/LineVertex.glsl");
    // Ref<IShader> geometryShader = IShader::create(*window.renderingContext(), IShader::Type::Geometry, "|TERes/line/LineGeometry.glsl");
    // Ref<IShader> pixelShader = IShader::create(*window.renderingContext(), IShader::Type::Pixel, "|TERes/line/LinePixel.glsl");
    //
    // vertexShader->loadShader();
    // geometryShader->loadShader();
    // pixelShader->loadShader();

    _shader->setVertexShader(*window.renderingContext(), vertexShader);
    _shader->setGeometryShader(*window.renderingContext(), geometryShader);
    _shader->setPixelShader(*window.renderingContext(), pixelShader);

    _shader->link(*window.renderingContext());

    _viewMatrixUni = _shader->getUniformMatrix4x4Float("viewMatrix", false);
    _lineThicknessUni = _shader->getUniformFloat("lineThickness");
    _viewportUni = _shader->getUniformVector4Float("viewport");
    _miterLimitUni = _shader->getUniformFloat("miterLimit");

    float bufferData[] = {
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
    };

    Ref<IBufferBuilder> lineDataBuilder = window.renderingContext()->createBuffer(2);
    lineDataBuilder->type(EBuffer::Type::ArrayBuffer);
    lineDataBuilder->usage(EBuffer::UsageType::StaticDraw);
    lineDataBuilder->bufferSize(sizeof(bufferData));
    lineDataBuilder->descriptor().addDescriptor(ShaderDataType::Vector2Float);
    lineDataBuilder->descriptor().addDescriptor(ShaderDataType::Vector4Float);

    Ref<IBuffer> lineData = Ref<IBuffer>(lineDataBuilder->build(nullptr));

    lineData->bind(*window.renderingContext());
    lineData->fillBuffer(*window.renderingContext(), bufferData);
    lineData->unbind(*window.renderingContext());

    Ref<IVertexArrayBuilder> vaBuilder = window.renderingContext()->createVertexArray(1);
    vaBuilder->setVertexBuffer(0, lineData);
    vaBuilder->drawCount(2);
    vaBuilder->drawType(DrawType::SeparatedTriangles);

    _vao = Ref<IVertexArray>(vaBuilder->build());

    // _vao->addVertexBuffer(*window.renderingContext(), lineData);
    // _vao->drawCount() = 2;
}

void LineLayer::onUpdate(float fixedDelta) noexcept
{
    UNUSED(fixedDelta);
}

void LineLayer::onRender(const DeltaTime& delta) noexcept
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
