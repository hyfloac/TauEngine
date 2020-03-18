#pragma once

#include <model/BufferDescriptor.hpp>
#include <model/OBJLoader.hpp>
#include <DLL.hpp>
#include <Objects.hpp>
#include "VertexArray.hpp"
#include "model/Material.hpp"
#include <glm/vec4.hpp>

class IRenderingContext;

class TAU_DLL RenderableObject final
{
    DEFAULT_DESTRUCT(RenderableObject);
    DEFAULT_COPY(RenderableObject);
private:
    CPPRef<IVertexArray> _va;
    CPPRef<ITexture> _reflectiveTexture;
    i32 _illumination;
    Material _material;
public:
    RenderableObject(IRenderingContext& context, const objl::Mesh& mesh, const char* materialFolder, const CPPRef<IShader>& shader, DrawType drawType = DrawType::SeparatedTriangles) noexcept;

    void preRender(IRenderingContext& context) const noexcept;
    void render(IRenderingContext& context) const noexcept;
    void postRender(IRenderingContext& context) const noexcept;

    [[nodiscard]] CPPRef<IVertexArray> vertexArray() const noexcept { return _va; }
    [[nodiscard]] CPPRef<ITexture> reflectiveTexture() const noexcept { return _reflectiveTexture; }
    [[nodiscard]] const Material& material() const noexcept { return _material; }
    [[nodiscard]] i32 illumination() const noexcept { return _illumination; }

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
