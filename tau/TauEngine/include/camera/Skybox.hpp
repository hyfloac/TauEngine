#pragma once

#include <glm/mat4x4.hpp>
#include <Objects.hpp>

#include "DLL.hpp"
#include "shader/IShaderProgram.hpp"
#include "texture/Texture.hpp"
#include "shader/Uniform.hpp"
#include "shader/TextureUploader.hpp"
#include "graphics/DepthStencilState.hpp"

class Camera3D;
class IGraphicsInterface;
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
    CPPRef<IShaderProgram> _shader;
    // CPPRef<IUniform<const glm::mat4&>> _projectionUni;
    // CPPRef<IUniform<const glm::mat4&>> _viewUni;
    // CPPRef<IUniform<int>> _skyboxUni;
    UniformBlockS<Uniforms> _uniforms;

    CPPRef<ITextureCube> _skybox;
    CPPRef<ITextureUploader> _textureUploader;
    CPPRef<IVertexArray> _cubeVA;

    NullableRef<IDepthStencilState> _skyboxDepthStencilState;
public:
    Skybox(IGraphicsInterface& gi, IRenderingContext& context, const char* vfsMount, const char* shaderPath, const char* vertexName, const char* pixelName, const char* skyboxPath, const char* fileExtension) noexcept;

    [[nodiscard]] CPPRef<ITextureCube> skybox() const noexcept { return _skybox; }
    [[nodiscard]] const CPPRef<ITextureCube>& skybox() noexcept { return _skybox; }

    void render(IRenderingContext& context, const Camera3D& camera) noexcept;
};
