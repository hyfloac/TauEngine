#include <model/RenderableObject.hpp>
#include <DynArray.hpp>
#include <NumTypes.hpp>
#include <Utils.hpp>
#include <RenderingMode.hpp>

VerticeSet::VerticeSet(const VertexBufferShared& _positions,
                       const VertexBufferShared& _normals,
                       const VertexBufferShared& _textures,
                       const VertexBufferShared& _indices) noexcept
    : positions(_positions),
      normals(_normals),
      textures(_textures),
      indices(_indices)
{ }

VerticeSet::VerticeSet(VertexBufferShared&& _positions,
                       VertexBufferShared&& _normals,
                       VertexBufferShared&& _textures,
                       VertexBufferShared&& _indices) noexcept
    : positions(_positions),
      normals(_normals),
      textures(_textures),
      indices(_indices)
{ }

RenderableObject::RenderableObject(objl::Mesh mesh) noexcept
    : _vao(IBufferDescriptor::create(RenderingMode::getGlobalMode())), _vertices(BufferType::ArrayBuffer, BufferType::ArrayBuffer, BufferType::ArrayBuffer, BufferType::ElementArrayBuffer)
{
    _vao->bind();

    DynArray<float> positionsLoaded(mesh.vertices.size() * 3);
    DynArray<float> normalsLoaded(mesh.vertices.size() * 3);
    DynArray<float> texturesLoaded(mesh.vertices.size() * 2);

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

    _vertices.positions.fillBuffer(static_cast<GLsizei>(mesh.vertices.size()), mesh.vertices.size() * 3 * sizeof(float), positionsLoaded);
    _vao->addAttribute(3, DataType::Float, false, 0, null);
    _vertices.normals.fillBuffer(static_cast<GLsizei>(mesh.vertices.size()), mesh.vertices.size() * 3 * sizeof(float), normalsLoaded);
    _vao->addAttribute(3, DataType::Float, false, 0, null);
    _vertices.textures.fillBuffer(static_cast<GLsizei>(mesh.vertices.size()), mesh.vertices.size() * 2 * sizeof(float), texturesLoaded);
    _vao->addAttribute(2, DataType::Float, false, 0, null);
    _vertices.indices.fillBuffer(static_cast<GLsizei>(mesh.indices.size()), mesh.indices.size() * sizeof(u32), mesh.indices.data());

    _vao->unbind();
}

RenderableObject::~RenderableObject() noexcept
{
    _vao->unbind();
    // VertexArray::unbind();
}

void RenderableObject::preRender() const noexcept
{
    glFrontFace(GL_CW);
    _vao->bind();
    _vao->enableAttributes();
}

void RenderableObject::render() const noexcept
{
    _vertices.indices.drawIndexed();
}

void RenderableObject::postRender() const noexcept
{
    _vao->disableAttributes();
    _vao->unbind();
    glFrontFace(GL_CCW);
}

void RenderableObject::preRender(RenderingPipeline& rp) const noexcept
{
    rp.pushGLFaceWinding(GL_CW);
    rp.pushEnableBufferDescriptor(_vao.get());
}

void RenderableObject::render(RenderingPipeline& rp) const noexcept
{
    _vertices.indices.drawIndexed(rp);
}

void RenderableObject::postRender(RenderingPipeline& rp) const noexcept
{
    rp.pushDisableBufferDescriptor(_vao.get());
    rp.pushGLFaceWinding(GL_CCW);
}
