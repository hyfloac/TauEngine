#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>
#include "DLL.hpp"

#include "Color.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class IGraphicsInterface;
class ICommandList;
class IResource;

class TAU_DLL BatchRenderer final
{
    DELETE_CONSTRUCT(BatchRenderer);
    DELETE_DESTRUCT(BatchRenderer);
    DELETE_CM(BatchRenderer);
public:
    static void init(IGraphicsInterface& gi, ICommandList& cmdList) noexcept;
    static void flush() noexcept;

    static void beginBatch(NullableRef<ICommandList> cmdList);
    static void endBatch();

    static void drawEquilateralTriangle(glm::vec2 centerPosition, float radius, RGBAColor color) noexcept;
    static void drawEquilateralTriangle(glm::vec3 centerPosition, float radius, RGBAColor color) noexcept;
    static void drawEquilateralTriangle(glm::vec2 centerPosition, float radius, float rotation, RGBAColor color) noexcept;
    static void drawEquilateralTriangle(glm::vec3 centerPosition, float radius, float rotation, RGBAColor color) noexcept;

    static void drawQuad(const glm::mat4& transform, RGBAColor color) noexcept;
    static void drawQuad(const glm::mat4& transform, NullableRef<IResource>& texture, RGBAColor tint = { 255, 255, 255, 255 }) noexcept;

    static void drawQuad(glm::vec2 position, glm::vec2 size, RGBAColor color) noexcept;
    static void drawQuad(glm::vec3 position, glm::vec2 size, RGBAColor color) noexcept;
    static void drawQuad(glm::vec2 position, glm::vec2 size, const NullableRef<IResource>& texture, RGBAColor tint = { 255, 255, 255, 255 }) noexcept;
    static void drawQuad(glm::vec3 position, glm::vec2 size, const NullableRef<IResource>& texture, RGBAColor tint = { 255, 255, 255, 255 }) noexcept;

    static void drawQuad(glm::vec2 position, glm::vec2 size, float rotation, RGBAColor color) noexcept;
    static void drawQuad(glm::vec3 position, glm::vec2 size, float rotation, RGBAColor color) noexcept;
    static void drawQuad(glm::vec2 position, glm::vec2 size, float rotation, const NullableRef<IResource>& texture, RGBAColor tint = { 255, 255, 255, 255 }) noexcept;
    static void drawQuad(glm::vec3 position, glm::vec2 size, float rotation, const NullableRef<IResource>& texture, RGBAColor tint = { 255, 255, 255, 255 }) noexcept;

    static void drawCircle(glm::vec2 centerPosition, float radius, uSys outerVertexCount, RGBAColor color) noexcept;
    static void drawCircle(glm::vec3 centerPosition, float radius, uSys outerVertexCount, RGBAColor color) noexcept;
    static void drawCircle(glm::vec2 centerPosition, float radius, uSys outerVertexCount, const NullableRef<IResource>& texture, RGBAColor color = { 255, 255, 255, 255 }) noexcept;
    static void drawCircle(glm::vec3 centerPosition, float radius, uSys outerVertexCount, const NullableRef<IResource>& texture, RGBAColor color = { 255, 255, 255, 255 }) noexcept;
};
