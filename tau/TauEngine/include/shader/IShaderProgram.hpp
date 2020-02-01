#pragma once

#include "DLL.hpp"
// #include "shader/IUniform.hpp"
#include "model/Buffer.hpp"
#include <Objects.hpp>
#include <glm/matrix.hpp>
#include <Safeties.hpp>
#include <type_traits>
#include <String.hpp>

class IShader;
class IRenderingContext;

class Vector2f;
class Vector3i;
class Vector3f;
class Vector4f;

class TAU_DLL NOVTABLE IShaderProgram
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

    // virtual Ref<IUniform<bool>> getUniformBool(const char* name) noexcept = 0;
    // virtual Ref<IUniform<int>> getUniformInt(const char* name) noexcept = 0;
    // virtual Ref<IUniform<unsigned int>> getUniformUInt(const char* name) noexcept = 0;
    // virtual Ref<IUniform<float>> getUniformFloat(const char* name) noexcept = 0;
    // virtual Ref<IUniform<double>> getUniformDouble(const char* name) noexcept = 0;
    // virtual Ref<IUniform<const Vector2f&>> getUniformVector2f(const char* name) noexcept = 0;
    // virtual Ref<IUniform<const Vector3i&>> getUniformVector3i(const char* name) noexcept = 0;
    // virtual Ref<IUniform<const Vector3f&>> getUniformVector3f(const char* name) noexcept = 0;
    // virtual Ref<IUniform<const Vector4f&>> getUniformVector4f(const char* name) noexcept = 0;
    //
    // virtual Ref<IUniform<const glm::bvec2&>> getUniformVector2Bool(const char* name) noexcept = 0;
    // virtual Ref<IUniform<const glm::bvec3&>> getUniformVector3Bool(const char* name) noexcept = 0;
    // virtual Ref<IUniform<const glm::bvec4&>> getUniformVector4Bool(const char* name) noexcept = 0;
    // virtual Ref<IUniform<const glm::ivec2&>> getUniformVector2Int(const char* name) noexcept = 0;
    // virtual Ref<IUniform<const glm::ivec3&>> getUniformVector3Int(const char* name) noexcept = 0;
    // virtual Ref<IUniform<const glm::ivec4&>> getUniformVector4Int(const char* name) noexcept = 0;
    // virtual Ref<IUniform<const glm::uvec2&>> getUniformVector2UInt(const char* name) noexcept = 0;
    // virtual Ref<IUniform<const glm::uvec3&>> getUniformVector3UInt(const char* name) noexcept = 0;
    // virtual Ref<IUniform<const glm::uvec4&>> getUniformVector4UInt(const char* name) noexcept = 0;
    // virtual Ref<IUniform<const glm:: vec2&>> getUniformVector2Float(const char* name) noexcept = 0;
    // virtual Ref<IUniform<const glm:: vec3&>> getUniformVector3Float(const char* name) noexcept = 0;
    // virtual Ref<IUniform<const glm:: vec4&>> getUniformVector4Float(const char* name) noexcept = 0;
    // virtual Ref<IUniform<const glm::dvec2&>> getUniformVector2Double(const char* name) noexcept = 0;
    // virtual Ref<IUniform<const glm::dvec3&>> getUniformVector3Double(const char* name) noexcept = 0;
    // virtual Ref<IUniform<const glm::dvec4&>> getUniformVector4Double(const char* name) noexcept = 0;
    //
    // virtual Ref<IUniform<const glm:: mat2x2&>> getUniformMatrix2x2Float(const char* name, bool transpose) noexcept = 0;
    // virtual Ref<IUniform<const glm:: mat2x3&>> getUniformMatrix2x3Float(const char* name, bool transpose) noexcept = 0;
    // virtual Ref<IUniform<const glm:: mat2x4&>> getUniformMatrix2x4Float(const char* name, bool transpose) noexcept = 0;
    // virtual Ref<IUniform<const glm:: mat3x2&>> getUniformMatrix3x2Float(const char* name, bool transpose) noexcept = 0;
    // virtual Ref<IUniform<const glm:: mat3x3&>> getUniformMatrix3x3Float(const char* name, bool transpose) noexcept = 0;
    // virtual Ref<IUniform<const glm:: mat3x4&>> getUniformMatrix3x4Float(const char* name, bool transpose) noexcept = 0;
    // virtual Ref<IUniform<const glm:: mat4x2&>> getUniformMatrix4x2Float(const char* name, bool transpose) noexcept = 0;
    // virtual Ref<IUniform<const glm:: mat4x3&>> getUniformMatrix4x3Float(const char* name, bool transpose) noexcept = 0;
    // virtual Ref<IUniform<const glm:: mat4x4&>> getUniformMatrix4x4Float(const char* name, bool transpose) noexcept = 0;
    // virtual Ref<IUniform<const glm::dmat2x2&>> getUniformMatrix2x2Double(const char* name, bool transpose) noexcept = 0;
    // virtual Ref<IUniform<const glm::dmat2x3&>> getUniformMatrix2x3Double(const char* name, bool transpose) noexcept = 0;
    // virtual Ref<IUniform<const glm::dmat2x4&>> getUniformMatrix2x4Double(const char* name, bool transpose) noexcept = 0;
    // virtual Ref<IUniform<const glm::dmat3x2&>> getUniformMatrix3x2Double(const char* name, bool transpose) noexcept = 0;
    // virtual Ref<IUniform<const glm::dmat3x3&>> getUniformMatrix3x3Double(const char* name, bool transpose) noexcept = 0;
    // virtual Ref<IUniform<const glm::dmat3x4&>> getUniformMatrix3x4Double(const char* name, bool transpose) noexcept = 0;
    // virtual Ref<IUniform<const glm::dmat4x2&>> getUniformMatrix4x2Double(const char* name, bool transpose) noexcept = 0;
    // virtual Ref<IUniform<const glm::dmat4x3&>> getUniformMatrix4x3Double(const char* name, bool transpose) noexcept = 0;
    // virtual Ref<IUniform<const glm::dmat4x4&>> getUniformMatrix4x4Double(const char* name, bool transpose) noexcept = 0;
    //
    // template<typename _T>
    // Ref<IUniform<_T>> getUniform(const char* name) noexcept;
    //
    // template<typename _T>
    // Ref<IUniform<_T>> getUniform(const DynString& name) noexcept
    // { return getUniform<_T>(name.c_str()); }
    //
    // template<typename _T>
    // Ref<IUniform<const typename ::std::remove_const<typename ::std::remove_reference<_T>::type>::type&>> getUniform(const char* name, bool transpose) noexcept;
    //
    // template<typename _T>
    // Ref<IUniform<const typename ::std::remove_const<typename ::std::remove_reference<_T>::type>::type&>> getUniform(const DynString& name, bool transpose) noexcept
    // { return getUniform<_T>(name.c_str(), transpose); }
    //
    // template<typename _T>
    // Ref<IUniform<const typename ::std::remove_const<typename ::std::remove_reference<_T>::type>::type&>> getUniformVector(const char* name) noexcept
    // { return getUniformVector<const _T&>(name); }
    //
    // template<typename _T>
    // Ref<IUniform<const typename ::std::remove_const<typename ::std::remove_reference<_T>::type>::type&>> getUniformVector(const DynString& name) noexcept
    // { return getUniformVector<_T>(name.c_str()); }
    //
    // template<typename _T>
    // Ref<IUniform<const typename ::std::remove_const<typename ::std::remove_reference<_T>::type>::type&>> getUniformMatrix(const char* name, bool transpose) noexcept
    // { return getUniform<const _T&>(name, transpose); }
    //
    // template<typename _T>
    // Ref<IUniform<const typename ::std::remove_const<typename ::std::remove_reference<_T>::type>::type&>> getUniformMatrix(const DynString& name, bool transpose) noexcept
    // { return getUniformMatrix<_T>(name.c_str(), transpose); }
protected:
    virtual bool attach(IRenderingContext& context, Ref<IShader> shader) noexcept = 0;
    virtual void detach(IRenderingContext& context, Ref<IShader> shader) noexcept = 0;
};

// template<> inline Ref<IUniform<bool>> IShaderProgram::getUniform<bool>(const char* name) noexcept { return getUniformBool(name); }
// template<> inline Ref<IUniform<int>> IShaderProgram::getUniform<int>(const char* name) noexcept { return getUniformInt(name); }
// template<> inline Ref<IUniform<unsigned int>> IShaderProgram::getUniform<unsigned int>(const char* name) noexcept { return getUniformUInt(name); }
// template<> inline Ref<IUniform<float>> IShaderProgram::getUniform<float>(const char* name) noexcept { return getUniformFloat(name); }
// template<> inline Ref<IUniform<double>> IShaderProgram::getUniform<double>(const char* name) noexcept { return getUniformDouble(name); }
// template<> inline Ref<IUniform<const Vector2f&>> IShaderProgram::getUniform<const Vector2f&>(const char* name) noexcept { return getUniformVector2f(name); }
// template<> inline Ref<IUniform<const Vector3i&>> IShaderProgram::getUniform<const Vector3i&>(const char* name) noexcept { return getUniformVector3i(name); }
// template<> inline Ref<IUniform<const Vector3f&>> IShaderProgram::getUniform<const Vector3f&>(const char* name) noexcept { return getUniformVector3f(name); }
// template<> inline Ref<IUniform<const Vector4f&>> IShaderProgram::getUniform<const Vector4f&>(const char* name) noexcept { return getUniformVector4f(name); }
// template<> inline Ref<IUniform<const Vector2f&>> IShaderProgram::getUniformVector<Vector2f>(const char* name) noexcept { return getUniformVector2f(name); }
// template<> inline Ref<IUniform<const Vector3i&>> IShaderProgram::getUniformVector<Vector3i>(const char* name) noexcept { return getUniformVector3i(name); }
// template<> inline Ref<IUniform<const Vector3f&>> IShaderProgram::getUniformVector<Vector3f>(const char* name) noexcept { return getUniformVector3f(name); }
// template<> inline Ref<IUniform<const Vector4f&>> IShaderProgram::getUniformVector<Vector4f>(const char* name) noexcept { return getUniformVector4f(name); }
//
// template<> inline Ref<IUniform<const glm::bvec2&>> IShaderProgram::getUniform<const glm::bvec2&>(const char* name) noexcept { return getUniformVector2Bool(name); }
// template<> inline Ref<IUniform<const glm::bvec3&>> IShaderProgram::getUniform<const glm::bvec3&>(const char* name) noexcept { return getUniformVector3Bool(name); }
// template<> inline Ref<IUniform<const glm::bvec4&>> IShaderProgram::getUniform<const glm::bvec4&>(const char* name) noexcept { return getUniformVector4Bool(name); }
// template<> inline Ref<IUniform<const glm::ivec2&>> IShaderProgram::getUniform<const glm::ivec2&>(const char* name) noexcept { return getUniformVector2Int(name); }
// template<> inline Ref<IUniform<const glm::ivec3&>> IShaderProgram::getUniform<const glm::ivec3&>(const char* name) noexcept { return getUniformVector3Int(name); }
// template<> inline Ref<IUniform<const glm::ivec4&>> IShaderProgram::getUniform<const glm::ivec4&>(const char* name) noexcept { return getUniformVector4Int(name); }
// template<> inline Ref<IUniform<const glm::uvec2&>> IShaderProgram::getUniform<const glm::uvec2&>(const char* name) noexcept { return getUniformVector2UInt(name); }
// template<> inline Ref<IUniform<const glm::uvec3&>> IShaderProgram::getUniform<const glm::uvec3&>(const char* name) noexcept { return getUniformVector3UInt(name); }
// template<> inline Ref<IUniform<const glm::uvec4&>> IShaderProgram::getUniform<const glm::uvec4&>(const char* name) noexcept { return getUniformVector4UInt(name); }
// template<> inline Ref<IUniform<const glm:: vec2&>> IShaderProgram::getUniform<const glm:: vec2&>(const char* name) noexcept { return getUniformVector2Float(name); }
// template<> inline Ref<IUniform<const glm:: vec3&>> IShaderProgram::getUniform<const glm:: vec3&>(const char* name) noexcept { return getUniformVector3Float(name); }
// template<> inline Ref<IUniform<const glm:: vec4&>> IShaderProgram::getUniform<const glm:: vec4&>(const char* name) noexcept { return getUniformVector4Float(name); }
// template<> inline Ref<IUniform<const glm::dvec2&>> IShaderProgram::getUniform<const glm::dvec2&>(const char* name) noexcept { return getUniformVector2Double(name); }
// template<> inline Ref<IUniform<const glm::dvec3&>> IShaderProgram::getUniform<const glm::dvec3&>(const char* name) noexcept { return getUniformVector3Double(name); }
// template<> inline Ref<IUniform<const glm::dvec4&>> IShaderProgram::getUniform<const glm::dvec4&>(const char* name) noexcept { return getUniformVector4Double(name); }
//
// template<> inline Ref<IUniform<const glm:: mat2x2&>> IShaderProgram::getUniform<glm:: mat2x2>(const char* name, bool transpose) noexcept { return getUniformMatrix2x2Float(name, transpose); }
// template<> inline Ref<IUniform<const glm:: mat2x3&>> IShaderProgram::getUniform<glm:: mat2x3>(const char* name, bool transpose) noexcept { return getUniformMatrix2x3Float(name, transpose); }
// template<> inline Ref<IUniform<const glm:: mat2x4&>> IShaderProgram::getUniform<glm:: mat2x4>(const char* name, bool transpose) noexcept { return getUniformMatrix2x4Float(name, transpose); }
// template<> inline Ref<IUniform<const glm:: mat3x2&>> IShaderProgram::getUniform<glm:: mat3x2>(const char* name, bool transpose) noexcept { return getUniformMatrix3x2Float(name, transpose); }
// template<> inline Ref<IUniform<const glm:: mat3x3&>> IShaderProgram::getUniform<glm:: mat3x3>(const char* name, bool transpose) noexcept { return getUniformMatrix3x3Float(name, transpose); }
// template<> inline Ref<IUniform<const glm:: mat3x4&>> IShaderProgram::getUniform<glm:: mat3x4>(const char* name, bool transpose) noexcept { return getUniformMatrix3x4Float(name, transpose); }
// template<> inline Ref<IUniform<const glm:: mat4x2&>> IShaderProgram::getUniform<glm:: mat4x2>(const char* name, bool transpose) noexcept { return getUniformMatrix4x2Float(name, transpose); }
// template<> inline Ref<IUniform<const glm:: mat4x3&>> IShaderProgram::getUniform<glm:: mat4x3>(const char* name, bool transpose) noexcept { return getUniformMatrix4x3Float(name, transpose); }
// template<> inline Ref<IUniform<const glm:: mat4x4&>> IShaderProgram::getUniform<glm:: mat4x4>(const char* name, bool transpose) noexcept { return getUniformMatrix4x4Float(name, transpose); }
// template<> inline Ref<IUniform<const glm::dmat2x2&>> IShaderProgram::getUniform<glm::dmat2x2>(const char* name, bool transpose) noexcept { return getUniformMatrix2x2Double(name, transpose); }
// template<> inline Ref<IUniform<const glm::dmat2x3&>> IShaderProgram::getUniform<glm::dmat2x3>(const char* name, bool transpose) noexcept { return getUniformMatrix2x3Double(name, transpose); }
// template<> inline Ref<IUniform<const glm::dmat2x4&>> IShaderProgram::getUniform<glm::dmat2x4>(const char* name, bool transpose) noexcept { return getUniformMatrix2x4Double(name, transpose); }
// template<> inline Ref<IUniform<const glm::dmat3x2&>> IShaderProgram::getUniform<glm::dmat3x2>(const char* name, bool transpose) noexcept { return getUniformMatrix3x2Double(name, transpose); }
// template<> inline Ref<IUniform<const glm::dmat3x3&>> IShaderProgram::getUniform<glm::dmat3x3>(const char* name, bool transpose) noexcept { return getUniformMatrix3x3Double(name, transpose); }
// template<> inline Ref<IUniform<const glm::dmat3x4&>> IShaderProgram::getUniform<glm::dmat3x4>(const char* name, bool transpose) noexcept { return getUniformMatrix3x4Double(name, transpose); }
// template<> inline Ref<IUniform<const glm::dmat4x2&>> IShaderProgram::getUniform<glm::dmat4x2>(const char* name, bool transpose) noexcept { return getUniformMatrix4x2Double(name, transpose); }
// template<> inline Ref<IUniform<const glm::dmat4x3&>> IShaderProgram::getUniform<glm::dmat4x3>(const char* name, bool transpose) noexcept { return getUniformMatrix4x3Double(name, transpose); }
// template<> inline Ref<IUniform<const glm::dmat4x4&>> IShaderProgram::getUniform<glm::dmat4x4>(const char* name, bool transpose) noexcept { return getUniformMatrix4x4Double(name, transpose); }
