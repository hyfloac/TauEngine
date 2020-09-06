#pragma once

#include "system/GraphicsCapabilities.hpp"

class TAU_DLL GLGraphicsCapabilities final : public IGraphicsCapabilities
{
    DEFAULT_CM_PU(GLGraphicsCapabilities);
    DEFAULT_DESTRUCT(GLGraphicsCapabilities);
private:
    CommandListCapabilities _commandListCapabilities;
    ShaderCapabilities _shaderCapabilities;
public:
    GLGraphicsCapabilities() noexcept
        : _commandListCapabilities { }
        , _shaderCapabilities { }
    {
        _commandListCapabilities.nativeCommandListSupport = false;
        _commandListCapabilities.bundleInheritsState = true;

        _shaderCapabilities.supportsGeometry = true;
        _shaderCapabilities.supportsTessellation = true;
        _shaderCapabilities.supportsMesh = false;
        _shaderCapabilities.supportsRayTracing = false;
        _shaderCapabilities.supportsCompute = true;
    }

    [[nodiscard]] const CommandListCapabilities& commandListCapabilities() const noexcept override { return _commandListCapabilities; }
    [[nodiscard]] const ShaderCapabilities& shaderCapabilities() const noexcept override { return _shaderCapabilities; }
};
