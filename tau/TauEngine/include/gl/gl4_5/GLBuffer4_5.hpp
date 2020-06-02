#pragma once

#include "gl/GLBuffer.hpp"

class TAU_DLL GLBuf4_5Interface final : public GLBufInterface
{
    DEFAULT_CONSTRUCT_PU(GLBuf4_5Interface);
    DEFAULT_DESTRUCT(GLBuf4_5Interface);
    DELETE_COPY(GLBuf4_5Interface);
protected:
    [[nodiscard]] GLuint createBuffer() const noexcept override;
};
