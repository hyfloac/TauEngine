#pragma once

#ifdef _WIN32
#include <Objects.hpp>
#include <d3dcompiler.h>
#include "DLL.hpp"
#include "graphics/BufferDescriptor.hpp"

class TAU_DLL DXShaderStubGenerator final
{
    DELETE_CONSTRUCT(DXShaderStubGenerator);
    DELETE_DESTRUCT(DXShaderStubGenerator);
    DELETE_CM(DXShaderStubGenerator);
public:
    [[nodiscard]] static ID3DBlob* genShader(uSys bufferCount, const BufferDescriptor* buffers, const char* shaderVersion) noexcept;
};
#endif
