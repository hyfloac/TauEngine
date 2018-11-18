#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)
#include <NumTypes.hpp>
#include <DLL.hpp>
#include <Safeties.hpp>

typedef void (GLAPIENTRY *debugMessageCallback_f)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

TAU_DLL void setupDebugMessageCallback(Nullable debugMessageCallback_f callback, Nullable void* userParam, bool synchronous);

TAU_DLL void __internal__clearGLErrors();

TAU_DLL bool __internal__logGLCall(const char* glFunc, const char* file, u32 line);

#define ASSERT(__TEST) if(!(__TEST)) { DEBUG_BREAK; }
#ifdef _DEBUG
  #define CALL_GL_FUNC(__CALL) __internal__clearGLErrors(), __CALL; ASSERT(__internal__logGLCall(#__CALL, __FILE__, __LINE__))
#else
  #define CALL_GL_FUNC(__CALL)
#endif

#define GLCall(__CALL) CALL_GL_FUNC(__CALL)
