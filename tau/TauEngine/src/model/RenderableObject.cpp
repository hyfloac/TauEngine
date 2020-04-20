#include "model/RenderableObject.hpp"
#include <DynArray.hpp>
#include <NumTypes.hpp>
#include "system/RenderingContext.hpp"
#include "model/VertexArray.hpp"
#include "Timings.hpp"
#include "texture/FITextureLoader.hpp"
#include "VFS.hpp"
#include "graphics/RasterizerState.hpp"
#include "system/GraphicsInterface.hpp"
#include <glm/vec4.hpp>

NullableRef<IRasterizerState> RenderableObject::cwRS = nullptr;
NullableRef<IRasterizerState> RenderableObject::ccwRS = nullptr;

RenderableObject::RenderableObject(IGraphicsInterface& gi, IRenderingContext& context, const objl::Mesh& mesh, const char* materialFolder, const CPPRef<IShader>& shader, bool counterClockwise, const DrawType drawType) noexcept
    : _va(null), _rs(cwRS)
{
    PERF();
    const size_t cnt1 = mesh.vertices.size();
    const size_t cnt2 = cnt1 + cnt1;
    const size_t cnt3 = cnt2 + cnt1;

    DynArray<float> positionsLoaded(cnt3);
    DynArray<float> normalsLoaded(cnt3);
    DynArray<float> texturesLoaded(cnt2);
    DynArray<float> tangentsLoaded(cnt3);

    u32 posIndex = 0;
    u32 normIndex = 0;
    u32 texIndex = 0;
    u32 tanIndex = 0;

    for(objl::Vertex vertex : mesh.vertices)
    {
        positionsLoaded[posIndex++] = vertex.position.x();
        positionsLoaded[posIndex++] = vertex.position.y();
        positionsLoaded[posIndex++] = vertex.position.z();

        normalsLoaded[normIndex++] = vertex.normal.x();
        normalsLoaded[normIndex++] = vertex.normal.y();
        normalsLoaded[normIndex++] = vertex.normal.z();

        tangentsLoaded[tanIndex++] = vertex.tangent.x();
        tangentsLoaded[tanIndex++] = vertex.tangent.y();
        tangentsLoaded[tanIndex++] = vertex.tangent.z();

        texturesLoaded[texIndex++] = vertex.textureCoordinate.x();
        texturesLoaded[texIndex++] = vertex.textureCoordinate.y();
    }

    BufferArgs pnBuilder(1);
    BufferArgs texturesBuilder(1);
    IndexBufferArgs indicesBuilder;

    pnBuilder.type = EBuffer::Type::ArrayBuffer;
    pnBuilder.usage = EBuffer::UsageType::StaticDraw;
    pnBuilder.elementCount = cnt1;
    pnBuilder.descriptor.addDescriptor(ShaderSemantic::Position, ShaderDataType::Vector3Float);

    texturesBuilder.type = EBuffer::Type::ArrayBuffer;
    texturesBuilder.usage = EBuffer::UsageType::StaticDraw;
    texturesBuilder.elementCount = cnt1;
    texturesBuilder.descriptor.addDescriptor(ShaderSemantic::TextureCoord, ShaderDataType::Vector2Float);

    indicesBuilder.usage = EBuffer::UsageType::StaticDraw;
    indicesBuilder.elementCount = mesh.indices.size();

    pnBuilder.initialBuffer = positionsLoaded;
    CPPRef<IBuffer> positions = gi.createBuffer().buildCPPRef(pnBuilder, nullptr);

    pnBuilder.initialBuffer = normalsLoaded;
    pnBuilder.descriptor.reset(1);
    pnBuilder.descriptor.addDescriptor(ShaderSemantic::Normal, ShaderDataType::Vector3Float);
    CPPRef<IBuffer> normals = gi.createBuffer().buildCPPRef(pnBuilder, nullptr);

    pnBuilder.initialBuffer = tangentsLoaded;
    pnBuilder.descriptor.reset(1);
    pnBuilder.descriptor.addDescriptor(ShaderSemantic::Tangent, ShaderDataType::Vector3Float);
    CPPRef<IBuffer> tangents = gi.createBuffer().buildCPPRef(pnBuilder, nullptr);

    texturesBuilder.initialBuffer = texturesLoaded;
    CPPRef<IBuffer> textures = gi.createBuffer().buildCPPRef(texturesBuilder, nullptr);

    indicesBuilder.initialBuffer = mesh.indices.data();
    CPPRef<IIndexBuffer> indices = gi.createIndexBuffer().buildCPPRef(indicesBuilder, nullptr);

    VertexArrayArgs vaArgs(4);
    vaArgs.shader = shader.get();
    vaArgs.buffers[0] = positions;
    vaArgs.buffers[1] = normals;
    vaArgs.buffers[2] = tangents;
    vaArgs.buffers[3] = textures;
    vaArgs.indexBuffer = indices;
    vaArgs.drawCount = mesh.indices.size();
    vaArgs.drawType = drawType;
    _va = gi.createVertexArray().buildCPPRef(vaArgs, null);

    MaterialBuilder matBuilder(gi);

    if(!mesh.material.map_Kd.empty())
    {
        const auto path = VFS::Instance().resolvePath(materialFolder, mesh.material.map_Kd.c_str());
        matBuilder.diffuseTexture(CPPRef<ITexture2D>(TextureLoader::loadTexture(gi, context, path.path)));
    }
    if(!mesh.material.map_Ks.empty())
    {
        const auto path = VFS::Instance().resolvePath(materialFolder, mesh.material.map_Ks.c_str());
        matBuilder.specularTexture(CPPRef<ITexture2D>(TextureLoader::loadTexture(gi, context, path.path)));
    }
    else
    {
        matBuilder.specularTexture(CPPRef<ITexture2D>(TextureLoader::generateBlackTexture(gi, context)));
    }
    if(!mesh.material.map_Ka.empty())
    {
        const auto path = VFS::Instance().resolvePath(materialFolder, mesh.material.map_Ka.c_str());
        _reflectiveTexture = CPPRef<ITexture2D>(TextureLoader::loadTexture(gi, context, path.path));
    }
    if(!mesh.material.map_bump.empty())
    {
        const auto path = VFS::Instance().resolvePath(materialFolder, mesh.material.map_bump.c_str());
        matBuilder.normalTexture(CPPRef<ITexture2D>(TextureLoader::loadTexture(gi, context, path.path)));
    }
    else
    {
        matBuilder.normalTexture(CPPRef<ITexture2D>(TextureLoader::generateNormalTexture(gi, context)));
    }

    matBuilder.specularExponent(mesh.material.Ns);
    _illumination = mesh.material.illum;

    TextureSamplerArgs textureSamplerArgs;
    textureSamplerArgs.magFilter() = ETexture::Filter::Linear;
    textureSamplerArgs.minFilter() = ETexture::Filter::Linear;
    textureSamplerArgs.mipFilter() = ETexture::Filter::Linear;
    textureSamplerArgs.wrapU = ETexture::WrapMode::Repeat;
    textureSamplerArgs.wrapV = ETexture::WrapMode::Repeat;
    textureSamplerArgs.wrapW = ETexture::WrapMode::Repeat;
    textureSamplerArgs.depthCompareFunc = ETexture::CompareFunc::Never;

    matBuilder.textureSampler(CPPRef<ITextureSampler>(gi.createTextureSampler().buildCPPRef(textureSamplerArgs, null)));

    _material = matBuilder.build();

    if(counterClockwise)
    {
        if(!ccwRS)
        {
            RasterizerArgs rArgs = context.getDefaultRasterizerArgs();
            if(rArgs.frontFaceCounterClockwise)
            {
                ccwRS = context.getDefaultRasterizerState();
            }
            else
            {
                rArgs.frontFaceCounterClockwise = true;
                ccwRS = gi.createRasterizerState().buildTauRef(rArgs, null);
            }
        }

        _rs = ccwRS;
    }
    else
    {
        if(!cwRS)
        {
            RasterizerArgs rArgs = context.getDefaultRasterizerArgs();
            if(!rArgs.frontFaceCounterClockwise)
            {
                cwRS = context.getDefaultRasterizerState();
            }
            else
            {
                rArgs.frontFaceCounterClockwise = false;
                cwRS = gi.createRasterizerState().buildTauRef(rArgs, null);
            }
        }

        _rs = cwRS;
    }
}

void RenderableObject::preRender(IRenderingContext& context) const noexcept
{
    _rs = context.setRasterizerState(_rs);
    _va->bind(context);
    _va->preDraw(context);
}

void RenderableObject::render(IRenderingContext& context) const noexcept
{
    _va->draw(context);
}

void RenderableObject::postRender(IRenderingContext& context) const noexcept
{
    _va->postDraw(context);
    _va->unbind(context);
    _rs = context.setRasterizerState(_rs);
}
