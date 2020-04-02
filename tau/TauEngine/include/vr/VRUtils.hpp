#pragma once

#include "DLL.hpp"
#include "RenderingMode.hpp"
#include <openvr.h>

typedef void* VRTextureHandle_t;

TAU_DLL vr::ETextureType tauGetTextureType(RenderingMode::Mode mode) noexcept;
static inline vr::ETextureType tauGetTextureType(const RenderingMode& mode) noexcept
{ return tauGetTextureType(mode.currentMode()); }

class ITexture;

TAU_DLL VRTextureHandle_t tauGetVRTextureHandle(ITexture* texture) noexcept;
