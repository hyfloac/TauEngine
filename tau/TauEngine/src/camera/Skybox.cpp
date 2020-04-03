#include "camera/Skybox.hpp"
#include "camera/Camera3D.hpp"
#include "system/RenderingContext.hpp"
#include "model/VertexArray.hpp"
#include "shader/IShader.hpp"
#include "texture/FITextureLoader.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "system/GraphicsInterface.hpp"

template<>
class UniformAccessor<Skybox::Uniforms> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_COPY(UniformAccessor);
public:
    static constexpr uSys MATRIX_SIZE = sizeof(float) * 4 * 4;

    [[nodiscard]] static inline uSys size() noexcept { return MATRIX_SIZE * 2; }

    static inline void set(IRenderingContext& context, const CPPRef<IUniformBuffer>& buffer, const Skybox::Uniforms& t) noexcept
    {
        buffer->beginModification(context);
        buffer->modifyBuffer(0, MATRIX_SIZE, glm::value_ptr(t.projectionMatrix));
        buffer->modifyBuffer(MATRIX_SIZE, MATRIX_SIZE, glm::value_ptr(t.viewMatrix));
        buffer->endModification(context);
    }
};

Skybox::Skybox(IGraphicsInterface& gi, IRenderingContext& context, const char* const vfsMount, const char* const shaderPath, const char* const vertexName, const char* const pixelName, const char* const skyboxPath, const char* const fileExtension) noexcept
    : _shader(IShaderProgram::create(context)),
      _uniforms(gi.createUniformBuffer()),
      _skybox(null), _textureUploader(null), _cubeVA(null), _skyboxDepthStencilState(null)
{
    ShaderArgs shaderArgs;
    shaderArgs.vfsMount = vfsMount;
    shaderArgs.path = shaderPath;

    shaderArgs.fileName = vertexName;
    shaderArgs.stage = EShader::Stage::Vertex;
    CPPRef<IShader> vertexShader = gi.createShader().buildCPPRef(shaderArgs, null);

    shaderArgs.fileName = pixelName;
    shaderArgs.stage = EShader::Stage::Pixel;
    CPPRef<IShader> pixelShader = gi.createShader().buildCPPRef(shaderArgs, null);

    _shader->setVertexShader(context, vertexShader);
    _shader->setPixelShader(context, pixelShader);

    _shader->link(context);

    _skybox = CPPRef<ITextureCube>(TextureLoader::loadTextureCubeEx(gi, context, skyboxPath, fileExtension, -1));

    TextureSamplerArgs textureSamplerArgs;
    textureSamplerArgs.magFilter() = ETexture::Filter::Linear;
    textureSamplerArgs.minFilter() = ETexture::Filter::Linear;
    textureSamplerArgs.mipFilter() = ETexture::Filter::Linear;
    textureSamplerArgs.wrapU = ETexture::WrapMode::ClampToEdge;
    textureSamplerArgs.wrapV = ETexture::WrapMode::ClampToEdge;
    textureSamplerArgs.wrapW = ETexture::WrapMode::ClampToEdge;
    textureSamplerArgs.depthCompareFunc = ETexture::DepthCompareFunc::Never;

    SingleTextureUploaderArgs uploaderArgs;
    uploaderArgs.texture = _skybox;
    uploaderArgs.textureSampler = gi.createTextureSampler().buildCPPRef(textureSamplerArgs, null);
    _textureUploader = gi.createSingleTextureUploader().buildTauRef(uploaderArgs, null);

    float skyboxVertices[] = {
        // back
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        
        // left
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        
        // right
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        
        // front
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        // top
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        // bottom
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    BufferArgs skyboxCubeBuilder(1);
    skyboxCubeBuilder.type = EBuffer::Type::ArrayBuffer;
    skyboxCubeBuilder.usage = EBuffer::UsageType::StaticDraw;
    skyboxCubeBuilder.elementCount = 6 * 6;
    skyboxCubeBuilder.initialBuffer = skyboxVertices;
    skyboxCubeBuilder.instanced = false;
    skyboxCubeBuilder.descriptor.addDescriptor(ShaderSemantic::Position, ShaderDataType::Type::Vector3Float);

    const CPPRef<IBuffer> skyboxCube = gi.createBuffer().buildCPPRef(skyboxCubeBuilder, nullptr);

    VertexArrayArgs vaArgs(1);
    vaArgs.shader = vertexShader;
    vaArgs.buffers[0] = skyboxCube;
    vaArgs.drawCount = 36;
    vaArgs.drawType = DrawType::SeparatedTriangles;

    _cubeVA = gi.createVertexArray().buildCPPRef(vaArgs, null);

    DepthStencilArgs dsArgs = context.getDefaultDepthStencilArgs();
    dsArgs.depthWriteMask = DepthStencilArgs::DepthWriteMask::Zero;
    dsArgs.depthCompareFunc = DepthStencilArgs::CompareFunc::LessThanOrEqual;
    _skyboxDepthStencilState = gi.createDepthStencilState().buildTauRef(dsArgs, null);

    RasterizerArgs rArgs = context.getDefaultRasterizerArgs();
    rArgs.frontFaceCounterClockwise = true;
    _skyboxRasterizerState = gi.createRasterizerState().buildTauRef(rArgs, null);
}

void Skybox::render(IRenderingContext& context, const Camera3D& camera) noexcept
{
    const NullableRef<IDepthStencilState> tmpDepthStencilState = context.setDepthStencilState(_skyboxDepthStencilState);
    const NullableRef<IRasterizerState> tmpRasterizerState = context.setRasterizerState(_skyboxRasterizerState);

    _shader->bind(context);
    _uniforms.data().projectionMatrix = camera.projectionMatrix();
    _uniforms.data().viewMatrix = camera.viewRotMatrix();
    _uniforms.upload(context, EShader::Stage::Vertex, 0);
    {
        (void) _textureUploader->upload(context, TextureIndices(0, 0, 0), EShader::Stage::Pixel);
    }

    _cubeVA->bind(context);
    _cubeVA->preDraw(context);
    _cubeVA->draw(context);
    _cubeVA->postDraw(context);
    _cubeVA->unbind(context);

    _uniforms.unbind(context, EShader::Stage::Vertex, 0);
    {
        (void) _textureUploader->unbind(context, TextureIndices(0, 0, 0), EShader::Stage::Pixel);
    }

    _shader->unbind(context);

    (void) context.setRasterizerState(tmpRasterizerState);
    (void) context.setDepthStencilState(tmpDepthStencilState);
}
