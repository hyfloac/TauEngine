#pragma once

#include <layer/Layer.hpp>


#include "camera/Skybox.hpp"
#include "Globals.hpp"

class PBRLayer final : public ILayer
{
    DEFAULT_DESTRUCT(PBRLayer);
    LAYER_IMPL(PBRLayer);
public:
    struct CameraUniforms final
    {
        glm::mat4 compoundMatrix;
        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 modelMatrix;
    };
private:
    Globals& _globals;
    Camera3D* _camera;
    Skybox _skybox;

    CPPRef<IShaderProgram> _shader;
    UniformBlockS<CameraUniforms> _cameraUniforms;
public:
    void init() noexcept;

    void onUpdate(float fixedDelta) noexcept override;
    void onEvent(Event& e) noexcept override;
    void onPreRender(const DeltaTime& delta) noexcept override;
    void onRender() noexcept override;
    void onPostRender() noexcept override;
};
