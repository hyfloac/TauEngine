#pragma once

#include "DLL.hpp"
#include "shader/IShaderProgram.hpp"
#include "texture/Texture.hpp"
#include <Objects.hpp>

class Camera3D;
class IRenderingContext;
class IVertexArray;

class TAU_DLL Skybox final
{
    DEFAULT_DESTRUCT(Skybox);
    DEFAULT_COPY(Skybox);
private:
    Ref<IShaderProgram> _shader;
    Ref<IUniform<const glm::mat4&>> _projectionUni;
    Ref<IUniform<const glm::mat4&>> _viewUni;
    Ref<IUniform<int>> _skyboxUni;

    Ref<ITextureCube> _skybox;
    Ref<IVertexArray> _cubeVA;
public:
    Skybox(IRenderingContext& context, const char* vertexShaderPath, const char* pixelShaderPath, const char* skyboxPath, const char* fileExtension) noexcept;

    [[nodiscard]] Ref<ITextureCube> skybox() const noexcept { return _skybox; }
    [[nodiscard]] const Ref<ITextureCube>& skybox() noexcept { return _skybox; }

    void render(IRenderingContext& context, const Camera3D& camera) const noexcept;
};
