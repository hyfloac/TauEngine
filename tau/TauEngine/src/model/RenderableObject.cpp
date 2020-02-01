#include <model/RenderableObject.hpp>
#include <DynArray.hpp>
#include <NumTypes.hpp>
#include "system/RenderingContext.hpp"
#include "model/VertexArray.hpp"
#include "Timings.hpp"
#include "texture/FITextureLoader.hpp"
#include "VFS.hpp"

// static Ref<IInputLayout> _inputLayoutCache = null;

RenderableObject::RenderableObject(IRenderingContext& context, const objl::Mesh& mesh, const char* materialFolder, const DrawType drawType) noexcept
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

    Ref<IBufferBuilder> pnBuilder = context.createBuffer(1);
    Ref<IBufferBuilder> texturesBuilder = context.createBuffer(1);
    Ref<IIndexBufferBuilder> indicesBuilder = context.createIndexBuffer();

    pnBuilder->type(EBuffer::Type::ArrayBuffer);
    pnBuilder->usage(EBuffer::UsageType::StaticDraw);
    // pnBuilder->bufferSize(cnt3 * sizeof(float));
    pnBuilder->elementCount(cnt1);
    pnBuilder->descriptor().addDescriptor(ShaderSemantic::Position, ShaderDataType::Vector3Float);

    texturesBuilder->type(EBuffer::Type::ArrayBuffer);
    texturesBuilder->usage(EBuffer::UsageType::StaticDraw);
    // texturesBuilder->bufferSize(cnt2 * sizeof(float));
    texturesBuilder->elementCount(cnt1);
    texturesBuilder->descriptor().addDescriptor(ShaderSemantic::TextureCoord, ShaderDataType::Vector2Float);

    indicesBuilder->usage(EBuffer::UsageType::StaticDraw);
    // indicesBuilder->bufferSize(mesh.indices.size() * sizeof(u32));
    indicesBuilder->elementCount(mesh.indices.size());

    Ref<IBuffer> positions = Ref<IBuffer>(pnBuilder->build(nullptr));

    pnBuilder->descriptor().reset(1);
    pnBuilder->descriptor().addDescriptor(ShaderSemantic::Normal, ShaderDataType::Vector3Float);
    Ref<IBuffer> normals = Ref<IBuffer>(pnBuilder->build(nullptr));

    pnBuilder->descriptor().reset(1);
    pnBuilder->descriptor().addDescriptor(ShaderSemantic::Tangent, ShaderDataType::Vector3Float);
    Ref<IBuffer> tangents = Ref<IBuffer>(pnBuilder->build(nullptr));
    // Ref<IBuffer> bitangents = Ref<IBuffer>(pnBuilder->build(nullptr));
    Ref<IBuffer> textures = Ref<IBuffer>(texturesBuilder->build(nullptr));
    Ref<IIndexBuffer> indices = Ref<IIndexBuffer>(indicesBuilder->build(nullptr));

    positions->bind(context);
    positions->fillBuffer(context, positionsLoaded);
    positions->unbind(context);

    normals->bind(context);
    normals->fillBuffer(context, normalsLoaded);
    normals->unbind(context);

    tangents->bind(context);
    tangents->fillBuffer(context, tangentsLoaded);
    tangents->unbind(context);

    // bitangents->bind(context);
    // bitangents->fillBuffer(context, bitangentsLoaded);
    // bitangents->unbind(context);

    textures->bind(context);
    textures->fillBuffer(context, texturesLoaded);
    textures->unbind(context);

    indices->bind(context);
    indices->fillBuffer(context, mesh.indices.data());
    indices->unbind(context);

    Ref<IVertexArrayBuilder> vaBuilder = context.createVertexArray(4);

    // if(!_inputLayoutCache)
    // {
        // Ref<IInputLayoutBuilder> ilBuilder = context.createInputLayout(4);
        // ilBuilder->setLayoutDescriptor(0, ShaderDataType::Vector3Float, ShaderSemantic::Position);
        // ilBuilder->setLayoutDescriptor(1, ShaderDataType::Vector3Float, ShaderSemantic::Normal);
        // ilBuilder->setLayoutDescriptor(2, ShaderDataType::Vector3Float, ShaderSemantic::Tangent);
        // ilBuilder->setLayoutDescriptor(3, ShaderDataType::Vector2Float, ShaderSemantic::TextureCoord);
        // _inputLayoutCache = Ref<IInputLayout>(ilBuilder->build());
    // }

    vaBuilder->setVertexBuffer(0, positions);
    vaBuilder->setVertexBuffer(1, normals);
    vaBuilder->setVertexBuffer(2, tangents);
    vaBuilder->setVertexBuffer(3, textures);
    vaBuilder->indexBuffer(indices);
    // vaBuilder->inputLayout(_inputLayoutCache);
    vaBuilder->drawCount(mesh.indices.size());
    vaBuilder->drawType(DrawType::SeparatedTriangles);

    _va = Ref<IVertexArray>(vaBuilder->build());

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
        matBuilder.diffuseTexture(Ref<ITexture>(TextureLoader::loadTexture(context, path.path)));
    }
    if(!mesh.material.map_Ks.empty())
    {
        const auto path = VFS::Instance().resolvePath(materialFolder, mesh.material.map_Ks.c_str());
        matBuilder.specularTexture(Ref<ITexture>(TextureLoader::loadTexture(context, path.path)));
    }
    else
    {
        matBuilder.specularTexture(Ref<ITexture>(TextureLoader::generateBlackTexture(context)));
    }
    if(!mesh.material.map_Ka.empty())
    {
        const auto path = VFS::Instance().resolvePath(materialFolder, mesh.material.map_Ka.c_str());
        _reflectiveTexture = Ref<ITexture>(TextureLoader::loadTexture(context, path.path));
    }
    if(!mesh.material.map_bump.empty())
    {
        const auto path = VFS::Instance().resolvePath(materialFolder, mesh.material.map_bump.c_str());
        matBuilder.normalTexture(Ref<ITexture>(TextureLoader::loadTexture(context, path.path)));
    }
    else
    {
        matBuilder.normalTexture(Ref<ITexture>(TextureLoader::generateNormalTexture(context)));
    }
    // const auto ka = mesh.material.Ka;
    // const auto kd = mesh.material.Kd;
    // const auto ks = mesh.material.Ks;
    // _ambientColor = glm::vec3(ka.x(), ka.y(), ka.z());
    // _diffuseColor = glm::vec3(kd.x(), kd.y(), kd.z());
    // _specularColor = glm::vec3(ks.x(), ks.y(), ks.z());
    matBuilder.specularExponent(mesh.material.Ns);
    _illumination = mesh.material.illum;

    Ref<ITextureSamplerBuilder> textureSamplerBuilder = context.createTextureSampler();
    textureSamplerBuilder->setFilterMode(ETexture::Filter::Linear,
                                         ETexture::Filter::Linear,
                                         ETexture::Filter::Linear);
    textureSamplerBuilder->setWrapMode(ETexture::WrapMode::Repeat,
                                       ETexture::WrapMode::Repeat,
                                       ETexture::WrapMode::Repeat);
    textureSamplerBuilder->setDepthComparison(ETexture::DepthCompareFunc::Never);

    matBuilder.textureSampler(Ref<ITextureSampler>(textureSamplerBuilder->build()));

    _material = matBuilder.build();
}

void RenderableObject::preRender(IRenderingContext& context) const noexcept
{
    glFrontFace(GL_CCW);
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
    glFrontFace(GL_CW);
}
