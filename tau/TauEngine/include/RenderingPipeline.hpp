/**
 * @file
 */
#pragma once

#pragma warning(push, 0)
#include <thread>
#include <GL/glew.h>
#pragma warning(pop)

#include <Objects.hpp>
#include <maths/Vector2f.hpp>
#include <maths/Vector3f.hpp>
#include <maths/Vector4f.hpp>
#include <maths/Matrix4x4f.hpp>
#include <DLL.hpp>
#include <NumTypes.hpp>
#include <system/Window.hpp>
#include <system/Win32Event.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

class TextHandler;
class ITexture;
class IBufferDescriptor;
struct ImDrawData;
class IRenderingContext;

enum class RenderingOpcode : u16
{
    FINISH_RENDER = 1,
    LOAD_CONTEXT,
    LOAD_SHADER_UNIFORM,
    ACTIVATE_SHADER_PROGRAM,
    BIND_TEXTURE,
    UNBIND_TEXTURE,
    ENABLE_BUFFER_DESCRIPTOR,
    DISABLE_BUFFER_DESCRIPTOR,
    BIND_VBO,
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
    IMGUI_RENDER,
    LAST = IMGUI_RENDER
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

class TAU_DLL RenderingPipeline final
{
    DELETE_COPY(RenderingPipeline);
public:
    /**
     *   A handle to function which runs some code on the
     * rendering thread. This is intended supersede the context
     * switching methods.
     *
     *   The function receives a reference to the rendering
     * pipeline, a reference to the window, and a void* to a user
     * set parameter. This can be anything that fits in an
     * intptr_t, or a pointer to any object. The lifetime of this
     * object is decided by the caller. It is recommended that it
     * is heap allocated and freed within the function pointed to
     * by this handle.
     */
    typedef void (*__cdecl ctxCtrl_f)(RenderingPipeline&, Window&, void*);

    using CtxCtrlObj = std::pair<ctxCtrl_f, void*>;
private:
    Window& _window;

    u8* _instBuffer;
    u8* _backBuffer;
    volatile u32 _instPtr;
    volatile u32 _insertPtr;

    CtxCtrlObj* _ctxCtrlsFront;
    CtxCtrlObj* _ctxCtrlsBack;
    u32 _ctxCtrlsInsertPtr;
    u32 _ctxCtrlsSize;
private:
    bool _async;
    std::thread* _renderThread;
    Win32Event   _insertSignal;
    Win32Event   _renderSignal;
    Win32Event   _exitSignal;
public:
    RenderingPipeline(Window& window, ctxCtrl_f setupParams, void* setupParam, bool async, u32 bufferSize = (16*1024*1024), u32 ctxControlsSize = 512) noexcept;
    ~RenderingPipeline() noexcept;

#define LOAD_VALUE(__VAR) std::memcpy(reinterpret_cast<void*>(_backBuffer + _insertPtr), reinterpret_cast<const void*>(&(__VAR)), sizeof(__VAR)); \
                          _insertPtr += sizeof(__VAR);

    void pushLoadContext(const IRenderingContext& context) noexcept
    {
        prePushInst<RenderingOpcode::LOAD_CONTEXT>();
        const IRenderingContext* contextPtr = &context;
        LOAD_VALUE(contextPtr);
        postPushInst();
    }

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

    void pushRenderText(const TextHandler* th, const char* str, GLfloat x, GLfloat y, GLfloat scale, u8 cr, u8 cg, u8 cb, const glm::mat4& proj) noexcept;

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
        _ctxCtrlsInsertPtr = 0;
        postPushInst();
        if(_async)
        {
            (void) _insertSignal.waitUntilSignaled();
        }

        u8* tmp = _instBuffer;
        _instBuffer = _backBuffer;
        _backBuffer = tmp;

        CtxCtrlObj* tmp0 = _ctxCtrlsFront;
        _ctxCtrlsFront = _ctxCtrlsBack;
        _ctxCtrlsBack = tmp0;

        if(_async)
        {
            _renderSignal.signal();
        }
        else
        {
            runRenderingCycle();
        }
    }

    void pushLoadUniInt(GLint uniformID, GLint param) noexcept
    {
        prePushInst<RenderingOpcode::LOAD_SHADER_UNIFORM>();
        _backBuffer[_insertPtr++] = static_cast<u8>(ShaderUniformType::INTEGER);
        LOAD_VALUE(uniformID);
        LOAD_VALUE(param);
        postPushInst();
    }

    void pushLoadUniFloat(GLint uniformID, GLfloat param) noexcept
    {
        prePushInst<RenderingOpcode::LOAD_SHADER_UNIFORM>();
        _backBuffer[_insertPtr++] = static_cast<u8>(ShaderUniformType::FLOAT);
        LOAD_VALUE(uniformID);
        LOAD_VALUE(param);
        postPushInst();
    }

    void pushLoadUniDouble(GLint uniformID, GLdouble param) noexcept
    {
        prePushInst<RenderingOpcode::LOAD_SHADER_UNIFORM>();
        _backBuffer[_insertPtr++] = static_cast<u8>(ShaderUniformType::DOUBLE);
        LOAD_VALUE(uniformID);
        LOAD_VALUE(param);
        postPushInst();
    }

    void pushLoadUniVec2f(GLint uniformID, Vector2f param) noexcept
    {
        prePushInst<RenderingOpcode::LOAD_SHADER_UNIFORM>();
        _backBuffer[_insertPtr++] = static_cast<u8>(ShaderUniformType::VEC2F);
        LOAD_VALUE(uniformID);
        LOAD_VALUE(param.x());
        LOAD_VALUE(param.y());
        postPushInst();
    }

    void pushLoadUniVec3f(GLint uniformID, Vector3f param) noexcept
    {
        prePushInst<RenderingOpcode::LOAD_SHADER_UNIFORM>();
        _backBuffer[_insertPtr++] = static_cast<u8>(ShaderUniformType::VEC3F);
        LOAD_VALUE(uniformID);
        LOAD_VALUE(param.x());
        LOAD_VALUE(param.y());
        LOAD_VALUE(param.z());
        postPushInst();
    }

    void pushLoadUniVec4f(GLint uniformID, Vector4f param) noexcept
    {
        prePushInst<RenderingOpcode::LOAD_SHADER_UNIFORM>();
        _backBuffer[_insertPtr++] = static_cast<u8>(ShaderUniformType::VEC4F);
        LOAD_VALUE(uniformID);
        LOAD_VALUE(param.x());
        LOAD_VALUE(param.y());
        LOAD_VALUE(param.z());
        LOAD_VALUE(param.w());
        postPushInst();
    }

    void pushLoadUniMat4f(GLint uniformID, const glm::mat4& param) noexcept
    {
        prePushInst<RenderingOpcode::LOAD_SHADER_UNIFORM>();
        _backBuffer[_insertPtr++] = static_cast<u8>(ShaderUniformType::MAT4F);
        LOAD_VALUE(uniformID);
        std::memcpy(reinterpret_cast<void*>(_backBuffer + _insertPtr), reinterpret_cast<const void*>(glm::value_ptr(param)), sizeof(float) * 16);
        _insertPtr += sizeof(float) * 16;
        postPushInst();
    }

    void pushLoadUni(GLint uniformID, GLint param) noexcept { pushLoadUniInt(uniformID, param); }
    void pushLoadUni(GLint uniformID, GLfloat param) noexcept { pushLoadUniFloat(uniformID, param); }
    void pushLoadUni(GLint uniformID, GLdouble param) noexcept { pushLoadUniDouble(uniformID, param); }
    void pushLoadUni(GLint uniformID, Vector2f param) noexcept { pushLoadUniVec2f(uniformID, param); }
    void pushLoadUni(GLint uniformID, Vector3f param) noexcept { pushLoadUniVec3f(uniformID, param); }
    void pushLoadUni(GLint uniformID, Vector4f param) noexcept { pushLoadUniVec4f(uniformID, param); }
    void pushLoadUni(GLint uniformID, const glm::mat4& param) noexcept { pushLoadUniMat4f(uniformID, param); }

    /**
     *   Add a function to the pipeline to run before the
     * rendering cycle.
     *
     * @param [in] func
     *   The function to be called.
     * @param [in] data
     *   An arbitrary set of data to pass to func.
     * @param [in] replace
     *   Replace the last entry if the function pointer matches.
     */
    bool addCtxCtrl(ctxCtrl_f func, void* data, bool replace = false) noexcept
    {
        if(_ctxCtrlsInsertPtr >= _ctxCtrlsSize) 
        { return false; }
        if(replace && _ctxCtrlsInsertPtr && 
           _ctxCtrlsBack[_ctxCtrlsInsertPtr - 1].first == func)
        { --_ctxCtrlsInsertPtr; }

        _ctxCtrlsBack[_ctxCtrlsInsertPtr].first = func;
        _ctxCtrlsBack[_ctxCtrlsInsertPtr].second = data;
        ++_ctxCtrlsInsertPtr;
        return true;
    }

    template<typename _T>
    bool addCtxCtrlT(void(*__cdecl func)(RenderingPipeline&, Window&, _T*), _T* data, bool replace = false) noexcept
    {
        return addCtxCtrl(reinterpret_cast<ctxCtrl_f>(func), reinterpret_cast<void*>(data), replace);
    }
private:
    template<RenderingOpcode _Opcode>
    void prePushInst() noexcept
    {
        const u16 op = static_cast<u16>(_Opcode);
        LOAD_VALUE(op);
    }

    void postPushInst() const noexcept { }

#undef LOAD_VALUE

    void runRenderingCycle() noexcept;

    void renderThreadFunc(ctxCtrl_f setupParams, void* setupParam) noexcept;
};
