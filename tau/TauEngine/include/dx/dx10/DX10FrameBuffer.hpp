#pragma once

#include "texture/FrameBuffer.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10GraphicsInterface;

class TAU_DLL DX10FrameBuffer final : public IFrameBuffer
{
    DELETE_COPY(DX10FrameBuffer);
private:
    ID3D10RenderTargetView** _d3dColorAttachments;
public:
    inline DX10FrameBuffer(const RefDynArray<CPPRef<ITexture>>& colorAttachments, const CPPRef<ITextureDepthStencil>& depthStencilAttachment, ID3D10RenderTargetView** const d3dColorAttachments) noexcept
        : IFrameBuffer(colorAttachments, depthStencilAttachment),  _d3dColorAttachments(d3dColorAttachments)
    { }

    ~DX10FrameBuffer() noexcept
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

class TAU_DLL DX10FrameBufferBuilder final : public IFrameBufferBuilder
{
    DEFAULT_DESTRUCT(DX10FrameBufferBuilder);
    DELETE_COPY(DX10FrameBufferBuilder);
public:
    struct DX10FrameBufferArgs final
    {
        DEFAULT_CONSTRUCT_PU(DX10FrameBufferArgs);
        DEFAULT_COPY(DX10FrameBufferArgs);
    public:
        ID3D10RenderTargetView** d3dColorAttachments;

        ~DX10FrameBufferArgs()
        {
            delete[] d3dColorAttachments;
            d3dColorAttachments = null;
        }
    };
private:
    DX10GraphicsInterface& _gi;
public:
    inline DX10FrameBufferBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX10FrameBuffer* build(const FrameBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX10FrameBuffer* build(const FrameBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IFrameBuffer> buildCPPRef(const FrameBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IFrameBuffer> buildTauRef(const FrameBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IFrameBuffer> buildTauSRef(const FrameBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const FrameBufferArgs& args, DX10FrameBufferArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};

#endif
