#pragma once

#include <maths/Matrix4x4f.hpp>
#include <maths/Vector3f.hpp>
#include <model/RenderableObject.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class WorldObject final
{
    DEFAULT_DESTRUCT(WorldObject);
    DEFAULT_COPY(WorldObject);
private:
    RenderableObject& _renderableObject;
    Vector3f   _position;
    Vector3f   _rotation;
    Vector3f   _scale;
    glm::mat4 _objViewMatrix;
public:
    WorldObject(RenderableObject& RESTRICT renderableObject) noexcept
        : _renderableObject(renderableObject), _position(0.0f), _rotation(0.0f), _scale(1.0f), _objViewMatrix(glm::mat4(1.0f))
    { }

    WorldObject(RenderableObject& RESTRICT renderableObject, const Vector3f& RESTRICT position) noexcept
        : _renderableObject(renderableObject), _position(position), _rotation(0.0f), _scale(1.0f), _objViewMatrix(glm::mat4(1.0f))
    { }

    WorldObject(RenderableObject& RESTRICT renderableObject, const Vector3f& RESTRICT position, const Vector3f& RESTRICT rotation) noexcept
        : _renderableObject(renderableObject), _position(position), _rotation(rotation), _scale(1.0f), _objViewMatrix(glm::mat4(1.0f))
    { }

    WorldObject(RenderableObject& RESTRICT renderableObject, const Vector3f& RESTRICT position, const Vector3f& RESTRICT rotation, const Vector3f& RESTRICT scale) noexcept
        : _renderableObject(renderableObject), _position(position), _rotation(rotation), _scale(scale), _objViewMatrix(glm::mat4(1.0f))
    { }

    WorldObject(RenderableObject& RESTRICT renderableObject, const Vector3f& RESTRICT position, const Vector3f& RESTRICT rotation, const float scale) noexcept
        : _renderableObject(renderableObject), _position(position), _rotation(rotation), _scale(scale), _objViewMatrix(glm::mat4(1.0f))
    { }

    const RenderableObject& renderableObject() const noexcept { return _renderableObject; }
    const Vector3f&         position()         const noexcept { return _position;         }
    const Vector3f&         rotation()         const noexcept { return _rotation;         }
    const Vector3f&         scale()            const noexcept { return _scale;            }
    const glm::mat4&        viewMatrix()       const noexcept { return _objViewMatrix;    }

    WorldObject& setPosition(const Vector3f position) noexcept
    {
        _position = position;
        _objViewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(_scale.x(), _scale.y(), _scale.z()));
        _objViewMatrix = glm::translate(_objViewMatrix, glm::vec3(position.x(), position.y(), position.z()));
        _objViewMatrix = glm::rotate(_objViewMatrix, _rotation.z(), glm::vec3(0.0f, 0.0f, 1.0f));
        _objViewMatrix = glm::rotate(_objViewMatrix, _rotation.y(), glm::vec3(0.0f, 1.0f, 0.0f));
        _objViewMatrix = glm::rotate(_objViewMatrix, _rotation.x(), glm::vec3(1.0f, 0.0f, 0.0f));

        return *this;
    }

    WorldObject& setRotation(const Vector3f rotation) noexcept
    {
        _rotation = rotation;
        rebuildMatrix();
        return *this;
    }

    WorldObject& setScale(const Vector3f scale) noexcept
    {
        _scale = scale;
        rebuildMatrix();
        return *this;
    }

    WorldObject& setScale(float scale) noexcept
    {
        _scale = scale;
        rebuildMatrix();
        return *this;
    }


    WorldObject& addPosition(const Vector3f position) noexcept
    {
        _position += position;
        rebuildMatrix();
        return *this;
    }

    WorldObject& addRotation(const Vector3f rotation) noexcept
    {
        _rotation += rotation;
        rebuildMatrix();
        return *this;
    }
    
    WorldObject& addScale(const Vector3f scale) noexcept
    {
        _scale += scale;
        rebuildMatrix();
        return *this;
    }

    WorldObject& addScale(float scale) noexcept
    {
        _scale += scale;
        rebuildMatrix();
        return *this;
    }


    WorldObject& subPosition(const Vector3f position) noexcept
    {
        _position -= position;
        rebuildMatrix();
        return *this;
    }

    WorldObject& subRotation(const Vector3f rotation) noexcept
    {
        _rotation -= rotation;
        rebuildMatrix();
        return *this;
    }

    WorldObject& subScale(const Vector3f scale) noexcept
    {
        _scale -= scale;
        rebuildMatrix();
        return *this;
    }

    WorldObject& subScale(float scale) noexcept
    {
        _scale -= scale;
        rebuildMatrix();
        return *this;
    }


    WorldObject& crossPosition(const Vector3f position) noexcept
    {
        _position.cross(position);
        rebuildMatrix();
        return *this;
    }

    WorldObject& crossRotation(const Vector3f rotation) noexcept
    {
        _rotation.cross(rotation);
        rebuildMatrix();
        return *this;
    }

    WorldObject& crossScale(const Vector3f scale) noexcept
    {
        _scale.cross(scale);
        rebuildMatrix();
        return *this;
    }

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

    WorldObject& buildMatrixPipelined() noexcept
    {
        rebuildMatrix();
        return *this;
    }

    WorldObject& buildPosRotMatrixPipelined() noexcept
    {
        _objViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(_position.x(), _position.y(), _position.z()));
        _objViewMatrix = glm::rotate(_objViewMatrix, _rotation.z(), glm::vec3(0.0f, 0.0f, 1.0f));
        _objViewMatrix = glm::rotate(_objViewMatrix, _rotation.y(), glm::vec3(0.0f, 1.0f, 0.0f));
        _objViewMatrix = glm::rotate(_objViewMatrix, _rotation.x(), glm::vec3(1.0f, 0.0f, 0.0f));
        return *this;
    }

    WorldObject& buildScaleMatrixPipelined() noexcept
    {
        _objViewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(_scale.x(), _scale.y(), _scale.z()));
        return *this;
    }
private:
    void rebuildMatrix() noexcept
    {
        _objViewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(_scale.x(), _scale.y(), _scale.z()));
        _objViewMatrix = glm::translate(_objViewMatrix, glm::vec3(_position.x(), _position.y(), _position.z()));
        _objViewMatrix = glm::rotate(_objViewMatrix, _rotation.z(), glm::vec3(0.0f, 0.0f, 1.0f));
        _objViewMatrix = glm::rotate(_objViewMatrix, _rotation.y(), glm::vec3(0.0f, 1.0f, 0.0f));
        _objViewMatrix = glm::rotate(_objViewMatrix, _rotation.x(), glm::vec3(1.0f, 0.0f, 0.0f));
    }
};
