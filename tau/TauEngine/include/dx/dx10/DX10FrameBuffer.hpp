#pragma once

#include "texture/FrameBuffer.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10GraphicsInterface;

class TAU_DLL DX10FrameBuffer final : public IFrameBuffer
{
    DELETE_CM(DX10FrameBuffer);
    FRAME_BUFFER_IMPL(DX10FrameBuffer);
private:
    ID3D10RenderTargetView* const * _d3dColorAttachments;
public:
    DX10FrameBuffer(const RefDynArray<NullableRef<IRenderTargetView>>& colorAttachments, const NullableRef<IDepthStencilView>& depthStencilAttachment, ID3D10RenderTargetView* const * const d3dColorAttachments) noexcept
        : IFrameBuffer(colorAttachments, depthStencilAttachment)
        , _d3dColorAttachments(d3dColorAttachments)
    { }

    ~DX10FrameBuffer() noexcept override
    { delete[] _d3dColorAttachments; }

    [[nodiscard]] ID3D10RenderTargetView* const * d3dColorAttachments() const noexcept { return _d3dColorAttachments; }
};

class TAU_DLL DX10FrameBufferBuilder final : public IFrameBufferBuilder
{
    DEFAULT_DESTRUCT(DX10FrameBufferBuilder);
    DEFAULT_CM_PU(DX10FrameBufferBuilder);
public:
    struct DX10FrameBufferArgs final
    {
        DEFAULT_CONSTRUCT_PU(DX10FrameBufferArgs);
        DELETE_CM(DX10FrameBufferArgs);
    public:
        ID3D10RenderTargetView** d3dColorAttachments;

        ~DX10FrameBufferArgs() noexcept
        { delete[] d3dColorAttachments; }
    };
public:
    [[nodiscard]] NullableRef<IFrameBuffer> buildTauRef(const FrameBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const FrameBufferArgs& args, DX10FrameBufferArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};

#endif
