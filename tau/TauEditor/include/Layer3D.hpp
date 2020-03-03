#pragma once

#include <layer/ILayer.hpp>
#include <vector>
#include <Safeties.hpp>
#include <camera/Camera3D.hpp>
#include <shader/IShaderProgram.hpp>
#include <shader/Uniform.hpp>
#include <texture/Texture.hpp>
#include <texture/FrameBuffer.hpp>
#include <glm/mat4x4.hpp>
#include <maths/Vector3f.hpp>
#include <events/WindowEvent.hpp>
#include <model/VertexArray.hpp>
#include <camera/Skybox.hpp>
#include <shader/PointLight.hpp>
#include <model/Material.hpp>
#include "State.hpp"
#include <shader/SpotLight.hpp>

class Window;
class RenderingPipeline;
class ResourceLoader;
class RenderableObject;

class Layer3D final : public ILayer
{
    DEFAULT_DESTRUCT(Layer3D);
    LAYER_IMPL(Layer3D);
public:
    struct Uniforms final
    {
        glm::mat4 compoundMatrix;
        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 modelMatrix;
    };

    struct ViewPosUniforms final
    {
        Vector3f cameraPos;
    };
private:
    Window& _window;
    RenderingPipeline& _rp;
    State& _state;
    FreeCamCamera3DController _camera;
    Skybox _skybox;

    PointLight _pointLight;
    SpotLight _spotLight;

    CPPRef<IShaderProgram> _shader;
    UniformBlockS<Uniforms> _uniforms;
    UniformBlockU<Material> _materialUniforms;
    UniformBlockU<PointLight> _pointLightUniforms;
    UniformBlockU<SpotLight> _spotLightUniforms;
    UniformBlockS<ViewPosUniforms> _cameraPosUni;

    CPPRef<IShaderProgram> _frameBufferShader;
    CPPRef<ITextureUploader> _frameBufferUploader;

    CPPRef<ITexture> _texture;
    CPPRef<ITexture> _overlay;
    CPPRef<IFrameBuffer> _frameBuffer;
    CPPRef<IVertexArray> _frameBufferVA;

    Vector3f _modelPos;
    glm::mat4 _modelViewMatrix;

    std::vector<CPPRef<RenderableObject>> _objects;
public:
    Layer3D(Window& window, RenderingPipeline& rp, GameRecorder* recorder, State& state) noexcept;

    [[nodiscard]] FreeCamCamera3DController& camera() noexcept { return _camera; }
    [[nodiscard]] const FreeCamCamera3DController& camera() const noexcept { return _camera; }

    void onUpdate(float fixedDelta) noexcept override;

    void onRender(const DeltaTime& delta) noexcept override;

    void onEvent(Event& e) noexcept override;

    bool onWindowResize(WindowResizeEvent& e) noexcept;
};
