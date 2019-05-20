#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)
#include <gl/GLRenderingContext.hpp>
#include <Utils.hpp>

GLRenderingContext::GLRenderingContext(GLContextSettings contextSettings) noexcept
    : _context(null), _contextSettings(contextSettings)
{ }


void GLRenderingContext::updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ)
{
    glViewport(x, y, width, height);
}

#ifdef _WIN32
void GLRenderingContext::createContext(void* param)
{
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    HDC hdc = *reinterpret_cast<HDC*>(param);

    const int pixelFormat = ChoosePixelFormat(hdc, &pfd);

    if(!pixelFormat) { return; }

    const BOOL res = SetPixelFormat(hdc, pixelFormat, &pfd);

    if(!res) { return; }

    HGLRC tmpContext = wglCreateContext(hdc);
    wglMakeCurrent(hdc, tmpContext);

    if(glewInit() != GLEW_OK) { return; }

    if(WGLEW_ARB_create_context)
    {
        const int attribs[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, this->_contextSettings.majorVersion,
            WGL_CONTEXT_MINOR_VERSION_ARB, this->_contextSettings.minorVersion,
            WGL_CONTEXT_FLAGS_ARB,        (this->_contextSettings.debug             ? WGL_CONTEXT_DEBUG_BIT_ARB                 : 0) |
                                          (this->_contextSettings.forwardCompatible ? WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB    : 0),
            WGL_CONTEXT_PROFILE_MASK_ARB, (this->_contextSettings.coreProfile       ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB          : 0) |
                                          (this->_contextSettings.compatProfile     ? WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB : 0),
            0
        };

        this->_context = wglCreateContextAttribsARB(hdc, null, attribs);
        wglMakeCurrent(null, null);
        wglDeleteContext(tmpContext);
        wglMakeCurrent(hdc, this->_context);
    }
    else
    {
        this->_context = tmpContext;
    }
}
#else
void GLRenderingContext::createContext()
{ }
#endif

IRenderingContext* createGLContext(GLContextSettings settings) noexcept
{
    return new GLRenderingContext(settings);
}
