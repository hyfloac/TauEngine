#pragma warning(push, 0)
#include <cstring>
#include <GL/glew.h>
#include <Windows.h>
#pragma warning(pop)

#include <TextHandler.hpp>
#include <RenderingPipeline.hpp>
#include <gl/GLUtils.hpp>
#include <texture/Texture.hpp>
#include <maths/Vector3f.hpp>
#include <maths/Matrix4x4f.hpp>
#include <imgui/ImGuiGLImpl.hpp>
#include "system/RenderingContext.hpp"
#include "model/VertexArray.hpp"
#include "Timings.hpp"

RenderingPipeline::RenderingPipeline(Window& window, ctxCtrl_f setupParams, void* setupParam, const bool async, const u32 bufferSize, const u32 ctxControlsSize) noexcept
    : _window(window),
      _instBuffer(new u8[bufferSize]), _backBuffer(new u8[bufferSize]), _instPtr(0), _insertPtr(0),
      _ctxCtrlsFront(new CtxCtrlObj[ctxControlsSize]), _ctxCtrlsBack(new CtxCtrlObj[ctxControlsSize]), _ctxCtrlsInsertPtr(0), _ctxCtrlsSize(ctxControlsSize),
      _async(async), _renderThread(null)
{
    std::memset(reinterpret_cast<void*>(_instBuffer), 0, bufferSize);
    std::memset(reinterpret_cast<void*>(_backBuffer), 0, bufferSize);

    std::memset(reinterpret_cast<void*>(_ctxCtrlsFront), 0, ctxControlsSize * sizeof(CtxCtrlObj));
    std::memset(reinterpret_cast<void*>(_ctxCtrlsBack), 0, ctxControlsSize * sizeof(CtxCtrlObj));

    if(async)
    {
        window.renderingContext()->deactivateContext();
        _renderThread = new std::thread(&RenderingPipeline::renderThreadFunc, this, setupParams, setupParam);
    }
    else
    {
        setupParams(*this, window, setupParam);
    }
}

RenderingPipeline::~RenderingPipeline() noexcept 
{ 
    _exitSignal.signal();
    if(_renderThread) { _renderThread->join(); }
    delete _renderThread;

    delete[] _instBuffer; 
    delete[] _backBuffer;

    delete[] _ctxCtrlsFront;
    delete[] _ctxCtrlsBack;
}

#define LOAD_VALUE(__VAR) std::memcpy(reinterpret_cast<void*>(_backBuffer + _insertPtr), reinterpret_cast<const void*>(&(__VAR)), sizeof(__VAR)); \
                          _insertPtr += sizeof(__VAR);

void RenderingPipeline::pushRenderText(const TextHandler* th, GlyphSetHandle glyphSetHandle, const char* str, GLfloat x, GLfloat y, GLfloat scale, u8 cr, u8 cg, u8 cb, const glm::mat4& proj) noexcept
{
    prePushInst<RenderingOpcode::RENDER_TEXT>();
    LOAD_VALUE(th);
    LOAD_VALUE(glyphSetHandle);
    LOAD_VALUE(x);
    LOAD_VALUE(y);
    LOAD_VALUE(scale);
    LOAD_VALUE(cr);
    LOAD_VALUE(cg);
    LOAD_VALUE(cb);

    const float* const mat = reinterpret_cast<const float*>(glm::value_ptr(proj));
    std::memcpy(reinterpret_cast<void*>(_backBuffer + _insertPtr), reinterpret_cast<const void*>(mat), sizeof(float) * 16);
    _insertPtr += sizeof(float) * 16;

    const u32 len = static_cast<u32>(strlen(str) + 1);
    LOAD_VALUE(len);

    (void) std::memcpy(reinterpret_cast<void*>(_backBuffer + _insertPtr), reinterpret_cast<const void*>(str), sizeof(char) * len);
    _insertPtr += sizeof(char) * len;

    postPushInst();
}

GLfloat RenderingPipeline::pushRenderTextLineWrapped(const TextHandler* th, GlyphSetHandle glyphSetHandle, const char* str, GLfloat x, GLfloat y, GLfloat scale, u8 cr, u8 cg, u8 cb, const Window* window, float lineHeight, const glm::mat4& proj) noexcept
{
    prePushInst<RenderingOpcode::RENDER_TEXT_LINE_WRAPPED>();
    LOAD_VALUE(th);
    LOAD_VALUE(glyphSetHandle);
    LOAD_VALUE(x);
    LOAD_VALUE(y);
    LOAD_VALUE(scale);
    LOAD_VALUE(cr);
    LOAD_VALUE(cg);
    LOAD_VALUE(cb);
    LOAD_VALUE(window);
    LOAD_VALUE(lineHeight);

    const float* const mat = reinterpret_cast<const float*>(glm::value_ptr(proj));
    std::memcpy(reinterpret_cast<void*>(_backBuffer + _insertPtr), reinterpret_cast<const void*>(mat), sizeof(float) * 16);
    _insertPtr += sizeof(float) * 16;

    const u32 len = static_cast<u32>(strlen(str) + 1);
    LOAD_VALUE(len);

    (void) std::memcpy(reinterpret_cast<void*>(_backBuffer + _insertPtr), reinterpret_cast<const void*>(str), sizeof(char)* len);
    _insertPtr += sizeof(char) * len;

    postPushInst();

    return th->computeHeight(glyphSetHandle, str, scale, x, *window, lineHeight);
}

#define GET_VALUE0(__TYPE, __VAR, __PTR) __TYPE __VAR = *reinterpret_cast<__TYPE*>(_instBuffer + (__PTR)); /* NOLINT(bugprone-macro-parentheses) */ \
                                         (__PTR) += sizeof(__VAR);

#define GET_VALUE(__TYPE, __VAR) GET_VALUE0(__TYPE, __VAR, _instPtr)

#define DECL_HANDLER(_NAME) static inline void _NAME(IRenderingContext& context, u8* _instBuffer, volatile u32& _instPtr) noexcept

DECL_HANDLER(rpActivateShaderProgram)
{
    GET_VALUE(GLuint, shaderProgramID);
    glUseProgram(shaderProgramID);
}

DECL_HANDLER(rpBindTexture)
{
    GET_VALUE(ITexture*, texture);
    GET_VALUE(u8, textureUnit);

    texture->bind(textureUnit);
}

DECL_HANDLER(rpUnbindTexture)
{
    GET_VALUE(ITexture*, texture);
    GET_VALUE(u8, textureUnit);

    texture->unbind(textureUnit);
}

DECL_HANDLER(rpEnableBufferDescriptor)
{
    GET_VALUE(IVertexArray*, vertexArray);

    vertexArray->bind(context);
    vertexArray->preDraw(context);
}

DECL_HANDLER(rpDisableBufferDescriptor)
{
    GET_VALUE(IVertexArray*, vertexArray);

    vertexArray->postDraw(context);
    vertexArray->unbind(context);
}

DECL_HANDLER(rpBindVBO)
{
    GET_VALUE(GLenum, target);
    GET_VALUE(GLuint, vboID);
    glBindBuffer(target, vboID);
}

DECL_HANDLER(rpGLDrawArrays)
{
    GET_VALUE(GLenum, drawArraysMode);
    GET_VALUE(GLint, drawArraysFirst);
    GET_VALUE(GLsizei, drawArraysCount);

    glDrawArrays(drawArraysMode, drawArraysFirst, drawArraysCount);
}

DECL_HANDLER(rpGLDrawElements)
{
    GET_VALUE(GLenum, drawElemsMode);
    GET_VALUE(GLsizei, drawElemsCount);
    GET_VALUE(GLenum, drawElemsType);
    GET_VALUE(void*, drawElemsIndices);

    glDrawElements(drawElemsMode, drawElemsCount, drawElemsType, drawElemsIndices);
}

DECL_HANDLER(rpGLClearBuffers)
{
    GET_VALUE(GLbitfield, clearBuffers);
    glClear(clearBuffers);
}

DECL_HANDLER(rpLoadBufferData)
{
    GET_VALUE(GLenum, target);
    GET_VALUE(GLsizeiptr, size);

    const void* data = reinterpret_cast<const void*>(_instBuffer + _instPtr);
    _instPtr += static_cast<u32>(size);

    GET_VALUE(GLenum, usage);

    glBufferData(target, size, data, usage);
}

DECL_HANDLER(rpModifyBufferData)
{
    GET_VALUE(GLenum, target);
    GET_VALUE(GLintptr, offset);
    GET_VALUE(GLsizeiptr, size);

    const void* data = reinterpret_cast<const void*>(_instBuffer + _instPtr);
    _instPtr += static_cast<u32>(size);

    glBufferSubData(target, offset, size, data);
}

DECL_HANDLER(rpGLEnable)
{
    GET_VALUE(GLenum, cap);
    glEnable(cap);
}

DECL_HANDLER(rpGLDisable)
{
    GET_VALUE(GLenum, cap);
    glDisable(cap);
}

DECL_HANDLER(rpGLFaceWinding)
{
    GET_VALUE(GLenum, mode);
    glFrontFace(mode);
}

DECL_HANDLER(rpResizeViewport)
{
    GET_VALUE(GLint, x);
    GET_VALUE(GLint, y);
    GET_VALUE(GLsizei, width);
    GET_VALUE(GLsizei, height);
    glViewport(x, y, width, height);
}

DECL_HANDLER(rpImGuiRender)
{
    GET_VALUE(ImDrawData*, data);
    ImGui_ImplGL_Render(data);
}

DECL_HANDLER(rpRenderText)
{
    GET_VALUE(TextHandler*, th);
    GET_VALUE(GlyphSetHandle, glyphSetHandle);
    GET_VALUE(GLfloat, x);
    GET_VALUE(GLfloat, y);
    GET_VALUE(GLfloat, scale);
    GET_VALUE(u8, r);
    GET_VALUE(u8, g);
    GET_VALUE(u8, b);
    Vector3f color(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f);

    float* matPtr = reinterpret_cast<float*>(_instBuffer + _instPtr);
    _instPtr += sizeof(float) * 16;
    glm::mat4 mat;
    (void) std::memcpy(reinterpret_cast<void*>(glm::value_ptr(mat)), reinterpret_cast<void*>(matPtr), sizeof(float) * 16);

    GET_VALUE(u32, len);

    char* str = reinterpret_cast<char*>(_instBuffer + _instPtr);
    _instPtr += sizeof(char) * len;

    th->renderText(context, glyphSetHandle, str, x, y, scale, color, mat);
}

DECL_HANDLER(rpRenderTextLineWrapped)
{
    GET_VALUE(TextHandler*, th);
    GET_VALUE(GlyphSetHandle, glyphSetHandle);
    GET_VALUE(GLfloat, x);
    GET_VALUE(GLfloat, y);
    GET_VALUE(GLfloat, scale);
    GET_VALUE(u8, r);
    GET_VALUE(u8, g);
    GET_VALUE(u8, b);
    GET_VALUE(const Window*, window);
    GET_VALUE(float, lineHeight);
    Vector3f color(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f);

    float* matPtr = reinterpret_cast<float*>(_instBuffer + _instPtr);
    _instPtr += sizeof(float) * 16;
    glm::mat4 mat;
    (void) std::memcpy(reinterpret_cast<void*>(glm::value_ptr(mat)), reinterpret_cast<void*>(matPtr), sizeof(float) * 16);

    GET_VALUE(u32, len);

    char* str = reinterpret_cast<char*>(_instBuffer + _instPtr);
    _instPtr += sizeof(char) * len;

    th->renderTextLineWrapped(context, glyphSetHandle, str, x, y, scale, color, mat, *window, lineHeight);
}

static inline void rpExecuteCommand(RenderingPipeline& rp, Window& window, IRenderingContext& context, u8* _instBuffer, volatile u32& _instPtr) noexcept
{
    GET_VALUE(RenderingPipeline::command_f, command);
    GET_VALUE(std::size_t, dataSize);

    command(rp, window, context, _instBuffer + _instPtr);

    _instPtr += dataSize;
}

void RenderingPipeline::runRenderingCycle() noexcept
{
    PERF();
    for(u32 ctxIndex = 0; _ctxCtrlsFront[ctxIndex].first && ctxIndex < _ctxCtrlsSize; ++ctxIndex)
    {
        CtxCtrlObj& obj = _ctxCtrlsFront[ctxIndex];
        obj.first(*this, _window, obj.second);
        obj.first = nullptr;
    }

#define RP_FUNC_HANDLER(_OP, _FUNC) case RenderingOpcode::_OP: _FUNC(*currentContext, _instBuffer, _instPtr); break

    _instPtr = 0;

    IRenderingContext* currentContext = _window.renderingContext();

    while(true)
    {
        GET_VALUE(u16, inst);
        const RenderingOpcode currOpcode = static_cast<RenderingOpcode>(inst);

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
            case RenderingOpcode::LOAD_CONTEXT:
            {
                GET_VALUE(IRenderingContext*, context);
                currentContext = context;
                break;
            }
            RP_FUNC_HANDLER(ACTIVATE_SHADER_PROGRAM, rpActivateShaderProgram);
            RP_FUNC_HANDLER(BIND_TEXTURE, rpBindTexture);
            RP_FUNC_HANDLER(UNBIND_TEXTURE, rpUnbindTexture);
            RP_FUNC_HANDLER(ENABLE_BUFFER_DESCRIPTOR, rpEnableBufferDescriptor);
            RP_FUNC_HANDLER(DISABLE_BUFFER_DESCRIPTOR, rpDisableBufferDescriptor);
            RP_FUNC_HANDLER(BIND_VBO, rpBindVBO);
            RP_FUNC_HANDLER(GL_DRAW_ARRAYS, rpGLDrawArrays);
            RP_FUNC_HANDLER(GL_DRAW_ELEMENTS, rpGLDrawElements);
            RP_FUNC_HANDLER(GL_CLEAR_BUFFERS, rpGLClearBuffers);
            RP_FUNC_HANDLER(LOAD_BUFFER_DATA, rpLoadBufferData);
            RP_FUNC_HANDLER(MODIFY_BUFFER_DATA, rpModifyBufferData);
            RP_FUNC_HANDLER(GL_ENABLE, rpGLEnable);
            RP_FUNC_HANDLER(GL_DISABLE, rpGLDisable);
            RP_FUNC_HANDLER(GL_FACE_WINDING, rpGLFaceWinding);
            RP_FUNC_HANDLER(RESIZE_VIEWPORT, rpResizeViewport);
            RP_FUNC_HANDLER(RENDER_TEXT, rpRenderText);
            RP_FUNC_HANDLER(RENDER_TEXT_LINE_WRAPPED, rpRenderTextLineWrapped);
            RP_FUNC_HANDLER(IMGUI_RENDER, rpImGuiRender);
            case RenderingOpcode::EXECUTE_COMMAND: rpExecuteCommand(*this, this->_window, *currentContext, _instBuffer, _instPtr); break;
            default: break;
        }
    }

#undef RP_FUNC_HANDLER

    std::memset(_instBuffer, 0, _instPtr);
    // _window.swapBuffers();
    currentContext->swapFrame();
}
#undef GET_VALUE

// void RenderingPipeline::_passContext() const noexcept
// {
//     if(_possession0Signal.checkIfSignaled())
//     {
//         _window.unloadMainContext();
//         _possession1Signal.signal();
//         (void) _possession2Signal.waitUntilSignaled();
//         _window.makeContextCurrent();
//     }
// }

void RenderingPipeline::renderThreadFunc(ctxCtrl_f setupParams, void* setupParam) noexcept
{
    PERF();
    // Re-assign OpenGL rendering context to this rendering thread.
    _window.renderingContext()->activateContext();

    setupParams(*this, _window, setupParam);

    _insertSignal.signal();
    // (void) _renderSignal.waitUntilSignaled();

    // bool renderSignaled = true;

    while(true)
    {
        if(_exitSignal.checkIfSignaled()) { break; }

        (void) _renderSignal.waitUntilSignaled();

        // _passContext();

        // while(!renderSignaled)
        // {
        //     renderSignaled = _renderSignal.waitUntilSignaled(1);
        //     _passContext();
        // }


        runRenderingCycle();

        _insertSignal.signal();
        // renderSignaled = _renderSignal.checkIfSignaled();
    }

    _window.renderingContext()->deactivateContext();
}

// void RenderingPipeline::takeControlOfContext() const noexcept
// {
//     if(_async)
//     {
//         _possession0Signal.signal();
//         (void) _possession1Signal.waitUntilSignaled();
//         _window.makeContextCurrent();
//     }
//
// }

// void RenderingPipeline::returnControlOfContext() const noexcept
// {
//     if(_async)
//     {
//         Window::unloadMainContext();
//         _possession2Signal.signal();
//     }
// }

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
