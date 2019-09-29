#pragma once

#include <maths/Matrix4x4f.hpp>
#include <maths/Vector3f.hpp>
#include <model/RenderableObject.hpp>

class WorldObject final
{
private:
    const RenderableObject* _renderableObject;
    Vector3f   _position;
    Vector3f   _rotation;
    Vector3f   _scale;
    Matrix4x4f _objViewMatrix;
public:
    WorldObject(const RenderableObject* RESTRICT renderableObject) noexcept
        : _renderableObject(renderableObject), _position(0.0f), _rotation(0.0f), _scale(1.0f)
    { }

    WorldObject(const RenderableObject* RESTRICT renderableObject, const Vector3f& RESTRICT position) noexcept
        : _renderableObject(renderableObject), _position(position), _rotation(0.0f), _scale(1.0f)
    { }

    WorldObject(const RenderableObject* RESTRICT renderableObject, const Vector3f& RESTRICT position, const Vector3f& RESTRICT rotation) noexcept
        : _renderableObject(renderableObject), _position(position), _rotation(rotation), _scale(1.0f)
    { }

    WorldObject(const RenderableObject* RESTRICT renderableObject, const Vector3f& RESTRICT position, const Vector3f& RESTRICT rotation, const Vector3f& RESTRICT scale) noexcept
        : _renderableObject(renderableObject), _position(position), _rotation(rotation), _scale(scale)
    { }

    WorldObject(const RenderableObject* RESTRICT renderableObject, const Vector3f& RESTRICT position, const Vector3f& RESTRICT rotation, const float scale) noexcept
        : _renderableObject(renderableObject), _position(position), _rotation(rotation), _scale(scale)
    { }

    ~WorldObject() = default;

    WorldObject(const WorldObject& copy) noexcept = default;
    WorldObject(WorldObject&& move)      noexcept = default;

    WorldObject& operator =(const WorldObject& copy) noexcept = default;
    WorldObject& operator =(WorldObject&& move)      noexcept = default;

    const RenderableObject* renderableObject() const noexcept { return _renderableObject; }
    const Vector3f&         position()         const noexcept { return _position;         }
    const Vector3f&         rotation()         const noexcept { return _rotation;         }
    const Vector3f&         scale()            const noexcept { return _scale;            }
    const Matrix4x4f&       viewMatrix()       const noexcept { return _objViewMatrix;    }

    WorldObject& setPosition(const Vector3f position) noexcept
    {
        _position = position;
        _objViewMatrix.transformation(position, _rotation);
        return *this;
    }

    WorldObject& setRotation(const Vector3f rotation) noexcept
    {
        _rotation = rotation;
        _objViewMatrix.transformation(_position, rotation);
        return *this;
    }

    WorldObject& setScale(const Vector3f scale) noexcept
    {
        _scale = scale;
        _objViewMatrix.setScale(scale);
        return *this;
    }

    WorldObject& setScale(float scale) noexcept
    {
        _scale = scale;
        _objViewMatrix.setScale(scale);
        return *this;
    }


    WorldObject& addPosition(const Vector3f position) noexcept
    {
        _position += position;
        _objViewMatrix.transformation(_position, _rotation);
        return *this;
    }

    WorldObject& addRotation(const Vector3f rotation) noexcept
    {
        _rotation += rotation;
        _objViewMatrix.transformation(_position, _rotation);
        return *this;
    }
    
    WorldObject& addScale(const Vector3f scale) noexcept
    {
        _scale += scale;
        _objViewMatrix.setScale(_scale);
        return *this;
    }

    WorldObject& addScale(float scale) noexcept
    {
        _scale += scale;
        _objViewMatrix.setScale(_scale);
        return *this;
    }


    WorldObject& subPosition(const Vector3f position) noexcept
    {
        _position -= position;
        _objViewMatrix.transformation(_position, _rotation);
        return *this;
    }

    WorldObject& subRotation(const Vector3f rotation) noexcept
    {
        _rotation -= rotation;
        _objViewMatrix.transformation(_position, _rotation);
        return *this;
    }

    WorldObject& subScale(const Vector3f scale) noexcept
    {
        _scale -= scale;
        _objViewMatrix.setScale(_scale);
        return *this;
    }

    WorldObject& subScale(float scale) noexcept
    {
        _scale -= scale;
        _objViewMatrix.setScale(_scale);
        return *this;
    }


    WorldObject& crossPosition(const Vector3f position) noexcept
    {
        _position.cross(position);
        _objViewMatrix.transformation(_position, _rotation);
        return *this;
    }

    WorldObject& crossRotation(const Vector3f rotation) noexcept
    {
        _rotation.cross(rotation);
        _objViewMatrix.transformation(_position, _rotation);
        return *this;
    }

    WorldObject& crossScale(const Vector3f scale) noexcept
    {
        _scale.cross(scale);
        _objViewMatrix.setScale(_scale);
        return *this;
    }


    // WorldObject& compMulPosition(const Vector3f& position) noexcept;
    // WorldObject& compMulRotation(const Vector3f& rotation) noexcept;
    // WorldObject& compMulScale(const Vector3f& scale)       noexcept;
    //
    // WorldObject& compDivPosition(const Vector3f& position) noexcept;
    // WorldObject& compDivRotation(const Vector3f& rotation) noexcept;
    // WorldObject& compDivScale(const Vector3f& scale)       noexcept;
    // WorldObject& compDivScale(float scale)                 noexcept;


    WorldObject& setPositionPipelined(const Vector3f position) noexcept
    {
        _position = position;
        return *this;
    }

    WorldObject& setRotationPipelined(const Vector3f rotation) noexcept
    {
        _rotation = rotation;
        return *this;
    }

    WorldObject& setScalePipelined(const Vector3f scale) noexcept
    {
        _scale = scale;
        return *this;
    }

    WorldObject& setScalePipelined(float scale) noexcept
    {
        _scale = scale;
        return *this;
    }


    WorldObject& addPositionPipelined(const Vector3f position) noexcept
    {
        _position += position;
        return *this;
    }

    WorldObject& addRotationPipelined(const Vector3f rotation) noexcept
    {
        _rotation += rotation;
        return *this;
    }

    WorldObject& addScalePipelined(const Vector3f scale) noexcept
    {
        _scale += scale;
        return *this;
    }

    WorldObject& addScalePipelined(float scale) noexcept
    {
        _scale += scale;
        return *this;
    }


    WorldObject& subPositionPipelined(const Vector3f position) noexcept
    {
        _position -= position;
        return *this;
    }

    WorldObject& subRotationPipelined(const Vector3f rotation) noexcept
    {
        _rotation -= rotation;
        return *this;
    }

    WorldObject& subScalePipelined(const Vector3f scale) noexcept
    {
        _scale -= scale;
        return *this;
    }

    WorldObject& subScalePipelined(float scale) noexcept
    {
        _scale -= scale;
        return *this;
    }


    WorldObject& crossPositionPipelined(const Vector3f position) noexcept
    {
        _position.cross(position);
        return *this;
    }

    WorldObject& crossRotationPipelined(const Vector3f rotation) noexcept
    {
        _rotation.cross(rotation);
        return *this;
    }

    WorldObject& crossScalePipelined(const Vector3f scale) noexcept
    {
        _scale.cross(scale);
        return *this;
    }

    // WorldObject& compMulPositionPipelined(const Vector3f& position) noexcept;
    // WorldObject& compMulRotationPipelined(const Vector3f& rotation) noexcept;
    // WorldObject& compMulScalePipelined(const Vector3f& scale)       noexcept;
    // WorldObject& compMulScalePipelined(float scale)                 noexcept;
    //
    // WorldObject& compDivPositionPipelined(const Vector3f& position) noexcept;
    // WorldObject& compDivRotationPipelined(const Vector3f& rotation) noexcept;
    // WorldObject& compDivScalePipelined(const Vector3f& scale)       noexcept;
    // WorldObject& compDivScalePipelined(float scale)                 noexcept;

    WorldObject& buildMatrixPipelined() noexcept
    {
        _objViewMatrix = Matrix4x4f::transformation(_position, _rotation, _scale);
        return *this;
    }

    WorldObject& buildPosRotMatrixPipelined() noexcept
    {
        _objViewMatrix.transformation(_position, _rotation);
        return *this;
    }

    WorldObject& buildScaleMatrixPipelined() noexcept
    {
        _objViewMatrix.setScale(_scale);
        return *this;
    }
};
