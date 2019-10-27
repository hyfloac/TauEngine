#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)
#include <NumTypes.hpp>
#include <DLL.hpp>
#include <Safeties.hpp>

typedef void (GLAPIENTRY *debugMessageCallback_f)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

TAU_DLL void setupDebugMessageCallback(Nullable debugMessageCallback_f callback, Nullable void* userParam, bool synchronous) noexcept;

TAU_DLL void stopDebugOutput() noexcept;

TAU_DLL void __internal__clearGLErrors() noexcept;

TAU_DLL bool __internal__logGLCall(const char* glFunc, const char* file, u32 line) noexcept;

#define ___ASSERT(__TEST) if(!(__TEST)) { DEBUG_BREAK; }

#if !defined(TAU_PRODUCTION)
  #define CALL_GL_FUNC(__CALL) __internal__clearGLErrors(), __CALL; ___ASSERT(__internal__logGLCall(#__CALL, __FILE__, __LINE__))
#else
  #define CALL_GL_FUNC(__CALL)
#endif

#define GLCall(__CALL) CALL_GL_FUNC(__CALL)

#define GL_STATE_CONTROLLER(__F_NAME, __E_NAME) inline static void enableGL##__F_NAME() noexcept { glEnable(__E_NAME); }   \
                                                inline static void disableGL##__F_NAME() noexcept { glDisable(__E_NAME); } \
                                                inline static void setGL##__F_NAME(bool state) noexcept                    \
                                                {                                                                          \
                                                    if(state) { enableGL##__F_NAME();  }                                   \
                                                    else      { disableGL##__F_NAME(); }                                   \
                                                }                                                                          \
                                                inline static void toggleGL##__F_NAME() noexcept { setGL##__F_NAME(!glIsEnabled(__E_NAME)); }

GL_STATE_CONTROLLER(Blend, GL_BLEND)
GL_STATE_CONTROLLER(CullFace, GL_CULL_FACE)
GL_STATE_CONTROLLER(DebugOutput, GL_DEBUG_OUTPUT)
GL_STATE_CONTROLLER(DebugOutputSync, GL_DEBUG_OUTPUT_SYNCHRONOUS)
GL_STATE_CONTROLLER(DepthTest, GL_DEPTH_TEST)

inline static void glWireframe()  noexcept { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  }
inline static void glFilled()     noexcept { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  }
inline static void glVertices()   noexcept { glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); }
