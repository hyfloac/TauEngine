#pragma once

#include <Objects.hpp>
#include "DLL.hpp"

struct CommandListCapabilities final
{
    bool nativeCommandListSupport;
    bool bundleInheritsState;
};

struct ShaderCapabilities final
{
    bool supportsGeometry;
    bool supportsTessellation;
    bool supportsMesh;
    bool supportsRayTracing;
    bool supportsCompute;
};

struct HeapCapabilities final
{
    bool supportsUserHeap;
    bool supportsMultiType;
};

class TAU_DLL TAU_NOVTABLE IGraphicsCapabilities
{
    DEFAULT_CONSTRUCT_PO(IGraphicsCapabilities);
    DEFAULT_DESTRUCT_VI(IGraphicsCapabilities);
    DEFAULT_CM_PO(IGraphicsCapabilities);
public:
    [[nodiscard]] virtual const CommandListCapabilities& commandListCapabilities() const noexcept = 0;
    [[nodiscard]] virtual const ShaderCapabilities& shaderCapabilities() const noexcept = 0;
};
