#pragma once

#include <glm/mat4x4.hpp>
#include <Objects.hpp>

#include "DLL.hpp"
#include "shader/ShaderProgram.hpp"
#include "graphics/Resource.hpp"
#include "shader/Uniform.hpp"
#include "shader/TextureUploader.hpp"
#include "graphics/DepthStencilState.hpp"
#include "graphics/RasterizerState.hpp"

class Camera3D;
class IGraphicsInterface;
class IRenderingContext;
class IVertexArray;

class TAU_DLL Skybox final
{
    DEFAULT_CONSTRUCT_PU(Skybox);
    DEFAULT_DESTRUCT(Skybox);
    DEFAULT_CM(Skybox);
public:
    struct Uniforms final
    {
        DEFAULT_DESTRUCT(Uniforms);
        DEFAULT_CM_PU(Uniforms);

        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;

        Uniforms(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) noexcept
            : projectionMatrix(projectionMatrix)
            , viewMatrix(viewMatrix)
        { }
    };
private:
    CPPRef<IShaderProgram> _shader;
    UniformBlockS<Uniforms> _uniforms;

    NullableRef<IResource> _skybox;
    NullableRef<ISingleTextureUploader> _textureUploader;
    CPPRef<IVertexArray> _cubeVA;

    NullableRef<IDepthStencilState> _skyboxDepthStencilState;
    NullableRef<IRasterizerState> _skyboxRasterizerState;
public:
    Skybox(IGraphicsInterface& gi, IRenderingContext& context, const char* vfsMount, const char* shaderPath, const char* vertexName, const char* pixelName, const char* skyboxPath, const char* fileExtension) noexcept;

    [[nodiscard]]       NullableRef<IResource>  skybox()       noexcept { return _skybox; }
    [[nodiscard]] const NullableRef<IResource>& skybox() const noexcept { return _skybox; }

    void render(IRenderingContext& context, const Camera3D& camera) noexcept;
};
