/**
 * @file
 *
 *   Defines a cross-platform asynchronous file/stream interface
 * built on top of stdexec senders.
 *
 *   Implementations submit work to a platform-specific completion
 * mechanism (io_uring on Linux, IOCP on Windows, kqueue on BSD)
 * and complete the returned sender once the I/O has settled.
 */
#pragma once

#include "IStream.hpp"

#include <stdexec/execution.hpp>
#include <exec/any_sender_of.hpp>

namespace tau {

/**
 *   Result type yielded by an async read/write completion.
 *
 *   On success the value is the number of bytes transferred; on
 * failure it carries an @link IStream::ErrorCode @endlink that
 * mirrors the synchronous API.
 */
using AsyncIoResult = ::std::expected<uSys, IStream::ErrorCode>;

namespace _detail {

using AsyncIoCompletions = ::stdexec::completion_signatures<
    ::stdexec::set_value_t(AsyncIoResult),
    ::stdexec::set_error_t(::std::exception_ptr),
    ::stdexec::set_stopped_t()
>;

using AsyncIoReceiverRef = ::exec::any_receiver_ref<AsyncIoCompletions>;

}

/**
 *   A type-erased sender produced by @link IAsyncStream @endlink
 * read and write operations.
 *
 *   Drop it into any stdexec pipeline: @c stdexec::sync_wait,
 * @c stdexec::then, @c stdexec::let_value, @c co_await (via
 *  @c exec::task), etc.
 */
using AsyncIoSender = _detail::AsyncIoReceiverRef::any_sender<>;

/**
 *   Asynchronous extension to @link IStream @endlink for streams
 * whose backing implementation can complete I/O without blocking
 * the calling thread (io_uring, IOCP, ...).
 *
 *   Implementations satisfy @link IStream @endlink as well, so
 * sync consumers continue to work; sync calls typically submit
 * the same kernel request and block the calling thread on
 * completion.
 *
 *   This interface itself is platform-agnostic. For access to
 * platform-specific handles (e.g. the backing POSIX file
 * descriptor or the io_uring submission queue) query the
 * appropriate platform-specific interface.
 */
class IAsyncStream : public com::IUnknown
{
    DEFAULT_CONSTRUCT_PO(IAsyncStream);
    DEFAULT_DESTRUCT_VIO(IAsyncStream);
    DEFAULT_CM_PO(IAsyncStream);
public:
    /**
     * Returns a sender that, when started, reads up to @p count
     * bytes from the stream's current position into @p buffer.
     *
     *   The buffer must remain valid until the sender completes.
     * The sender publishes an @link AsyncIoResult @endlink: the
     * number of bytes read (zero indicates end-of-stream) on
     * success, or an @link IStream::ErrorCode @endlink on
     * failure.
     */
    [[nodiscard]] virtual AsyncIoSender ReadAsync(void* buffer, uSys count) = 0;

    /**
     *   Returns a sender that writes @p count bytes from
     * @p buffer to the stream's current position.
     *
     *   The buffer must remain valid until the sender
     * completes. The sender publishes the number of bytes
     * actually written.
     */
    [[nodiscard]] virtual AsyncIoSender WriteAsync(const void* buffer, uSys count) = 0;
};

}

TAU_DECL_UUID(tau::IAsyncStream, 0xB16228A97ADD469Aull, 0xAE3BB2707EAED2BBull);
