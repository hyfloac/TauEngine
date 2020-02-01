#pragma once

#include <Objects.hpp>

#include "DLL.hpp"
#include "shader/IShaderProgram.hpp"
#include "texture/Texture.hpp"
#include "shader/Uniform.hpp"
#include "shader/TextureUploader.hpp"

class Camera3D;
class IRenderingContext;
class IVertexArray;

class TAU_DLL Skybox final
{
    DEFAULT_DESTRUCT(Skybox);
    DEFAULT_COPY(Skybox);
public:
    struct Uniforms final
    {
        DEFAULT_DESTRUCT(Uniforms);
        DEFAULT_COPY(Uniforms);

        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;

        Uniforms() noexcept
            : projectionMatrix(1.0f), viewMatrix(1.0f)
        { }

        Uniforms(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) noexcept
            : projectionMatrix(projectionMatrix), viewMatrix(viewMatrix)
        { }
    };
private:
    Ref<IShaderProgram> _shader;
    // Ref<IUniform<const glm::mat4&>> _projectionUni;
    // Ref<IUniform<const glm::mat4&>> _viewUni;
    // Ref<IUniform<int>> _skyboxUni;
    UniformBlockS<Uniforms> _uniforms;

    Ref<ITextureCube> _skybox;
    Ref<ITextureUploader> _textureUploader;
    Ref<IVertexArray> _cubeVA;
public:
    Skybox(IRenderingContext& context, const char* vertexShaderPath, const char* pixelShaderPath, const char* skyboxPath, const char* fileExtension) noexcept;

    [[nodiscard]] Ref<ITextureCube> skybox() const noexcept { return _skybox; }
    [[nodiscard]] const Ref<ITextureCube>& skybox() noexcept { return _skybox; }

    void render(IRenderingContext& context, const Camera3D& camera) noexcept;
};
