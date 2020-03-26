#include <model/RenderableObject.hpp>
#include <DynArray.hpp>
#include <NumTypes.hpp>
#include "system/RenderingContext.hpp"
#include "model/VertexArray.hpp"
#include "Timings.hpp"
#include "texture/FITextureLoader.hpp"
#include "VFS.hpp"

// static CPPRef<IInputLayout> _inputLayoutCache = null;

RenderableObject::RenderableObject(IRenderingContext& context, const objl::Mesh& mesh, const char* materialFolder, const CPPRef<IShader>& shader, const DrawType drawType) noexcept
    : _va(null)
{
    PERF();
    const size_t cnt1 = mesh.vertices.size();
    const size_t cnt2 = cnt1 + cnt1;
    const size_t cnt3 = cnt2 + cnt1;

    DynArray<float> positionsLoaded(cnt3);
    DynArray<float> normalsLoaded(cnt3);
    DynArray<float> texturesLoaded(cnt2);
    DynArray<float> tangentsLoaded(cnt3);
    // DynArray<float> bitangentsLoaded(cnt3);

    u32 posIndex = 0;
    u32 normIndex = 0;
    u32 texIndex = 0;
    u32 tanIndex = 0;
    // u32 biTanIndex = 0;

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

        // bitangentsLoaded[biTanIndex++] = vertex.bitangent.x();
        // bitangentsLoaded[biTanIndex++] = vertex.bitangent.y();
        // bitangentsLoaded[biTanIndex++] = vertex.bitangent.z();

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

    // CPPRef<IBuffer> positions = CPPRef<IBuffer>(pnBuilder->build(nullptr));
    pnBuilder.initialBuffer = positionsLoaded;
    CPPRef<IBuffer> positions = context.createBuffer().buildCPPRef(pnBuilder, nullptr);

    pnBuilder.initialBuffer = normalsLoaded;
    pnBuilder.descriptor.reset(1);
    pnBuilder.descriptor.addDescriptor(ShaderSemantic::Normal, ShaderDataType::Vector3Float);
    // CPPRef<IBuffer> normals = CPPRef<IBuffer>(pnBuilder->build(nullptr));
    CPPRef<IBuffer> normals = context.createBuffer().buildCPPRef(pnBuilder, nullptr);

    pnBuilder.initialBuffer = tangentsLoaded;
    pnBuilder.descriptor.reset(1);
    pnBuilder.descriptor.addDescriptor(ShaderSemantic::Tangent, ShaderDataType::Vector3Float);
    // CPPRef<IBuffer> tangents = CPPRef<IBuffer>(pnBuilder->build(nullptr));
    CPPRef<IBuffer> tangents = context.createBuffer().buildCPPRef(pnBuilder, nullptr);
    // CPPRef<IBuffer> textures = CPPRef<IBuffer>(texturesBuilder->build(nullptr));
    texturesBuilder.initialBuffer = texturesLoaded;
    CPPRef<IBuffer> textures = context.createBuffer().buildCPPRef(texturesBuilder, nullptr);
    // CPPRef<IIndexBuffer> indices = CPPRef<IIndexBuffer>(indicesBuilder->build(nullptr));
    indicesBuilder.initialBuffer = mesh.indices.data();
    CPPRef<IIndexBuffer> indices = context.createIndexBuffer().buildCPPRef(indicesBuilder, nullptr);

    // positions->fillBuffer(context, positionsLoaded);
    // normals->fillBuffer(context, normalsLoaded);
    // tangents->fillBuffer(context, tangentsLoaded);
    // textures->fillBuffer(context, texturesLoaded);
    // indices->fillBuffer(context, mesh.indices.data());

    VertexArrayArgs vaArgs(4);

    vaArgs.shader = shader;
    vaArgs.buffers[0] = positions;
    vaArgs.buffers[1] = normals;
    vaArgs.buffers[2] = tangents;
    vaArgs.buffers[3] = textures;
    vaArgs.indexBuffer = indices;
    vaArgs.drawCount = mesh.indices.size();
    vaArgs.drawType = DrawType::SeparatedTriangles;

    _va = context.createVertexArray().buildCPPRef(vaArgs, null);

    // _va->addVertexBuffer(context, positions);
    // _va->addVertexBuffer(context, normals);
    // _va->addVertexBuffer(context, tangents);
    // // _va->addVertexBuffer(context, bitangents);
    // _va->addVertexBuffer(context, textures);
    // _va->setIndexBuffer(context, indices);
    // _va->drawCount() = mesh.indices.size();

    MaterialBuilder matBuilder(context);

    if(!mesh.material.map_Kd.empty())
    {
        const auto path = VFS::Instance().resolvePath(materialFolder, mesh.material.map_Kd.c_str());
        matBuilder.diffuseTexture(CPPRef<ITexture>(TextureLoader::loadTexture(context, path.path)));
    }
    if(!mesh.material.map_Ks.empty())
    {
        const auto path = VFS::Instance().resolvePath(materialFolder, mesh.material.map_Ks.c_str());
        matBuilder.specularTexture(CPPRef<ITexture>(TextureLoader::loadTexture(context, path.path)));
    }
    else
    {
        matBuilder.specularTexture(CPPRef<ITexture>(TextureLoader::generateBlackTexture(context)));
    }
    if(!mesh.material.map_Ka.empty())
    {
        const auto path = VFS::Instance().resolvePath(materialFolder, mesh.material.map_Ka.c_str());
        _reflectiveTexture = CPPRef<ITexture>(TextureLoader::loadTexture(context, path.path));
    }
    if(!mesh.material.map_bump.empty())
    {
        const auto path = VFS::Instance().resolvePath(materialFolder, mesh.material.map_bump.c_str());
        matBuilder.normalTexture(CPPRef<ITexture>(TextureLoader::loadTexture(context, path.path)));
    }
    else
    {
        matBuilder.normalTexture(CPPRef<ITexture>(TextureLoader::generateNormalTexture(context)));
    }
    // const auto ka = mesh.material.Ka;
    // const auto kd = mesh.material.Kd;
    // const auto ks = mesh.material.Ks;
    // _ambientColor = glm::vec3(ka.x(), ka.y(), ka.z());
    // _diffuseColor = glm::vec3(kd.x(), kd.y(), kd.z());
    // _specularColor = glm::vec3(ks.x(), ks.y(), ks.z());
    matBuilder.specularExponent(mesh.material.Ns);
    _illumination = mesh.material.illum;

    TextureSamplerArgs textureSamplerArgs;
    textureSamplerArgs.magFilter() = ETexture::Filter::Linear;
    textureSamplerArgs.minFilter() = ETexture::Filter::Linear;
    textureSamplerArgs.mipFilter() = ETexture::Filter::Linear;
    textureSamplerArgs.wrapU = ETexture::WrapMode::Repeat;
    textureSamplerArgs.wrapV = ETexture::WrapMode::Repeat;
    textureSamplerArgs.wrapW = ETexture::WrapMode::Repeat;
    textureSamplerArgs.depthCompareFunc = ETexture::DepthCompareFunc::Never;

    matBuilder.textureSampler(CPPRef<ITextureSampler>(context.createTextureSampler().buildCPPRef(textureSamplerArgs, null)));

    _material = matBuilder.build();
}

void RenderableObject::preRender(IRenderingContext& context) const noexcept
{
    context.setFaceWinding(false);
    _va->bind(context);
    _va->preDraw(context);
}

void RenderableObject::render(IRenderingContext& context) const noexcept
{
    // _va->drawIndexed(context);
    _va->draw(context);
}

void RenderableObject::postRender(IRenderingContext& context) const noexcept
{
    _va->postDraw(context);
    _va->unbind(context);
    context.setFaceWinding(true);
}
