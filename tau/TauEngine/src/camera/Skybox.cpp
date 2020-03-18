#include "camera/Skybox.hpp"
#include "camera/Camera3D.hpp"
#include "system/RenderingContext.hpp"
#include "model/VertexArray.hpp"
#include "shader/IShader.hpp"
#include "texture/FITextureLoader.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "maths/glmExt/GlmMatrixTransformExt.hpp"

template<>
class UniformAccessor<Skybox::Uniforms> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_COPY(UniformAccessor);
public:
    [[nodiscard]] static inline uSys size() noexcept { return sizeof(float) * 4 * 4 * 2; }

    static inline void set(IRenderingContext& context, const CPPRef<IUniformBuffer>& buffer, const Skybox::Uniforms& t) noexcept
    {
        buffer->beginModification(context);
        buffer->modifyBuffer(0, sizeof(float) * 4 * 4, glm::value_ptr(t.projectionMatrix));
        buffer->modifyBuffer(sizeof(float) * 4 * 4, sizeof(float) * 4 * 4, glm::value_ptr(t.viewMatrix));
        buffer->endModification(context);
    }
};

Skybox::Skybox(IRenderingContext& context, const char* const vfsMount, const char* const shaderPath, const char* const vertexName, const char* const pixelName, const char* const skyboxPath, const char* const fileExtension) noexcept
    : _shader(IShaderProgram::create(context)),
      _uniforms(context.createUniformBuffer()),
      _skybox(null), _textureUploader(null), _cubeVA(null), _skyboxDepthStencilState(null)
{
    ShaderArgs shaderArgs;
    shaderArgs.vfsMount = vfsMount;
    shaderArgs.path = shaderPath;

    shaderArgs.fileName = vertexName;
    shaderArgs.stage = EShader::Stage::Vertex;
    CPPRef<IShader> vertexShader = context.createShader().buildCPPRef(shaderArgs, null);

    shaderArgs.fileName = pixelName;
    shaderArgs.stage = EShader::Stage::Pixel;
    CPPRef<IShader> pixelShader = context.createShader().buildCPPRef(shaderArgs, null);

    _shader->setVertexShader(context, vertexShader);
    _shader->setPixelShader(context, pixelShader);

    _shader->link(context);

    _skybox = CPPRef<ITextureCube>(TextureLoader::loadTextureCubeEx(context, skyboxPath, fileExtension, -1));

    TextureSamplerArgs textureSamplerArgs;
    textureSamplerArgs.magFilter() = ETexture::Filter::Linear;
    textureSamplerArgs.minFilter() = ETexture::Filter::Linear;
    textureSamplerArgs.mipFilter() = ETexture::Filter::Linear;
    textureSamplerArgs.wrapU = ETexture::WrapMode::ClampToEdge;
    textureSamplerArgs.wrapV = ETexture::WrapMode::ClampToEdge;
    textureSamplerArgs.wrapW = ETexture::WrapMode::ClampToEdge;
    textureSamplerArgs.depthCompareFunc = ETexture::DepthCompareFunc::Never;

    CPPRef<ITextureUploaderBuilder> uploaderBuilder = context.createTextureUploader(1);
    uploaderBuilder->setTexture(0, _skybox);
    uploaderBuilder->textureSampler(CPPRef<ITextureSampler>(context.createTextureSampler().buildCPPRef(textureSamplerArgs, null)));

    _textureUploader = CPPRef<ITextureUploader>(uploaderBuilder->build());

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
    skyboxCubeBuilder.descriptor.addDescriptor(ShaderSemantic::Position, ShaderDataType::Type::Vector3Float);

    CPPRef<IBuffer> skyboxCube = context.createBuffer().buildCPPRef(skyboxCubeBuilder, nullptr);
    skyboxCube->fillBuffer(context, skyboxVertices);

    CPPRef<IVertexArrayBuilder> vaBuilder = context.createVertexArray(1);
    vaBuilder->shader(vertexShader);
    vaBuilder->setVertexBuffer(0, skyboxCube);
    vaBuilder->drawCount(36);
    vaBuilder->drawType(DrawType::SeparatedTriangles);

    _cubeVA = CPPRef<IVertexArray>(vaBuilder->build());

    DepthStencilArgs params = context.getDefaultDepthStencilStateParams();
    params.depthWriteMask = DepthStencilArgs::DepthWriteMask::Zero;
    params.depthCompareFunc = DepthStencilArgs::CompareFunc::LessThanOrEqual;
    _skyboxDepthStencilState = context.createDepthStencilState().buildTauRef(params, null);
}

void Skybox::render(IRenderingContext& context, const Camera3D& camera) noexcept
{
    // context.enableDepthWriting(false);
    context.setFaceWinding(false);

    // glDepthFunc(GL_LEQUAL);

    const auto tmpState = context.setDepthStencilState(_skyboxDepthStencilState);

    _shader->bind(context);
    if(glmExt::useTranspose(context.mode()))
    {
        _uniforms.data().projectionMatrix = camera.projectionMatrixTrans();
        _uniforms.data().viewMatrix = camera.viewRotMatrixTrans();
    }
    else
    {
        _uniforms.data().projectionMatrix = camera.projectionMatrix();
        _uniforms.data().viewMatrix = camera.viewRotMatrix();
    }
    _uniforms.upload(context, EShader::Stage::Vertex, 0);
    {
        auto indices = TextureIndices(0, 0, 0);
        (void) _textureUploader->upload(context, indices, EShader::Stage::Pixel);
    }

    // _skybox->bind(0, EShader::Stage::Pixel);

    _cubeVA->bind(context);
    _cubeVA->preDraw(context);
    _cubeVA->draw(context);
    _cubeVA->postDraw(context);
    _cubeVA->unbind(context);

    _uniforms.unbind(context, EShader::Stage::Vertex, 0);
    {
        auto indices = TextureIndices(0, 0, 0);
        (void) _textureUploader->unbind(context, indices, EShader::Stage::Pixel);
    }
    // _skybox->unbind(0, EShader::Stage::Pixel);
    _shader->unbind(context);

    // glDepthFunc(GL_LESS);

    // context.enableDepthWriting(true);

    context.setDepthStencilState(tmpState);
}
