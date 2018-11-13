#include <gl/GLUtils.hpp>
#include <Utils.hpp>

void setupDebugMessageCallback(debugMessageCallback_f callback, void* userParam, bool synchronous)
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
