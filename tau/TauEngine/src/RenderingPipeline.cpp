#pragma warning(push, 0)
#include <cstring>
#include <GL/glew.h>
#include <Windows.h>
#pragma warning(pop)

#include <RenderingPipeline.hpp>
#include <gl/GLUtils.hpp>

RenderingPipeline::RenderingPipeline(Window* window, const u32 bufferSize) noexcept
    : _instBuffer(new u8[bufferSize]), _insertPtr(0), _instPtr(0),
      _initialReady(false), _readyForInsert(true), _running(true), _renderThread(&RenderingPipeline::renderThreadFunc, this, window)
{
    _mutex.lock();
    /* Unload context on main thread.
       The context will be loaded on the rendering thread.
       Not entirely sure if this required.
     */
    Window::unloadCurrentContext();
    std::memset(reinterpret_cast<void*>(_instBuffer), 0, bufferSize);
    _initialReady = true;
    _mutex.unlock();
}

RenderingPipeline::~RenderingPipeline() noexcept 
{ 
    _mutex.lock();
    _running = false;
    _mutex.unlock();
    _renderThread.join();
    delete[] _instBuffer; 
}

void RenderingPipeline::pushInstruction(const RenderingOpcode opcode, const ParameterPack&& params) noexcept
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
    _instBuffer[_insertPtr++] = static_cast<u8>(opcode);


#define LOAD_VALUE(__VAR) std::memcpy(reinterpret_cast<void*>(_instBuffer + _insertPtr), reinterpret_cast<const void*>(&__VAR), sizeof(__VAR)); \
                          _insertPtr += sizeof(__VAR);

    switch(opcode)
    {
        case RenderingOpcode::FINISH_RENDER: 
        {
            _readyForInsert = false;
            break;
        }
        case RenderingOpcode::LOAD_SHADER_UNIFORM: 
        {
            const ShaderUniformType uniformType = static_cast<ShaderUniformType>(params.p0);
            const i32 uniformID = static_cast<i32>(params.p1);

            _instBuffer[_insertPtr++] = static_cast<u8>(uniformType);
            LOAD_VALUE(uniformID);

            switch(uniformType)
            {
                case ShaderUniformType::INTEGER:
                {
                    LOAD_VALUE(params.p2);
                    break;
                }
                case ShaderUniformType::FLOAT:
                {
                    const u32 iVal = static_cast<u32>(params.p2);
                    const float fVal = reinterpret_cast<const float&>(iVal);
                    LOAD_VALUE(fVal);
                    break;
                }
                case ShaderUniformType::DOUBLE:
                {
                    LOAD_VALUE(params.p2);
                    break;
                }
                case ShaderUniformType::VEC2F:
                {
                    const u32 iv0 = static_cast<u32>(params.p2);
                    const float fv0 = reinterpret_cast<const float&>(iv0);
                    LOAD_VALUE(fv0);
                    const u32 iv1 = static_cast<u32>(params.p3);
                    const float fv1 = reinterpret_cast<const float&>(iv1);
                    LOAD_VALUE(fv1);
                    break;
                }
                case ShaderUniformType::VEC3F:
                {
                    const u32 iv0 = static_cast<u32>(params.p2);
                    const float fv0 = reinterpret_cast<const float&>(iv0);
                    LOAD_VALUE(fv0);
                    const u32 iv1 = static_cast<u32>(params.p3);
                    const float fv1 = reinterpret_cast<const float&>(iv1);
                    LOAD_VALUE(fv1);
                    const u32 iv2 = static_cast<u32>(params.p4);
                    const float fv2 = reinterpret_cast<const float&>(iv2);
                    LOAD_VALUE(fv2);
                    break;
                }
                case ShaderUniformType::VEC4F:
                {
                    const u32 iv0 = static_cast<u32>(params.p2);
                    const float fv0 = reinterpret_cast<const float&>(iv0);
                    LOAD_VALUE(fv0);
                    const u32 iv1 = static_cast<u32>(params.p3);
                    const float fv1 = reinterpret_cast<const float&>(iv1);
                    LOAD_VALUE(fv1);
                    const u32 iv2 = static_cast<u32>(params.p4);
                    const float fv2 = reinterpret_cast<const float&>(iv2);
                    LOAD_VALUE(fv2);
                    const u32 iv3 = static_cast<u32>(params.p5);
                    const float fv3 = reinterpret_cast<const float&>(iv3);
                    LOAD_VALUE(fv3);
                    break;
                }
                case ShaderUniformType::MAT4F:
                {
                    const float* const mat = reinterpret_cast<const float*>(params.p2);
                    LOAD_VALUE(mat);
                    break; 
                }
                default: break;
            }
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
            const GLuint vboID  = static_cast<GLuint>(params.p1);
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
            const GLenum  drawArraysMode  = static_cast<GLenum>(params.p0);
            const GLint   drawArraysFirst = static_cast<GLint>(params.p1);
            const GLsizei drawArraysCount = static_cast<GLsizei>(params.p2);

            LOAD_VALUE(drawArraysMode);
            LOAD_VALUE(drawArraysFirst);
            LOAD_VALUE(drawArraysCount);
            break;
        }
        case RenderingOpcode::GL_DRAW_ELEMENTS:
        {
            const GLenum      drawElemsMode  = static_cast<GLenum>(params.p0);
            const GLsizei     drawElemsCount = static_cast<GLsizei>(params.p1);
            const GLenum      drawElemsType  = static_cast<GLenum>(params.p2);
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
        default: break;
    }

#undef LOAD_VALUE

    _mutex.unlock();
}

void RenderingPipeline::runRenderingCycle() noexcept
{
#define GET_VALUE(__TYPE, __VAR) const __TYPE __VAR = *reinterpret_cast<const __TYPE*>(_instBuffer + _instPtr); \
                                 _instPtr += sizeof(__VAR);
    _instPtr = 0;

    while(true)
    {
        u8 instByte;

        /*
         *  If there is no instruction ready yield the thread.
         *
         *  This is why the first `RenderingOpcode` has a value of 1 instead of 0.
         */
        while(true)
        {
            _mutex.lock();
            instByte = _instBuffer[_instPtr];
            _mutex.unlock();
            if(instByte != 0) { break; }
            std::this_thread::yield();
        }
        
        ++_instPtr;
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
                        GET_VALUE(float*, mat);
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
            case RenderingOpcode::ACTIVATE_TEXTURE_UNIT:
            {
                GET_VALUE(u8, textureUnitOffset);
                const GLenum textureUnit = GL_TEXTURE0 + textureUnitOffset;
                glActiveTexture(textureUnit);
                break;
            }
            case RenderingOpcode::BIND_TEXTURE:
            {
                GET_VALUE(GLuint, textureID);
                glBindTexture(GL_TEXTURE_2D, textureID);
                break;
            }
            case RenderingOpcode::BIND_VAO:
            {
                GET_VALUE(GLuint, vaoID);
                glBindVertexArray(vaoID);
                break;
            }
            case RenderingOpcode::BIND_VBO:
            {
                GET_VALUE(GLenum, target);
                GET_VALUE(GLuint, vboID);
                glBindBuffer(target, vboID);
                break;
            }
            case RenderingOpcode::ENABLE_VAO_ATTRIBUTE:
            {
                GET_VALUE(GLuint, vaoAttribEnable);
                glEnableVertexAttribArray(vaoAttribEnable);
                break;
            }
            case RenderingOpcode::DISABLE_VAO_ATTRIBUTE:
            {
                GET_VALUE(GLuint, vaoAttribDisable);
                glDisableVertexAttribArray(vaoAttribDisable);
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
                _instPtr += size;

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
                _instPtr += size;

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
            default: break;
        }
    }

    _mutex.lock();
    std::memset(_instBuffer, 0, _instPtr);
    _insertPtr = 0;
    _readyForInsert = true;
    _mutex.unlock();
#undef GET_VALUE
}

void RenderingPipeline::renderThreadFunc(Window* window) noexcept
{
    // Wait for instruction buffer to be cleared.
    do
    {
        _mutex.lock();
        if(_initialReady)
        {
            _mutex.unlock();
            break;
        }
        _mutex.unlock();
        std::this_thread::yield();
    } while(true);

    // Re-assign OpenGL rendering context to this rendering thread.
    window->makeContextCurrent();

    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);

    enableGLDepthTest();

    enableGLCullFace();
    glCullFace(GL_BACK);

    glFrontFace(GL_CW);

    setGLBlend(true);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while(true)
    {
        _mutex.lock();
        if(!_running) 
        {
            _mutex.unlock();
            break; 
        }
        _mutex.unlock();

        runRenderingCycle();
        window->swapBuffers();
    }
}
