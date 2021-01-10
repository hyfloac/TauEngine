#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include "DLL.hpp"

struct CommandListCapabilities final
{
    b8 nativeCommandListSupport : 1;
    b8 bundleInheritsState : 1;
};

struct ShaderCapabilities final
{
    b8 supportsGeometry : 1;
    b8 supportsTessellation : 1;
    b8 supportsMesh : 1;
    b8 supportsRayTracing : 1;
    b8 supportsCompute : 1;
};

struct HeapCapabilities final
{
    b8 supportsUserHeap : 1;
    b8 supportsMultiType : 1;
};

struct ResourceCapabilities final
{
    b8 supportsAliasing : 1;
    b8 supportsDirectModify : 1;
};

class TAU_DLL TAU_NOVTABLE IGraphicsCapabilities
{
    DEFAULT_CONSTRUCT_PO(IGraphicsCapabilities);
    DEFAULT_DESTRUCT_VI(IGraphicsCapabilities);
    DEFAULT_CM_PO(IGraphicsCapabilities);
public:
    [[nodiscard]] virtual const CommandListCapabilities& commandListCapabilities() const noexcept = 0;
    [[nodiscard]] virtual const ShaderCapabilities& shaderCapabilities() const noexcept = 0;
    [[nodiscard]] virtual const HeapCapabilities& heapCapabilities() const noexcept = 0;
    [[nodiscard]] virtual const ResourceCapabilities& resourceCapabilities() const noexcept = 0;
};
