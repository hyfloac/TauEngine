#pragma once

#include "texture/FrameBuffer.hpp"

#ifdef _WIN32
#include <d3d11.h>

class DX11GraphicsInterface;

class TAU_DLL DX11FrameBuffer final : public IFrameBuffer
{
    DELETE_COPY(DX11FrameBuffer);
private:
    ID3D11RenderTargetView** _d3dColorAttachments;
public:
    inline DX11FrameBuffer(const RefDynArray<CPPRef<ITexture>>& colorAttachments, const CPPRef<ITextureDepthStencil>& depthStencilAttachment, ID3D11RenderTargetView** const d3dColorAttachments) noexcept
        : IFrameBuffer(colorAttachments, depthStencilAttachment),  _d3dColorAttachments(d3dColorAttachments)
    { }

    ~DX11FrameBuffer() noexcept
    {
        for(uSys i = 0; i < _colorAttachments.count(); ++i)
        {
            _d3dColorAttachments[i]->Release();
        }

        delete[] _d3dColorAttachments;
        _d3dColorAttachments = null;
    }

    void bind(IRenderingContext& context, AccessMode mode) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    void clearFrameBuffer(IRenderingContext& context, bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue = 1.0f, u8 stencilValue = 0) noexcept override;
};

class TAU_DLL DX11FrameBufferBuilder final : public IFrameBufferBuilder
{
    DEFAULT_DESTRUCT(DX11FrameBufferBuilder);
    DELETE_COPY(DX11FrameBufferBuilder);
public:
    struct DX11FrameBufferArgs final
    {
        DEFAULT_CONSTRUCT_PU(DX11FrameBufferArgs);
        DEFAULT_COPY(DX11FrameBufferArgs);
    public:
        ID3D11RenderTargetView** d3dColorAttachments;

        ~DX11FrameBufferArgs()
        {
            delete[] d3dColorAttachments;
            d3dColorAttachments = null;
        }
    };
private:
    DX11GraphicsInterface& _gi;
public:
    inline DX11FrameBufferBuilder(DX11GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX11FrameBuffer* build(const FrameBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11FrameBuffer* build(const FrameBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IFrameBuffer> buildCPPRef(const FrameBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IFrameBuffer> buildTauRef(const FrameBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IFrameBuffer> buildTauSRef(const FrameBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const FrameBufferArgs& args, DX11FrameBufferArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};

#endif
