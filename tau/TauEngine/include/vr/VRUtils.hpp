#pragma once

#include "DLL.hpp"
#include "RenderingMode.hpp"
#include <openvr.h>

typedef void* VRTextureHandle_t;

TAU_DLL vr::ETextureType tauGetTextureType(RenderingMode::Mode mode) noexcept;
inline vr::ETextureType tauGetTextureType(const RenderingMode& mode) noexcept
{ return tauGetTextureType(mode.currentMode()); }

class IResource;

TAU_DLL VRTextureHandle_t tauGetVRTextureHandle(const IResource* texture) noexcept;
