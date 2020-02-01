#include "Layer3D.hpp"
#include <RenderingPipeline.hpp>
#include <system/Window.hpp>
#include <model/RenderableObject.hpp>
#include <model/OBJLoader.hpp>
#include <maths/GlmMatrixTransformExt.hpp>
#include <shader/IShader.hpp>
#include "texture/FITextureLoader.hpp"
#include "Timings.hpp"
#include "VFS.hpp"

template<>
class UniformAccessor<Layer3D::Uniforms> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_COPY(UniformAccessor);
public:
    static constexpr uSys MATRIX_SIZE = sizeof(float) * 4 * 4;

    [[nodiscard]] static inline uSys size() noexcept { return MATRIX_SIZE * 4; }

    static inline void set(IRenderingContext& context, const Ref<IUniformBuffer>& buffer, const Layer3D::Uniforms& t) noexcept
    {
        void* buf = buffer->mapBuffer(context);

        ::std::memcpy(reinterpret_cast<void*>(reinterpret_cast<u8*>(buf) + MATRIX_SIZE * 0), reinterpret_cast<const void*>(glm::value_ptr(t.compoundMatrix)), MATRIX_SIZE);
        ::std::memcpy(reinterpret_cast<void*>(reinterpret_cast<u8*>(buf) + MATRIX_SIZE * 1), reinterpret_cast<const void*>(glm::value_ptr(t.projectionMatrix)), MATRIX_SIZE);
        ::std::memcpy(reinterpret_cast<void*>(reinterpret_cast<u8*>(buf) + MATRIX_SIZE * 2), reinterpret_cast<const void*>(glm::value_ptr(t.viewMatrix)), MATRIX_SIZE);
        ::std::memcpy(reinterpret_cast<void*>(reinterpret_cast<u8*>(buf) + MATRIX_SIZE * 3), reinterpret_cast<const void*>(glm::value_ptr(t.modelMatrix)), MATRIX_SIZE);

        buffer->unmapBuffer(context);
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

    static inline void set(IRenderingContext& context, const Ref<IUniformBuffer>& buffer, const Layer3D::ViewPosUniforms& t) noexcept
    {
        const __m128 vec = t.cameraPos.data().vec;
        buffer->fillBuffer(context, reinterpret_cast<const void*>(&vec));
    }
};

static Vector3f fromPolar(Vector3f polar) noexcept;

Layer3D::Layer3D(Window& window, RenderingPipeline& rp, GameRecorder* recorder, State& state) noexcept
    : ILayer(true),
      _window(window), _rp(rp), _state(state),
      _camera(window, 90, 0.0001f, 1000.0f, 10.0f, 40.0f, 0.03f, false,
              Keyboard::Key::Shift, Keyboard::Key::W, Keyboard::Key::S,
              Keyboard::Key::A, Keyboard::Key::D, Keyboard::Key::Space, Keyboard::Key::Ctrl,
              recorder),
      _skybox(*window.renderingContext(), "|TERes/shader/SkyboxVertexShader.glsl", "|TERes/shader/SkyboxPixelShader.glsl", "|TERes/Skybox/OceanMountain/", ".png"),
      _gr(*recorder),
      _shader(IShaderProgram::create(*window.renderingContext())),
      _o_shader(IShaderProgram::create(*window.renderingContext())),
      _reflectionShader(IShaderProgram::create(*window.renderingContext())),
      _refractionShader(IShaderProgram::create(*window.renderingContext())),
      _frameBufferShader(IShaderProgram::create(*window.renderingContext())),
      _texture(TextureLoader::loadTexture(*window.renderingContext(), "|TERes/TestTexture.png")),
      _overlay(TextureLoader::loadTexture(*window.renderingContext(), "|TERes/Overlay.png")),
      _frameBufferVA(null),
      _modelPos(0, 0, 0), _modelViewMatrix(1.0f), _cubePolarPos(3, 3, 0), _cubeViewMatrix(1.0f), _objects(),
      _uniforms(window.renderingContext()->createUniformBuffer()), _pointLightUniforms(window.renderingContext()->createUniformBuffer()),
      _spotLightUniforms(window.renderingContext()->createUniformBuffer()), _cameraPosUni(window.renderingContext()->createUniformBuffer()),
      _materialUniforms(window.renderingContext()->createUniformBuffer())
{
    PERF();
    objl::Loader loader;
    bool loaded = loader.loadFile("|TERes/nanosuit/nanosuit.obj");

    if(loaded)
    {
        for(const objl::Mesh& mesh : loader.meshes())
        {
            _objects.emplace_back(std::make_shared<RenderableObject>(*window.renderingContext(), mesh, "|TERes/nanosuit/"));
        }
    }

    loaded = loader.loadFile("|TERes/Cube.obj");

    if(loaded)
    {
        _cube = std::make_shared<RenderableObject>(*window.renderingContext(), loader.meshes()[0], "|TERes/", DrawType::SeparatedTriangles);
    }

    Ref<IShaderBuilder> shaderBuilder = window.renderingContext()->createShader();

    shaderBuilder->type(IShader::Type::Vertex);
    shaderBuilder->file(VFS::Instance().openFile("|TERes/shader/SimpleVertexShader.glsl", FileProps::Read));
    Ref<IShader> vertexShader = Ref<IShader>(shaderBuilder->build());

    // shaderBuilder->type(IShader::Type::Pixel);
    // shaderBuilder->file(VFS::Instance().openFile("|TERes/shader/SimplePixelShader.glsl", FileProps::Read));
    // Ref<IShader> pixelShader = Ref<IShader>(shaderBuilder->build());
    
    shaderBuilder->type(IShader::Type::Pixel);
    shaderBuilder->file(VFS::Instance().openFile("|TERes/nanosuit/NanosuitPixel.glsl", FileProps::Read));
    Ref<IShader> pixelShader = Ref<IShader>(shaderBuilder->build());

    shaderBuilder->type(IShader::Type::Vertex);
    shaderBuilder->file(VFS::Instance().openFile("|TERes/shader/OutlineVertexShader.glsl", FileProps::Read));
    Ref<IShader> outlineVertexShader = Ref<IShader>(shaderBuilder->build());

    shaderBuilder->type(IShader::Type::Pixel);
    shaderBuilder->file(VFS::Instance().openFile("|TERes/shader/OutlinePixelShader.glsl", FileProps::Read));
    Ref<IShader> outlinePixelShader = Ref<IShader>(shaderBuilder->build());

    shaderBuilder->type(IShader::Type::Vertex);
    shaderBuilder->file(VFS::Instance().openFile("|TERes/shader/CubeMap/RefVertexShader.glsl", FileProps::Read));
    Ref<IShader> refVertexShader = Ref<IShader>(shaderBuilder->build());

    shaderBuilder->type(IShader::Type::Pixel);
    shaderBuilder->file(VFS::Instance().openFile("|TERes/shader/CubeMap/ReflectionPixelShader.glsl", FileProps::Read));
    Ref<IShader> reflectionPixelShader = Ref<IShader>(shaderBuilder->build());

    shaderBuilder->type(IShader::Type::Pixel);
    shaderBuilder->file(VFS::Instance().openFile("|TERes/shader/CubeMap/RefractionPixelShader.glsl", FileProps::Read));
    Ref<IShader> refractionPixelShader = Ref<IShader>(shaderBuilder->build());

    shaderBuilder->type(IShader::Type::Vertex);
    shaderBuilder->file(VFS::Instance().openFile("|TERes/shader/FrameBufferVertexShader.glsl", FileProps::Read));
    Ref<IShader> frameBufferVertexShader = Ref<IShader>(shaderBuilder->build());

    shaderBuilder->type(IShader::Type::Pixel);
    shaderBuilder->file(VFS::Instance().openFile("|TERes/shader/FrameBufferPixelShader.glsl", FileProps::Read));
    Ref<IShader> frameBufferPixelShader = Ref<IShader>(shaderBuilder->build());

    // Ref<IShader> vertexShader = IShader::create(*window.renderingContext(), IShader::Type::Vertex, "|TERes/shader/SimpleVertexShader.glsl");
    // // Ref<IShader> pixelShader = IShader::create(*window.renderingContext(), IShader::Type::Pixel, "|TERes/shader/SimplePixelShader.glsl");
    // Ref<IShader> pixelShader = IShader::create(*window.renderingContext(), IShader::Type::Pixel, "|TERes/nanosuit/NanosuitPixel.glsl");
    // Ref<IShader> outlineVertexShader = IShader::create(*window.renderingContext(), IShader::Type::Vertex, "|TERes/shader/OutlineVertexShader.glsl");
    // Ref<IShader> outlinePixelShader = IShader::create(*window.renderingContext(), IShader::Type::Pixel, "|TERes/shader/OutlinePixelShader.glsl");
    // Ref<IShader> refVertexShader = IShader::create(*window.renderingContext(), IShader::Type::Vertex, "|TERes/shader/CubeMap/RefVertexShader.glsl");
    // Ref<IShader> reflectionPixelShader = IShader::create(*window.renderingContext(), IShader::Type::Pixel, "|TERes/shader/CubeMap/ReflectionPixelShader.glsl");
    // Ref<IShader> refractionPixelShader = IShader::create(*window.renderingContext(), IShader::Type::Pixel, "|TERes/shader/CubeMap/RefractionPixelShader.glsl");
    // Ref<IShader> frameBufferVertexShader = IShader::create(*window.renderingContext(), IShader::Type::Vertex, "|TERes/shader/FrameBufferVertexShader.glsl");
    // Ref<IShader> frameBufferPixelShader = IShader::create(*window.renderingContext(), IShader::Type::Pixel, "|TERes/shader/FrameBufferPixelShader.glsl");
    //
    // vertexShader->loadShader();
    // pixelShader->loadShader();
    // outlineVertexShader->loadShader();
    // outlinePixelShader->loadShader();
    // refVertexShader->loadShader();
    // reflectionPixelShader->loadShader();
    // refractionPixelShader->loadShader();
    // frameBufferVertexShader->loadShader();
    // frameBufferPixelShader->loadShader();

    _shader->setVertexShader(*window.renderingContext(), vertexShader);
    _shader->setPixelShader(*window.renderingContext(), pixelShader);

    _o_shader->setVertexShader(*window.renderingContext(), outlineVertexShader);
    _o_shader->setPixelShader(*window.renderingContext(), outlinePixelShader);

    _reflectionShader->setVertexShader(*window.renderingContext(), refVertexShader);
    _reflectionShader->setPixelShader(*window.renderingContext(), reflectionPixelShader);

    _refractionShader->setVertexShader(*window.renderingContext(), refVertexShader);
    _refractionShader->setPixelShader(*window.renderingContext(), refractionPixelShader);

    _frameBufferShader->setVertexShader(*window.renderingContext(), frameBufferVertexShader);
    _frameBufferShader->setPixelShader(*window.renderingContext(), frameBufferPixelShader);

    _shader->link(*window.renderingContext());
    _o_shader->link(*window.renderingContext());
    _reflectionShader->link(*window.renderingContext());
    _refractionShader->link(*window.renderingContext());
    _frameBufferShader->link(*window.renderingContext());

    // _compoundMatrixUni = _shader->getUniform<glm::mat4>("compoundMatrix", false);
    // _projMatrixUni = _shader->getUniform<glm::mat4>("projectionMatrix", false);
    // _viewMatrixUni = _shader->getUniform<glm::mat4>("cameraViewMatrix", false);
    // _modelViewMatrixUni = _shader->getUniform<glm::mat4>("modelViewMatrix", false);
    // _materialUniforms = Ref<MaterialUniforms>(new(std::nothrow) MaterialUniforms(_shader, "material."));
    // _pointLightUniforms = Ref<PointLightUniforms>(new(std::nothrow) PointLightUniforms(_shader, "pointLight."));
    // _spotLightUniforms = Ref<SpotLightUniforms>(new(std::nothrow) SpotLightUniforms(_shader, "spotLight."));
    // _cameraPosUni = _shader->getUniformVector<Vector3f>("viewPos");

    // _o_projMatrixUni = _o_shader->getUniform<glm::mat4>("projectionMatrix", false);
    // _o_viewMatrixUni = _o_shader->getUniform<glm::mat4>("cameraViewMatrix", false);
    // _o_modelViewMatrixUni = _o_shader->getUniform<glm::mat4>("modelViewMatrix", false);
    // _o_scaleFactorUni = _o_shader->getUniform<float>("scaleFactor");

    // _reflectionProjMatrixUni = _reflectionShader->getUniform<glm::mat4>("projectionMatrix", false);
    // _reflectionViewMatrixUni = _reflectionShader->getUniform<glm::mat4>("cameraViewMatrix", false);
    // _reflectionModelViewMatrixUni = _reflectionShader->getUniform<glm::mat4>("modelViewMatrix", false);
    // _reflectionCameraPosUni = _reflectionShader->getUniform<const Vector3f&>("cameraPos");
    // _reflectionTexture = _reflectionShader->getUniform<int>("cubeMap");

    // _refractionProjMatrixUni = _refractionShader->getUniform<glm::mat4>("projectionMatrix", false);
    // _refractionViewMatrixUni = _refractionShader->getUniform<glm::mat4>("cameraViewMatrix", false);
    // _refractionModelViewMatrixUni = _refractionShader->getUniform<glm::mat4>("modelViewMatrix", false);
    // _refractionCameraPosUni = _refractionShader->getUniform<const Vector3f&>("cameraPos");
    // _refractionTexture = _refractionShader->getUniform<int>("cubeMap");

    // _frameBufferUni = _frameBufferShader->getUniform<int>("frameBufferSampler");

    Ref<IFrameBufferBuilder> builder = window.renderingContext()->createFrameBuffer();

    IFrameBufferAttachment* colorBuffer   = IFrameBufferAttachment::create(*window.renderingContext(), IFrameBufferAttachment::Color, window.width(), window.height());
    IFrameBufferAttachment* depthStencilBuffer = IFrameBufferAttachment::create(*window.renderingContext(), IFrameBufferAttachment::DepthStencil, window.width(), window.height());

    builder->attach(colorBuffer, nullptr);
    builder->attach(depthStencilBuffer, nullptr);

    _frameBuffer = Ref<IFrameBuffer>(builder->build(nullptr));

    // delete builder;

    float quadVertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    Ref<IBufferBuilder> positionsBuilder = window.renderingContext()->createBuffer(2);
    positionsBuilder->type(EBuffer::Type::ArrayBuffer);
    positionsBuilder->usage(EBuffer::UsageType::StaticDraw);
    // positionsBuilder->bufferSize(sizeof(quadVertices));
    positionsBuilder->elementCount(6);
    positionsBuilder->initialBuffer(quadVertices);
    positionsBuilder->descriptor().addDescriptor(ShaderSemantic::Position, ShaderDataType::Vector2Float);
    positionsBuilder->descriptor().addDescriptor(ShaderSemantic::TextureCoord, ShaderDataType::Vector2Float);

    const Ref<IBuffer> positions = Ref<IBuffer>(positionsBuilder->build(nullptr));

    // Ref<IInputLayoutBuilder> ilBuilder = window.renderingContext()->createInputLayout(2);
    // ilBuilder->setLayoutDescriptor(0, ShaderDataType::Vector2Float, ShaderSemantic::Position);
    // ilBuilder->setLayoutDescriptor(1, ShaderDataType::Vector2Float, ShaderSemantic::TextureCoord);
    // const Ref<IInputLayout> inputLayout = Ref<IInputLayout>(ilBuilder->build());

    Ref<IVertexArrayBuilder> vaBuilder = window.renderingContext()->createVertexArray(1);
    vaBuilder->setVertexBuffer(0, positions);
    // vaBuilder->inputLayout(inputLayout);
    vaBuilder->drawCount(6);
    vaBuilder->drawType(DrawType::SeparatedTriangles);
    _frameBufferVA = Ref<IVertexArray>(vaBuilder->build());

    // _frameBufferVA->addVertexBuffer(*window.renderingContext(), positions);
    // _frameBufferVA->drawCount() = 6;

    _modelViewMatrix = glmExt::translate(_modelViewMatrix, _modelPos);
    _modelViewMatrix = glmExt::rotateDegrees(_modelViewMatrix, 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    _cubeViewMatrix = glmExt::translate(glmExt::translate(glm::mat4(1.0f), _camera->position()), fromPolar(_cubePolarPos));

    _pointLight.position() = Vector3f(0.0f, -10.0f, -5.0f);
    _pointLight.ambient({ 255, 255, 255 });
    _pointLight.diffuse({ 255, 255, 255 });
    _pointLight.specular({ 255, 255, 255 });
    _pointLight.constant() = 1.0f;
    _pointLight.linear() = 0.007f;
    _pointLight.quadratic() = 0.0002f;

    _spotLight.position() = Vector3f(0.0f);
    _spotLight.direction() = Vector3f(0.0f);
    _spotLight.ambient({ 255, 255, 255 });
    _spotLight.diffuse({ 255, 255, 255 });
    _spotLight.specular({ 255, 255, 255 });
    _spotLight.constant() = 1.0f;
    _spotLight.linear() = 0.027f;
    _spotLight.quadratic() = 0.0028f;
    _spotLight.cutOff() = fastCosD(30.0f);
    _spotLight.outerCutOff() = fastCosD(35.0f);

    _pointLightUniforms.set(*window.renderingContext(), _pointLight);
    _spotLightUniforms.set(*window.renderingContext(), _spotLight);


    Ref<ITextureSamplerBuilder> textureSamplerBuilder = window.renderingContext()->createTextureSampler();
    textureSamplerBuilder->setFilterMode(ETexture::Filter::Linear,
                                         ETexture::Filter::Linear,
                                         ETexture::Filter::Linear);
    textureSamplerBuilder->setWrapMode(ETexture::WrapMode::Repeat,
                                       ETexture::WrapMode::Repeat,
                                       ETexture::WrapMode::Repeat);
    textureSamplerBuilder->setDepthComparison(ETexture::DepthCompareFunc::Never);


    Ref<ITextureUploaderBuilder> uploaderBuilder = window.renderingContext()->createTextureUploader(1);
    uploaderBuilder->setTexture(0, colorBuffer->texture());
    uploaderBuilder->textureSampler(Ref<ITextureSampler>(textureSamplerBuilder->build()));
    _frameBufferUploader = Ref<ITextureUploader>(uploaderBuilder->build());
}

void Layer3D::onUpdate(float fixedDelta) noexcept
{
    PERF();
    static const glm::mat4 identity(1.0f);

    if(_state == State::Game)
    {
        if(_gr.playing())
        {
            _gr.playUpdate();
        }
        else
        {
            if(_gr.recording())
            {
                _gr.addBeginUpdate();
            }

            const u32 screenCenterW = _window.width() >> 1;
            const u32 screenCenterH = _window.height() >> 1;
            const Mouse::Pos pos = Mouse::mousePos(_window);
            Mouse::mousePos(_window, screenCenterW, screenCenterH);
            const i32 mouseDifX = static_cast<i32>(screenCenterW) - static_cast<i32>(pos.x);
            const i32 mouseDifY = static_cast<i32>(screenCenterH) - static_cast<i32>(pos.y);
            // _camera.updateRotation(fixedDelta, mouseDifX, mouseDifY);
            _camera.update(fixedDelta, mouseDifX, mouseDifY);

            _cubePolarPos.z() += 100 * fixedDelta / 1000000.0f;
            _cubeViewMatrix = glmExt::translate(glmExt::translate(identity, _camera->position()), fromPolar(_cubePolarPos));
        }
    }
}

void Layer3D::onRender(const DeltaTime& delta) noexcept
{
    PERF();

    if(_state == State::Game)
    {
        if(_gr.playing())
        {
            _gr.playRender();
        }
        else
        {
            if(_gr.recording())
            {
                _gr.addBeginRender();
            }

            _camera.lerp(delta);
        }
    }
    
    TAU_RENDER_S(_rp, {
        // self->_frameBuffer->bind(context);
        context.clearScreen(true, true, true, { 127, 127, 255, 255 });
    
        self->_shader->bind(context);
        // self->_refractionShader->bind(context);
        // self->_reflectionShader->bind(context);
        
        self->_spotLight.position() = self->_camera.camera().position();
        self->_spotLight.direction() = self->_camera.camera().front();
    
        self->_uniforms.data().compoundMatrix = self->_camera->compoundedMatrix();
        self->_uniforms.data().projectionMatrix = self->_camera->projectionMatrix();
        self->_uniforms.data().viewMatrix = self->_camera->viewMatrix();
        self->_uniforms.data().modelMatrix = self->_modelViewMatrix;
    
        self->_cameraPosUni.data().cameraPos = self->_camera.camera().position();

        self->_spotLightUniforms.set(context, self->_spotLight);

        self->_uniforms.upload(context, 0);
        self->_pointLightUniforms.upload(context, 2);
        self->_spotLightUniforms.upload(context, 3);
        self->_cameraPosUni.upload(context, 4);
    
        // self->_compoundMatrixUni->set(self->_camera->compoundedMatrix());
        // self->_projMatrixUni->set(self->_camera->projectionMatrix());
        // self->_viewMatrixUni->set(self->_camera->viewMatrix());
        // self->_modelViewMatrixUni->set(self->_modelViewMatrix);
        // self->_cameraPosUni->set(self->_camera.camera().position());
        // self->_pointLight.set(*self->_pointLightUniforms.get());
        // self->_spotLight.set(*self->_spotLightUniforms.get());
        // self->_overlayUni->set(1);
        // self->_reflectionProjMatrixUni->set(self->_camera->projectionMatrix());
        // self->_reflectionViewMatrixUni->set(self->_camera->viewMatrix());
        // self->_reflectionModelViewMatrixUni->set(self->_modelViewMatrix);
        // self->_refractionProjMatrixUni->set(self->_camera->projectionMatrix());
        // self->_refractionViewMatrixUni->set(self->_camera->viewMatrix());
        // self->_refractionModelViewMatrixUni->set(self->_modelViewMatrix);
        // self->_reflectionCameraPosUni->set(self->_camera->position());
        // self->_refractionCameraPosUni->set(self->_camera->position());
        // self->_reflectionTexture->set(0);
        // self->_refractionTexture->set(0);
    
        // self->_skybox.skybox()->bind(0);
        for(const Ref<RenderableObject>& ro : self->_objects)
        {
            // ro->material().set(*self->_materialUniforms.get(), 0);
            TextureIndices indices(0, 0, 0);
            ro->material().upload(context, self->_materialUniforms, 1, indices);
            ro->preRender(context);
            ro->render(context);
            ro->postRender(context);
            indices = TextureIndices(0, 0, 0);
            ro->material().unbind(context, self->_materialUniforms, 1, indices);
            // ro->material().unbind(0);
        }
        // self->_skybox.skybox()->unbind(0);
        // glStencilMask(0x00);
    
        // self->_modelViewMatrixUni->set(self->_cubeViewMatrix);
        // self->_reflectionModelViewMatrixUni->set(self->_cubeViewMatrix);
        // self->_refractionModelViewMatrixUni->set(self->_cubeViewMatrix);
    
        // self->_texture->bind(1);
        // self->_overlay->bind(0);
        // self->_skybox.skybox()->bind(0);
    
        // self->_cube->preRender(context);
        // self->_cube->render(context);
        // self->_cube->postRender(context);
    
        // self->_skybox.skybox()->unbind(0);
        // self->_overlay->unbind(0);
        // self->_texture->unbind(1);
    
        // self->_reflectionShader->unbind(context);
        // self->_refractionShader->unbind(context);
    
        self->_cameraPosUni.unbind(context, 4);
        self->_spotLightUniforms.unbind(context, 3);
        self->_pointLightUniforms.unbind(context, 2);
        self->_uniforms.unbind(context, 0);
        self->_shader->unbind(context);
    
        // glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        // glStencilMask(0x00);
        // glDisable(GL_DEPTH_TEST);
        //
        // self->_o_shader->bind(context);
        //
        // self->_o_projMatrixUni->set(self->_camera->projectionMatrix());
        // self->_o_viewMatrixUni->set(self->_camera->viewMatrix());
        // self->_o_scaleFactorUni->set(0.1f);
        //
        // self->_o_modelViewMatrixUni->set(self->_cubeViewMatrix);
        // self->_cube->preRender(context);
        // self->_cube->render(context);
        // self->_cube->postRender(context);
        //
        // self->_o_modelViewMatrixUni->set(self->_modelViewMatrix);
        // for(const Ref<RenderableObject>& ro : self->_objects)
        // {
        //     ro->preRender(context);
        //     ro->render(context);
        //     ro->postRender(context);
        // }
        //
        // self->_o_shader->unbind(context);
        // glStencilMask(0x00);
        // glEnable(GL_DEPTH_TEST);
    
        self->_skybox.render(context, self->_camera.camera());
    
        // self->_frameBuffer->unbind(context);
        // context.clearScreen(true, true, true, { 0, 160, 0, 255 });
        //
        // glDisable(GL_DEPTH_TEST);
        //
        // self->_frameBufferShader->bind(context);
        // // self->_frameBufferUni->set(0);
        //
        // TextureIndices indices(0, 0, 0);
        // (void) self->_frameBufferUploader->upload(context, indices);
        //
        // context.setFaceWinding(false);
        // // self->_frameBuffer->color()->texture()->bind(0);
        // self->_frameBufferVA->bind(context);
        // self->_frameBufferVA->preDraw(context);
        // self->_frameBufferVA->draw(context);
        // self->_frameBufferVA->postDraw(context);
        // self->_frameBufferVA->unbind(context);
        // // self->_frameBuffer->color()->texture()->unbind(0);
        //
        // indices = TextureIndices(0, 0, 0);
        // (void) self->_frameBufferUploader->unbind(context, indices);
        // self->_frameBufferShader->unbind(context);
        //
        // glEnable(GL_DEPTH_TEST);
    });
     
    // auto self = this; do { struct TauRenderCommand628
    // {
    //     typename ::std::remove_const< typename ::std::remove_reference< decltype(self) >::type >::type self; TauRenderCommand628(typename ::std::remove_const< typename ::std::remove_reference< decltype(self) >::type >::type self) : self(self) { } static void execute([maybe_unused] RenderingPipeline& rp, [maybe_unused] Window& window, [maybe_unused] IRenderingContext& context, void* argBuffer)
    //     {
    //         ((void) (rp)); ((void) (window)); ((void) (context)); auto& self = reinterpret_cast<TauRenderCommand628*>(argBuffer)->self;
    //         {
    //             {
    //                 // self->_frameBuffer->bind(context);
    //                 context.clearScreen(true, true, true, { 127, 127, 255, 255 });
    //                 self->_shader->bind(context);
    //                 self->_spotLight.position() = self->_camera.camera().position();
    //                 self->_spotLight.direction() = self->_camera.camera().front();
    //                 self->_uniforms.data().compoundMatrix = self->_camera->compoundedMatrix();
    //                 self->_uniforms.data().projectionMatrix = self->_camera->projectionMatrix();
    //                 self->_uniforms.data().viewMatrix = self->_camera->viewMatrix();
    //                 self->_uniforms.data().modelMatrix = self->_modelViewMatrix;
    //                 self->_cameraPosUni.data().cameraPos = self->_camera.camera().position();
    //                 self->_uniforms.upload(context, 0);
    //                 self->_pointLightUniforms.upload(context, 2);
    //                 self->_spotLightUniforms.set(context, self->_spotLight);
    //                 self->_spotLightUniforms.upload(context, 3);
    //                 self->_cameraPosUni.upload(context, 4);
    //                 for(const Ref<RenderableObject>& ro : self->_objects)
    //                 {
    //                     TextureIndices indices(0, 0, 0);
    //                     ro->material().upload(context, self->_materialUniforms, 1, indices);
    //                     ro->preRender(context);
    //                     ro->render(context);
    //                     ro->postRender(context);
    //                     indices = TextureIndices(0, 0, 0);
    //                     ro->material().unbind(context, self->_materialUniforms, 1, indices);
    //                 }
    //                 self->_cameraPosUni.unbind(context, 4);
    //                 self->_spotLightUniforms.unbind(context, 3);
    //                 self->_pointLightUniforms.unbind(context, 2);
    //                 self->_uniforms.unbind(context, 0);
    //                 self->_shader->unbind(context);
    //                 self->_skybox.render(context, self->_camera.camera());
    //                 // self->_frameBuffer->unbind(context);
    //                 // context.clearScreen(true, true, true, { 0, 160, 0, 255 });
    //                 // glDisable(0x0B71);
    //                 // self->_frameBufferShader->bind(context);
    //                 // TextureIndices indices(0, 0, 0);
    //                 // (void) self->_frameBufferUploader->upload(context, indices);
    //                 // context.setFaceWinding(false);
    //                 // self->_frameBufferVA->bind(context);
    //                 // self->_frameBufferVA->preDraw(context);
    //                 // self->_frameBufferVA->draw(context);
    //                 // self->_frameBufferVA->postDraw(context);
    //                 // self->_frameBufferVA->unbind(context);
    //                 // indices = TextureIndices(0, 0, 0);
    //                 // (void) self->_frameBufferUploader->unbind(context, indices);
    //                 // self->_frameBufferShader->unbind(context);
    //                 // glEnable(0x0B71);
    //             };
    //         }
    //     }
    // }; { void* mem = (_rp).pushExecuteCommand(TauRenderCommand628::execute, sizeof(TauRenderCommand628)); new(mem) TauRenderCommand628(self); } } while(0);;
}

void Layer3D::onEvent(Event& e) noexcept
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowResizeEvent>(this, &Layer3D::onWindowResize);
}

bool Layer3D::onWindowResize(WindowResizeEvent& e) noexcept
{
    Ref<IFrameBufferBuilder> builder = _window.renderingContext()->createFrameBuffer();

    IFrameBufferAttachment* colorBuffer = IFrameBufferAttachment::create(*_window.renderingContext(), IFrameBufferAttachment::Color, e.newWidth(), e.newHeight());
    IFrameBufferAttachment* depthStencilBuffer = IFrameBufferAttachment::create(*_window.renderingContext(), IFrameBufferAttachment::DepthStencil, e.newWidth(), e.newHeight());

    builder->attach(colorBuffer);
    builder->attach(depthStencilBuffer);

    _frameBuffer = Ref<IFrameBuffer>(builder->build());

    return true;
}

static Vector3f fromPolar(Vector3f polar) noexcept
{
    const SinCos<float> sc = fastSinCosD(polar.z());
    return Vector3f(polar.x() * sc.cos, polar.y(), polar.x() * sc.sin);
}
