#include <model/RenderableObject.hpp>
#include <DynArray.hpp>
#include <NumTypes.hpp>
#include <Utils.hpp>
#include "system/RenderingContext.hpp"

// VerticeSet::VerticeSet(const VertexBufferShared& _positions,
//                        const VertexBufferShared& _normals,
//                        const VertexBufferShared& _textures,
//                        const VertexBufferShared& _indices) noexcept
//     : positions(_positions),
//       normals(_normals),
//       textures(_textures),
//       indices(_indices)
// { }
//
// VerticeSet::VerticeSet(VertexBufferShared&& _positions,
//                        VertexBufferShared&& _normals,
//                        VertexBufferShared&& _textures,
//                        VertexBufferShared&& _indices) noexcept
//     : positions(_positions),
//       normals(_normals),
//       textures(_textures),
//       indices(_indices)
// { }

RenderableObject::RenderableObject(IRenderingContext& context, const objl::Mesh& mesh) noexcept
    : _bufferDescriptor(context.createBufferDescriptor(3)), _buffers(4)//, _vertices(BufferType::ArrayBuffer, BufferType::ArrayBuffer, BufferType::ArrayBuffer, BufferType::ElementArrayBuffer)
{
    // _vao->bind(null);

    const size_t cnt1 = mesh.vertices.size();
    const size_t cnt2 = cnt1 + cnt1;
    const size_t cnt3 = cnt2 + cnt1;

    DynArray<float> positionsLoaded(cnt3);
    DynArray<float> normalsLoaded(cnt3);
    DynArray<float> texturesLoaded(cnt2);

    u32 vec3Index = 0;
    u32 vec2Index = 0;

    for(objl::Vertex vertex : mesh.vertices)
    {
        normalsLoaded[vec3Index]     = vertex.normal.x();
        positionsLoaded[vec3Index++] = vertex.position.x();
        normalsLoaded[vec3Index]     = vertex.normal.y();
        positionsLoaded[vec3Index++] = vertex.position.y();
        normalsLoaded[vec3Index]     = vertex.normal.z();
        positionsLoaded[vec3Index++] = vertex.position.z();

        texturesLoaded[vec2Index++] = vertex.textureCoordinate.x();
        texturesLoaded[vec2Index++] = vertex.textureCoordinate.y();
    }

    _buffers[0] = IVertexBuffer::create(context, IVertexBuffer::Type::ArrayBuffer);
    _buffers[1] = IVertexBuffer::create(context, IVertexBuffer::Type::ArrayBuffer);
    _buffers[2] = IVertexBuffer::create(context, IVertexBuffer::Type::ArrayBuffer);
    _buffers[3] = IVertexBuffer::create(context, IVertexBuffer::Type::ElementArrayBuffer);

    _buffers[0]->bind(context);
    _buffers[0]->fillBuffer(context, cnt1, cnt3 * sizeof(float), positionsLoaded);
    _bufferDescriptor->addAttribute(_buffers[0], 3, DataType::Float, false, 0, null);
    _buffers[0]->unbind(context);

    _buffers[1]->bind(context);
    _buffers[1]->fillBuffer(context, cnt1, cnt3 * sizeof(float), normalsLoaded);
    _bufferDescriptor->addAttribute(_buffers[1], 3, DataType::Float, false, 0, null);
    _buffers[1]->unbind(context);

    _buffers[2]->bind(context);
    _buffers[2]->fillBuffer(context, cnt1, cnt2 * sizeof(float), texturesLoaded);
    _bufferDescriptor->addAttribute(_buffers[2], 3, DataType::Float, false, 0, null);
    _buffers[2]->unbind(context);

    _buffers[3]->bind(context);
    _buffers[3]->fillBuffer(context, mesh.indices.size(), mesh.indices.size() * sizeof(u32), mesh.indices.data());
    _buffers[3]->unbind(context);

    // _vertices.positions.fillBuffer(static_cast<GLsizei>(mesh.vertices.size()), mesh.vertices.size() * 3 * sizeof(float), positionsLoaded);
    // _vao->addAttribute(_vertices.positions.buffer(), 3, DataType::Float, false, 0, null);
    // _vertices.normals.fillBuffer(static_cast<GLsizei>(mesh.vertices.size()), mesh.vertices.size() * 3 * sizeof(float), normalsLoaded);
    // _vao->addAttribute(_vertices.normals.buffer(), 3, DataType::Float, false, 0, null);
    // _vertices.textures.fillBuffer(static_cast<GLsizei>(mesh.vertices.size()), mesh.vertices.size() * 2 * sizeof(float), texturesLoaded);
    // _vao->addAttribute(_vertices.textures.buffer(), 2, DataType::Float, false, 0, null);
    //
    // _vertices.indices.fillBuffer(static_cast<GLsizei>(mesh.indices.size()), mesh.indices.size() * sizeof(u32), mesh.indices.data());

    // _vao->unbind(null);
}

// RenderableObject::~RenderableObject() noexcept
// {
//     // _vao->unbind(null);
//     // VertexArray::unbind();
// }

void RenderableObject::preRender(IRenderingContext& context) const noexcept
{
    glFrontFace(GL_CW);
    _bufferDescriptor->bind(context);
    _bufferDescriptor->enableAttributes(context);
}

void RenderableObject::render(IRenderingContext& context) const noexcept
{
    _buffers[3]->drawIndexed(context);
}

void RenderableObject::postRender(IRenderingContext& context) const noexcept
{
    _bufferDescriptor->disableAttributes(context);
    _bufferDescriptor->unbind(context);
    glFrontFace(GL_CCW);
}

// void RenderableObject::preRender(RenderingPipeline& rp) const noexcept
// {
//     rp.pushGLFaceWinding(GL_CW);
//     rp.pushEnableBufferDescriptor(_vao.get());
// }
//
// void RenderableObject::render(RenderingPipeline& rp) const noexcept
// {
//     _vertices.indices.drawIndexed(rp);
// }
//
// void RenderableObject::postRender(RenderingPipeline& rp) const noexcept
// {
//     rp.pushDisableBufferDescriptor(_vao.get());
//     rp.pushGLFaceWinding(GL_CCW);
// }
