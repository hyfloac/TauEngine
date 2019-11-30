#pragma once

#include <model/BufferDescriptor.hpp>
#include <model/OBJLoader.hpp>
#include <RenderingPipeline.hpp>
#include <DLL.hpp>
#include <Objects.hpp>
#include "IVertexArray.hpp"

class IRenderingContext;

class TAU_DLL RenderableObject final
{
    DEFAULT_DESTRUCT(RenderableObject);
    DEFAULT_COPY(RenderableObject);
private:
    Ref<IVertexArray> _va;
public:
    RenderableObject(IRenderingContext& context, const objl::Mesh& mesh, DrawType drawType = DrawType::SeparatedTriangles) noexcept;

    void preRender(IRenderingContext& context) const noexcept;
    void render(IRenderingContext& context) const noexcept;
    void postRender(IRenderingContext& context) const noexcept;

    [[nodiscard]] Ref<IVertexArray> vertexArray() noexcept { return _va; }

    [[nodiscard]] inline size_t hashCode() const noexcept { return reinterpret_cast<size_t>(_va.get()); }

    inline bool operator ==(const RenderableObject& other) const noexcept { return _va.get() == other._va.get(); }
    inline bool operator !=(const RenderableObject& other) const noexcept { return !(*this == other); }
};

namespace std
{
    template<>
    struct hash<RenderableObject>
    {
        inline size_t operator()(const RenderableObject& obj) const noexcept
        { return static_cast<size_t>(obj.hashCode()); }
    };
}
