#pragma once

#include <TauEngine.hpp>
#include <model/BufferDescriptor.hpp>
#include <model/VertexBuffer.hpp>
#include <model/OBJLoader.hpp>
#include <RenderingPipeline.hpp>
#include <DLL.hpp>
#include <DynArray.hpp>
#include <Objects.hpp>

// struct VerticeSet final
// {
//     VertexBufferShared positions;
//     VertexBufferShared normals;
//     VertexBufferShared textures;
//     VertexBufferShared indices;
//
//     VerticeSet(const VertexBufferShared& _positions, 
//                const VertexBufferShared& _normals, 
//                const VertexBufferShared& _textures, 
//                const VertexBufferShared& _indices) noexcept;
//
//     VerticeSet(VertexBufferShared&& _positions,
//                VertexBufferShared&& _normals,
//                VertexBufferShared&& _textures,
//                VertexBufferShared&& _indices) noexcept;
//
//     VerticeSet(const VerticeSet& copy) noexcept = default;
//     VerticeSet(VerticeSet&& move) noexcept = default;
//
//     ~VerticeSet() noexcept = default;
//
//     VerticeSet& operator =(const VerticeSet& copy) noexcept = default;
//     VerticeSet& operator =(VerticeSet&& move) noexcept = default;
// };

class IRenderingContext;

class TAU_DLL RenderableObject final
{
    DEFAULT_DESTRUCT(RenderableObject);
    DEFAULT_COPY(RenderableObject);
private:
    // VertexArrayShared _vao;
    Ref<IBufferDescriptor> _bufferDescriptor;
    // VerticeSet _vertices;
    RefDynArray<Ref<IVertexBuffer>> _buffers;

public:
    RenderableObject(IRenderingContext& context, const objl::Mesh& mesh) noexcept;

    void preRender(IRenderingContext& context) const noexcept;
    void render(IRenderingContext& context) const noexcept;
    void postRender(IRenderingContext& context) const noexcept;
    
    // void preRender(RenderingPipeline& rp) const noexcept;
    // void render(RenderingPipeline& rp) const noexcept;
    // void postRender(RenderingPipeline& rp) const noexcept;

    inline size_t hashCode() const noexcept { return reinterpret_cast<size_t>(_bufferDescriptor.get()); }
    // inline GLuint getVAO() const noexcept { return _vao.array(); }
    // inline const VerticeSet& getVerticeSet() const noexcept { return _vertices; }

    inline bool operator ==(const RenderableObject& other) const noexcept { return _bufferDescriptor.get() == other._bufferDescriptor.get(); }
    inline bool operator !=(const RenderableObject& other) const noexcept { return !(*this == other); }
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
