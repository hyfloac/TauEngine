#ifdef _WIN32
#include "gl/GLRenderingContext.hpp"
#include "system/Window.hpp"

GLRenderingContext::~GLRenderingContext() noexcept
{
    delete _bufferBuilder;
    delete _indexBufferBuilder;
    delete _uniformBufferBuilder;

    deactivateContext();
    wglDeleteContext(_context);
}

#if defined(TAU_PRODUCTION)
void GLRenderingContext::handleCtxError(int profileMask) const noexcept
{
    const DWORD errorCode = GetLastError();

    fputs("Error 0x847FA8BB.\n", stderr);
    fprintf(stderr, "  EC: %08lX\n", errorCode);

    fputs("  R_OGL\n", stderr);
    fprintf(stderr, "  V%u.%u|%d\n", this->_majorVersion, this->_minorVersion, this->_forwardCompatible);
    if(this->_compat == GLProfile::Core)
    {
        fputs("  CorP\n", stderr);
    }
    else if(this->_compat == GLProfile::Compat)
    {
        fputs("  ComP\n", stderr);
    }
    if(this->_debug)
    {
        fputs("  DbgC\n", stderr);
    }
}
#else
void GLRenderingContext::handleCtxError(int profileMask) const noexcept
{
    const DWORD errorCode = GetLastError();
    const DWORD errorCodeLow = errorCode & 0xFFFF;
    const DWORD errorCodeHigh = errorCode >> 16;

    fputs("Error creating context with attributes.\n", stderr);

    LPVOID lpMsgBuf;
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   null,
                   errorCodeLow,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   reinterpret_cast<LPSTR>(&lpMsgBuf),
                   0, null);

    fprintf(stderr, "Error string from system (may not be accurate): %s\n", reinterpret_cast<LPSTR>(lpMsgBuf));
    LocalFree(lpMsgBuf);

    fprintf(stderr, "Error: %lu (0x%08lX)\n", errorCode, errorCode);
    fprintf(stderr, "Error Low: %lu (0x%04lX)\n", errorCodeLow, errorCodeLow);
    fprintf(stderr, "Error High: %lu [%lu] (0x%04lX)\n", errorCodeHigh, errorCodeHigh << 16, errorCodeHigh);

    fputs("Requested OpenGL\n", stderr);
    fprintf(stderr, "  Version: %u.%u%s\n", this->_majorVersion, this->_minorVersion, this->_forwardCompatible ? " Forward Compatible" : "");
    if(this->_compat == GLProfile::Core)
    {
        fputs("  Core Profile\n", stderr);
    }
    else if(this->_compat == GLProfile::Compat)
    {
        fputs("  Compatibility Profile\n", stderr);
    }
    if(this->_mode.debugMode())
    {
        fputs("  Debug Context\n", stderr);
    }

    if(errorCodeLow == ERROR_INVALID_VERSION_ARB)
    {
        fputs("Error Code Enum: ERROR_INVALID_VERSION_ARB\n", stderr);
        fputs("Invalid OpenGL Version Requested\n", stderr);

        if(this->_majorVersion < 1)
        {
            fputs("The minimum requestable OpenGL major version is 1.\n", stderr);
        }
        else if(this->_majorVersion == 1 && this->_minorVersion > 5)
        {
            fputs("The maximum requestable OpenGL minor version with major version 1 is 5.\n", stderr);
        }
        else if(this->_majorVersion == 2 && this->_minorVersion > 1)
        {
            fputs("The maximum requestable OpenGL minor version with major version 2 is 1.\n", stderr);
        }
        else if(this->_majorVersion > 4)
        {
            fputs("At the time of compilation the maximum requestable OpenGL major version is 4.\n", stderr);
        }

        if(this->_majorVersion < 3 && this->_majorVersion > 0 && this->_forwardCompatible)
        {
            fputs("Forward Compatible profiles are not compatible with OpenGL major version less than 3.\n", stderr);
        }
    }
    else if(errorCodeLow == ERROR_INVALID_PROFILE_ARB)
    {
        fputs("Error Code Enum: ERROR_INVALID_PROFILE_ARB\n", stderr);
        fputs("Invalid OpenGL Profile Requested\n", stderr);

        if(this->_compat != GLProfile::Core && this->_compat != GLProfile::Compat)
        {
            fputs("Neither Core Profile nor Compatibility Profile was requested. This is an invalid request.\n", stderr);
        }
        // else if(this->_compat == GLProfile::Core && this->_compat == GLProfile::Compat)
        // {
        //     fputs("Core Profile and Compatibility Profile was requested. This is an invalid request.\n", stderr);
        // }

        const u32 otherFlags = profileMask & ~(WGL_CONTEXT_CORE_PROFILE_BIT_ARB | WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB);
        if(otherFlags != 0)
        {
            fputs("Flags other than `WGL_CONTEXT_CORE_PROFILE_BIT_ARB` and `WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB` were set.\n", stderr);
            fprintf(stderr, "Flags: 0x%x\n", profileMask);
            fprintf(stderr, "Other Flags: 0x%08x\n", otherFlags);
        }
    }
    else if(errorCodeLow == ERROR_INVALID_OPERATION)
    {
        fputs("Error Code Enum: ERROR_INVALID_OPERATION\n", stderr);
        fprintf(stderr, "hSharedContext is neither null nor a valid context.\n");
    }
    else if(errorCodeLow == ERROR_DC_NOT_FOUND)
    {
        fputs("Error Code Enum: ERROR_DC_NOT_FOUND\n", stderr);
        fprintf(stderr, "Invalid Device Context\n");
    }
    else if(errorCodeLow == ERROR_INVALID_PIXEL_FORMAT)
    {
        fputs("Error Code Enum: ERROR_INVALID_PIXEL_FORMAT\n", stderr);
        fprintf(stderr, "Invalid Pixel Format\n");
    }
    else if(errorCodeLow == ERROR_NO_SYSTEM_RESOURCES)
    {
        fputs("Error Code Enum: ERROR_NO_SYSTEM_RESOURCES\n", stderr);
        fprintf(stderr, "Insufficient System Resources\n");
    }
    else if(errorCodeLow == ERROR_INVALID_PARAMETER)
    {
        fputs("Error Code Enum: ERROR_INVALID_PARAMETER\n", stderr);
        fprintf(stderr, "Unrecognized Attribute In Attribute List\n");
    }
    else
    {
        fprintf(stderr, "Error code did not match any expected errors.\n");
    }
}
#endif

bool GLRenderingContext::createContext(Window& window) noexcept
{
    // return createContextsShared(window, null, 0);

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

     // ReSharper disable once CppLocalVariableMayBeConst
     this->_device = GetDC(window.sysWindowContainer().windowHandle);

     const int pixelFormat = ChoosePixelFormat(this->_device, &pfd);

     if(!pixelFormat) { return false; }

     const BOOL res = SetPixelFormat(this->_device, pixelFormat, &pfd);

     if(!res) { return false; }

     // ReSharper disable once CppLocalVariableMayBeConst
     HGLRC tmpContext = wglCreateContext(this->_device);

     if(!tmpContext)
     {
         handleCtxError(0);
         return false;
     }

     wglMakeCurrent(this->_device, tmpContext);

     if(glewInit() != GLEW_OK) { return false; }

     if(WGLEW_ARB_create_context)
     {
         const int attribs[] =
         {
             WGL_CONTEXT_MAJOR_VERSION_ARB, this->_majorVersion,
             WGL_CONTEXT_MINOR_VERSION_ARB, this->_minorVersion,
             WGL_CONTEXT_FLAGS_ARB,        (this->_mode.debugMode() ? WGL_CONTEXT_DEBUG_BIT_ARB : 0) |
                                           (this->_forwardCompatible ? WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB : 0),
             WGL_CONTEXT_PROFILE_MASK_ARB, (this->_compat == GLProfile::Core ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB :
                                            this->_compat == GLProfile::Compat ? WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB : 0),
             0
         };

         // ReSharper disable once CppLocalVariableMayBeConst
         HGLRC attribContext = wglCreateContextAttribsARB(this->_device, null, attribs);

         if(!attribContext)
         {
             handleCtxError(attribs[7]);
             return false;
         }

         this->_context = attribContext;

         wglMakeCurrent(null, null);
         wglDeleteContext(tmpContext);
     }
     else
     {
         this->_context = tmpContext;
     }

     return true;
}

struct ContextShareData final
{
    HDC hdc;
    HGLRC hglrc;
};

// bool GLRenderingContext::createContextsShared(Window& window, IRenderingContext** sharers, std::size_t count) noexcept
// {
//     PIXELFORMATDESCRIPTOR pfd;
//     memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
//
//     pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
//     pfd.nVersion = 1;
//     pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
//     pfd.iPixelType = PFD_TYPE_RGBA;
//     pfd.cColorBits = 32;
//     pfd.cDepthBits = 24;
//     pfd.cStencilBits = 8;
//     pfd.iLayerType = PFD_MAIN_PLANE;
//
//     // ReSharper disable once CppLocalVariableMayBeConst
//     // this->_device = *reinterpret_cast<HDC*>(param);
//     // this->_device = GetDC(*reinterpret_cast<HWND*>(param));
//     this->_device = GetDC(window.sysWindowContainer().windowHandle);
//
//     const int pixelFormat = ChoosePixelFormat(this->_device, &pfd);
//
//     if(!pixelFormat) { return false; }
//
//     const BOOL res = SetPixelFormat(this->_device, pixelFormat, &pfd);
//
//     if(!res) { return false; }
//
//     // ReSharper disable once CppLocalVariableMayBeConst
//     HGLRC tmpContext = wglCreateContext(this->_device);
//
//     if(!tmpContext)
//     {
//         handleCtxError(0);
//         return false;
//     }
//
//     wglMakeCurrent(this->_device, tmpContext);
//
//     if(glewInit() != GLEW_OK) { return false; }
//
//     if(WGLEW_ARB_create_context)
//     {
//         const int attribs[] =
//         {
//             WGL_CONTEXT_MAJOR_VERSION_ARB, this->_majorVersion,
//             WGL_CONTEXT_MINOR_VERSION_ARB, this->_minorVersion,
//             WGL_CONTEXT_FLAGS_ARB,        (this->_mode.debugMode() ? WGL_CONTEXT_DEBUG_BIT_ARB : 0) |
//                                           (this->_forwardCompatible ? WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB : 0),
//             WGL_CONTEXT_PROFILE_MASK_ARB, (this->_compat == GLProfile::Core ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB :
//                                            this->_compat == GLProfile::Compat ? WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB : 0),
//             0
//         };
//
//         // ReSharper disable once CppLocalVariableMayBeConst
//         HGLRC attribContext = wglCreateContextAttribsARB(this->_device, null, attribs);
//
//         if(!attribContext)
//         {
//             handleCtxError(attribs[7]);
//             return false;
//         }
//
//         this->_context = attribContext;
//
//         wglMakeCurrent(null, null);
//         wglDeleteContext(tmpContext);
//
//         for(std::size_t i = 0; i < count; ++i)
//         {
//             // ReSharper disable once CppLocalVariableMayBeConst
//             HGLRC sharedContext = wglCreateContextAttribsARB(this->_device, null, attribs);
//             wglShareLists(attribContext, sharedContext);
//             ContextShareData shareData { this->_device, sharedContext };
//             sharers[i]->createFromShared(&shareData);
//         }
//
//         // wglMakeCurrent(this->_device, this->_context);
//     }
//     else
//     {
//         this->_context = tmpContext;
//     }
//
//     return true;
// }
//
// void GLRenderingContext::createFromShared(void* param) noexcept
// {
//     ContextShareData* shareData = reinterpret_cast<ContextShareData*>(param);
//     this->_device = shareData->hdc;
//     this->_context = shareData->hglrc;
// }

void GLRenderingContext::deactivateContext() noexcept
{
    wglMakeCurrent(null, null);
}

void GLRenderingContext::activateContext() noexcept
{
    wglMakeCurrent(this->_device, this->_context);
}

void GLRenderingContext::setVSync(bool vsync) noexcept
{
    if(WGLEW_EXT_swap_control)
    {
        wglSwapIntervalEXT(vsync ? TRUE : FALSE);
    }
}

void GLRenderingContext::swapFrame() noexcept
{
    if(this->_device)
    {
        SwapBuffers(this->_device);
    }
}
#endif
