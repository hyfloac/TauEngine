#pragma warning(push, 0)
#include <cstring>
#include <GL/glew.h>
#include <Windows.h>
#pragma warning(pop)

#include <TextHandler.hpp>
#include <RenderingPipeline.hpp>
#include <gl/GLUtils.hpp>
#include <texture/Texture.hpp>
#include <model/BufferDescriptor.hpp>
#include <maths/Vector3f.hpp>
#include <maths/Matrix4x4f.hpp>
#include <imgui/ImGuiGLImpl.hpp>

RenderingPipeline::RenderingPipeline(Window* window, const u32 bufferSize) noexcept
    : _instBuffer(new u8[bufferSize]), _backBuffer(new u8[bufferSize]), _instPtr(0), _insertPtr(0),
      _async(true), _renderThread(null)
{
    /* Unload context on main thread.
       The context will be loaded on the rendering thread.
       Not entirely sure if this required.
     */
    Window::unloadCurrentContext();
    std::memset(reinterpret_cast<void*>(_instBuffer), 0, bufferSize);
    std::memset(reinterpret_cast<void*>(_backBuffer), 0, bufferSize);
    _renderThread = new std::thread(&RenderingPipeline::renderThreadFunc, this, window);
}

RenderingPipeline::RenderingPipeline(const u32 bufferSize) noexcept
    : _instBuffer(new u8[bufferSize]), _backBuffer(new u8[bufferSize]), _instPtr(0), _insertPtr(0),
      _async(false), _renderThread(null)
{
    std::memset(reinterpret_cast<void*>(_instBuffer), 0, bufferSize);
    std::memset(reinterpret_cast<void*>(_backBuffer), 0, bufferSize);
}

RenderingPipeline::~RenderingPipeline() noexcept 
{ 
    _exitSignal.signal();
    if(_renderThread) { _renderThread->join(); }
    delete _renderThread;

    delete[] _instBuffer; 
    delete[] _backBuffer; 
}

#define LOAD_VALUE(__VAR) std::memcpy(reinterpret_cast<void*>(_backBuffer + _insertPtr), reinterpret_cast<const void*>(&(__VAR)), sizeof(__VAR)); \
                          _insertPtr += sizeof(__VAR);

void RenderingPipeline::pushRenderText(const TextHandler* th, const char* str, GLfloat x, GLfloat y, GLfloat scale, Vector3f color, const Matrix4x4f& proj) noexcept
{
    prePushInst<RenderingOpcode::RENDER_TEXT>();
    LOAD_VALUE(th);
    LOAD_VALUE(x);
    LOAD_VALUE(y);
    LOAD_VALUE(scale);
    LOAD_VALUE(color.x());
    LOAD_VALUE(color.y());
    LOAD_VALUE(color.z());

    const float* const mat = reinterpret_cast<const float*>(proj.data().m);
    std::memcpy(reinterpret_cast<void*>(_backBuffer + _insertPtr), reinterpret_cast<const void*>(mat), sizeof(float) * 16);
    _insertPtr += sizeof(float) * 16;

    const u32 len = static_cast<u32>(strlen(str) + 1);
    LOAD_VALUE(len);

    (void) std::memcpy(reinterpret_cast<void*>(_backBuffer + _insertPtr), reinterpret_cast<const void*>(str), sizeof(char) * len);
    _insertPtr += sizeof(char) * len;

    postPushInst();
}

#define GET_VALUE0(__TYPE, __VAR, __PTR) __TYPE __VAR = *reinterpret_cast<__TYPE*>(_instBuffer + (__PTR)); \
                                         (__PTR) += sizeof(__VAR);

#define GET_VALUE(__TYPE, __VAR) GET_VALUE0(__TYPE, __VAR, _instPtr)

static inline void rpBindTexture(u8* _instBuffer, volatile u32& _instPtr) noexcept
{
    GET_VALUE(ITexture*, texture);
    GET_VALUE(u8, textureUnit);

    texture->bind(textureUnit);
}

static inline void rpUnbindTexture(u8* _instBuffer, volatile u32& _instPtr) noexcept
{
    GET_VALUE(ITexture*, texture);
    GET_VALUE(u8, textureUnit);

    texture->unbind(textureUnit);
}

static inline void rpEnableBufferDescriptor(u8* _instBuffer, volatile u32& _instPtr) noexcept
{
    GET_VALUE(IBufferDescriptor*, bufferDescriptor);

    bufferDescriptor->bind();
    bufferDescriptor->enableAttributes();
}

static inline void rpDisableBufferDescriptor(u8* _instBuffer, volatile u32& _instPtr) noexcept
{
    GET_VALUE(IBufferDescriptor*, bufferDescriptor);

    bufferDescriptor->disableAttributes();
    bufferDescriptor->unbind();
}

static inline void rpRenderText(u8* _instBuffer, volatile u32& _instPtr) noexcept
{
    GET_VALUE(TextHandler*, th);
    GET_VALUE(GLfloat, x);
    GET_VALUE(GLfloat, y);
    GET_VALUE(GLfloat, scale);
    GET_VALUE(GLfloat, r);
    GET_VALUE(GLfloat, g);
    GET_VALUE(GLfloat, b);
    Vector3f color(r, g, b);

    float* matPtr = reinterpret_cast<float*>(_instBuffer + _instPtr);
    _instPtr += sizeof(float) * 16;
    Matrix4f mat;
    (void) std::memcpy(reinterpret_cast<void*>(mat.data().m), reinterpret_cast<void*>(matPtr), sizeof(float) * 16);

    GET_VALUE(u32, len);

    char* str = reinterpret_cast<char*>(_instBuffer + _instPtr);
    _instPtr += sizeof(char) * len;

    th->renderText(str, x, y, scale, color, mat);
}

void RenderingPipeline::runRenderingCycle() noexcept
{
    _instPtr = 0;

    while(true)
    {
        const u8 instByte = _instBuffer[_instPtr++];
        const RenderingOpcode currOpcode = static_cast<RenderingOpcode>(instByte);

        if(currOpcode == RenderingOpcode::FINISH_RENDER) { break; }

        switch(currOpcode)
        {
            case RenderingOpcode::FINISH_RENDER: break;
            case RenderingOpcode::LOAD_SHADER_UNIFORM:
            {
                GET_VALUE(ShaderUniformType, uniformType);
                GET_VALUE(i32, uniformID);

                switch(uniformType)
                {
                    case ShaderUniformType::INTEGER:
                    {
                        GET_VALUE(u64, iVal);
                        glUniform1i(uniformID, static_cast<GLint>(iVal));
                        break;
                    }
                    case ShaderUniformType::FLOAT:
                    {
                        GET_VALUE(float, fVal);
                        glUniform1f(uniformID, fVal);
                        break;
                    }
                    case ShaderUniformType::DOUBLE:
                    {
                        GET_VALUE(double, dVal);
                        glUniform1d(uniformID, dVal);
                        break;
                    }
                    case ShaderUniformType::VEC2F:
                    {
                        GET_VALUE(float, fv0);
                        GET_VALUE(float, fv1);
                        glUniform2f(uniformID, fv0, fv1);
                        break;
                    }
                    case ShaderUniformType::VEC3F:
                    {
                        GET_VALUE(float, fv0);
                        GET_VALUE(float, fv1);
                        GET_VALUE(float, fv2);
                        glUniform3f(uniformID, fv0, fv1, fv2);
                        break;
                    }
                    case ShaderUniformType::VEC4F:
                    {
                        GET_VALUE(float, fv0);
                        GET_VALUE(float, fv1);
                        GET_VALUE(float, fv2);
                        GET_VALUE(float, fv3);
                        glUniform4f(uniformID, fv0, fv1, fv2, fv3);
                        break;
                    }
                    case ShaderUniformType::MAT4F:
                    {
                        float* mat = reinterpret_cast<float*>(_instBuffer + _instPtr); 
                        _instPtr += sizeof(float) * 16;
                        glUniformMatrix4fv(uniformID, 1, GL_FALSE, mat);
                        break;
                    }
                    default: break;
                }
                break;
            }
            case RenderingOpcode::ACTIVATE_SHADER_PROGRAM:
            {
                GET_VALUE(GLuint, shaderProgramID);
                glUseProgram(shaderProgramID);
                break;
            }
            case RenderingOpcode::BIND_TEXTURE:
            {
                rpBindTexture(_instBuffer, _instPtr);
                break;
            }
            case RenderingOpcode::UNBIND_TEXTURE:
            {
                rpUnbindTexture(_instBuffer, _instPtr);
                break;
            }
            case RenderingOpcode::ENABLE_BUFFER_DESCRIPTOR:
            {
                rpEnableBufferDescriptor(_instBuffer, _instPtr);
                break;
            }
            case RenderingOpcode::DISABLE_BUFFER_DESCRIPTOR:
            {
                rpDisableBufferDescriptor(_instBuffer, _instPtr);
                break;
            }
            case RenderingOpcode::BIND_VBO:
            {
                GET_VALUE(GLenum, target);
                GET_VALUE(GLuint, vboID);
                glBindBuffer(target, vboID);
                break;
            }
            case RenderingOpcode::GL_DRAW_ARRAYS:
            {
                GET_VALUE(GLenum, drawArraysMode);
                GET_VALUE(GLint, drawArraysFirst);
                GET_VALUE(GLsizei, drawArraysCount);

                glDrawArrays(drawArraysMode, drawArraysFirst, drawArraysCount);
                break;
            }
            case RenderingOpcode::GL_DRAW_ELEMENTS:
            {
                GET_VALUE(GLenum, drawElemsMode);
                GET_VALUE(GLsizei, drawElemsCount);
                GET_VALUE(GLenum, drawElemsType);
                GET_VALUE(void*, drawElemsIndices);

                glDrawElements(drawElemsMode, drawElemsCount, drawElemsType, drawElemsIndices);
                break;
            }
            case RenderingOpcode::GL_CLEAR_BUFFERS:
            {
                GET_VALUE(GLbitfield, clearBuffers);
                glClear(clearBuffers);
                break;
            }
            case RenderingOpcode::LOAD_BUFFER_DATA:
            {
                GET_VALUE(GLenum, target);
                GET_VALUE(GLsizeiptr, size);

                const void* data = reinterpret_cast<const void*>(_instBuffer + _instPtr);
                _instPtr += static_cast<u32>(size);

                GET_VALUE(GLenum, usage);

                glBufferData(target, size, data, usage);
                break;
            }
            case RenderingOpcode::MODIFY_BUFFER_DATA:
            {
                GET_VALUE(GLenum, target);
                GET_VALUE(GLintptr, offset);
                GET_VALUE(GLsizeiptr, size);

                const void* data = reinterpret_cast<const void*>(_instBuffer + _instPtr);
                _instPtr += static_cast<u32>(size);

                glBufferSubData(target, offset, size, data);
                break;
            }
            case RenderingOpcode::GL_ENABLE:
            {
                GET_VALUE(GLenum, cap);
                glEnable(cap);
                break;
            }
            case RenderingOpcode::GL_DISABLE:
            {
                GET_VALUE(GLenum, cap);
                glDisable(cap);
                break;
            }
            case RenderingOpcode::GL_FACE_WINDING:
            {
                GET_VALUE(GLenum, mode);
                glFrontFace(mode);
                break;
            }
            case RenderingOpcode::RESIZE_VIEWPORT:
            {
                GET_VALUE(GLint, x);
                GET_VALUE(GLint, y);
                GET_VALUE(GLsizei, width);
                GET_VALUE(GLsizei, height);
                glViewport(x, y, width, height);
                break;
            }
            case RenderingOpcode::RENDER_TEXT:
            {
                rpRenderText(_instBuffer, _instPtr);
                break;
            }
            case RenderingOpcode::IMGUI_RENDER:
            {
                GET_VALUE(ImDrawData*, data);
                ImGui_ImplGL_Render(data);
                break;
            }
            default: break;
        }
    }

    std::memset(_instBuffer, 0, _instPtr);
}
#undef GET_VALUE

void RenderingPipeline::_passContext(Window* window) const noexcept
{
    if(_possession0Signal.checkIfSignaled())
    {
        Window::unloadCurrentContext();
        _possession1Signal.signal();
        (void) _possession2Signal.waitUntilSignaled();
        window->makeContextCurrent();
    }
}

void RenderingPipeline::renderThreadFunc(Window* window) noexcept
{
    // Re-assign OpenGL rendering context to this rendering thread.
    window->makeContextCurrent();

    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);

    enableGLDepthTest();

    enableGLCullFace();
    glCullFace(GL_BACK);

    glFrontFace(GL_CW);

    setGLBlend(true);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _insertSignal.signal();
    (void) _renderSignal.waitUntilSignaled();

    bool renderSignaled = true;

    while(true)
    {
        if(_exitSignal.checkIfSignaled()) { break; }

        _passContext(window);

        while(!renderSignaled)
        {
            renderSignaled = _renderSignal.waitUntilSignaled(50);
            _passContext(window);
        }

        runRenderingCycle();
        window->swapBuffers();

        _insertSignal.signal();
        renderSignaled = _renderSignal.waitUntilSignaled(50);
    }
}

void RenderingPipeline::takeControlOfContext(Window* window) noexcept
{
    _possession0Signal.signal();
    (void) _possession1Signal.waitUntilSignaled();
    window->makeContextCurrent();

}

void RenderingPipeline::returnControlOfContext() noexcept
{
    Window::unloadCurrentContext();
    _possession2Signal.signal();
}

// void RenderingPipeline::dumpCommandBufferToFile(FILE* file) const noexcept
// {
//     if(1) return;
// #define GET_VALUE(__TYPE, __VAR) GET_VALUE0(__TYPE, __VAR, _dumpPtr)
//     u64 _dumpPtr = 0;
//
//     // if(_async) { _mutex.lock(); }
//
//     if(!file)
//     {
//         file = stdout;
//     }
//
//     while(true)
//     {
//         const u8 instByte = _instBuffer[_dumpPtr++];
//         const RenderingOpcode currOpcode = static_cast<RenderingOpcode>(instByte);
//
//         if(currOpcode == RenderingOpcode::FINISH_RENDER) 
//         { 
//             fprintf(file, "FINISH_RENDER\n");
//             break; 
//         }
//         else if(instByte == 0)
//         {
//             fprintf(file, "Command Buffer Unexpectedly Ended.\n");
//             break;
//         }
//
//         switch(currOpcode)
//         {
//             case RenderingOpcode::FINISH_RENDER: 
//             {
//                 fprintf(file, "FINISH_RENDER\n");
//                 break; 
//             }
//             case RenderingOpcode::LOAD_SHADER_UNIFORM:
//             {
//                 GET_VALUE(ShaderUniformType, uniformType);
//                 GET_VALUE(i32, uniformID);
//
//                 switch(uniformType)
//                 {
//                     case ShaderUniformType::INTEGER:
//                     {
//                         GET_VALUE(u64, iVal);
//                         fprintf(file, "LOAD_SHADER_UNIFORM<INTEGER>@%" PRIi32 "(%" PRIu64 ")\n", uniformID, iVal);
//                         break;
//                     }
//                     case ShaderUniformType::FLOAT:
//                     {
//                         GET_VALUE(float, fVal);
//                         fprintf(file, "LOAD_SHADER_UNIFORM<FLOAT>@%" PRIi32 "(%f)\n", uniformID, fVal);
//                         break;
//                     }
//                     case ShaderUniformType::DOUBLE:
//                     {
//                         GET_VALUE(double, dVal);
//                         fprintf(file, "LOAD_SHADER_UNIFORM<DOUBLE>@%" PRIi32 "(%f)\n", uniformID, dVal);
//                         break;
//                     }
//                     case ShaderUniformType::VEC2F:
//                     {
//                         GET_VALUE(float, fv0);
//                         GET_VALUE(float, fv1);
//                         fprintf(file, "LOAD_SHADER_UNIFORM<VEC2F>@%" PRIi32 "(%f, %f)\n", uniformID, fv0, fv1);
//                         break;
//                     }
//                     case ShaderUniformType::VEC3F:
//                     {
//                         GET_VALUE(float, fv0);
//                         GET_VALUE(float, fv1);
//                         GET_VALUE(float, fv2);
//                         fprintf(file, "LOAD_SHADER_UNIFORM<VEC3F>@%" PRIi32 "(%f, %f, %f)\n", uniformID, fv0, fv1, fv2);
//                         break;
//                     }
//                     case ShaderUniformType::VEC4F:
//                     {
//                         GET_VALUE(float, fv0);
//                         GET_VALUE(float, fv1);
//                         GET_VALUE(float, fv2);
//                         GET_VALUE(float, fv3);
//                         fprintf(file, "LOAD_SHADER_UNIFORM<VEC4F>@%" PRIi32 "(%f, %f, %f, %f)\n", uniformID, fv0, fv1, fv2, fv3);
//                         break;
//                     }
//                     case ShaderUniformType::MAT4F:
//                     {
//                         GET_VALUE(float*, mat);
//                         fprintf(file, "LOAD_SHADER_UNIFORM<MAT4F>@%" PRIi32 "({ %f, %f, %f, %f }, { %f, %f, %f, %f }, { %f, %f, %f, %f }, { %f, %f, %f, %f })\n", uniformID,
//                                 mat[0x0], mat[0x1], mat[0x2], mat[0x3],
//                                 mat[0x4], mat[0x5], mat[0x6], mat[0x7],
//                                 mat[0x8], mat[0x9], mat[0xA], mat[0xB],
//                                 mat[0xC], mat[0xD], mat[0xE], mat[0xF]);
//                         break;
//                     }
//                     default: break;
//                 }
//                 break;
//             }
//             case RenderingOpcode::ACTIVATE_SHADER_PROGRAM:
//             {
//                 GET_VALUE(GLuint, shaderProgramID);
//                 fprintf(file, "ACTIVATE_SHADER_PROGRAM(%" PRIu32 ")\n", shaderProgramID);
//                 break;
//             }
//             // case RenderingOpcode::ACTIVATE_TEXTURE_UNIT:
//             // {
//             //     GET_VALUE(u8, textureUnitOffset);
//             //     const GLenum textureUnit = GL_TEXTURE0 + textureUnitOffset;
//             //     fprintf(file, "ACTIVATE_TEXTURE_UNIT(%u)\n", textureUnitOffset);
//             //     break;
//             // }
//             case RenderingOpcode::BIND_TEXTURE:
//             {
//                 GET_VALUE(u8, unit);
//                 GET_VALUE(ITexture*, texture);
//
//                 const char* textureType;
//
//                 switch(texture->textureType())
//                 {
//                     case TextureType::TEXTURE_2D: textureType = "Texture2D"; break;
//                     case TextureType::TEXTURE_3D: textureType = "Texture3D"; break;
//                     case TextureType::TEXTURE_CUBE: textureType = "TextureCube"; break;
//                     default: textureType = "UnknownTextureType"; break;
//                 }
//
//                 fprintf(file, "BIND_TEXTURE<%s>(%" PRIu32 ", %p)\n", textureType, unit, texture);
//                 break;
//             }
//             case RenderingOpcode::UNBIND_TEXTURE:
//             {
//                 GET_VALUE(u8, unit);
//
//                 fprintf(file, "UNBIND_TEXTURE(%" PRIu32 ")\n", unit);
//                 break;
//             }
//             // case RenderingOpcode::BIND_VAO:
//             // {
//             //     GET_VALUE(GLuint, vaoID);
//             //     fprintf(file, "BIND_VAO(%" PRIu32 ")\n", vaoID);
//             //     break;
//             // }
//             case RenderingOpcode::BIND_VBO:
//             {
//                 GET_VALUE(GLenum, target);
//                 GET_VALUE(GLuint, vboID);
//                 fprintf(file, "BIND_VBO(%" PRIu32 ", %" PRIu32 ")\n", target, vboID);
//                 break;
//             }
//             // case RenderingOpcode::ENABLE_VAO_ATTRIBUTE:
//             // {
//             //     GET_VALUE(GLuint, vaoAttribEnable);
//             //     fprintf(file, "ENABLE_VAO_ATTRIBUTE(%" PRIu32 ")\n", vaoAttribEnable);
//             //     break;
//             // }
//             // case RenderingOpcode::DISABLE_VAO_ATTRIBUTE:
//             // {
//             //     GET_VALUE(GLuint, vaoAttribDisable);
//             //     fprintf(file, "DISABLE_VAO_ATTRIBUTE(%" PRIu32 ")\n", vaoAttribDisable);
//             //     break;
//             // }
//             case RenderingOpcode::GL_DRAW_ARRAYS:
//             {
//                 GET_VALUE(GLenum, drawArraysMode);
//                 GET_VALUE(GLint, drawArraysFirst);
//                 GET_VALUE(GLsizei, drawArraysCount);
//
//                 fprintf(file, "GL_DRAW_ARRAYS(%" PRIu32 ", %" PRIi32 ", %" PRIu32 ")\n", drawArraysMode, drawArraysFirst, drawArraysCount);
//                 break;
//             }
//             case RenderingOpcode::GL_DRAW_ELEMENTS:
//             {
//                 GET_VALUE(GLenum, drawElemsMode);
//                 GET_VALUE(GLsizei, drawElemsCount);
//                 GET_VALUE(GLenum, drawElemsType);
//                 GET_VALUE(void*, drawElemsIndices);
//
//                 fprintf(file, "GL_DRAW_ELEMENTS(%" PRIu32 ", %" PRIu32 ", %" PRIu32 ", %p)\n", drawElemsMode, drawElemsCount, drawElemsType, drawElemsIndices);
//                 break;
//             }
//             case RenderingOpcode::GL_CLEAR_BUFFERS:
//             {
//                 GET_VALUE(GLbitfield, clearBuffers);
//                 fprintf(file, "GL_CLEAR_BUFFERS(%" PRIu32 ")\n", clearBuffers);
//                 break;
//             }
//             case RenderingOpcode::LOAD_BUFFER_DATA:
//             {
//                 GET_VALUE(GLenum, target);
//                 GET_VALUE(GLsizeiptr, size);
//
//                 const void* data = reinterpret_cast<const void*>(_instBuffer + _dumpPtr);
//                 _dumpPtr += static_cast<u32>(size);
//
//                 GET_VALUE(GLenum, usage);
//
//                 fprintf(file, "LOAD_BUFFER_DATA(%" PRIu32 ", %" PRIiPTR ", %p, %" PRIu32 ")\n", target, size, data, usage);
//                 break;
//             }
//             case RenderingOpcode::MODIFY_BUFFER_DATA:
//             {
//                 GET_VALUE(GLenum, target);
//                 GET_VALUE(GLintptr, offset);
//                 GET_VALUE(GLsizeiptr, size);
//
//                 const void* data = reinterpret_cast<const void*>(_instBuffer + _dumpPtr);
//                 _dumpPtr += static_cast<u32>(size);
//
//                 fprintf(file, "MODIFY_BUFFER_DATA(%" PRIu32 ", %" PRIuPTR ", %" PRIiPTR ", %p)\n", target, offset, size, data);
//                 break;
//             }
//             case RenderingOpcode::GL_ENABLE:
//             {
//                 GET_VALUE(GLenum, cap);
//                 fprintf(file, "GL_ENABLE(%" PRIu32 ")\n", cap);
//                 break;
//             }
//             case RenderingOpcode::GL_DISABLE:
//             {
//                 GET_VALUE(GLenum, cap);
//                 fprintf(file, "GL_DISABLE(%" PRIu32 ")\n", cap);
//                 break;
//             }
//             case RenderingOpcode::GL_FACE_WINDING:
//             {
//                 GET_VALUE(GLenum, mode);
//                 fprintf(file, "GL_FACE_WINDING(%" PRIu32 ")\n", mode);
//                 break;
//             }
//             case RenderingOpcode::RESIZE_VIEWPORT:
//             {
//                 GET_VALUE(GLint, x);
//                 GET_VALUE(GLint, y);
//                 GET_VALUE(GLsizei, width);
//                 GET_VALUE(GLsizei, height);
//                 fprintf(file, "RESIZE_VIEWPORT(%" PRIu32 ", %" PRIu32 ", %" PRIu32 ", %" PRIu32 ")\n", x, y, width, height);
//                 break;
//             }
//             default: break;
//         }
//     }
//
//     // if(_async) { _mutex.unlock(); }
//
// #undef GET_VALUE
// }
