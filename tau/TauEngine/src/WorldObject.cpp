#include <WorldObject.hpp>

WorldObject::WorldObject(const RenderableObject* RESTRICT renderableObject) noexcept
    : _renderableObject(renderableObject), _position(0.0f), _rotation(0.0f), _scale(1.0f)
{ }

WorldObject::WorldObject(const RenderableObject* RESTRICT renderableObject, const Vector3f& RESTRICT position) noexcept
    : _renderableObject(renderableObject), _position(position), _rotation(0.0f), _scale(1.0f)
{ }

WorldObject::WorldObject(const RenderableObject* RESTRICT renderableObject, const Vector3f& RESTRICT position, const Vector3f& RESTRICT rotation) noexcept
    : _renderableObject(renderableObject), _position(position), _rotation(rotation), _scale(1.0f)
{ }

WorldObject::WorldObject(const RenderableObject* RESTRICT renderableObject, const Vector3f& RESTRICT position, const Vector3f& RESTRICT rotation, const Vector3f& RESTRICT scale) noexcept
    : _renderableObject(renderableObject), _position(position), _rotation(rotation), _scale(scale)
{ }

WorldObject::WorldObject(const RenderableObject* RESTRICT renderableObject, const Vector3f& RESTRICT position, const Vector3f& rotation, const float scale) noexcept
    : _renderableObject(renderableObject), _position(position), _rotation(rotation), _scale(scale)
{ }

WorldObject& WorldObject::setPosition(const Vector3f& position) noexcept
{
    _position = position;
    _objViewMatrix.transformation(position, _rotation);
    return *this;
}

WorldObject& WorldObject::setRotation(const Vector3f& rotation) noexcept
{
    _rotation = rotation;
    _objViewMatrix.transformation(_position, rotation);
    return *this;
}

WorldObject& WorldObject::setScale(const Vector3f& scale) noexcept
{
    _scale = scale;
    _objViewMatrix.setScale(scale);
    return *this;
}

WorldObject& WorldObject::setScale(float scale) noexcept
{
    _scale = scale;
    _objViewMatrix.setScale(scale);
    return *this;
}

WorldObject& WorldObject::addPosition(const Vector3f& position) noexcept
{
    _position += position;
    _objViewMatrix.transformation(_position, _rotation);
    return *this;
}

WorldObject& WorldObject::addRotation(const Vector3f& rotation) noexcept
{
    _rotation += rotation;
    _objViewMatrix.transformation(_position, _rotation);
    return *this;
}

WorldObject& WorldObject::addScale(const Vector3f& scale)       noexcept
{
    _scale += scale;
    _objViewMatrix.setScale(_scale);
    return *this;
}

WorldObject& WorldObject::addScale(float scale)                 noexcept
{
    _scale += scale;
    _objViewMatrix.setScale(_scale);
    return *this;
}

WorldObject& WorldObject::subPosition(const Vector3f& position) noexcept
{
    _position -= position;
    _objViewMatrix.transformation(_position, _rotation);
    return *this;
}

WorldObject& WorldObject::subRotation(const Vector3f& rotation) noexcept
{
    _rotation -= rotation;
    _objViewMatrix.transformation(_position, _rotation);
    return *this;
}

WorldObject& WorldObject::subScale(const Vector3f& scale) noexcept
{
    _scale -= scale;
    _objViewMatrix.setScale(_scale);
    return *this;
}

WorldObject& WorldObject::subScale(float scale) noexcept
{
    _scale -= scale;
    _objViewMatrix.setScale(_scale);
    return *this;
}

WorldObject& WorldObject::crossPosition(const Vector3f& position) noexcept
{
    _position.cross(position);
    _objViewMatrix.transformation(_position, _rotation);
    return *this;
}

WorldObject& WorldObject::crossRotation(const Vector3f& rotation) noexcept
{

    _rotation.cross(rotation);
    _objViewMatrix.transformation(_position, _rotation);
    return *this;
}

WorldObject& WorldObject::crossScale(const Vector3f& scale) noexcept
{
    _scale.cross(scale);
    _objViewMatrix.setScale(_scale);
    return *this;
}

WorldObject& WorldObject::setPositionPipelined(const Vector3f& position) noexcept
{
    _position = position;
    return *this;
}

WorldObject& WorldObject::setRotationPipelined(const Vector3f& rotation) noexcept
{
    _rotation = rotation;
    return *this;
}

WorldObject& WorldObject::setScalePipelined(const Vector3f& scale) noexcept
{
    _scale = scale;
    return *this;
}

WorldObject& WorldObject::setScalePipelined(float scale) noexcept
{
    _scale = scale;
    return *this;
}

WorldObject& WorldObject::addPositionPipelined(const Vector3f& position) noexcept
{
    _position += position;
    return *this;
}

WorldObject& WorldObject::addRotationPipelined(const Vector3f& rotation) noexcept
{
    _rotation += rotation;
    return *this;
}

WorldObject& WorldObject::addScalePipelined(const Vector3f& scale) noexcept
{
    _scale += scale;
    return *this;
}

WorldObject& WorldObject::addScalePipelined(float scale) noexcept
{
    _scale += scale;
    return *this;
}

WorldObject& WorldObject::subPositionPipelined(const Vector3f& position) noexcept
{
    _position -= position;
    return *this;
}

WorldObject& WorldObject::subRotationPipelined(const Vector3f& rotation) noexcept
{
    _rotation -= rotation;
    return *this;
}

WorldObject& WorldObject::subScalePipelined(const Vector3f& scale) noexcept
{
    _scale -= scale;
    return *this;
}

WorldObject& WorldObject::subScalePipelined(float scale) noexcept
{
    _scale -= scale;
    return *this;
}

WorldObject& WorldObject::crossPositionPipelined(const Vector3f& position) noexcept
{
    _position.cross(position);
    return *this;
}

WorldObject& WorldObject::crossRotationPipelined(const Vector3f& rotation) noexcept
{
    _rotation.cross(rotation);
    return *this;
}

WorldObject& WorldObject::crossScalePipelined(const Vector3f& scale) noexcept
{
    _scale.cross(scale);
    return *this;
}

WorldObject& WorldObject::buildMatrixPipelined() noexcept
{
    _objViewMatrix = Matrix4x4f::transformation(_position, _rotation, _scale);
    return *this;
}

WorldObject& WorldObject::buildPosRotMatrixPipelined() noexcept
{
    _objViewMatrix.transformation(_position, _rotation);
    return *this;
}

WorldObject& WorldObject::buildScaleMatrixPipelined() noexcept
{
    _objViewMatrix.setScale(_scale);
    return *this;
}
