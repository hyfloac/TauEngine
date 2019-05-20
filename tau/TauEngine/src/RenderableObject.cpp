#include <model/RenderableObject.hpp>
#include <DynArray.hpp>
#include <NumTypes.hpp>
#include <Utils.hpp>

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
    : _vertices(BufferType::ArrayBuffer, BufferType::ArrayBuffer, BufferType::ArrayBuffer, BufferType::ElementArrayBuffer)
{
    _vao.bind();

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
    VertexArray::setAttribute(0, 3, DataType::Float, GL_FALSE, 0, null);
    _vertices.normals.fillBuffer(static_cast<GLsizei>(mesh.vertices.size()), mesh.vertices.size() * 3 * sizeof(float), normalsLoaded);
    VertexArray::setAttribute(1, 3, DataType::Float, GL_FALSE, 0, null);
    _vertices.textures.fillBuffer(static_cast<GLsizei>(mesh.vertices.size()), mesh.vertices.size() * 2 * sizeof(float), texturesLoaded);
    VertexArray::setAttribute(2, 2, DataType::Float, GL_FALSE, 0, null);
    _vertices.indices.fillBuffer(static_cast<GLsizei>(mesh.indices.size()), mesh.indices.size() * sizeof(u32), mesh.indices.data());

    VertexArray::unbind();
}

RenderableObject::~RenderableObject() noexcept
{
    VertexArray::unbind();
}

void RenderableObject::preRender() const noexcept
{
    glFrontFace(GL_CW);
    _vao.bind();
    VertexArray::enableAttribute(0);
    VertexArray::enableAttribute(1);
    VertexArray::enableAttribute(2);
}

void RenderableObject::render() const noexcept
{
    _vertices.indices.drawIndexed();
}

void RenderableObject::postRender() noexcept
{
    VertexArray::disableAttribute(2);
    VertexArray::disableAttribute(1);
    VertexArray::disableAttribute(0);
    VertexArray::unbind();
    glFrontFace(GL_CCW);
}

void RenderableObject::preRender(RenderingPipeline& rp) const noexcept
{
    rp.pushInstruction(RenderingOpcode::GL_FACE_WINDING, ParameterPack(GL_CW));
    rp.pushInstruction(RenderingOpcode::BIND_VAO, ParameterPack(_vao.array()));
    rp.pushInstruction(RenderingOpcode::ENABLE_VAO_ATTRIBUTE, ParameterPack(0));
    rp.pushInstruction(RenderingOpcode::ENABLE_VAO_ATTRIBUTE, ParameterPack(1));
    rp.pushInstruction(RenderingOpcode::ENABLE_VAO_ATTRIBUTE, ParameterPack(2));
}

void RenderableObject::render(RenderingPipeline& rp) const noexcept
{
    _vertices.indices.drawIndexed(rp);
}

void RenderableObject::postRender(RenderingPipeline& rp) noexcept
{
    rp.pushInstruction(RenderingOpcode::DISABLE_VAO_ATTRIBUTE, ParameterPack(2));
    rp.pushInstruction(RenderingOpcode::DISABLE_VAO_ATTRIBUTE, ParameterPack(1));
    rp.pushInstruction(RenderingOpcode::DISABLE_VAO_ATTRIBUTE, ParameterPack(0));
    rp.pushInstruction(RenderingOpcode::BIND_VAO, ParameterPack(0));
    rp.pushInstruction(RenderingOpcode::GL_FACE_WINDING, ParameterPack(GL_CCW));
}
