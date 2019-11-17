#pragma once

#include "DLL.hpp"
#include "shader/IUniform.hpp"
#include <Objects.hpp>
#include <glm/matrix.hpp>
#include <Safeties.hpp>

class IShader;
class IRenderingContext;

class Vector2f;
class Vector3i;
class Vector3f;
class Vector4f;

class TAU_DLL IShaderProgram
{
    DEFAULT_DESTRUCT_VI(IShaderProgram);
    DELETE_COPY(IShaderProgram);
public:
    static Ref<IShaderProgram> create(IRenderingContext& context) noexcept;
protected:
    Ref<IShader> _vertexShader;
    Ref<IShader> _tessellationControlShader;
    Ref<IShader> _tessellationEvaluationShader;
    Ref<IShader> _geometryShader;
    Ref<IShader> _pixelShader;
protected:
    IShaderProgram() noexcept;
public:
    virtual bool setVertexShader(IRenderingContext& context, Ref<IShader>& vs) noexcept;
    virtual bool setTessellationControlShader(IRenderingContext& context, Ref<IShader>& tcs) noexcept;
    virtual bool setTessellationEvaluationShader(IRenderingContext& context, Ref<IShader>& tes) noexcept;
    virtual bool setGeometryShader(IRenderingContext& context, Ref<IShader>& gs) noexcept;
    virtual bool setPixelShader(IRenderingContext& context, Ref<IShader>& ps) noexcept;

    virtual void bind(IRenderingContext& context) noexcept = 0;
    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual bool link(IRenderingContext& context) noexcept = 0;

    virtual Ref<IUniform<bool>> getUniformBool(const char* name) noexcept = 0;
    virtual Ref<IUniform<int>> getUniformInt(const char* name) noexcept = 0;
    virtual Ref<IUniform<unsigned int>> getUniformUInt(const char* name) noexcept = 0;
    virtual Ref<IUniform<float>> getUniformFloat(const char* name) noexcept = 0;
    virtual Ref<IUniform<double>> getUniformDouble(const char* name) noexcept = 0;
    virtual Ref<IUniform<const Vector2f&>> getUniformVector2f(const char* name) noexcept = 0;
    virtual Ref<IUniform<const Vector3i&>> getUniformVector3i(const char* name) noexcept = 0;
    virtual Ref<IUniform<const Vector3f&>> getUniformVector3f(const char* name) noexcept = 0;
    virtual Ref<IUniform<const Vector4f&>> getUniformVector4f(const char* name) noexcept = 0;

    virtual Ref<IUniform<const glm::bvec2&>> getUniformVector2Bool(const char* name) noexcept = 0;
    virtual Ref<IUniform<const glm::bvec3&>> getUniformVector3Bool(const char* name) noexcept = 0;
    virtual Ref<IUniform<const glm::bvec4&>> getUniformVector4Bool(const char* name) noexcept = 0;
    virtual Ref<IUniform<const glm::ivec2&>> getUniformVector2Int(const char* name) noexcept = 0;
    virtual Ref<IUniform<const glm::ivec3&>> getUniformVector3Int(const char* name) noexcept = 0;
    virtual Ref<IUniform<const glm::ivec4&>> getUniformVector4Int(const char* name) noexcept = 0;
    virtual Ref<IUniform<const glm::uvec2&>> getUniformVector2UInt(const char* name) noexcept = 0;
    virtual Ref<IUniform<const glm::uvec3&>> getUniformVector3UInt(const char* name) noexcept = 0;
    virtual Ref<IUniform<const glm::uvec4&>> getUniformVector4UInt(const char* name) noexcept = 0;
    virtual Ref<IUniform<const glm:: vec2&>> getUniformVector2Float(const char* name) noexcept = 0;
    virtual Ref<IUniform<const glm:: vec3&>> getUniformVector3Float(const char* name) noexcept = 0;
    virtual Ref<IUniform<const glm:: vec4&>> getUniformVector4Float(const char* name) noexcept = 0;
    virtual Ref<IUniform<const glm::dvec2&>> getUniformVector2Double(const char* name) noexcept = 0;
    virtual Ref<IUniform<const glm::dvec3&>> getUniformVector3Double(const char* name) noexcept = 0;
    virtual Ref<IUniform<const glm::dvec4&>> getUniformVector4Double(const char* name) noexcept = 0;

    virtual Ref<IUniform<const glm:: mat2x2&>> getUniformMatrix2x2Float(const char* name, bool transpose) noexcept = 0;
    virtual Ref<IUniform<const glm:: mat2x3&>> getUniformMatrix2x3Float(const char* name, bool transpose) noexcept = 0;
    virtual Ref<IUniform<const glm:: mat2x4&>> getUniformMatrix2x4Float(const char* name, bool transpose) noexcept = 0;
    virtual Ref<IUniform<const glm:: mat3x2&>> getUniformMatrix3x2Float(const char* name, bool transpose) noexcept = 0;
    virtual Ref<IUniform<const glm:: mat3x3&>> getUniformMatrix3x3Float(const char* name, bool transpose) noexcept = 0;
    virtual Ref<IUniform<const glm:: mat3x4&>> getUniformMatrix3x4Float(const char* name, bool transpose) noexcept = 0;
    virtual Ref<IUniform<const glm:: mat4x2&>> getUniformMatrix4x2Float(const char* name, bool transpose) noexcept = 0;
    virtual Ref<IUniform<const glm:: mat4x3&>> getUniformMatrix4x3Float(const char* name, bool transpose) noexcept = 0;
    virtual Ref<IUniform<const glm:: mat4x4&>> getUniformMatrix4x4Float(const char* name, bool transpose) noexcept = 0;
    virtual Ref<IUniform<const glm::dmat2x2&>> getUniformMatrix2x2Double(const char* name, bool transpose) noexcept = 0;
    virtual Ref<IUniform<const glm::dmat2x3&>> getUniformMatrix2x3Double(const char* name, bool transpose) noexcept = 0;
    virtual Ref<IUniform<const glm::dmat2x4&>> getUniformMatrix2x4Double(const char* name, bool transpose) noexcept = 0;
    virtual Ref<IUniform<const glm::dmat3x2&>> getUniformMatrix3x2Double(const char* name, bool transpose) noexcept = 0;
    virtual Ref<IUniform<const glm::dmat3x3&>> getUniformMatrix3x3Double(const char* name, bool transpose) noexcept = 0;
    virtual Ref<IUniform<const glm::dmat3x4&>> getUniformMatrix3x4Double(const char* name, bool transpose) noexcept = 0;
    virtual Ref<IUniform<const glm::dmat4x2&>> getUniformMatrix4x2Double(const char* name, bool transpose) noexcept = 0;
    virtual Ref<IUniform<const glm::dmat4x3&>> getUniformMatrix4x3Double(const char* name, bool transpose) noexcept = 0;
    virtual Ref<IUniform<const glm::dmat4x4&>> getUniformMatrix4x4Double(const char* name, bool transpose) noexcept = 0;
protected:
    virtual void attach(IRenderingContext& context, Ref<IShader> shader) noexcept = 0;
    virtual void detach(IRenderingContext& context, Ref<IShader> shader) noexcept = 0;
};