#pragma once

#pragma warning(push, 0)
#include <thread>
#pragma warning(pop)

#include <DLL.hpp>
#include <NumTypes.hpp>
#include <system/Window.hpp>
#include <system/Win32Event.hpp>

class TextHandler;
class Vector3f;
class Matrix4x4f;
class ITexture;
class IBufferDescriptor;
class ImDrawData;

enum class RenderingOpcode : u8
{
    FINISH_RENDER = 1,
    LOAD_SHADER_UNIFORM,
    ACTIVATE_SHADER_PROGRAM,
    // ACTIVATE_TEXTURE_UNIT,
    BIND_TEXTURE,
    UNBIND_TEXTURE,
    ENABLE_BUFFER_DESCRIPTOR,
    DISABLE_BUFFER_DESCRIPTOR,
    // BIND_VAO,
    BIND_VBO,
    // ENABLE_VAO_ATTRIBUTE,
    // DISABLE_VAO_ATTRIBUTE,
    GL_DRAW_ARRAYS,
    GL_DRAW_ELEMENTS,
    GL_CLEAR_BUFFERS,
    LOAD_BUFFER_DATA,
    MODIFY_BUFFER_DATA,
    GL_ENABLE,
    GL_DISABLE,
    GL_FACE_WINDING,
    RESIZE_VIEWPORT,
    RENDER_TEXT,
    IMGUI_RENDER
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

class TAU_DLL RenderingPipeline final
{
private:
    u8* _instBuffer;
    u8* _backBuffer;
    volatile u32 _instPtr;
    volatile u32 _insertPtr;
private:
    bool _async;
    std::thread* _renderThread;
    Win32Event   _insertSignal;
    Win32Event   _renderSignal;
    Win32Event   _possession0Signal;
    Win32Event   _possession1Signal;
    Win32Event   _possession2Signal;
    Win32Event   _exitSignal;
public:
    RenderingPipeline(Window* window, const u32 bufferSize = 16384) noexcept;

    RenderingPipeline(const u32 bufferSize = 16384) noexcept;
    
    ~RenderingPipeline() noexcept;

    RenderingPipeline(const RenderingPipeline& copy) noexcept = delete;
    RenderingPipeline(RenderingPipeline&& move) noexcept = delete;

    RenderingPipeline& operator =(const RenderingPipeline& copy) noexcept = delete;
    RenderingPipeline& operator =(RenderingPipeline&& move) noexcept = delete;

#define LOAD_VALUE(__VAR) std::memcpy(reinterpret_cast<void*>(_backBuffer + _insertPtr), reinterpret_cast<const void*>(&(__VAR)), sizeof(__VAR)); \
                          _insertPtr += sizeof(__VAR);

    void pushActivateShaderProgram(const GLuint shaderProgramID) noexcept
    {
        prePushInst<RenderingOpcode::ACTIVATE_SHADER_PROGRAM>();
        LOAD_VALUE(shaderProgramID);
        postPushInst();
    }

    void pushBindTexture(const ITexture* texture, const u8 textureUnit) noexcept
    {
        prePushInst<RenderingOpcode::BIND_TEXTURE>();
        LOAD_VALUE(texture);
        _backBuffer[_insertPtr++] = textureUnit;
        postPushInst();
    }

    void pushUnbindTexture(const ITexture* texture, const u8 textureUnit) noexcept
    {
        prePushInst<RenderingOpcode::UNBIND_TEXTURE>();
        LOAD_VALUE(texture);
        _backBuffer[_insertPtr++] = textureUnit;
        postPushInst();
    }

    void pushEnableBufferDescriptor(const IBufferDescriptor* bufferDescriptor) noexcept
    {
        prePushInst<RenderingOpcode::ENABLE_BUFFER_DESCRIPTOR>();
        LOAD_VALUE(bufferDescriptor);
        postPushInst();
    }
    
    void pushDisableBufferDescriptor(const IBufferDescriptor* bufferDescriptor) noexcept
    {
        prePushInst<RenderingOpcode::DISABLE_BUFFER_DESCRIPTOR>();
        LOAD_VALUE(bufferDescriptor);
        postPushInst();
    }
    
    void pushBindVBO(const GLenum target, const GLuint vboID) noexcept
    {
        prePushInst<RenderingOpcode::BIND_VBO>();
        LOAD_VALUE(target);
        LOAD_VALUE(vboID);
        postPushInst();
    }
    
    void pushGLDrawArrays(const GLenum mode, const GLint first, const GLsizei count) noexcept
    {
        prePushInst<RenderingOpcode::GL_DRAW_ARRAYS>();
        LOAD_VALUE(mode);
        LOAD_VALUE(first);
        LOAD_VALUE(count);
        postPushInst();
    }

    void pushGLDrawElements(const GLenum mode, const GLsizei count, const GLenum type, const void* const indices) noexcept
    {
        prePushInst<RenderingOpcode::GL_DRAW_ELEMENTS>();
        LOAD_VALUE(mode);
        LOAD_VALUE(count);
        LOAD_VALUE(type);
        LOAD_VALUE(indices);
        postPushInst();
    }

    void pushGLClearBuffers(const GLbitfield clearBuffers) noexcept
    {
        prePushInst<RenderingOpcode::GL_CLEAR_BUFFERS>();
        LOAD_VALUE(clearBuffers);
        postPushInst();
    }

    void pushLoadBufferData(const GLenum target, const GLsizeiptr size, const void* const data, GLenum usage) noexcept
    {
        prePushInst<RenderingOpcode::LOAD_BUFFER_DATA>();
        LOAD_VALUE(target);
        LOAD_VALUE(size);

        std::memcpy(reinterpret_cast<void*>(_backBuffer + _insertPtr), data, size);
        _insertPtr += static_cast<u32>(size);

        LOAD_VALUE(usage);
        postPushInst();
    }

    void pushModifyBufferData(const GLenum target, const GLintptr offset, const GLsizeiptr size, const void* const data) noexcept
    {
        prePushInst<RenderingOpcode::MODIFY_BUFFER_DATA>();
        LOAD_VALUE(target);
        LOAD_VALUE(offset);
        LOAD_VALUE(size);

        std::memcpy(reinterpret_cast<void*>(_backBuffer + _insertPtr), data, size);
        _insertPtr += static_cast<u32>(size);

        postPushInst();
    }

    template<RenderingOpcode _Op>
    void pushGLFunc(const GLenum cap) noexcept
    {
        prePushInst<_Op>();
        LOAD_VALUE(cap);
        postPushInst();
    }

    void pushGLEnable(const GLenum cap) noexcept { pushGLFunc<RenderingOpcode::GL_ENABLE>(cap); }
    void pushGLDisable(const GLenum cap) noexcept { pushGLFunc<RenderingOpcode::GL_DISABLE>(cap); }
    void pushGLFaceWinding(const GLenum mode) noexcept { pushGLFunc<RenderingOpcode::GL_FACE_WINDING>(mode); }

    void pushResizeViewport(const GLint x, const GLint y, const GLsizei width, const GLsizei height) noexcept
    {
        prePushInst<RenderingOpcode::RESIZE_VIEWPORT>();
        LOAD_VALUE(x);
        LOAD_VALUE(y);
        LOAD_VALUE(width);
        LOAD_VALUE(height);
        postPushInst();
    }

    void pushRenderText(const TextHandler* th, const char* str, GLfloat x, GLfloat y, GLfloat scale, Vector3f color, const Matrix4x4f& proj) noexcept;

    void pushImGuiRender(const ImDrawData* data) noexcept
    {
        prePushInst<RenderingOpcode::IMGUI_RENDER>();
        LOAD_VALUE(data);
        postPushInst();
    }

    void pushFinishRender() noexcept
    {
        prePushInst<RenderingOpcode::FINISH_RENDER>();
        _insertPtr = 0;
        postPushInst();
        (void) _insertSignal.waitUntilSignaled();
        u8* tmp = _instBuffer;
        _instBuffer = _backBuffer;
        _backBuffer = tmp;
        _renderSignal.signal();
    }

    template<ShaderUniformType _UniType>
    void pushLoadUniform(const ParameterPack&& params) noexcept
    {
        _backBuffer[_insertPtr++] = static_cast<u8>(RenderingOpcode::LOAD_SHADER_UNIFORM);
        _backBuffer[_insertPtr++] = static_cast<u8>(_UniType);

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
                std::memcpy(reinterpret_cast<void*>(_backBuffer + _insertPtr), reinterpret_cast<const void*>(mat), sizeof(float) * 16); 
                _insertPtr += sizeof(float) * 16;
                break;
            }
            default: break;
        }
    }

#undef LOAD_VALUE

    inline void runRenderingCycleSync() noexcept
    {
        if(!_async) { runRenderingCycle(); }
    }

    // void dumpCommandBufferToFile(FILE* file) const noexcept;

    void takeControlOfContext(Window* window) noexcept;

    void returnControlOfContext() noexcept;
private:
    void prePushInst(const RenderingOpcode opcode)
    {
        _backBuffer[_insertPtr++] = static_cast<u8>(opcode);
    }

    template<RenderingOpcode _Opcode>
    void prePushInst() noexcept
    {
        static_assert(_Opcode != RenderingOpcode::LOAD_SHADER_UNIFORM, "`LOAD_SHADER_UNIFORM` cannot be used with `pushInstruction`, you must use `pushLoadUniform`");
        prePushInst(_Opcode);
    }

    void postPushInst() const noexcept { }

    void _passContext(Window* window) const noexcept;

    void runRenderingCycle() noexcept;

    void renderThreadFunc(Window* window) noexcept;
};
