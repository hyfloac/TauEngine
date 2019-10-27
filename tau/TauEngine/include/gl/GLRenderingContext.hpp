#pragma once

#include <unordered_map>
#include <system/RenderingContext.hpp>
#include <GL/glew.h>
#ifdef _WIN32
#include <GL/wglew.h>
#endif

class GLRenderingContext final : public IRenderingContext
{
public:
    enum class GLProfile
    {
        Core = 0,
        Compat,
        Neither
    };
private:
#if defined(_WIN32)
    HDC _device;
    HGLRC _context;
#endif
    std::unordered_map<IBufferDescriptor*, GLuint> _vaos;
    int _majorVersion;
    int _minorVersion;
    GLProfile _compat;
    bool _forwardCompatible;
public:
    GLRenderingContext(const RenderingMode& mode, const bool debug, const int majorVersion, const int minorVersion, const GLProfile core, const bool forwardCompatible) noexcept;
    ~GLRenderingContext() noexcept override final;

    [[nodiscard]] bool createContext(void* param) noexcept override final;

    void createFromShared(void* param) noexcept override final;

    void deactivateContext() noexcept override final;
    void activateContext() noexcept override final;

    [[nodiscard]] Ref<IBufferDescriptor> createBufferDescriptor(std::size_t attribCount) noexcept override;
    [[nodiscard]] void* getBufferDescriptorHandle(IBufferDescriptor* bufferDescriptor) noexcept override final;

    // void bindBD(IBufferDescriptor* bufferDescriptor) noexcept override final;
    // void unbindBD(IBufferDescriptor* bufferDescriptor) noexcept override final;

    void destroyBD(IBufferDescriptor* bufferDescriptor) noexcept override final;

    void clearBDs() noexcept override final;

    void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ = 0, float maxZ = 0) noexcept override final;

    void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue = 1.0f, int stencilValue = 0) noexcept override final;
protected:
    bool createContextsShared(void* param, IRenderingContext** sharers, std::size_t count) noexcept override final;

    // void initBufferDescriptor(IBufferDescriptor* bufferDescriptor) noexcept override final;
private:
    void handleCtxError(int profileMask) const noexcept;

    static u32 getBDUid() noexcept;

    RC_IMPL(GLRenderingContext);
};
