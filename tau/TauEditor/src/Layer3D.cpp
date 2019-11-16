#include "Layer3D.hpp"
#include <RenderingPipeline.hpp>
#include <system/Window.hpp>
#include <model/RenderableObject.hpp>
#include <model/OBJLoader.hpp>
#include <maths/GlmMatrixTransformExt.hpp>
#include <shader/IShader.hpp>
#include "texture/FITextureLoader.hpp"
#include "Timings.hpp"

static Vector3f fromPolar(Vector3f polar) noexcept;

Layer3D::Layer3D(Window& window, RenderingPipeline& rp, GameRecorder* recorder, State& state) noexcept
    : ILayer(true),
      _window(window), _rp(rp), _state(state),
      _camera(window, 90, 0.0001f, 1000.0f, 10.0f, 20.0f, 0.03f, false,
              Keyboard::Key::Shift, Keyboard::Key::W, Keyboard::Key::S,
              Keyboard::Key::A, Keyboard::Key::D, Keyboard::Key::Space, Keyboard::Key::Ctrl,
              recorder),
      _gr(*recorder),
      _shader(IShaderProgram::create(*window.renderingContext())),
      _o_shader(IShaderProgram::create(*window.renderingContext())),
      _texture(TextureLoader::loadTexture("|TERes/TestTexture.png", TextureLoader::FilterType::Linear)), _overlay(TextureLoader::loadTexture("|TERes/Overlay.png", TextureLoader::FilterType::Linear)),
      _modelPos(0, 0, 0), _modelViewMatrix(1.0f), _cubePolarPos(3, -3, 0), _cubeViewMatrix(1.0f), _objects()
{
    PERF();
    objl::Loader loader;
    bool loaded = loader.loadFile("|TERes/nanosuit/nanosuit.obj");

    if(loaded)
    {
        for(const objl::Mesh& mesh : loader.meshes())
        {
            _objects.emplace_back(std::make_shared<RenderableObject>(*window.renderingContext(), mesh));
        }
    }

    loaded = loader.loadFile("|TERes/Cube.obj");

    if(loaded)
    {
        _cube = std::make_shared<RenderableObject>(*window.renderingContext(), loader.meshes()[0]);
    }

    Ref<IShader> vertexShader = IShader::create(*window.renderingContext(), IShader::Type::Vertex, "|TERes/shader/SimpleVertexShader.glsl");
    Ref<IShader> pixelShader = IShader::create(*window.renderingContext(), IShader::Type::Pixel, "|TERes/shader/SimplePixelShader.glsl");
    Ref<IShader> outlineVertexShader = IShader::create(*window.renderingContext(), IShader::Type::Vertex, "|TERes/shader/OutlineVertexShader.glsl");
    Ref<IShader> outlinePixelShader = IShader::create(*window.renderingContext(), IShader::Type::Pixel, "|TERes/shader/OutlinePixelShader.glsl");

    vertexShader->loadShader();
    pixelShader->loadShader();
    outlineVertexShader->loadShader();
    outlinePixelShader->loadShader();

    _shader->setVertexShader(*window.renderingContext(), vertexShader);
    _shader->setPixelShader(*window.renderingContext(), pixelShader);

    _o_shader->setVertexShader(*window.renderingContext(), outlineVertexShader);
    _o_shader->setPixelShader(*window.renderingContext(), outlinePixelShader);

    _shader->link(*window.renderingContext());
    _o_shader->link(*window.renderingContext());

    _compoundMatrixUni = _shader->getUniformMatrix4x4Float("compoundMatrix", false);
    _projMatrixUni = _shader->getUniformMatrix4x4Float("projectionMatrix", false);
    _viewMatrixUni = _shader->getUniformMatrix4x4Float("cameraViewMatrix", false);
    _modelViewMatrixUni = _shader->getUniformMatrix4x4Float("modelViewMatrix", false);
    _textureUni = _shader->getUniformInt("textureSampler");
    _overlayUni = _shader->getUniformInt("textureOverlay");

    _o_projMatrixUni = _o_shader->getUniformMatrix4x4Float("projectionMatrix", false);
    _o_viewMatrixUni = _o_shader->getUniformMatrix4x4Float("cameraViewMatrix", false);
    _o_modelViewMatrixUni = _o_shader->getUniformMatrix4x4Float("modelViewMatrix", false);
    _o_scaleFactorUni = _o_shader->getUniformFloat("scaleFactor");

    _modelViewMatrix = glmExt::translate(_modelViewMatrix, _modelPos);
    _cubeViewMatrix = glmExt::translate(glmExt::translate(glm::mat4(1.0f), _camera->position()), fromPolar(_cubePolarPos));
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

            _camera.update(fixedDelta);

            _cubePolarPos.z() += 100 * fixedDelta / 1000000.0f;
            _cubeViewMatrix = glmExt::translate(glmExt::translate(identity, _camera->position()), fromPolar(_cubePolarPos));
        }
    }
}

void Layer3D::onRender(float delta) noexcept
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

            const u32 screenCenterW = _window.width() >> 1;
            const u32 screenCenterH = _window.height() >> 1;
            const Mouse::Pos pos = Mouse::mousePos(_window);
            Mouse::mousePos(_window, screenCenterW, screenCenterH);
            const i32 mouseDifX = static_cast<i32>(screenCenterW) - static_cast<i32>(pos.x);
            const i32 mouseDifY = static_cast<i32>(screenCenterH) - static_cast<i32>(pos.y);
            _camera.updateRotation(delta, mouseDifX, mouseDifY);
        }
    }

    TAU_RENDER_S(_rp, {
        self->_shader->bind(context);
        self->_compoundMatrixUni->set(self->_camera->compoundedMatrix());
        self->_projMatrixUni->set(self->_camera->projectionMatrix());
        self->_viewMatrixUni->set(self->_camera->viewMatrix());
        self->_modelViewMatrixUni->set(self->_modelViewMatrix);
        self->_textureUni->set(0);
        self->_overlayUni->set(1);

        self->_texture->bind(0);
        self->_overlay->bind(1);
        for(const Ref<RenderableObject>& ro : self->_objects)
        {
            ro->preRender(context);
            ro->render(context);
            ro->postRender(context);
        }
        self->_overlay->unbind(1);
        self->_texture->unbind(0);
        // glStencilMask(0x00);

        self->_modelViewMatrixUni->set(self->_cubeViewMatrix);

        self->_texture->bind(1);
        self->_overlay->bind(0);

        self->_cube->preRender(context);
        self->_cube->render(context);
        self->_cube->postRender(context);

        self->_overlay->unbind(0);
        self->_texture->unbind(1);

        self->_shader->unbind(context);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        self->_o_shader->bind(context);

        self->_o_projMatrixUni->set(self->_camera->projectionMatrix());
        self->_o_viewMatrixUni->set(self->_camera->viewMatrix());
        self->_o_scaleFactorUni->set(0.1f);

        self->_o_modelViewMatrixUni->set(self->_cubeViewMatrix);
        self->_cube->preRender(context);
        self->_cube->render(context);
        self->_cube->postRender(context);

        self->_o_modelViewMatrixUni->set(self->_modelViewMatrix);
        for(const Ref<RenderableObject>& ro : self->_objects)
        {
            ro->preRender(context);
            ro->render(context);
            ro->postRender(context);
        }

        self->_o_shader->unbind(context);
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);
    });
}

void Layer3D::onEvent(Event& e) noexcept
{
    UNUSED(e);
}

static Vector3f fromPolar(Vector3f polar) noexcept
{
    const SinCos<float> sc = fastSinCosD(polar.z());
    return Vector3f(polar.x() * sc.cos, polar.y(), polar.x() * sc.sin);
}
