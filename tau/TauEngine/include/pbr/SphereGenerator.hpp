#pragma once

#include <Safeties.hpp>

#include "DLL.hpp"

class IVertexArray;

TAU_DLL CPPRef<IVertexArray> generateNormalizedCubeSphere(uSys subdivisions) noexcept;
