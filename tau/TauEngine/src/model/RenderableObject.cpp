#include <model/RenderableObject.hpp>
#include <DynArray.hpp>
#include <NumTypes.hpp>
#include "system/RenderingContext.hpp"
#include "model/IVertexArray.hpp"
#include "Timings.hpp"


RenderableObject::RenderableObject(IRenderingContext& context, const objl::Mesh& mesh) noexcept
    : _va(context.createVertexArray(3))
{
    PERF();
    const size_t cnt1 = mesh.vertices.size();
    const size_t cnt2 = cnt1 + cnt1;
    const size_t cnt3 = cnt2 + cnt1;

    DynArray<float> positionsLoaded(cnt3);
    DynArray<float> normalsLoaded(cnt3);
    DynArray<float> texturesLoaded(cnt2);

    u32 posIndex = 0;
    u32 normIndex = 0;
    u32 texIndex = 0;

    for(objl::Vertex vertex : mesh.vertices)
    {
        positionsLoaded[posIndex++] = vertex.position.x();
        positionsLoaded[posIndex++] = vertex.position.y();
        positionsLoaded[posIndex++] = vertex.position.z();

        normalsLoaded[normIndex++] = vertex.normal.x();
        normalsLoaded[normIndex++] = vertex.normal.y();
        normalsLoaded[normIndex++] = vertex.normal.z();

        texturesLoaded[texIndex++] = vertex.textureCoordinate.x();
        texturesLoaded[texIndex++] = vertex.textureCoordinate.y();
    }

    Ref<IBuffer> positions = IBuffer::create(context, 1, IBuffer::Type::ArrayBuffer);
    Ref<IBuffer> normals = IBuffer::create(context, 1, IBuffer::Type::ArrayBuffer);
    Ref<IBuffer> textures = IBuffer::create(context, 1, IBuffer::Type::ArrayBuffer);
    Ref<IIndexBuffer> indices = IIndexBuffer::create(context);

    positions->bind(context);
    positions->fillBuffer(context, cnt3 * sizeof(float), positionsLoaded);
    positions->descriptor().addDescriptor(ShaderDataType::Vector3Float);
    positions->unbind(context);

    normals->bind(context);
    normals->fillBuffer(context, cnt3 * sizeof(float), normalsLoaded);
    normals->descriptor().addDescriptor(ShaderDataType::Vector3Float);
    normals->unbind(context);

    textures->bind(context);
    textures->fillBuffer(context, cnt2 * sizeof(float), texturesLoaded);
    textures->descriptor().addDescriptor(ShaderDataType::Vector2Float);
    textures->unbind(context);

    indices->bind(context);
    indices->fillBuffer(context, mesh.indices.size() * sizeof(u32), mesh.indices.data());
    indices->unbind(context);

    _va->addVertexBuffer(context, positions);
    _va->addVertexBuffer(context, normals);
    _va->addVertexBuffer(context, textures);
    _va->setIndexBuffer(context, indices);
    _va->drawCount() = mesh.indices.size();
}

void RenderableObject::preRender(IRenderingContext& context) const noexcept
{
    glFrontFace(GL_CCW);
    _va->bind(context);
    _va->preDraw(context);
}

void RenderableObject::render(IRenderingContext& context) const noexcept
{
    _va->drawIndexed(context);
}

void RenderableObject::postRender(IRenderingContext& context) const noexcept
{
    _va->postDraw(context);
    _va->unbind(context);
    glFrontFace(GL_CW);
}
