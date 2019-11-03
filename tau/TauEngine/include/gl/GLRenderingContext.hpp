#pragma once

#include <unordered_map>
#include <system/RenderingContext.hpp>
#include "model/IVertexArray.hpp"
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
    std::unordered_map<IVertexArray*, GLuint> _vaos;
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

    [[nodiscard]] Ref<IVertexArray> createVertexArray(std::size_t attribCount) noexcept override;
    [[nodiscard]] void* getVertexArrayHandle(IVertexArray* vertexArray) noexcept override final;

    void destroyVA(IVertexArray* vertexArray) noexcept override final;

    void clearVAs() noexcept override final;

    void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ = 0, float maxZ = 0) noexcept override final;

    void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue = 1.0f, int stencilValue = 0) noexcept override final;
protected:
    bool createContextsShared(void* param, IRenderingContext** sharers, std::size_t count) noexcept override final;
private:
    void handleCtxError(int profileMask) const noexcept;

    static u32 getVAUid() noexcept;

    RC_IMPL(GLRenderingContext);
};
