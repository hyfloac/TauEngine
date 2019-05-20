#pragma once

#include <DLL.hpp>
#include <maths/Matrix4x4f.hpp>
#include <maths/Vector3f.hpp>
#include <model/RenderableObject.hpp>


class TAU_DLL WorldObject final
{
private:
    const RenderableObject* _renderableObject;
    Vector3f   _position;
    Vector3f   _rotation;
    Vector3f   _scale;
    Matrix4x4f _objViewMatrix;
public:
    WorldObject(const RenderableObject* RESTRICT renderableObject) noexcept;
    WorldObject(const RenderableObject* RESTRICT renderableObject, const Vector3f& RESTRICT position) noexcept;
    WorldObject(const RenderableObject* RESTRICT renderableObject, const Vector3f& RESTRICT position, const Vector3f& RESTRICT rotation) noexcept;
    WorldObject(const RenderableObject* RESTRICT renderableObject, const Vector3f& RESTRICT position, const Vector3f& RESTRICT rotation, const Vector3f& RESTRICT scale) noexcept;
    WorldObject(const RenderableObject* RESTRICT renderableObject, const Vector3f& RESTRICT position, const Vector3f& RESTRICT rotation, const float scale) noexcept;

    WorldObject(const WorldObject& copy) noexcept = default;
    WorldObject(WorldObject&& move)      noexcept = default;

    ~WorldObject() = default;

    WorldObject& operator =(const WorldObject& copy) noexcept = default;
    WorldObject& operator =(WorldObject&& move)      noexcept = default;

    const RenderableObject* renderableObject() const noexcept { return _renderableObject; }
    const Vector3f&         position()         const noexcept { return _position;         }
    const Vector3f&         rotation()         const noexcept { return _rotation;         }
    const Vector3f&         scale()            const noexcept { return _scale;            }
    const Matrix4x4f&       viewMatrix()       const noexcept { return _objViewMatrix;    }

    WorldObject& setPosition(const Vector3f& position) noexcept;
    WorldObject& setRotation(const Vector3f& rotation) noexcept;
    WorldObject& setScale(const Vector3f& scale)       noexcept;
    WorldObject& setScale(float scale)                 noexcept;

    WorldObject& addPosition(const Vector3f& position) noexcept;
    WorldObject& addRotation(const Vector3f& rotation) noexcept;
    WorldObject& addScale(const Vector3f& scale)       noexcept;
    WorldObject& addScale(float scale)                 noexcept;

    WorldObject& subPosition(const Vector3f& position) noexcept;
    WorldObject& subRotation(const Vector3f& rotation) noexcept;
    WorldObject& subScale(const Vector3f& scale)       noexcept;
    WorldObject& subScale(float scale)                 noexcept;

    WorldObject& crossPosition(const Vector3f& position) noexcept;
    WorldObject& crossRotation(const Vector3f& rotation) noexcept;
    WorldObject& crossScale(const Vector3f& scale)       noexcept;

    WorldObject& compMulPosition(const Vector3f& position) noexcept;
    WorldObject& compMulRotation(const Vector3f& rotation) noexcept;
    WorldObject& compMulScale(const Vector3f& scale)       noexcept;

    WorldObject& compDivPosition(const Vector3f& position) noexcept;
    WorldObject& compDivRotation(const Vector3f& rotation) noexcept;
    WorldObject& compDivScale(const Vector3f& scale)       noexcept;
    WorldObject& compDivScale(float scale)                 noexcept;


    WorldObject& setPositionPipelined(const Vector3f& position) noexcept;
    WorldObject& setRotationPipelined(const Vector3f& rotation) noexcept;
    WorldObject& setScalePipelined(const Vector3f& scale)       noexcept;
    WorldObject& setScalePipelined(float scale)                 noexcept;

    WorldObject& addPositionPipelined(const Vector3f& position) noexcept;
    WorldObject& addRotationPipelined(const Vector3f& rotation) noexcept;
    WorldObject& addScalePipelined(const Vector3f& scale)       noexcept;
    WorldObject& addScalePipelined(float scale)                 noexcept;

    WorldObject& subPositionPipelined(const Vector3f& position) noexcept;
    WorldObject& subRotationPipelined(const Vector3f& rotation) noexcept;
    WorldObject& subScalePipelined(const Vector3f& scale)       noexcept;
    WorldObject& subScalePipelined(float scale)                 noexcept;

    WorldObject& crossPositionPipelined(const Vector3f& position) noexcept;
    WorldObject& crossRotationPipelined(const Vector3f& rotation) noexcept;
    WorldObject& crossScalePipelined(const Vector3f& scale)       noexcept;

    WorldObject& compMulPositionPipelined(const Vector3f& position) noexcept;
    WorldObject& compMulRotationPipelined(const Vector3f& rotation) noexcept;
    WorldObject& compMulScalePipelined(const Vector3f& scale)       noexcept;
    WorldObject& compMulScalePipelined(float scale)                 noexcept;

    WorldObject& compDivPositionPipelined(const Vector3f& position) noexcept;
    WorldObject& compDivRotationPipelined(const Vector3f& rotation) noexcept;
    WorldObject& compDivScalePipelined(const Vector3f& scale)       noexcept;
    WorldObject& compDivScalePipelined(float scale)                 noexcept;

    WorldObject& buildMatrixPipelined()       noexcept;
    WorldObject& buildPosRotMatrixPipelined() noexcept;
    WorldObject& buildScaleMatrixPipelined()  noexcept;
};
