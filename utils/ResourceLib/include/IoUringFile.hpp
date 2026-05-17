/**
 * @file
 *
 *   io_uring-backed file handle. Implements both the synchronous
 * @link IFileStream @endlink and the asynchronous
 * @link IAsyncStream @endlink interfaces.
 *
 *   The async path submits read/write SQEs against the shared
 * @link IoUringContext @endlink ring; completions arrive on the
 * context's CQE-polling thread which invokes the sender's
 * receiver inline.
 *
 *   The sync path submits the same SQE and blocks the calling
 * thread on a condition variable until the CQE returns.
 */
#pragma once

#if !defined(_WIN32)

#include "IFile.hpp"
#include "IAsyncStream.hpp"
#include "PosixFile.hpp"

#include <TauCOM.hpp>

#include <memory>

namespace tau {

/**
 *   Owns an @c io_uring instance and a dedicated thread that
 * pumps the completion queue. One context is shared by any
 * number of @link IoUringFile @endlink handles.
 *
 *   Thread-safe: SQE submission is serialized with an internal
 * mutex; CQE consumption happens on the owned thread.
 */
class IoUringContext final
{
public:
    /**
     * @param entries
     *      Size of the submission/completion queues. Rounded up
     *      to a power of two by the kernel.
     */
    explicit IoUringContext(u32 entries = 128);
    ~IoUringContext() noexcept;

    IoUringContext(const IoUringContext&) = delete;
    IoUringContext& operator=(const IoUringContext&) = delete;
    IoUringContext(IoUringContext&&) = delete;
    IoUringContext& operator=(IoUringContext&&) = delete;

    /// Returns the opaque pimpl. Used by sender operation states to
    /// submit SQEs without dragging liburing into this header.
    [[nodiscard]] class IoUringContextImpl* Impl() const noexcept { return m_Impl.get(); }
private:
    ::std::unique_ptr<IoUringContextImpl, void(*)(IoUringContextImpl*)> m_Impl;
};

/**
 *   File handle backed by io_uring.
 *
 *   Implements @link IFileStream @endlink (synchronous reads/
 * writes via submit-and-wait), @link IAsyncStream @endlink (real
 * async via stdexec senders), and @link IPosixFile @endlink (raw
 * fd access).
 */
class IoUringFile;

/**
 *   Constructs a file loader producing @link IoUringFile @endlink
 * handles, bound to the given context.
 *
 *   Ownership: the loader, every file it produces, and every
 * in-flight async operation hold a strong reference to the
 * context. The context lives as long as any of those references
 * exist, so the caller can drop their @c shared_ptr immediately
 * after creating the loader if they want to.
 *
 *   Typical usage:
 * @code
 *   auto ctx = ::std::make_shared<tau::IoUringContext>();
 *   auto loader = tau::CreateIoUringFileLoader(ctx);
 * @endcode
 */
[[nodiscard]] com::ComRef<IFileLoader> CreateIoUringFileLoader(::std::shared_ptr<IoUringContext> context) noexcept;

}

#endif
