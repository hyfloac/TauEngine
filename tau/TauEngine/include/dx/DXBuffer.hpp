#pragma once

#include "model/IBuffer.hpp"

#ifdef _WIN32
#pragma warning(push, 0)
#include <d3d9.h>
#pragma warning(pop)

class TAU_DLL DXBuffer final : public IBuffer
{
    DELETE_COPY(DXBuffer);
public:

};
#endif
