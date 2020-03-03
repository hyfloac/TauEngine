#pragma once

#include "system/GraphicsInterface.hpp"

#ifdef _WIN32

class TAU_DLL DX10GraphicsInterface final : public IGraphicsInterface
{
    DEFAULT_CONSTRUCT_PU(DX10GraphicsInterface);
    DEFAULT_DESTRUCT(DX10GraphicsInterface);
    DELETE_COPY(DX10GraphicsInterface);
public:
    [[nodiscard]] RefDynArray<CPPRef<IGraphicsAccelerator>> graphicsAccelerators() noexcept override;
};

#endif
