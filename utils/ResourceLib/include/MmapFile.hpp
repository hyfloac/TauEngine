/**
 * @file
 *
 *   Portable interface for file streams whose contents are mapped
 * directly into the process address space.
 *
 *   Concrete implementations are platform-specific
 * (@link PosixMmapFile @endlink on Linux/macOS/BSD, a forthcoming
 *  @c Win32MmapFile on Windows).
 */
#pragma once

#include "IFile.hpp"

#include <TauCOM.hpp>

namespace tau {

/**
 *   A custom interface exposing the mapped region so callers can
 * memcpy / parse in place without going through the stream's
 * sequential read API.
 *
 *   This interface is intentionally platform-agnostic. For access
 * to platform-specific handles (e.g. the backing POSIX file
 * descriptor) query a more specific interface like
 * @link IPosixMmapFile @endlink.
 */
class IMmapFile : public com::IUnknown
{
    DEFAULT_CONSTRUCT_PO(IMmapFile);
    DEFAULT_DESTRUCT_VIO(IMmapFile);
    DEFAULT_CM_PO(IMmapFile);
public:
    [[nodiscard]] virtual const void* MappedData() const noexcept = 0;
    [[nodiscard]] virtual uSys MappedSize() const noexcept = 0;
};

}

TAU_DECL_UUID(tau::IMmapFile, 0xB659B06CF0F64A74ull, 0xB0071A52E0385271ull);
