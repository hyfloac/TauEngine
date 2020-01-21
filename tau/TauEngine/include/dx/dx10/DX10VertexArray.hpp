#pragma once

#include "model/IVertexArray.hpp"

#ifdef _WIN32
#include <d3d10.h>

class TAU_DLL DX10VertexArray final : public IVertexArray
{
    DELETE_COPY(DX10VertexArray);
public:
    static DXGI_FORMAT getDXType(ShaderDataType::Type type) noexcept;

    /**
     *   DirectX doesn't natively allow you to pass doubles or
     * matrices to a shader. As such these types take up more
     * than one element in the input layout.
     */
    static uSys computeNumElements(ShaderDataType::Type type) noexcept;

    static D3D10_PRIMITIVE_TOPOLOGY getDXDrawType(DrawType drawType) noexcept;
private:
    uSys _iaBufferCount;
    ID3D10Buffer** _iaBuffers;
    UINT* _iaStrides;
    UINT* _iaOffsets;
    D3D10_INPUT_ELEMENT_DESC* _elementDescriptors;
    ID3D10Buffer* _dxIndexBuffer;
    D3D10_PRIMITIVE_TOPOLOGY _drawTypeCache;
public:
    DX10VertexArray(uSys bufferCount, DrawType drawType) noexcept;

    ~DX10VertexArray() noexcept override;

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    void internalSetup(IRenderingContext& context) noexcept override;
    void preDraw(IRenderingContext& context) noexcept override;
    void postDraw(IRenderingContext& context) noexcept override;
    void draw(IRenderingContext& context) noexcept override;
    void drawIndexed(IRenderingContext& context) noexcept override;

    void setIndexBuffer(IRenderingContext& context, const Ref<IIndexBuffer>& indexBuffer) noexcept override;

    void drawType(DrawType drawType) noexcept override;

    void addVertexBuffer(IRenderingContext& context, const Ref<IBuffer>& vertexBuffer) noexcept override;
private:
    void handleInsertion(uSys& insertIndex, ID3D10Buffer* buffer, const BufferElementDescriptor& bed) const noexcept;
};
#endif
