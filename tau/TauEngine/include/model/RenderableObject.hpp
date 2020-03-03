#pragma once

#include <model/BufferDescriptor.hpp>
#include <model/OBJLoader.hpp>
#include <RenderingPipeline.hpp>
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
    // CPPRef<ITexture> _diffuseTexture;
    // CPPRef<ITexture> _specularTexture;
    CPPRef<ITexture> _reflectiveTexture;
    // CPPRef<ITexture> _normalTexture;
    // glm::vec3 _ambientColor;
    // glm::vec3 _diffuseColor;
    // glm::vec3 _specularColor;
    // float _specularExponent;
    i32 _illumination;
    Material _material;
public:
    RenderableObject(IRenderingContext& context, const objl::Mesh& mesh, const char* materialFolder, const CPPRef<IShader>& shader, DrawType drawType = DrawType::SeparatedTriangles) noexcept;

    void preRender(IRenderingContext& context) const noexcept;
    void render(IRenderingContext& context) const noexcept;
    void postRender(IRenderingContext& context) const noexcept;

    [[nodiscard]] CPPRef<IVertexArray> vertexArray() noexcept { return _va; }
    // [[nodiscard]] CPPRef<ITexture> diffuseTexture() const noexcept { return _diffuseTexture; }
    // [[nodiscard]] CPPRef<ITexture> specularTexture() const noexcept { return _specularTexture; }
    [[nodiscard]] CPPRef<ITexture> reflectiveTexture() const noexcept { return _reflectiveTexture; }
    // [[nodiscard]] CPPRef<ITexture> normalTexture() const noexcept { return _normalTexture; }
    // [[nodiscard]] glm::vec3 ambientColor() const noexcept { return _ambientColor; }
    // [[nodiscard]] glm::vec3 diffuseColor() const noexcept { return _diffuseColor; }
    // [[nodiscard]] glm::vec3 specularColor() const noexcept { return _specularColor; }
    // [[nodiscard]] float specularExponent() const noexcept { return _specularExponent; }
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
