#pragma once

#pragma warning(push, 0)
#include <thread>
#include <mutex>
#pragma warning(pop)

#include <NumTypes.hpp>
#include <system/Window.hpp>
#include <Safeties.hpp>

enum class RenderingOpcode : u8
{
    FINISH_RENDER = 1,
    LOAD_SHADER_UNIFORM,
    ACTIVATE_SHADER_PROGRAM,
    ACTIVATE_TEXTURE_UNIT,
    BIND_TEXTURE,
    BIND_VAO,
    BIND_VBO,
    ENABLE_VAO_ATTRIBUTE,
    DISABLE_VAO_ATTRIBUTE,
    GL_DRAW_ARRAYS,
    GL_DRAW_ELEMENTS,
    GL_CLEAR_BUFFERS,
    LOAD_BUFFER_DATA,
    MODIFY_BUFFER_DATA,
    GL_ENABLE,
    GL_DISABLE,
    GL_FACE_WINDING,
    RESIZE_VIEWPORT
};

enum class ShaderUniformType : u8
{
    INTEGER,
    FLOAT,
    DOUBLE,
    VEC2F,
    VEC3F,
    VEC4F,
    MAT4F
};

#define PACK_FLOAT(__F)  (reinterpret_cast<u32&>(__F))
#define PACK_DOUBLE(__D) (reinterpret_cast<u64&>(__F))
#define PACK_PTR(__P)    (reinterpret_cast<u64 >(__P))

struct ParameterPack final
{
    u64 p0, p1, p2, p3, p4, p5, p6, p7;

    ParameterPack(u64 _p0 = 0, u64 _p1 = 0, u64 _p2 = 0, u64 _p3 = 0, 
                  u64 _p4 = 0, u64 _p5 = 0, u64 _p6 = 0, u64 _p7 = 0) noexcept
        : p0(_p0), p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7)
    { }

    ~ParameterPack() noexcept = default;

    ParameterPack(const ParameterPack&) noexcept = default;
    ParameterPack(ParameterPack&&) noexcept = default;

    ParameterPack& operator =(const ParameterPack&) noexcept = default;
    ParameterPack& operator =(ParameterPack&&) noexcept = default;
};

class RenderingPipeline final
{
private:
    u8* _instBuffer;
    volatile u32 _insertPtr;
    volatile u32 _instPtr;
private:
    volatile bool _initialReady;
    volatile bool _readyForInsert;
    volatile bool _running;
    std::mutex    _mutex;
    std::thread   _renderThread;
public:
    RenderingPipeline(Window* window, const u32 bufferSize = 16384) noexcept;
    
    ~RenderingPipeline() noexcept;

    RenderingPipeline(const RenderingPipeline& copy) noexcept = delete;
    RenderingPipeline(RenderingPipeline&& move) noexcept = delete;

    RenderingPipeline& operator =(const RenderingPipeline& copy) noexcept = delete;
    RenderingPipeline& operator =(RenderingPipeline&& move) noexcept = delete;

    template<RenderingOpcode _Opcode>
    void pushInstruction(const ParameterPack&& params) noexcept
    {
        static_assert(_Opcode == RenderingOpcode::LOAD_SHADER_UNIFORM, "`LOAD_SHADER_UNIFORM` cannot be used with `pushInstruction`, you must use `pushLoadUniform`");

        do
        {
            _mutex.lock();
            if(_readyForInsert)
            {
                _mutex.unlock();
                break;
            }
            _mutex.unlock();
            std::this_thread::yield();
        } while(true);

        _mutex.lock();

        _instBuffer[_insertPtr++] = static_cast<u8>(_Opcode);

#define LOAD_VALUE(__VAR) std::memcpy(reinterpret_cast<volatile void*>(_instBuffer + _insertPtr), reinterpret_cast<const void*>(&__VAR), sizeof(__VAR)); \
                          _insertPtr += sizeof(__VAR);

        switch(_Opcode)
        {
            case RenderingOpcode::FINISH_RENDER:
            {
                _readyForInsert = false;
                break;
            }
            case RenderingOpcode::ACTIVATE_SHADER_PROGRAM:
            {
                const GLuint shaderProgramID = static_cast<GLuint>(params.p0);
                LOAD_VALUE(shaderProgramID);
                break;
            }
            case RenderingOpcode::ACTIVATE_TEXTURE_UNIT:
                _instBuffer[_insertPtr++] = static_cast<u8>(params.p0);
                break;
            case RenderingOpcode::BIND_TEXTURE:
            {
                const GLuint textureID = static_cast<GLuint>(params.p0);
                LOAD_VALUE(textureID);
                break;
            }
            case RenderingOpcode::BIND_VAO:
            {
                const GLuint vaoID = static_cast<GLuint>(params.p0);
                LOAD_VALUE(vaoID);
                break;
            }
            case RenderingOpcode::BIND_VBO:
            {
                const GLenum target = static_cast<GLenum>(params.p0);
                const GLuint vboID = static_cast<GLuint>(params.p1);
                LOAD_VALUE(target);
                LOAD_VALUE(vboID);
                break;
            }
            case RenderingOpcode::ENABLE_VAO_ATTRIBUTE:
            {
                const GLuint vaoAttribEnable = static_cast<GLuint>(params.p0);
                LOAD_VALUE(vaoAttribEnable);
                break;
            }
            case RenderingOpcode::DISABLE_VAO_ATTRIBUTE:
            {
                const GLuint vaoAttribDisable = static_cast<GLuint>(params.p0);
                LOAD_VALUE(vaoAttribDisable);
                break;
            }
            case RenderingOpcode::GL_DRAW_ARRAYS:
            {
                const GLenum  drawArraysMode = static_cast<GLenum>(params.p0);
                const GLint   drawArraysFirst = static_cast<GLint>(params.p1);
                const GLsizei drawArraysCount = static_cast<GLsizei>(params.p2);

                LOAD_VALUE(drawArraysMode);
                LOAD_VALUE(drawArraysFirst);
                LOAD_VALUE(drawArraysCount);
                break;
            }
            case RenderingOpcode::GL_DRAW_ELEMENTS:
            {
                const GLenum      drawElemsMode = static_cast<GLenum>(params.p0);
                const GLsizei     drawElemsCount = static_cast<GLsizei>(params.p1);
                const GLenum      drawElemsType = static_cast<GLenum>(params.p2);
                const void* const drawElemsIndices = reinterpret_cast<const void*>(params.p3);

                LOAD_VALUE(drawElemsMode);
                LOAD_VALUE(drawElemsCount);
                LOAD_VALUE(drawElemsType);
                LOAD_VALUE(drawElemsIndices);
                break;
            }
            case RenderingOpcode::GL_CLEAR_BUFFERS:
            {
                const GLbitfield clearBuffers = static_cast<GLbitfield>(params.p0);
                LOAD_VALUE(clearBuffers);
                break;
            }
            case RenderingOpcode::LOAD_BUFFER_DATA:
            {
                const GLenum target = static_cast<GLenum>(params.p0);
                const GLsizeiptr size = static_cast<GLsizeiptr>(params.p1);
                const void* data = reinterpret_cast<const void*>(params.p2);
                const GLenum usage = static_cast<GLenum>(params.p3);
                LOAD_VALUE(target);
                LOAD_VALUE(size);

                std::memcpy(reinterpret_cast<void*>(_instBuffer + _insertPtr), data, size);
                _insertPtr += size;

                LOAD_VALUE(usage);
                break;
            }
            case RenderingOpcode::MODIFY_BUFFER_DATA:
            {
                const GLenum target = static_cast<GLenum>(params.p0);
                const GLintptr offset = static_cast<GLenum>(params.p1);
                const GLsizeiptr size = static_cast<GLsizeiptr>(params.p2);
                const void* data = reinterpret_cast<const void*>(params.p3);
                LOAD_VALUE(target);
                LOAD_VALUE(offset);
                LOAD_VALUE(size);

                std::memcpy(reinterpret_cast<void*>(_instBuffer + _insertPtr), data, size);
                _insertPtr += size;
                break;
            }
            case RenderingOpcode::GL_ENABLE:
            case RenderingOpcode::GL_DISABLE:
            case RenderingOpcode::GL_FACE_WINDING:
            {
                const GLenum cap = static_cast<GLenum>(params.p0);
                LOAD_VALUE(cap);
                break;
            }
            case RenderingOpcode::RESIZE_VIEWPORT:
            {
                const GLint x = static_cast<GLint>(params.p0);
                const GLint y = static_cast<GLint>(params.p1);
                const GLsizei width = static_cast<GLsizei>(params.p2);
                const GLsizei height = static_cast<GLsizei>(params.p3);
                LOAD_VALUE(x);
                LOAD_VALUE(y);
                LOAD_VALUE(width);
                LOAD_VALUE(height);
                break;
            }
            default: 
            {
                RUNTIME_ERROR("Invalid Opcode Pushed");
                break;
            }
        }

#undef LOAD_VALUE

        _mutex.unlock();
    }

    template<RenderingOpcode _Opcode>
    void pushInstruction(const u64 param) noexcept
    {
        static_assert(_Opcode == RenderingOpcode::LOAD_SHADER_UNIFORM, "`RenderingOpcode::LOAD_SHADER_UNIFORM` cannot be used with `pushInstruction(u64)`, you must use `pushInstruction(const ParameterPack&&)`");
        static_assert(_Opcode == RenderingOpcode::BIND_VBO, "`RenderingOpcode::BIND_VBO` cannot be used with `pushInstruction()`, you must use `pushInstruction(const ParameterPack&&)`");
        static_assert(_Opcode == RenderingOpcode::GL_DRAW_ARRAYS, "`RenderingOpcode::GL_DRAW_ARRAYS` cannot be used with `pushInstruction(u64)`, you must use `pushInstruction(const ParameterPack&&)`");
        static_assert(_Opcode == RenderingOpcode::GL_DRAW_ELEMENTS, "`RenderingOpcode::GL_DRAW_ELEMENTS` cannot be used with `pushInstruction(u64)`, you must use `pushInstruction(const ParameterPack&&)`");
        static_assert(_Opcode == RenderingOpcode::LOAD_BUFFER_DATA, "`RenderingOpcode::LOAD_BUFFER_DATA` cannot be used with `pushInstruction(u64)`, you must use `pushInstruction(const ParameterPack&&)`");
        static_assert(_Opcode == RenderingOpcode::MODIFY_BUFFER_DATA, "`RenderingOpcode::MODIFY_BUFFER_DATA` cannot be used with `pushInstruction(u64)`, you must use `pushInstruction(const ParameterPack&&)`");
        static_assert(_Opcode == RenderingOpcode::RESIZE_VIEWPORT, "`RenderingOpcode::RESIZE_VIEWPORT` cannot be used with `pushInstruction(u64)`, you must use `pushInstruction(const ParameterPack&&)`");

        do
        {
            _mutex.lock();
            if(_readyForInsert)
            {
                _mutex.unlock();
                break;
            }
            _mutex.unlock();
            std::this_thread::yield();
        } while(true);

        _mutex.lock();

        _instBuffer[_insertPtr++] = static_cast<u8>(_Opcode);

#define LOAD_VALUE(__VAR) std::memcpy(reinterpret_cast<volatile void*>(_instBuffer + _insertPtr), reinterpret_cast<const void*>(&__VAR), sizeof(__VAR)); \
                          _insertPtr += sizeof(__VAR);

        switch(_Opcode)
        {
            case RenderingOpcode::FINISH_RENDER:
            {
                _readyForInsert = false;
                break;
            }
            case RenderingOpcode::ACTIVATE_SHADER_PROGRAM:
            {
                const GLuint shaderProgramID = static_cast<GLuint>(param);
                LOAD_VALUE(shaderProgramID);
                break;
            }
            case RenderingOpcode::ACTIVATE_TEXTURE_UNIT:
                _instBuffer[_insertPtr++] = static_cast<u8>(param);
                break;
            case RenderingOpcode::BIND_TEXTURE:
            {
                const GLuint textureID = static_cast<GLuint>(param);
                LOAD_VALUE(textureID);
                break;
            }
            case RenderingOpcode::BIND_VAO:
            {
                const GLuint vaoID = static_cast<GLuint>(param);
                LOAD_VALUE(vaoID);
                break;
            }
            case RenderingOpcode::ENABLE_VAO_ATTRIBUTE:
            {
                const GLuint vaoAttribEnable = static_cast<GLuint>(param);
                LOAD_VALUE(vaoAttribEnable);
                break;
            }
            case RenderingOpcode::DISABLE_VAO_ATTRIBUTE:
            {
                const GLuint vaoAttribDisable = static_cast<GLuint>(param);
                LOAD_VALUE(vaoAttribDisable);
                break;
            }
            case RenderingOpcode::GL_CLEAR_BUFFERS:
            {
                const GLbitfield clearBuffers = static_cast<GLbitfield>(param);
                LOAD_VALUE(clearBuffers);
                break;
            }
            case RenderingOpcode::GL_ENABLE:
            case RenderingOpcode::GL_DISABLE:
            case RenderingOpcode::GL_FACE_WINDING:
            {
                const GLenum cap = static_cast<GLenum>(param);
                LOAD_VALUE(cap);
                break;
            }
            default: break;
        }

#undef LOAD_VALUE

        _mutex.unlock();
    }

    template<RenderingOpcode _Opcode, u64 _Param>
    void pushInstruction() noexcept
    {
        static_assert(_Opcode == RenderingOpcode::LOAD_SHADER_UNIFORM, "`RenderingOpcode::LOAD_SHADER_UNIFORM` cannot be used with `pushInstruction()`, you must use `pushInstruction(const ParameterPack&&)`");
        static_assert(_Opcode == RenderingOpcode::ACTIVATE_SHADER_PROGRAM, "`RenderingOpcode::ACTIVATE_SHADER_PROGRAM` cannot be used with `pushInstruction()`, you must use `pushInstruction(const ParameterPack&&)` or `pushInstruction(u64)`");
        static_assert(_Opcode == RenderingOpcode::BIND_VAO, "`RenderingOpcode::BIND_VAO` cannot be used with `pushInstruction()`, you must use `pushInstruction(const ParameterPack&&)` or `pushInstruction(u64)`");
        static_assert(_Opcode == RenderingOpcode::BIND_VBO, "`RenderingOpcode::BIND_VBO` cannot be used with `pushInstruction()`, you must use `pushInstruction(const ParameterPack&&)`");
        static_assert(_Opcode == RenderingOpcode::GL_DRAW_ARRAYS, "`RenderingOpcode::GL_DRAW_ARRAYS` cannot be used with `pushInstruction()`, you must use `pushInstruction(const ParameterPack&&)`");
        static_assert(_Opcode == RenderingOpcode::GL_DRAW_ELEMENTS, "`RenderingOpcode::GL_DRAW_ELEMENTS` cannot be used with `pushInstruction()`, you must use `pushInstruction(const ParameterPack&&)`");
        static_assert(_Opcode == RenderingOpcode::LOAD_BUFFER_DATA, "`RenderingOpcode::LOAD_BUFFER_DATA` cannot be used with `pushInstruction()`, you must use `pushInstruction(const ParameterPack&&)`");
        static_assert(_Opcode == RenderingOpcode::MODIFY_BUFFER_DATA, "`RenderingOpcode::MODIFY_BUFFER_DATA` cannot be used with `pushInstruction()`, you must use `pushInstruction(const ParameterPack&&)`");
        static_assert(_Opcode == RenderingOpcode::RESIZE_VIEWPORT, "`RenderingOpcode::RESIZE_VIEWPORT` cannot be used with `pushInstruction()`, you must use `pushInstruction(const ParameterPack&&)`");

        do
        {
            _mutex.lock();
            if(_readyForInsert)
            {
                _mutex.unlock();
                break;
            }
            _mutex.unlock();
            std::this_thread::yield();
        } while(true);

        _mutex.lock();

        _instBuffer[_insertPtr++] = static_cast<u8>(_Opcode);

#define LOAD_VALUE(__VAR) std::memcpy(reinterpret_cast<volatile void*>(_instBuffer + _insertPtr), reinterpret_cast<const void*>(&__VAR), sizeof(__VAR)); \
                          _insertPtr += sizeof(__VAR);

        switch(_Opcode)
        {
            case RenderingOpcode::FINISH_RENDER:
            {
                _readyForInsert = false;
                break;
            }
            case RenderingOpcode::ACTIVATE_TEXTURE_UNIT:
                _instBuffer[_insertPtr++] = static_cast<u8>(_Param);
                break;
            case RenderingOpcode::BIND_TEXTURE:
            {
                const GLuint textureID = static_cast<GLuint>(_Param);
                LOAD_VALUE(textureID);
                break;
            }
            case RenderingOpcode::BIND_VAO:
            {
                const GLuint vaoID = static_cast<GLuint>(_Param);
                LOAD_VALUE(vaoID);
                break;
            }
            case RenderingOpcode::ENABLE_VAO_ATTRIBUTE:
            {
                const GLuint vaoAttribEnable = static_cast<GLuint>(_Param);
                LOAD_VALUE(vaoAttribEnable);
                break;
            }
            case RenderingOpcode::DISABLE_VAO_ATTRIBUTE:
            {
                const GLuint vaoAttribDisable = static_cast<GLuint>(_Param);
                LOAD_VALUE(vaoAttribDisable);
                break;
            }
            case RenderingOpcode::GL_CLEAR_BUFFERS:
            {
                const GLbitfield clearBuffers = static_cast<GLbitfield>(_Param);
                LOAD_VALUE(clearBuffers);
                break;
            }
            case RenderingOpcode::GL_ENABLE:
            case RenderingOpcode::GL_DISABLE:
            case RenderingOpcode::GL_FACE_WINDING:
            {
                const GLenum cap = static_cast<GLenum>(_Param);
                LOAD_VALUE(cap);
                break;
            }
            default: break;
        }

#undef LOAD_VALUE

        _mutex.unlock();
    }

    template<ShaderUniformType _UniType>
    void pushLoadUniform(const ParameterPack&& params) noexcept
    {
        do
        {
            _mutex.lock();
            if(_readyForInsert)
            {
                _mutex.unlock();
                break;
            }
            _mutex.unlock();
            std::this_thread::yield();
        } while(true);

        _mutex.lock();


#define LOAD_VALUE(__VAR) std::memcpy(reinterpret_cast<volatile void*>(_instBuffer + _insertPtr), reinterpret_cast<const void*>(&__VAR), sizeof(__VAR)); \
                          _insertPtr += sizeof(__VAR);

        _instBuffer[_insertPtr++] = static_cast<u8>(RenderingOpcode::LOAD_SHADER_UNIFORM);
        _instBuffer[_insertPtr++] = static_cast<u8>(_UniType);

        const i32 uniformID = static_cast<i32>(params.p0);
        LOAD_VALUE(uniformID);

        switch(_UniType)
        {
            case ShaderUniformType::INTEGER:
            {
                LOAD_VALUE(params.p1);
                break;
            }
            case ShaderUniformType::FLOAT:
            {
                const u32 iVal = static_cast<u32>(params.p1);
                const float fVal = reinterpret_cast<const float&>(iVal);
                LOAD_VALUE(fVal);
                break;
            }
            case ShaderUniformType::DOUBLE:
            {
                LOAD_VALUE(params.p1);
                break;
            }
            case ShaderUniformType::VEC2F:
            {
                const u32 iv0 = static_cast<u32>(params.p1);
                const float fv0 = reinterpret_cast<const float&>(iv0);
                LOAD_VALUE(fv0);
                const u32 iv1 = static_cast<u32>(params.p2);
                const float fv1 = reinterpret_cast<const float&>(iv1);
                LOAD_VALUE(fv1);
                break;
            }
            case ShaderUniformType::VEC3F:
            {
                const u32 iv0 = static_cast<u32>(params.p1);
                const float fv0 = reinterpret_cast<const float&>(iv0);
                LOAD_VALUE(fv0);
                const u32 iv1 = static_cast<u32>(params.p2);
                const float fv1 = reinterpret_cast<const float&>(iv1);
                LOAD_VALUE(fv1);
                const u32 iv2 = static_cast<u32>(params.p3);
                const float fv2 = reinterpret_cast<const float&>(iv2);
                LOAD_VALUE(fv2);
                break;
            }
            case ShaderUniformType::VEC4F:
            {
                const u32 iv0 = static_cast<u32>(params.p1);
                const float fv0 = reinterpret_cast<const float&>(iv0);
                LOAD_VALUE(fv0);
                const u32 iv1 = static_cast<u32>(params.p2);
                const float fv1 = reinterpret_cast<const float&>(iv1);
                LOAD_VALUE(fv1);
                const u32 iv2 = static_cast<u32>(params.p3);
                const float fv2 = reinterpret_cast<const float&>(iv2);
                LOAD_VALUE(fv2);
                const u32 iv3 = static_cast<u32>(params.p4);
                const float fv3 = reinterpret_cast<const float&>(iv3);
                LOAD_VALUE(fv3);
                break;
            }
            case ShaderUniformType::MAT4F:
            {
                const float* const mat = reinterpret_cast<const float*>(params.p1);
                LOAD_VALUE(mat);
                break;
            }
            default: break;
        }

#undef LOAD_VALUE

        _mutex.unlock();
    }
private:
    void pushInstruction(const RenderingOpcode opcode, const ParameterPack&& params) noexcept;

    void runRenderingCycle() noexcept;

    void renderThreadFunc(Window* window) noexcept;
};
