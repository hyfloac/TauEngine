/**
 * @file
 *
 * Describes a POSIX file handle backed by raw syscalls (open/pread/pwrite).
 */
#pragma once

#if !defined(_WIN32)

#include "IFile.hpp"

#include <TauCOM.hpp>

namespace tau {

/**
 *   A custom interface exposing the underlying POSIX file descriptor
 * for callers that need to interact with the kernel directly (e.g. an
 * io_uring backend that wants to submit on the same fd).
 */
class IPosixFile : public com::IUnknown
{
    DEFAULT_CONSTRUCT_PO(IPosixFile);
    DEFAULT_DESTRUCT_VIO(IPosixFile);
    DEFAULT_CM_PO(IPosixFile);
public:
    [[nodiscard]] virtual int GetFileDescriptor() noexcept = 0;
};

/**
 *   Returns a shared loader instance that produces
 * @link PosixFile @endlink handles.
 */
[[nodiscard]] com::ComRef<IFileLoader> GetPosixFileLoader() noexcept;

}

TAU_DECL_UUID(tau::IPosixFile, 0x346B0B8664114DDDull, 0xB49B1D6325992776ull);

#endif
