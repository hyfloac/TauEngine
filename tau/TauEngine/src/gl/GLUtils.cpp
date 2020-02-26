#include <spdlog/spdlog.h>
#include <gl/GLUtils.hpp>
#include <Utils.hpp>

static void __cdecl openGLDebugErrorDefaultCallback(GLDebugSource source, GLDebugType type, GLDebugSeverity severity, u32 id, iSys length, const char* message, const void* userParam) noexcept;

static void GLAPIENTRY openGLDebugErrorWrapperCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) noexcept;
static void GLAPIENTRY openGLDebugErrorWrapperCallbackARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) noexcept;
static void GLAPIENTRY openGLDebugErrorWrapperCallbackAMD(GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar* message, void* userParam) noexcept;

[[nodiscard]] static GLDebugSource glSource(GLenum source);
[[nodiscard]] static GLDebugSource glSourceARB(GLenum source);
[[nodiscard]] static GLDebugSource glSourceAMD(GLenum source);

[[nodiscard]] static GLDebugType glType(GLenum type);
[[nodiscard]] static GLDebugType glTypeARB(GLenum type);

[[nodiscard]] static GLDebugSeverity glSeverity(GLenum filter) noexcept;
[[nodiscard]] static GLDebugSeverity glSeverityARB(GLenum filter) noexcept;
[[nodiscard]] static GLDebugSeverity glSeverityAMD(GLenum filter) noexcept;

[[nodiscard]] static GLenum glSeverity(GLDebugSeverity filter) noexcept;
[[nodiscard]] static GLenum glSeverityARB(GLDebugSeverity filter) noexcept;
[[nodiscard]] static GLenum glSeverityAMD(GLDebugSeverity filter) noexcept;

struct GLDebugCallbackData final
{
    debugMessageCallback_f callback;
    void* userParam;
};

static GLDebugCallbackData* callbackData = null;

void setupDebugMessageCallback(Nullable debugMessageCallback_f callback, Nullable void* userParam, bool synchronous) noexcept
{
    delete callbackData;

    callbackData = new GLDebugCallbackData{ callback, userParam };

    if(GLEW_KHR_debug)
    {
        glEnable(GL_DEBUG_OUTPUT);
        if(synchronous)
        {
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        }
        else
        {
            glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        }

        glDebugMessageCallback(openGLDebugErrorWrapperCallback, callbackData);
    }
    else if(GLEW_ARB_debug_output)
    {
        if(synchronous)
        {
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
        }
        else
        {
            glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
        }

        glDebugMessageCallbackARB(openGLDebugErrorWrapperCallbackARB, callbackData);
    }
    else if(GLEW_AMD_debug_output)
    {
        glDebugMessageEnableAMD(0, 0, 0, null, true);
        glDebugMessageCallbackAMD(openGLDebugErrorWrapperCallbackAMD, callbackData);
    }
    else
    {
        delete callbackData;
    }
}

void stopDebugOutput() noexcept
{
    delete callbackData;
    callbackData = null;

    if(GLEW_KHR_debug)
    {
        glDisable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(null, null);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, null, GL_FALSE);
    }
    else if(GLEW_ARB_debug_output)
    {
        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, null, GL_FALSE);
        glDebugMessageCallbackARB(null, null);
    }
    else if(GLEW_AMD_debug_output)
    {
        glDebugMessageEnableAMD(0, 0, 0, null, GL_FALSE);
        glDebugMessageCallbackAMD(null, null);
    }
}

void filterDebugOutput(GLDebugSeverity filter, bool allowed) noexcept
{
    if(GLEW_KHR_debug)
    {
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, glSeverity(filter), 0, null, allowed);
    }
    else if(GLEW_ARB_debug_output)
    {
        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, glSeverityARB(filter), 0, null, allowed);
    }
    else if(GLEW_AMD_debug_output)
    {
        glDebugMessageEnableAMD(0, glSeverityAMD(filter), 0, null, allowed);
    }
}

static Ref<spdlog::logger> _logger;

void setupDefaultDebugMessageCallback(const Ref<spdlog::logger>& logger, bool synchronous) noexcept
{
    _logger = logger;
    setupDebugMessageCallback(openGLDebugErrorDefaultCallback, &_logger, synchronous);
}

void stopDefaultDebugOutput() noexcept
{
    _logger = null;
    stopDebugOutput();
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
        }
        else
        {
            break;
        }
    } while(true);

    return noErrors;
}

static void __cdecl openGLDebugErrorDefaultCallback(GLDebugSource source, GLDebugType type, GLDebugSeverity severity, u32 id, iSys length, const char* message, const void* userParam) noexcept
{
    UNUSED4(length, userParam, id, message);

    const Ref<spdlog::logger>& logger = *reinterpret_cast<const Ref<spdlog::logger>*>(userParam);

#define STR_CASE_GL(__ENUM, __STR) case __ENUM: str = __STR; break
#define DEFAULT_CASE default: str = "Unknown"; break

    logger->error("--OpenGL Error Callback--");
    logger->error("Message: {0}", message);
    const char* str;

    switch(source)
    {
        STR_CASE_GL(GLDebugSource::OpenGLCall, "API");
        STR_CASE_GL(GLDebugSource::WindowSystem, "Window System");
        STR_CASE_GL(GLDebugSource::ShaderCompiler, "Shader Compiler");
        STR_CASE_GL(GLDebugSource::ThirdParty, "Third Party");
        STR_CASE_GL(GLDebugSource::Application, "Application");
        STR_CASE_GL(GLDebugSource::Other, "Other");
        DEFAULT_CASE;
    }
    logger->error("Source {1}", source, str);

    switch(type)
    {
        STR_CASE_GL(GLDebugType::Error, "Error");
        STR_CASE_GL(GLDebugType::DeprecatedBehavior, "Deprecated Behavior");
        STR_CASE_GL(GLDebugType::UndefinedBehavior, "Undefined Behavior");
        STR_CASE_GL(GLDebugType::Portability, "Portability");
        STR_CASE_GL(GLDebugType::Performance, "Performance");
        STR_CASE_GL(GLDebugType::Marker, "Marker");
        STR_CASE_GL(GLDebugType::PushGroup, "Push Group");
        STR_CASE_GL(GLDebugType::PopGroup, "Pop Group");
        STR_CASE_GL(GLDebugType::Other, "Other");
        DEFAULT_CASE;
    }
    logger->error("Type: {1}", type, str);

    switch(severity)
    {
        STR_CASE_GL(GLDebugSeverity::High, "High");
        STR_CASE_GL(GLDebugSeverity::Medium, "Medium");
        STR_CASE_GL(GLDebugSeverity::Low, "Low");
        STR_CASE_GL(GLDebugSeverity::Notification, "Notification");
        DEFAULT_CASE;
    }
    logger->error("Severity: {1}", severity, str);

    logger->error("Id: 0x{0:X} // {0:d}", id);

    logger->error("-------------------------");

#undef DEFAULT_CASE
#undef STR_CASE_GL
}

static void GLAPIENTRY openGLDebugErrorWrapperCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) noexcept
{
    const GLDebugCallbackData* const callbackData = reinterpret_cast<const GLDebugCallbackData*>(userParam);

    const GLDebugSource passSource = glSource(source);
    const GLDebugType passType = glType(type);
    const GLDebugSeverity passSeverity = glSeverity(severity);
    const u32 passID = id;
    const iSys passLength = length;
    const char* const passMessage = message;
    const void* const passUserParam = callbackData->userParam;

    callbackData->callback(passSource, passType, passSeverity, passID, passLength, passMessage, passUserParam);
}

static void GLAPIENTRY openGLDebugErrorWrapperCallbackARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) noexcept
{
    const GLDebugCallbackData* const callbackData = reinterpret_cast<const GLDebugCallbackData*>(userParam);

    const GLDebugSource passSource = glSourceARB(source);
    const GLDebugType passType = glTypeARB(type);
    const GLDebugSeverity passSeverity = glSeverityARB(severity);
    const u32 passID = id;
    const iSys passLength = length;
    const char* const passMessage = message;
    const void* const passUserParam = callbackData->userParam;

    callbackData->callback(passSource, passType, passSeverity, passID, passLength, passMessage, passUserParam);
}

static void GLAPIENTRY openGLDebugErrorWrapperCallbackAMD(GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar* message, void* userParam) noexcept
{
    const GLDebugCallbackData* const callbackData = reinterpret_cast<const GLDebugCallbackData*>(userParam);

    const GLDebugSource passSource = glSourceAMD(category);
    const GLDebugType passType = GLDebugType::Unknown;
    const GLDebugSeverity passSeverity = glSeverityAMD(severity);
    const u32 passID = id;
    const iSys passLength = length;
    const char* const passMessage = message;
    const void* const passUserParam = callbackData->userParam;

    callbackData->callback(passSource, passType, passSeverity, passID, passLength, passMessage, passUserParam);
}

static GLDebugSource glSource(const GLenum source)
{
    switch(source)
    {
        case GL_DEBUG_SOURCE_API: return GLDebugSource::OpenGLCall;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return GLDebugSource::WindowSystem;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return GLDebugSource::ShaderCompiler;
        case GL_DEBUG_SOURCE_THIRD_PARTY: return GLDebugSource::ThirdParty;
        case GL_DEBUG_SOURCE_APPLICATION: return GLDebugSource::Application;
        case GL_DEBUG_SOURCE_OTHER: return GLDebugSource::Other;
        default: return GLDebugSource::Unknown;
    }
}

static GLDebugSource glSourceARB(const GLenum source)
{
    switch(source)
    {
        case GL_DEBUG_SOURCE_API_ARB: return GLDebugSource::OpenGLCall;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB: return GLDebugSource::WindowSystem;
        case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: return GLDebugSource::ShaderCompiler;
        case GL_DEBUG_SOURCE_THIRD_PARTY_ARB: return GLDebugSource::ThirdParty;
        case GL_DEBUG_SOURCE_APPLICATION_ARB: return GLDebugSource::Application;
        case GL_DEBUG_SOURCE_OTHER_ARB: return GLDebugSource::Other;
        default: return GLDebugSource::Unknown;
    }
}

static GLDebugSource glSourceAMD(const GLenum source)
{
    switch(source)
    {
        case GL_DEBUG_CATEGORY_API_ERROR_AMD: return GLDebugSource::OpenGLCall;
        case GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD: return GLDebugSource::WindowSystem;
        case GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD: return GLDebugSource::ShaderCompiler;
        case GL_DEBUG_CATEGORY_APPLICATION_AMD: return GLDebugSource::Application;
        case GL_DEBUG_CATEGORY_OTHER_AMD: return GLDebugSource::Other;
        default: return GLDebugSource::Unknown;
    }
}

static GLDebugType glType(const GLenum type)
{
    switch(type)
    {
        case GL_DEBUG_TYPE_ERROR: return GLDebugType::Error;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return GLDebugType::DeprecatedBehavior;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return GLDebugType::UndefinedBehavior;
        case GL_DEBUG_TYPE_PORTABILITY: return GLDebugType::Portability;
        case GL_DEBUG_TYPE_PERFORMANCE: return GLDebugType::Performance;
        case GL_DEBUG_TYPE_MARKER: return GLDebugType::Marker;
        case GL_DEBUG_TYPE_PUSH_GROUP: return GLDebugType::PushGroup;
        case GL_DEBUG_TYPE_POP_GROUP: return GLDebugType::PopGroup;
        case GL_DEBUG_TYPE_OTHER: return GLDebugType::Other;
        default: return GLDebugType::Unknown;
    }
}

static GLDebugType glTypeARB(const GLenum type)
{
    switch(type)
    {
        case GL_DEBUG_TYPE_ERROR_ARB: return GLDebugType::Error;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: return GLDebugType::DeprecatedBehavior;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB: return GLDebugType::UndefinedBehavior;
        case GL_DEBUG_TYPE_PORTABILITY_ARB: return GLDebugType::Portability;
        case GL_DEBUG_TYPE_PERFORMANCE_ARB: return GLDebugType::Performance;
        case GL_DEBUG_TYPE_OTHER_ARB: return GLDebugType::Other;
        default: return GLDebugType::Unknown;
    }
}

static GLDebugSeverity glSeverity(const GLenum filter) noexcept
{
    switch(filter)
    {
        case GL_DEBUG_SEVERITY_HIGH: return GLDebugSeverity::High;
        case GL_DEBUG_SEVERITY_MEDIUM: return GLDebugSeverity::Medium;
        case GL_DEBUG_SEVERITY_LOW: return GLDebugSeverity::Low;
        case GL_DEBUG_SEVERITY_NOTIFICATION: return GLDebugSeverity::Notification;
        default: return GLDebugSeverity::Unknown;
    }
}

static GLDebugSeverity glSeverityARB(const GLenum filter) noexcept
{
    switch(filter)
    {
        case GL_DEBUG_SEVERITY_HIGH_ARB: return GLDebugSeverity::High;
        case GL_DEBUG_SEVERITY_MEDIUM_ARB: return GLDebugSeverity::Medium;
        case GL_DEBUG_SEVERITY_LOW_ARB: return GLDebugSeverity::Low;
        default: return GLDebugSeverity::Unknown;
    }
}

static GLDebugSeverity glSeverityAMD(const GLenum filter) noexcept
{
    switch(filter)
    {
        case GL_DEBUG_SEVERITY_HIGH_AMD: return GLDebugSeverity::High;
        case GL_DEBUG_SEVERITY_MEDIUM_AMD: return GLDebugSeverity::Medium;
        case GL_DEBUG_SEVERITY_LOW_AMD: return GLDebugSeverity::Low;
        default: return GLDebugSeverity::Unknown;
    }
}

static GLenum glSeverity(const GLDebugSeverity filter) noexcept
{
    switch(filter)
    {
        case GLDebugSeverity::High:         return GL_DEBUG_SEVERITY_HIGH;
        case GLDebugSeverity::Medium:       return GL_DEBUG_SEVERITY_MEDIUM;
        case GLDebugSeverity::Low:          return GL_DEBUG_SEVERITY_LOW;
        case GLDebugSeverity::Notification: return GL_DEBUG_SEVERITY_NOTIFICATION;
        default: return GL_DONT_CARE;
    }
}

static GLenum glSeverityARB(const GLDebugSeverity filter) noexcept
{
    switch(filter)
    {
        case GLDebugSeverity::High:         return GL_DEBUG_SEVERITY_HIGH_ARB;
        case GLDebugSeverity::Medium:       return GL_DEBUG_SEVERITY_MEDIUM_ARB;
        case GLDebugSeverity::Low:          return GL_DEBUG_SEVERITY_LOW_ARB;
        case GLDebugSeverity::Notification: return GL_DONT_CARE;
        default: return GL_DONT_CARE;
    }
}

static GLenum glSeverityAMD(const GLDebugSeverity filter) noexcept
{
    switch(filter)
    {
        case GLDebugSeverity::High:         return GL_DEBUG_SEVERITY_HIGH_AMD;
        case GLDebugSeverity::Medium:       return GL_DEBUG_SEVERITY_MEDIUM_AMD;
        case GLDebugSeverity::Low:          return GL_DEBUG_SEVERITY_LOW_AMD;
        case GLDebugSeverity::Notification: return 0;
        default: return 0;
    }
}
