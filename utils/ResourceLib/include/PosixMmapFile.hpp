/**
 * @file
 *
 *   Describes a read-only file handle that maps the entire file into
 * the process address space using the POSIX @c mmap syscall.
 */
#pragma once

#if !defined(_WIN32)

#include "IFile.hpp"
#include "MmapFile.hpp"

#include <TauCOM.hpp>

namespace tau {

/**
 *   POSIX-specific extension of @link IMmapFile @endlink that
 * exposes the file descriptor backing the mapping.
 *
 *   Callers that want to submit io_uring SQEs against the same
 * handle, or call @c madvise / @c fcntl, should query this
 * interface.
 */
class IPosixMmapFile : public com::IUnknown
{
    DEFAULT_CONSTRUCT_PO(IPosixMmapFile);
    DEFAULT_DESTRUCT_VIO(IPosixMmapFile);
    DEFAULT_CM_PO(IPosixMmapFile);
public:
    [[nodiscard]] virtual int GetFileDescriptor() noexcept = 0;
};

/**
 *   Returns a loader instance that produces read-only POSIX
 * @c mmap-backed file handles.
 */
[[nodiscard]] com::ComRef<IFileLoader> CreatePosixMmapFileLoader() noexcept;

}

TAU_DECL_UUID(tau::IPosixMmapFile, 0xB0455920217E48F4ull, 0xB4FD4A2C0B1D5E16ull);

#endif
