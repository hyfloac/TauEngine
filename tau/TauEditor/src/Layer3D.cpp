#include "Layer3D.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <system/Window.hpp>
#include <model/RenderableObject.hpp>
#include <model/OBJLoader.hpp>
#include <maths/glmExt/GlmMatrixTransformExt.hpp>
#include <shader/IShader.hpp>
#include "texture/FITextureLoader.hpp"
#include "Timings.hpp"
#include "system/GraphicsInterface.hpp"
#include <EnumBitFields.hpp>

template<>
class UniformAccessor<Layer3D::Uniforms> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_COPY(UniformAccessor);
public:
    static constexpr uSys MATRIX_SIZE = sizeof(float) * 4 * 4;

    [[nodiscard]] static inline uSys size() noexcept { return MATRIX_SIZE * 3; }

    static inline void set(IRenderingContext& context, const CPPRef<IUniformBuffer>& buffer, const Layer3D::Uniforms& t) noexcept
    {
        buffer->beginModification(context);
    	// buffer->modifyBuffer(MATRIX_SIZE * 0, MATRIX_SIZE, glm::value_ptr(t.compoundMatrix));
        buffer->modifyBuffer(MATRIX_SIZE * 0, MATRIX_SIZE, glm::value_ptr(t.projectionMatrix));
        buffer->modifyBuffer(MATRIX_SIZE * 1, MATRIX_SIZE, glm::value_ptr(t.viewMatrix));
        buffer->modifyBuffer(MATRIX_SIZE * 2, MATRIX_SIZE, glm::value_ptr(t.modelMatrix));
        buffer->endModification(context);
    }
};

template<>
class UniformAccessor<Layer3D::ViewPosUniforms> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_COPY(UniformAccessor);
public:
    [[nodiscard]] static inline uSys size() noexcept { return sizeof(float) * 4; }

    static inline void set(IRenderingContext& context, const CPPRef<IUniformBuffer>& buffer, const Layer3D::ViewPosUniforms& t) noexcept
    {
        const __m128 vec = t.cameraPos.data().vec;
        buffer->fillBuffer(context, reinterpret_cast<const void*>(&vec));
    }
};

Layer3D::Layer3D(Globals& globals) noexcept
    : ILayer(true),
      _globals(globals),
      _camera(globals.window, 90.0f, 0.0001f, 1000.0f, 10.0f, 40.0f, 0.03f, false,
              Keyboard::Key::Shift, Keyboard::Key::W, Keyboard::Key::S,
              Keyboard::Key::A, Keyboard::Key::D, Keyboard::Key::Space, Keyboard::Key::Ctrl,
              &globals.gr),
      _skybox(globals.gi, globals.rc, "|TERes", "/shader/", "SkyboxVertexShader", "SkyboxPixelShader", "|TERes/Skybox/OceanMountain/", ".png"),
      _shader(IShaderProgram::create(globals.rc)),
      _uniforms(globals.gi.createUniformBuffer()),
      _materialUniforms(globals.gi.createUniformBuffer()),
      _pointLightUniforms(globals.gi.createUniformBuffer()),
      _spotLightUniforms(globals.gi.createUniformBuffer()), _cameraPosUni(globals.gi.createUniformBuffer()),
      _frameBufferShader(IShaderProgram::create(globals.rc)),
      _texture(TextureLoader::loadTexture(globals.gi, globals.rc, "|TERes/TestTexture.png")),
      _overlay(TextureLoader::loadTexture(globals.gi, globals.rc, "|TERes/Overlay.png")),
      _frameBufferVA(null),
      _modelPos(0, 0, 0), _modelViewMatrix(1.0f), _poses{}, _validPoses{}, _validPoseCount(0)
{
    PERF();

    ShaderArgs shaderArgs;
    shaderArgs.vfsMount = "|TERes";

    shaderArgs.path = "/nanosuit/";
    shaderArgs.fileName = "NanosuitPixel";
    shaderArgs.stage = EShader::Stage::Pixel;
    CPPRef<IShader> pixelShader = globals.gi.createShader().buildCPPRef(shaderArgs, null);

    shaderArgs.path = "/shader/";
    shaderArgs.fileName = "SimpleVertexShader";
    shaderArgs.stage = EShader::Stage::Vertex;
    CPPRef<IShader> vertexShader = globals.gi.createShader().buildCPPRef(shaderArgs, null);

    objl::Loader loader;
    if(loader.loadFile("|TERes/nanosuit/nanosuit.obj"))
    {
        for(const objl::Mesh& mesh : loader.meshes())
        {
            _objects.emplace_back(std::make_shared<RenderableObject>(globals.gi, globals.rc, mesh, "|TERes/nanosuit/", vertexShader));
        }
    }

    // shaderBuilder->type(EShader::Stage::Vertex);
    // shaderBuilder->file(VFS::Instance().openFile("|TERes/shader/OutlineVertexShader.glsl", FileProps::Read));
    // CPPRef<IShader> outlineVertexShader = CPPRef<IShader>(shaderBuilder->build());
    //
    // shaderBuilder->type(EShader::Stage::Pixel);
    // shaderBuilder->file(VFS::Instance().openFile("|TERes/shader/OutlinePixelShader.glsl", FileProps::Read));
    // CPPRef<IShader> outlinePixelShader = CPPRef<IShader>(shaderBuilder->build());
    //
    // shaderBuilder->type(EShader::Stage::Vertex);
    // shaderBuilder->file(VFS::Instance().openFile("|TERes/shader/CubeMap/RefVertexShader.glsl", FileProps::Read));
    // CPPRef<IShader> refVertexShader = CPPRef<IShader>(shaderBuilder->build());
    //
    // shaderBuilder->type(EShader::Stage::Pixel);
    // shaderBuilder->file(VFS::Instance().openFile("|TERes/shader/CubeMap/ReflectionPixelShader.glsl", FileProps::Read));
    // CPPRef<IShader> reflectionPixelShader = CPPRef<IShader>(shaderBuilder->build());
    //
    // shaderBuilder->type(EShader::Stage::Pixel);
    // shaderBuilder->file(VFS::Instance().openFile("|TERes/shader/CubeMap/RefractionPixelShader.glsl", FileProps::Read));
    // CPPRef<IShader> refractionPixelShader = CPPRef<IShader>(shaderBuilder->build());

    shaderArgs.fileName = "FrameBufferVertexShader";
    shaderArgs.stage = EShader::Stage::Vertex;
    CPPRef<IShader> frameBufferVertexShader = globals.gi.createShader().buildCPPRef(shaderArgs, null);

    shaderArgs.fileName = "FrameBufferPixelShader";
    shaderArgs.stage = EShader::Stage::Pixel;
    CPPRef<IShader> frameBufferPixelShader = globals.gi.createShader().buildCPPRef(shaderArgs, null);

    _shader->setVertexShader(globals.rc, vertexShader);
    _shader->setPixelShader(globals.rc, pixelShader);

    _frameBufferShader->setVertexShader(globals.rc, frameBufferVertexShader);
    _frameBufferShader->setPixelShader(globals.rc, frameBufferPixelShader);

    _shader->link(globals.rc);
    _frameBufferShader->link(globals.rc);

    Texture2DArgs tArgs;
    tArgs.width = globals.window.width();
    tArgs.height = globals.window.height();
    tArgs.mipmapLevels = 1;
    tArgs.dataFormat = ETexture::Format::RedGreenBlueAlpha8UnsignedInt;
    tArgs.flags = ETexture::BindFlags::RenderTarget | ETexture::BindFlags::ShaderAccess;
    tArgs.initialBuffer = null;

    const CPPRef<ITexture2D> colorTexture = globals.gi.createTexture().buildCPPRef(tArgs, null);

    TextureDepthStencilArgs tdsArgs;
    tdsArgs.width = globals.window.width();
    tdsArgs.height = globals.window.height();
    tdsArgs.flags = ETexture::DepthStencilBindFlags::RenderTarget;
    tdsArgs.initialBuffer = null;
    const CPPRef<ITextureDepthStencil> dsTexture = globals.gi.createTexture().buildCPPRef(tdsArgs, null);

    FrameBufferArgs fbArgs(1);
    fbArgs.colorAttachments[0] = colorTexture;
    fbArgs.depthStencilAttachment = dsTexture;

    _frameBuffer = globals.gi.createFrameBuffer().buildCPPRef(fbArgs, null);

    // CPPRef<IFrameBufferBuilder> builder = window.renderingContext()->createFrameBuffer();
    //
    // IFrameBufferAttachment* colorBuffer   = IFrameBufferAttachment::create(*window.renderingContext(), IFrameBufferAttachment::Color, window.width(), window.height());
    // IFrameBufferAttachment* depthStencilBuffer = IFrameBufferAttachment::create(*window.renderingContext(), IFrameBufferAttachment::DepthStencil, window.width(), window.height());
    //
    // builder->attach(colorBuffer, nullptr);
    // builder->attach(depthStencilBuffer, nullptr);
    //
    // _frameBuffer = CPPRef<IFrameBuffer>(builder->build(nullptr));

    float quadVertices[] = {
        -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };

    BufferArgs positionsBuilder(2);
    positionsBuilder.type = EBuffer::Type::ArrayBuffer;
    positionsBuilder.usage = EBuffer::UsageType::StaticDraw;
    positionsBuilder.elementCount = 6;
    positionsBuilder.initialBuffer = quadVertices;
    positionsBuilder.descriptor.addDescriptor(ShaderSemantic::Position, ShaderDataType::Vector2Float);
    positionsBuilder.descriptor.addDescriptor(ShaderSemantic::TextureCoord, ShaderDataType::Vector2Float);

    const CPPRef<IBuffer> positions = globals.gi.createBuffer().buildCPPRef(positionsBuilder, nullptr);

    VertexArrayArgs vaArgs(1);
    vaArgs.shader = frameBufferVertexShader;
    vaArgs.buffers[0] = positions;
    vaArgs.drawCount = 6;
    vaArgs.drawType = DrawType::SeparatedTriangles;
    _frameBufferVA = globals.gi.createVertexArray().buildCPPRef(vaArgs, null);

    TextureSamplerArgs tsArgs;
    tsArgs.magFilter() = ETexture::Filter::Linear;
    tsArgs.minFilter() = ETexture::Filter::Linear;
    tsArgs.mipFilter() = ETexture::Filter::Linear;
    tsArgs.wrapU = ETexture::WrapMode::Repeat;
    tsArgs.wrapV = ETexture::WrapMode::Repeat;
    tsArgs.wrapW = ETexture::WrapMode::Repeat;
    tsArgs.depthCompareFunc = ETexture::CompareFunc::Never;
    _frameBufferSampler = _globals.gi.createTextureSampler().buildCPPRef(tsArgs, null);

    _testTexture = TextureLoader::generateDebugTexture8(globals.gi, globals.rc, 7);

    SingleTextureUploaderArgs stuArgs;
    stuArgs.texture = colorTexture->textureView();
    stuArgs.textureSampler = _frameBufferSampler;
    _frameBufferUploader = _globals.gi.createSingleTextureUploader().buildCPPRef(stuArgs, null);

    // _modelViewMatrix = glmExt::translate(_modelViewMatrix, _modelPos);
    _modelViewMatrix = glm::translate(_modelViewMatrix, glm::vec3(0.0f, 0.0f, -1.0f));
    _modelViewMatrix = glmExt::rotateDegrees(_modelViewMatrix, 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    _uniforms.data().modelMatrix = _modelViewMatrix;

    _pointLight.position() = Vector3f(0.0f, -10.0f, -5.0f);
    _pointLight.ambient({255, 255, 255});
    _pointLight.diffuse({255, 255, 255});
    _pointLight.specular({255, 255, 255});
    _pointLight.constant() = 1.0f;
    _pointLight.linear() = 0.007f;
    _pointLight.quadratic() = 0.0002f;

    _spotLight.position() = Vector3f(0.0f);
    _spotLight.direction() = Vector3f(0.0f);
    _spotLight.ambient({255, 255, 255});
    _spotLight.diffuse({255, 255, 255});
    _spotLight.specular({255, 255, 255});
    _spotLight.constant() = 1.0f;
    _spotLight.linear() = 0.027f;
    _spotLight.quadratic() = 0.0028f;
    _spotLight.cutOff() = fastCosD(30.0f);
    _spotLight.outerCutOff() = fastCosD(35.0f);

    _pointLightUniforms.set(globals.rc, _pointLight);
    _spotLightUniforms.set(globals.rc, _spotLight);

    // CPPRef<ITextureUploaderBuilder> uploaderBuilder = window.renderingContext()->createTextureUploader(1);
    // uploaderBuilder->setTexture(0, colorBuffer->texture());
    // uploaderBuilder->textureSampler(CPPRef<ITextureSampler>(window.renderingContext()->createTextureSampler().buildCPPRef(textureSamplerArgs, null)));
    // _frameBufferUploader = CPPRef<ITextureUploader>(uploaderBuilder->build());
}

void Layer3D::onUpdate(float fixedDelta) noexcept
{
    PERF();

    if(_globals.gameState == State::Game && !_globals.vr)
    {
        const u32 screenCenterW = _globals.window.width() >> 1;
        const u32 screenCenterH = _globals.window.height() >> 1;
        const Mouse::Pos pos = Mouse::mousePos(_globals.window);
        Mouse::mousePos(_globals.window, screenCenterW, screenCenterH);
        const i32 mouseDifX = static_cast<i32>(screenCenterW) - static_cast<i32>(pos.x);
        const i32 mouseDifY = static_cast<i32>(screenCenterH) - static_cast<i32>(pos.y);
        _camera.update(fixedDelta, mouseDifX, mouseDifY);
        _uniforms.data().projectionMatrix = _camera->projectionMatrix();
    }
}

void Layer3D::onPreRender(const DeltaTime& delta) noexcept
{
    PERF();

    if(_globals.gameState == State::Game && !_globals.vr)
    {
        _camera.lerp(delta);
    }

    if(_globals.vr && _globals.vrCamera)
    {
        vr::TrackedDevicePose_t poses[vr::k_unMaxTrackedDeviceCount];
        vr::VRCompositor()->WaitGetPoses(poses, vr::k_unMaxTrackedDeviceCount, NULL, 0);


        _globals.vrCamera->update(delta, poses[vr::k_unTrackedDeviceIndex_Hmd]);
    }
}

void Layer3D::onRender() noexcept
{
    PERF();

    auto& context = _globals.rc;

    if(_globals.vr)
    {
        _shader->bind(context);
        _spotLight.position() = _globals.vrCamera->camera().position();
        _spotLight.direction() = _globals.vrCamera->camera().front();
        if(_globals.currentLeftEye)
        {
            _uniforms.data().projectionMatrix = _globals.vrCamera->leftVP();
        }
        else
        {
            _uniforms.data().projectionMatrix = _globals.vrCamera->rightVP();
        }
        _uniforms.data().projectionMatrix = glmExt::translate(glm::mat4(1.0f), _globals.vrCamera->camera().position());

        _cameraPosUni.data().cameraPos = _globals.vrCamera->camera().position();
        _spotLightUniforms.set(context, _spotLight);
        _uniforms.upload(context, EShader::Stage::Vertex, 0);
        _pointLightUniforms.upload(context, EShader::Stage::Pixel, 2);
        _spotLightUniforms.upload(context, EShader::Stage::Pixel, 3);
        _cameraPosUni.upload(context, EShader::Stage::Pixel, 4);
        for(const CPPRef<RenderableObject>& ro : _objects)
        {
            TextureIndices indices(0, 0, 0);
            ro->material().upload(context, _materialUniforms, EShader::Stage::Pixel, 1, indices);
            ro->preRender(context);
            ro->render(context);
            ro->postRender(context);
            indices = TextureIndices(0, 0, 0);
            ro->material().unbind(context, _materialUniforms, EShader::Stage::Pixel, 1, indices);
        }
        _cameraPosUni.unbind(context, EShader::Stage::Pixel, 4);
        _spotLightUniforms.unbind(context, EShader::Stage::Pixel, 3);
        _pointLightUniforms.unbind(context, EShader::Stage::Pixel, 2);
        _uniforms.unbind(context, EShader::Stage::Vertex, 0);
        _shader->unbind(context);
        _skybox.render(context, _globals.vrCamera->camera());
    }
    else
    {
        _frameBuffer->bind(_globals.rc);

        _frameBuffer->clearFrameBuffer(_globals.rc, true, true, false, { 255, 255, 127, 255 });

        _shader->bind(context);
        _spotLight.position() = _camera.camera().position();
        _spotLight.direction() = _camera.camera().front();
        // _uniforms.data().compoundMatrix = _camera->compoundedMatrix();
        _uniforms.data().viewMatrix = _camera->viewMatrix();

        _cameraPosUni.data().cameraPos = _camera.camera().position();
        _spotLightUniforms.set(context, _spotLight);
        _uniforms.upload(context, EShader::Stage::Vertex, 0);
        _pointLightUniforms.upload(context, EShader::Stage::Pixel, 2);
        _spotLightUniforms.upload(context, EShader::Stage::Pixel, 3);
        _cameraPosUni.upload(context, EShader::Stage::Pixel, 4);
        for(const CPPRef<RenderableObject>& ro : _objects)
        {
            TextureIndices indices(0, 0, 0);
            ro->material().upload(context, _materialUniforms, EShader::Stage::Pixel, 1, indices);
            ro->preRender(context);
            ro->render(context);
            ro->postRender(context);
            indices = TextureIndices(0, 0, 0);
            ro->material().unbind(context, _materialUniforms, EShader::Stage::Pixel, 1, indices);
        }
        _cameraPosUni.unbind(context, EShader::Stage::Pixel, 4);
        _spotLightUniforms.unbind(context, EShader::Stage::Pixel, 3);
        _pointLightUniforms.unbind(context, EShader::Stage::Pixel, 2);
        _uniforms.unbind(context, EShader::Stage::Vertex, 0);
        _shader->unbind(context);
        _skybox.render(context, _camera.camera());

        _frameBuffer->unbind(_globals.rc);

        _frameBufferShader->bind(_globals.rc);
        _frameBufferUploader->upload(_globals.rc, TextureIndices(0, 0, 0), EShader::Stage::Pixel);
        _frameBufferVA->bind(_globals.rc);
        _frameBufferVA->preDraw(_globals.rc);
        _frameBufferVA->draw(_globals.rc);
        _frameBufferVA->postDraw(_globals.rc);
        _frameBufferVA->unbind(_globals.rc);
        _frameBufferUploader->unbind(_globals.rc, TextureIndices(0, 0, 0), EShader::Stage::Pixel);
        _frameBufferShader->unbind(_globals.rc);
    }
}

void Layer3D::onEvent(Event& e) noexcept
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowResizeEvent>(this, &Layer3D::onWindowResize);
}

bool Layer3D::onWindowResize(WindowResizeEvent& e) noexcept
{
    UNUSED(e);

    Texture2DArgs tArgs;
    tArgs.width = e.newWidth();
    tArgs.height = e.newHeight();
    tArgs.mipmapLevels = 1;
    tArgs.dataFormat = ETexture::Format::RedGreenBlueAlpha8UnsignedInt;
    tArgs.flags = ETexture::BindFlags::RenderTarget | ETexture::BindFlags::ShaderAccess;
    tArgs.initialBuffer = null;

    const CPPRef<ITexture2D> colorTexture = _globals.gi.createTexture().buildCPPRef(tArgs, null);

    TextureDepthStencilArgs tdsArgs;
    tdsArgs.width = e.newWidth();
    tdsArgs.height = e.newHeight();
    tdsArgs.flags = ETexture::DepthStencilBindFlags::RenderTarget;
    tdsArgs.initialBuffer = null;
    const CPPRef<ITextureDepthStencil> dsTexture = _globals.gi.createTexture().buildCPPRef(tdsArgs, null);

    FrameBufferArgs fbArgs(1);
    fbArgs.colorAttachments[0] = colorTexture;
    fbArgs.depthStencilAttachment = dsTexture;

    _frameBuffer = _globals.gi.createFrameBuffer().buildCPPRef(fbArgs, null);

    _frameBufferUploader->texture(colorTexture->textureView());

    // CPPRef<IFrameBufferBuilder> builder = _window.renderingContext()->createFrameBuffer();
    //
    // IFrameBufferAttachment* colorBuffer = IFrameBufferAttachment::create(*_window.renderingContext(), IFrameBufferAttachment::Color, e.newWidth(), e.newHeight());
    // IFrameBufferAttachment* depthStencilBuffer = IFrameBufferAttachment::create(*_window.renderingContext(), IFrameBufferAttachment::DepthStencil, e.newWidth(), e.newHeight());
    //
    // builder->attach(colorBuffer);
    // builder->attach(depthStencilBuffer);
    //
    // _frameBuffer = CPPRef<IFrameBuffer>(builder->build());

    return true;
}
