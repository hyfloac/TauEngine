#pragma once

#include <vector>
#include <model/RenderableObject.hpp>

class TAU_DLL MultiSegmentRenderableObject final
{
private:
    std::vector<RenderableObject> _objects;
public:
    MultiSegmentRenderableObject(std::vector<objl::Mesh> meshes) noexcept;
    MultiSegmentRenderableObject(const MultiSegmentRenderableObject& copy) noexcept = default;
    MultiSegmentRenderableObject(MultiSegmentRenderableObject&& move) noexcept = default;

    ~MultiSegmentRenderableObject() noexcept;

    MultiSegmentRenderableObject& operator =(const MultiSegmentRenderableObject& copy) noexcept = default;
    MultiSegmentRenderableObject& operator =(MultiSegmentRenderableObject&& move) noexcept = default;

    void preRender() const noexcept;

    void render() const noexcept;

    void postRender() noexcept;
};
