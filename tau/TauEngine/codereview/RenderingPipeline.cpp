    #include <cstring>
    #include <GL/glew.h>

    #include "RenderingPipeline.hpp"

    RenderingPipeline::RenderingPipeline(HDC hdc, HGLRC renderingContext, const std::uint32_t bufferSize) 
        : _instBuffer(new std::uint8_t[bufferSize]), _insertPtr(0), _instPtr(0),
          _initialReady(false), _readyForInsert(true), _running(true), _renderThread(&RenderingPipeline::renderThreadFunc, this, window)
    {
        _mutex.lock();
        /* Unload context on main thread. 
           The context will be loaded on the rendering thread.
           Not entirely sure if this required. 
         */
        wglMakeCurrent(nullptr, nullptr);
        std::memset(_instBuffer, 0, bufferSize);
        _initialReady = true; // Notify the rendering thread that the buffer is cleared.
        _mutex.unlock();
    }

    RenderingPipeline::~RenderingPipeline()  
    { 
        _mutex.lock();
        _running = false;
        _mutex.unlock();
        _renderThread.join();
        delete[] _instBuffer; 
    }

    void RenderingPipeline::pushInstruction(const RenderingOpcode opcode, const ParameterPack&& params) 
    {
        /*
         *    After the `RenderingOpcode::FINISH_RENDER` insruction
         *  is inserted we lock insertion until the rendering thread
         *  loops through everything.
         *
         *  This will yield the main thread until the rendering thread finishes.
         */
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
        _instBuffer[_insertPtr++] = static_cast<std::uint8_t>(opcode);

    // Macro to copy data into the instruction buffer.
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
                const std::int32_t uniformID = static_cast<std::int32_t>(params.p1);

                _instBuffer[_insertPtr++] = static_cast<std::uint8_t>(uniformType);
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
                        const std::uint32_t iVal = static_cast<std::uint32_t>(params.p2);
                        const float fVal = reinterpret_cast<const float&>(iVal);
                        LOAD_VALUE(fVal);
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
            case RenderingOpcode::GL_CLEAR_BUFFERS:
            {
                const GLbitfield clearBuffers = static_cast<GLbitfield>(params.p0);
                LOAD_VALUE(clearBuffers);
                break;
            }
            default: break;
        }

    #undef LOAD_VALUE

        _mutex.unlock();
    }

    void RenderingPipeline::runRenderingCycle() 
    {
    // Macro to retrieve values from the instruction buffer.
    #define GET_VALUE(__TYPE, __VAR) const __TYPE __VAR = *reinterpret_cast<const __TYPE*>(_instBuffer + _instPtr); \
                                     _instPtr += sizeof(__VAR);
        _instPtr = 0;

        while(true)
        {
            std::uint8_t instByte;

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
                if(instByte == 0) { break; }
                std::this_thread::yield();
            }
            ++_instPtr;

            const RenderingOpcode currOpcode = static_cast<RenderingOpcode>(instByte);

            if(currOpcode == RenderingOpcode::FINISH_RENDER) { break; }

            switch(currOpcode)
            {
                case RenderingOpcode::LOAD_SHADER_UNIFORM:
                {
                    GET_VALUE(ShaderUniformType, uniformType);
                    GET_VALUE(std::int32_t, uniformID);

                    switch(uniformType)
                    {
                        case ShaderUniformType::INTEGER:
                        {
                            GET_VALUE(std::uint64_t, iVal);
                            glUniform1i(uniformID, static_cast<GLint>(iVal));
                            break;
                        }
                        case ShaderUniformType::FLOAT:
                        {
                            GET_VALUE(float, fVal);
                            glUniform1f(uniformID, fVal);
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
                case RenderingOpcode::GL_DRAW_ARRAYS:
                {
                    GET_VALUE(GLenum, drawArraysMode);
                    GET_VALUE(GLint, drawArraysFirst);
                    GET_VALUE(GLsizei, drawArraysCount);

                    glDrawArrays(drawArraysMode, drawArraysFirst, drawArraysCount);
                    break;
                }
                case RenderingOpcode::GL_CLEAR_BUFFERS:
                {
                    GET_VALUE(GLbitfield, clearBuffers);
                    glClear(clearBuffers);
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

    void RenderingPipeline::renderThreadFunc(HDC hdc, HGLRC renderingContext) 
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
        wglMakeCurrent(hdc, renderingContext);

        // Some basic OpenGL setup.
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);

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
            SwapBuffers(hdc);
        }
    }
