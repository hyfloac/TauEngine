#include <TauEngine.hpp>
#include <gl/GLUtils.hpp>
#include <Utils.hpp>

void setupDebugMessageCallback(Nullable debugMessageCallback_f callback, Nullable void* userParam, bool synchronous) noexcept
{
    if(glDebugMessageCallback)
    {
        glEnable(GL_DEBUG_OUTPUT);
        if(synchronous)
        {
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        }
        glDebugMessageCallback(callback, userParam);
    }
    else if(GLEW_ARB_debug_output)
    {
        glEnable(GL_DEBUG_OUTPUT);
        if(synchronous)
        {
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
        }
        glDebugMessageCallbackARB(callback, userParam);
    }
    else if(glDebugMessageCallbackAMD)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageEnableAMD(0, 0, 0, null, true);
        glDebugMessageCallbackAMD(reinterpret_cast<GLDEBUGPROCAMD>(callback), userParam);
    }
}

void stopDebugOutput() noexcept
{
    if(glDebugMessageCallback)
    {
        glDisable(GL_DEBUG_OUTPUT);
        glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }
    else if(GLEW_ARB_debug_output)
    {
        glDisable(GL_DEBUG_OUTPUT);
        glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    }
    else if(glDebugMessageCallbackAMD)
    {
        glDisable(GL_DEBUG_OUTPUT);
    }
}

void __internal__clearGLErrors() noexcept
{
    while(glGetError() != GL_NO_ERROR);
}

bool __internal__logGLCall(const char* glFunc, const char* file, u32 line) noexcept
{
    bool noErrors = true;
    do
    {
        const GLenum error = glGetError();
        if(error != GL_NO_ERROR)
        {
            noErrors = false;
            printf("[OpenGL Error] (0x%X): %s %s %d\n", error, glFunc, file, line);
            // getEngineLogger()->trace("[OpenGL Error] (0x{0:X}): {1} {2} {3:d}", error, glFunc, file, line);
        }
        else
        {
            break;
        }
    } while(true);

    return noErrors;
}
