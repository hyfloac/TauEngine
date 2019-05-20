#pragma once

#pragma warning(push, 0)
#include <unordered_map>
#pragma warning(pop)

#include <model/VertexArray.hpp>
#include <model/VertexBuffer.hpp>
#include <model/OBJLoader.hpp>
#include <RenderingPipeline.hpp>
#include <DLL.hpp>

struct VerticeSet final
{
    VertexBufferShared positions;
    VertexBufferShared normals;
    VertexBufferShared textures;
    VertexBufferShared indices;

    VerticeSet(const VertexBufferShared& _positions, 
               const VertexBufferShared& _normals, 
               const VertexBufferShared& _textures, 
               const VertexBufferShared& _indices) noexcept;

    VerticeSet(VertexBufferShared&& _positions,
               VertexBufferShared&& _normals,
               VertexBufferShared&& _textures,
               VertexBufferShared&& _indices) noexcept;

    VerticeSet(const VerticeSet& copy) noexcept = default;
    VerticeSet(VerticeSet&& move) noexcept = default;

    ~VerticeSet() noexcept = default;

    VerticeSet& operator =(const VerticeSet& copy) noexcept = default;
    VerticeSet& operator =(VerticeSet&& move) noexcept = default;
};

class TAU_DLL RenderableObject final
{
private:
    VertexArrayShared _vao;
    VerticeSet  _vertices;
public:
    RenderableObject(objl::Mesh mesh) noexcept;
    RenderableObject(const RenderableObject& copy) noexcept = default;
    RenderableObject(RenderableObject&& move) noexcept = default;

    ~RenderableObject() noexcept;

    RenderableObject& operator =(const RenderableObject& copy) noexcept = default;
    RenderableObject& operator =(RenderableObject&& move) noexcept = default;

    void preRender() const noexcept;

    void render() const noexcept;

    static void postRender() noexcept;

    void preRender(RenderingPipeline& rp) const noexcept;

    void render(RenderingPipeline& rp) const noexcept;

    static void postRender(RenderingPipeline& rp) noexcept;

    inline size_t hashCode() const noexcept { return _vao.array(); }
    inline GLuint getVAO() const noexcept { return _vao.array(); }
    inline const VerticeSet& getVerticeSet() const noexcept { return _vertices; }

    inline bool operator ==(const RenderableObject& other) const noexcept { return _vao.array() == other._vao.array(); }
    inline bool operator !=(const RenderableObject& other) const noexcept { return _vao.array() != other._vao.array(); }
};

namespace std
{
    template<>
    struct hash<RenderableObject>
    {
        inline size_t operator()(const RenderableObject& obj) const noexcept
        {
            return static_cast<size_t>(obj.hashCode());
        }
    };
}
