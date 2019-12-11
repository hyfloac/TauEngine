#pragma once

#include <layer/ILayer.hpp>
#include <vector>
#include <Safeties.hpp>
#include <camera/Camera3D.hpp>
#include <shader/IShaderProgram.hpp>
#include <shader/IUniform.hpp>
#include <texture/Texture.hpp>
#include <texture/FrameBuffer.hpp>
#include <glm/mat4x4.hpp>
#include <maths/Vector3f.hpp>
#include <events/WindowEvent.hpp>
#include <model/IVertexArray.hpp>
#include <camera/Skybox.hpp>
#include "State.hpp"

class Window;
class RenderingPipeline;
class ResourceLoader;
class RenderableObject;

class Layer3D final : public ILayer
{
    DEFAULT_DESTRUCT(Layer3D);
    LAYER_IMPL(Layer3D);
private:
    Window& _window;
    RenderingPipeline& _rp;
    State& _state;
    FreeCamCamera3DController _camera;
    Skybox _skybox;
    GameRecorder& _gr;

    Ref<IShaderProgram> _shader;
    Ref<IUniform<const glm::mat4&>> _compoundMatrixUni;
    Ref<IUniform<const glm::mat4&>> _projMatrixUni;
    Ref<IUniform<const glm::mat4&>> _viewMatrixUni;
    Ref<IUniform<const glm::mat4&>> _modelViewMatrixUni;
    Ref<IUniform<int>> _textureUni;
    Ref<IUniform<int>> _overlayUni;

    Ref<IShaderProgram> _o_shader;
    Ref<IUniform<const glm::mat4&>> _o_projMatrixUni;
    Ref<IUniform<const glm::mat4&>> _o_viewMatrixUni;
    Ref<IUniform<const glm::mat4&>> _o_modelViewMatrixUni;
    Ref<IUniform<float>> _o_scaleFactorUni;

    Ref<IShaderProgram> _reflectionShader;
    Ref<IUniform<const glm::mat4&>> _reflectionProjMatrixUni;
    Ref<IUniform<const glm::mat4&>> _reflectionViewMatrixUni;
    Ref<IUniform<const glm::mat4&>> _reflectionModelViewMatrixUni;
    Ref<IUniform<const Vector3f&>> _reflectionCameraPosUni;
    Ref<IUniform<int>> _reflectionTexture;

    Ref<IShaderProgram> _refractionShader;
    Ref<IUniform<const glm::mat4&>> _refractionProjMatrixUni;
    Ref<IUniform<const glm::mat4&>> _refractionViewMatrixUni;
    Ref<IUniform<const glm::mat4&>> _refractionModelViewMatrixUni;
    Ref<IUniform<const Vector3f&>> _refractionCameraPosUni;
    Ref<IUniform<int>> _refractionTexture;

    Ref<IShaderProgram> _frameBufferShader;
    Ref<IUniform<int>> _frameBufferUni;

    Ref<ITexture> _texture;
    Ref<ITexture> _overlay;
    Ref<IFrameBuffer> _frameBuffer;
    Ref<IVertexArray> _frameBufferVA;

    Vector3f _modelPos;
    glm::mat4 _modelViewMatrix;

    Vector3f _cubePolarPos;
    glm::mat4 _cubeViewMatrix;
    Ref<RenderableObject> _cube;

    std::vector<Ref<RenderableObject>> _objects;
public:
    Layer3D(Window& window, RenderingPipeline& rp, GameRecorder* recorder, State& state) noexcept;

    void onUpdate(float fixedDelta) noexcept override;

    void onRender(float delta) noexcept override;

    void onEvent(Event& e) noexcept override;

    bool onWindowResize(WindowResizeEvent& e) noexcept;
};
