#pragma once

#include <layer/ILayer.hpp>
#include <vector>
#include <Safeties.hpp>
#include <camera/Camera3D.hpp>
#include <shader/IShaderProgram.hpp>
#include <shader/IUniform.hpp>
#include <texture/Texture.hpp>
#include <glm/mat4x4.hpp>
#include <maths/Vector3f.hpp>
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

    Ref<ITexture> _texture;
    Ref<ITexture> _overlay;

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
};
