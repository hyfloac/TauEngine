#pragma once

#include "system/GraphicsCapabilities.hpp"

namespace tau::gl {

class TAU_DLL GLGraphicsCapabilities final : public IGraphicsCapabilities
{
    DEFAULT_CM_PU(GLGraphicsCapabilities);
    DEFAULT_DESTRUCT(GLGraphicsCapabilities);
public:
    GLGraphicsCapabilities() noexcept
        : m_CommandListCapabilities { }
        , m_ShaderCapabilities { }
        , m_HeapCapabilities { }
        , m_ResourceCapabilities { }
    {
        m_CommandListCapabilities.nativeCommandListSupport = false;
        m_CommandListCapabilities.bundleInheritsState = true;

        m_ShaderCapabilities.supportsGeometry = true;
        m_ShaderCapabilities.supportsTessellation = true;
        m_ShaderCapabilities.supportsMesh = false;
        m_ShaderCapabilities.supportsRayTracing = false;
        m_ShaderCapabilities.supportsCompute = true;

        m_HeapCapabilities.supportsUserHeap = false;
        m_HeapCapabilities.supportsMultiType = false;

        m_ResourceCapabilities.supportsAliasing = false;
        m_ResourceCapabilities.supportsDirectModify = false;
    }

    [[nodiscard]] const CommandListCapabilities& commandListCapabilities() const noexcept override { return m_CommandListCapabilities; }
    [[nodiscard]] const ShaderCapabilities& shaderCapabilities() const noexcept override { return m_ShaderCapabilities; }
    [[nodiscard]] const HeapCapabilities& heapCapabilities() const noexcept override { return m_HeapCapabilities; }
    [[nodiscard]] const ResourceCapabilities& resourceCapabilities() const noexcept override { return m_ResourceCapabilities; }
private:
    CommandListCapabilities m_CommandListCapabilities;
    ShaderCapabilities m_ShaderCapabilities;
    HeapCapabilities m_HeapCapabilities;
    ResourceCapabilities m_ResourceCapabilities;
};

}
