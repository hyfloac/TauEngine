/**
 * @file 
 */
#pragma once

#include <NumTypes.hpp>
#include <RunTimeType.hpp>
#include <Objects.hpp>
#include "Color.hpp"
#include "DLL.hpp"
#include "events/Exception.hpp"
#include "RenderingMode.hpp"
// #include "model/Buffer.hpp"

#define RC_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                            RTT_IMPL(_TYPE, IRenderingContext)

#define RC_IMPL(_TYPE) RC_IMPL_BASE(_TYPE)

class Window;

class IVertexArrayBuilder;
class IBufferBuilder;
class IIndexBufferBuilder;
class IUniformBufferBuilder;
class IFrameBufferBuilder;
class ITextureBuilder;
class ITextureCubeBuilder;
class ITextureSamplerBuilder;
class ITextureUploaderBuilder;
class ISingleTextureUploaderBuilder;
class IShaderBuilder;

class TAU_DLL IRenderingContext
{
    DEFAULT_DESTRUCT_VI(IRenderingContext);
    DELETE_COPY(IRenderingContext);
public:
    static IRenderingContext* create(const RenderingMode& mode) noexcept;

    using ContextType = RunTimeType<IRenderingContext>;
protected:
    const RenderingMode& _mode;
protected:
    IRenderingContext(const RenderingMode & mode)
        : _mode(mode)
    { }
public:
    [[nodiscard]] const RenderingMode& mode() const noexcept { return _mode; }

    [[nodiscard]] virtual bool createContext(Window& window) noexcept = 0;

    virtual void deactivateContext() noexcept = 0;

    virtual void activateContext() noexcept = 0;

    virtual void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ = 0.0f, float maxZ = 1.0f) noexcept = 0;

    virtual void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue = 1.0f, u8 stencilValue = 0) noexcept = 0;

    virtual void setVSync(bool vsync) noexcept = 0;

    virtual void setFaceWinding(bool clockwise) noexcept = 0;

    virtual void enableDepthWriting(bool writing) noexcept = 0;

    virtual void beginFrame() noexcept = 0;

    virtual void endFrame() noexcept = 0;

    virtual void swapFrame() noexcept = 0;

    [[nodiscard]] virtual Ref<IVertexArrayBuilder> createVertexArray(uSys bufferCount) noexcept = 0;
    [[nodiscard]] virtual IBufferBuilder& createBuffer() noexcept = 0;
    [[nodiscard]] virtual IIndexBufferBuilder& createIndexBuffer() noexcept = 0;
    [[nodiscard]] virtual IUniformBufferBuilder& createUniformBuffer() noexcept = 0;
    [[nodiscard]] virtual Ref<IFrameBufferBuilder> createFrameBuffer() noexcept = 0;
    [[nodiscard]] virtual Ref<ITextureBuilder> createTexture2D() noexcept = 0;
    [[nodiscard]] virtual Ref<ITextureBuilder> createNullTexture() noexcept = 0;
    [[nodiscard]] virtual Ref<ITextureBuilder> createTextureDepth() noexcept = 0;
    [[nodiscard]] virtual Ref<ITextureCubeBuilder> createTextureCube() noexcept = 0;
    [[nodiscard]] virtual Ref<ITextureSamplerBuilder> createTextureSampler() noexcept = 0;
    [[nodiscard]] virtual Ref<ITextureUploaderBuilder> createTextureUploader(uSys textureCount) noexcept = 0;
    [[nodiscard]] virtual Ref<ISingleTextureUploaderBuilder> createSingleTextureUploader() noexcept = 0;
    [[nodiscard]] virtual Ref<IShaderBuilder> createShader() noexcept = 0;

    RTT_BASE_IMPL(IRenderingContext);
    RTT_BASE_CHECK(IRenderingContext);
    RTT_BASE_CAST(IRenderingContext);
};

class IncorrectContextException final : public Exception
{
public:
    IncorrectContextException() noexcept = default;
    ~IncorrectContextException() noexcept override final = default;

    EXCEPTION_IMPL(IncorrectContextException);
};
