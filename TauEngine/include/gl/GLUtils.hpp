#pragma once

#include <GL/glew.h>

typedef void (GLAPIENTRY *debugMessageCallback_f)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

void setupDebugMessageCallback(debugMessageCallback_f callback, void* userParam, bool synchronous);
