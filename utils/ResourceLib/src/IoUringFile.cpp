#include "IoUringFile.hpp"

#if !defined(_WIN32)

#include <TauCOM.impl.hpp>

#include <liburing.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <memory>
#include <mutex>
#include <optional>
#include <semaphore>
#include <thread>
#include <utility>

namespace tau {

// ---------------------------------------------------------------------------
//  Operation base type. Each in-flight io_uring SQE carries a pointer to
//  one of these in its user_data slot so the completion thread can route
//  the CQE back to the right operation state.
// ---------------------------------------------------------------------------

struct IoUringOpBase
{
    virtual ~IoUringOpBase() noexcept = default;
    /// Invoked by the CQE-pumping thread with the kernel's result code
    /// (negative @c errno on failure, else byte count) and CQE flags.
    virtual void OnComplete(i32 res, u32 flags) noexcept = 0;
};

// ---------------------------------------------------------------------------
//  IoUringContextImpl. Owns the ring + completion thread.
// ---------------------------------------------------------------------------

class IoUringContextImpl final
{
public:
    explicit IoUringContextImpl(const u32 entries)
    {
        const int initRc = ::io_uring_queue_init(entries, &m_Ring, 0);
        if(initRc < 0)
        {
            // Exceptions are disabled in this build; this is a fatal,
            // system-level configuration error.
            (void) ::fprintf(stderr, "io_uring_queue_init failed: %s\n", ::strerror(-initRc));
            ::std::abort();
        }
        m_Thread = ::std::thread(&IoUringContextImpl::CqeLoop, this);
    }

    ~IoUringContextImpl() noexcept
    {
        // Signal the loop and wake it via a NOP SQE.
        m_Stop.store(true, ::std::memory_order_release);
        {
            ::std::lock_guard<::std::mutex> guard(m_SqeMutex);
            io_uring_sqe* const sqe = ::io_uring_get_sqe(&m_Ring);
            if(sqe)
            {
                ::io_uring_prep_nop(sqe);
                ::io_uring_sqe_set_data64(sqe, 0);
                (void) ::io_uring_submit(&m_Ring);
            }
        }
        if(m_Thread.joinable())
        {
            m_Thread.join();
        }
        ::io_uring_queue_exit(&m_Ring);
    }

    /// Submit an SQE described by @p prep, with @p op recorded as its
    /// user_data. The operation is expected to remain alive until its
    /// OnComplete is invoked.
    template <typename PrepFn>
    void Submit(IoUringOpBase* const op, PrepFn&& prep) noexcept
    {
        ::std::lock_guard<::std::mutex> guard(m_SqeMutex);
        // The queue is sized at context construction; if it's full we
        // submit, then retry. liburing's get_sqe returns nullptr on
        // backlog.
        io_uring_sqe* sqe = ::io_uring_get_sqe(&m_Ring);
        while(!sqe)
        {
            (void) ::io_uring_submit(&m_Ring);
            sqe = ::io_uring_get_sqe(&m_Ring);
        }
        prep(sqe);
        ::io_uring_sqe_set_data(sqe, op);
        (void) ::io_uring_submit(&m_Ring);
    }
private:
    void DispatchOne(io_uring_cqe* const cqe) noexcept
    {
        IoUringOpBase* const op = static_cast<IoUringOpBase*>(::io_uring_cqe_get_data(cqe));
        const i32 res = cqe->res;
        const u32 flags = cqe->flags;
        ::io_uring_cqe_seen(&m_Ring, cqe);

        if(op)
        {
            op->OnComplete(res, flags);
        }
    }

    void CqeLoop() noexcept
    {
        while(true)
        {
            io_uring_cqe* cqe = nullptr;
            const int rc = ::io_uring_wait_cqe(&m_Ring, &cqe);
            if(rc < 0)
            {
                if(rc == -EINTR)
                {
                    continue;
                }
                break;
            }

            // Drain every CQE that's already ready before going back
            // to sleep. wait_cqe may have been woken by one CQE while
            // several have queued up behind it.
            DispatchOne(cqe);
            while(::io_uring_peek_cqe(&m_Ring, &cqe) == 0)
            {
                DispatchOne(cqe);
            }

            if(m_Stop.load(::std::memory_order_acquire))
            {
                break;
            }
        }
    }

    io_uring m_Ring {};
    ::std::mutex m_SqeMutex;
    ::std::thread m_Thread;
    ::std::atomic<bool> m_Stop { false };
};

// ---------------------------------------------------------------------------
//  IoUringContext: thin owner of the pimpl.
// ---------------------------------------------------------------------------

static void DestroyContextImpl(IoUringContextImpl* const p) noexcept
{
    delete p;
}

IoUringContext::IoUringContext(const u32 entries)
    : m_Impl(new(::std::nothrow) IoUringContextImpl(entries), &DestroyContextImpl)
{
    if(!m_Impl)
    {
        (void) ::fprintf(stderr, "IoUringContextImpl allocation failed (out of memory)\n");
        ::std::abort();
    }
}

IoUringContext::~IoUringContext() noexcept = default;

// ---------------------------------------------------------------------------
//  Helpers for translating kernel results into the IStream/AsyncIoResult
//  protocol.
// ---------------------------------------------------------------------------

namespace {

AsyncIoResult ResultFromKernel(const i32 res) noexcept
{
    if(res >= 0)
    {
        return static_cast<uSys>(res);
    }
    // res is a negative errno
    return ::std::unexpected(IStream::ErrorCode::Unexpected);
}

}

// ---------------------------------------------------------------------------
//  Async operation state. One state per ReadAsync / WriteAsync invocation.
//
//  Templated on the receiver because stdexec hands us a concrete receiver
//  type at connect() time.
// ---------------------------------------------------------------------------

enum class IoUringOpKind : u8
{
    Read,
    Write
};

struct IoUringOpParams final
{
    IoUringOpKind Kind;
    int Fd;
    void* Buffer;            // For writes this is logically const; we cast at submit time.
    uSys Count;
    i64 Offset;
};

namespace {

void PrepareSqe(io_uring_sqe* const sqe, const IoUringOpParams& params) noexcept
{
    if(params.Kind == IoUringOpKind::Read)
    {
        ::io_uring_prep_read(sqe, params.Fd, params.Buffer, static_cast<unsigned>(params.Count), static_cast<__u64>(params.Offset));
    }
    else
    {
        ::io_uring_prep_write(sqe, params.Fd, params.Buffer, static_cast<unsigned>(params.Count), static_cast<__u64>(params.Offset));
    }
}

}

/// State machine for an in-flight async operation. The CQE-handling thread
/// and the receiver's stop-callback thread race against this; the CAS
/// chain dictates which one wins.
enum class IoUringOpStatus : u8
{
    NotStarted,  ///< Op_state constructed, start() not yet called (or in progress).
    InFlight,    ///< SQE submitted, no completion yet, no cancel requested.
    Cancelling,  ///< Stop was requested; cancel handled per the path that won.
    Completed    ///< OnComplete has fired; receiver has been signalled.
};

template <typename Receiver>
struct IoUringOpState final : IoUringOpBase
{
    /// Holds a strong ref to the context so the ring outlives this op
    /// even if the user drops their @c shared_ptr and the file mid-flight.
    ::std::shared_ptr<IoUringContext> ContextOwner;
    IoUringOpParams Params;
    Receiver Recv;
    ::std::atomic<IoUringOpStatus> Status { IoUringOpStatus::NotStarted };

    /// Functor invoked when the receiver's stop_token requests stop.
    struct StopFn
    {
        IoUringOpState* Self;

        void operator()() noexcept
        {
            // First try the common race: op is in flight. CAS to
            // Cancelling and submit the cancel SQE.
            IoUringOpStatus expected = IoUringOpStatus::InFlight;
            if(Self->Status.compare_exchange_strong(
                expected,
                IoUringOpStatus::Cancelling,
                ::std::memory_order_acq_rel))
            {
                Self->ContextOwner->Impl()->Submit(
                    nullptr,
                    [self = Self](io_uring_sqe* const sqe)
                    {
                        ::io_uring_prep_cancel(sqe, self, 0);
                    }
                );
                return;
            }

            // The other race: stop arrived between Cancel.emplace() and
            // the SQE submission in start(). The op hasn't been submitted
            // yet, so there's nothing to cancel in the ring; we just flip
            // the state and start() will see it and bail out.
            expected = IoUringOpStatus::NotStarted;
            (void) Self->Status.compare_exchange_strong(
                expected,
                IoUringOpStatus::Cancelling,
                ::std::memory_order_acq_rel
            );
            // If neither CAS succeeded, the op is already Completed (or
            // someone else cancelled it). Nothing to do.
        }
    };

    using ReceiverEnv = ::stdexec::env_of_t<Receiver>;
    using StopToken = ::stdexec::stop_token_of_t<ReceiverEnv>;
    using StopCallback = typename StopToken::template callback_type<StopFn>;
    ::std::optional<StopCallback> Cancel;

    IoUringOpState(
        ::std::shared_ptr<IoUringContext> ctxOwner,
        const IoUringOpParams& params,
        Receiver&& r
    ) noexcept
        : ContextOwner(::std::move(ctxOwner))
        , Params(params)
        , Recv(::std::move(r))
    { }

    void start() & noexcept
    {
        // Register the stop callback BEFORE submitting. If the token
        // already has stop_requested, the callback fires synchronously
        // right here and transitions NotStarted -> Cancelling.
        Cancel.emplace(::stdexec::get_stop_token(::stdexec::get_env(Recv)), StopFn{this});

        // Try to move into InFlight. If the callback already raced us
        // and flipped state to Cancelling, bail out without submitting
        // the SQE at all.
        IoUringOpStatus expected = IoUringOpStatus::NotStarted;
        if(!Status.compare_exchange_strong(
            expected,
            IoUringOpStatus::InFlight,
            ::std::memory_order_acq_rel))
        {
            // Must be Cancelling. Deliver set_stopped synchronously.
            Cancel.reset();
            Status.store(IoUringOpStatus::Completed, ::std::memory_order_release);
            ::stdexec::set_stopped(::std::move(Recv));
            return;
        }

        ContextOwner->Impl()->Submit(
            this,
            [this](io_uring_sqe* const sqe) { PrepareSqe(sqe, Params); }
        );
    }

    void OnComplete(const i32 res, const u32 /*flags*/) noexcept override
    {
        // Tear down the stop callback first. Its destructor synchronizes
        // with a concurrent invocation, guaranteeing that after this
        // returns, the callback won't fire again.
        Cancel.reset();

        const IoUringOpStatus prev = Status.exchange(IoUringOpStatus::Completed, ::std::memory_order_acq_rel);

        // ECANCELED comes back when the cancel SQE actually intercepted
        // the op; treat it as set_stopped regardless of which side won
        // the CAS race.
        if(prev == IoUringOpStatus::Cancelling || res == -ECANCELED)
        {
            ::stdexec::set_stopped(::std::move(Recv));
            return;
        }

        ::stdexec::set_value(::std::move(Recv), ResultFromKernel(res));
    }
};

// ---------------------------------------------------------------------------
//  Concrete sender type that connect()s into an IoUringOpState. Type-erases
//  into AsyncIoSender via exec::any_sender_of.
// ---------------------------------------------------------------------------

class IoUringSender final
{
public:
    using sender_concept = ::stdexec::sender_t;
    using completion_signatures = _detail::AsyncIoCompletions;

    IoUringSender(::std::shared_ptr<IoUringContext> ctx, const IoUringOpParams& params) noexcept
        : m_Context(::std::move(ctx))
        , m_Params(params)
    { }

    template <typename Receiver>
    auto connect(Receiver receiver) && -> IoUringOpState<Receiver>
    {
        return IoUringOpState<Receiver>(::std::move(m_Context), m_Params, ::std::move(receiver));
    }
private:
    ::std::shared_ptr<IoUringContext> m_Context;
    IoUringOpParams m_Params;
};

// ---------------------------------------------------------------------------
//  IoUringFile. Implements the sync IFileStream + IPosixFile interfaces
//  plus the async IAsyncStream interface.
// ---------------------------------------------------------------------------

class IoUringFile final : public IFileStream, IAsyncStream, IPosixFile
{
    DELETE_CM(IoUringFile);
    TAU_COM_IMPL_REF_COUNT();
public:
    IoUringFile(
        ::std::shared_ptr<IoUringContext> ctx,
        const int fd,
        const C8DynString& name,
        const FileProps props
    ) noexcept
        : m_Context(::std::move(ctx))
        , m_Fd(fd)
        , m_Name(name)
        , m_Props(props)
        , m_Position(0)
    { }

    ~IoUringFile() noexcept override
    {
        if(m_Fd >= 0)
        {
            (void) ::close(m_Fd);
        }
    }

    // IUnknown

    com::EResultCode QueryInterface(const com::UUID& iid, void** const pInterface) noexcept override
    {
        using namespace tau::com;

        if(!pInterface)
        {
            return RC_NullParam;
        }

        if(iid == iid_of<IUnknown> || iid == iid_of<IStream> || iid == iid_of<IFileStream>)
        {
            *pInterface = static_cast<IFileStream*>(this);
        }
        else if(iid == iid_of<IAsyncStream>)
        {
            *pInterface = static_cast<IAsyncStream*>(this);
        }
        else if(iid == iid_of<IPosixFile>)
        {
            *pInterface = static_cast<IPosixFile*>(this);
        }
        else
        {
            return RC_InterfaceNotFound;
        }

        AddReference();
        return RC_Success;
    }

    // IStream

    [[nodiscard]] bool CanRead() const noexcept override
    {
        return m_Props == FileProps::Read || m_Props == FileProps::ReadWrite;
    }

    [[nodiscard]] bool CanWrite() const noexcept override
    {
        switch(m_Props)
        {
            case FileProps::WriteNew:
            case FileProps::WriteOverwrite:
            case FileProps::WriteAppend:
            case FileProps::ReadWrite:
                return true;
            default:
                return false;
        }
    }

    [[nodiscard]] bool CanSeek() const noexcept override { return true; }

    [[nodiscard]] i64 Length() const noexcept override
    {
        struct stat st;
        if(::fstat(m_Fd, &st) != 0)
        {
            return -1;
        }
        return static_cast<i64>(st.st_size);
    }

    [[nodiscard]] i64 Position() const noexcept override { return m_Position.load(::std::memory_order_acquire); }

    i64 Position(const i64 pos, const ESeekOrigin origin) noexcept override
    {
        i64 newPos = 0;
        switch(origin)
        {
            case ESeekOrigin::Begin:   newPos = pos; break;
            case ESeekOrigin::Current: newPos = m_Position.load(::std::memory_order_acquire) + pos; break;
            case ESeekOrigin::End:     newPos = Length() + pos; break;
            default: return m_Position.load(::std::memory_order_acquire);
        }
        m_Position.store(newPos, ::std::memory_order_release);
        return newPos;
    }

    ::std::expected<uSys, ErrorCode> Read(void* const buffer, const uSys count) override
    {
        if(!CanRead())
        {
            return ::std::unexpected(ErrorCode::ReadOnly);
        }
        return SubmitSync(IoUringOpKind::Read, buffer, count);
    }

    ::std::expected<u8, ErrorCode> ReadByte() override
    {
        u8 byte = 0;
        const auto r = Read(&byte, 1);
        if(!r)
        {
            return ::std::unexpected(r.error());
        }
        if(r.value() == 0)
        {
            return ::std::unexpected(ErrorCode::EndOfFile);
        }
        return byte;
    }

    void Write(const void* const buffer, const uSys count) override
    {
        if(!CanWrite())
        {
            assert(false);
            return;
        }
        (void) SubmitSync(IoUringOpKind::Write, const_cast<void*>(buffer), count);
    }

    // IAsyncStream

    [[nodiscard]] AsyncIoSender ReadAsync(void* const buffer, const uSys count) override
    {
        const IoUringOpParams params = MakeParamsAndAdvance(IoUringOpKind::Read, buffer, count);
        return AsyncIoSender(IoUringSender(m_Context, params));  // copy shared_ptr per submit
    }

    [[nodiscard]] AsyncIoSender WriteAsync(const void* const buffer, const uSys count) override
    {
        const IoUringOpParams params = MakeParamsAndAdvance(IoUringOpKind::Write, const_cast<void*>(buffer), count);
        return AsyncIoSender(IoUringSender(m_Context, params));  // copy shared_ptr per submit
    }

    // IFileStream

    [[nodiscard]] C8DynString Name() noexcept override { return m_Name; }

    // IPosixFile

    [[nodiscard]] int GetFileDescriptor() noexcept override { return m_Fd; }
private:
    IoUringOpParams MakeParamsAndAdvance(const IoUringOpKind kind, void* const buffer, const uSys count) noexcept
    {
        // Snapshot the position at submission time and eagerly advance by
        // the requested count. Multiple async ops submitted in sequence
        // operate on non-overlapping regions; short reads/writes don't
        // roll the position back.
        const i64 offset = m_Position.fetch_add(static_cast<i64>(count), ::std::memory_order_acq_rel);
        return IoUringOpParams{ kind, m_Fd, buffer, count, offset };
    }

    /// Synchronous SQE submission used by the IStream API. Submits, blocks
    /// on a semaphore, and returns the kernel's result.
    ::std::expected<uSys, ErrorCode> SubmitSync(const IoUringOpKind kind, void* const buffer, const uSys count) noexcept
    {
        struct SyncOp final : IoUringOpBase
        {
            ::std::binary_semaphore Done { 0 };
            i32 Result { 0 };

            void OnComplete(const i32 res, const u32 /*flags*/) noexcept override
            {
                Result = res;
                Done.release();
            }
        };

        SyncOp op;
        const i64 offset = m_Position.load(::std::memory_order_acquire);
        const IoUringOpParams params { kind, m_Fd, buffer, count, offset };
        m_Context->Impl()->Submit(&op, [&params](io_uring_sqe* const sqe) { PrepareSqe(sqe, params); });
        op.Done.acquire();

        if(op.Result < 0)
        {
            return ::std::unexpected(ErrorCode::Unexpected);
        }
        m_Position.fetch_add(op.Result, ::std::memory_order_acq_rel);
        return static_cast<uSys>(op.Result);
    }

    ::std::shared_ptr<IoUringContext> m_Context;
    int m_Fd;
    C8DynString m_Name;
    FileProps m_Props;
    ::std::atomic<i64> m_Position;
};

// ---------------------------------------------------------------------------
//  IoUringFileLoader.
// ---------------------------------------------------------------------------

class IoUringFileLoader final : public IFileLoader
{
    DELETE_CM(IoUringFileLoader);
    TAU_COM_IMPL_REF_COUNT();
public:
    explicit IoUringFileLoader(::std::shared_ptr<IoUringContext> ctx) noexcept
        : m_Context(::std::move(ctx))
    { }

    com::EResultCode QueryInterface(const com::UUID& iid, void** const pInterface) noexcept override
    {
        using namespace tau::com;
        if(!pInterface)
        {
            return RC_NullParam;
        }
        if(iid == iid_of<IUnknown> || iid == iid_of<IFileLoader>)
        {
            *pInterface = static_cast<IFileLoader*>(this);
            AddReference();
            return RC_Success;
        }
        return RC_InterfaceNotFound;
    }

    [[nodiscard]] bool Exists(const C8DynString& path) const noexcept override
    {
        ::std::error_code ec;
        return ::std::filesystem::exists(path.String(), ec);
    }

    [[nodiscard]] bool IsFolder(const C8DynString& path) const noexcept override
    {
        ::std::error_code ec;
        return ::std::filesystem::is_directory(path.String(), ec);
    }

    [[nodiscard]] bool IsSymlink(const C8DynString& path) const noexcept override
    {
        ::std::error_code ec;
        return ::std::filesystem::is_symlink(path.String(), ec);
    }

    [[nodiscard]] IFileStream* Load(const C8DynString& path, FileProps props) const noexcept override
    {
        int flags = 0;
        switch(props)
        {
            case FileProps::Read:           flags = O_RDONLY; break;
            case FileProps::WriteNew:       flags = O_WRONLY | O_CREAT | O_TRUNC; break;
            case FileProps::WriteOverwrite: flags = O_RDWR | O_CREAT; break;
            case FileProps::WriteAppend:    flags = O_WRONLY | O_CREAT | O_APPEND; break;
            case FileProps::ReadWrite:      flags = O_RDWR | O_CREAT; break;
            default: return nullptr;
        }

        const char* const pathStr = reinterpret_cast<const char*>(path.String());
        const int fd = ::open(pathStr, flags, mode_t { 0644 });
        if(fd < 0)
        {
            return nullptr;
        }

        return BasicTauAllocator<AllocationTracking::None>::Instance().AllocateT<IoUringFile>(m_Context, fd, path, props);  // copies shared_ptr
    }

    [[nodiscard]] bool CreateFolders(const C8DynString& path) const noexcept override
    {
        ::std::error_code ec;
        ::std::filesystem::create_directories(path.String(), ec);
        return !ec;
    }

    [[nodiscard]] bool Delete(const C8DynString& path) const noexcept override
    {
        ::std::error_code ec;
        return ::std::filesystem::remove(path.String(), ec);
    }

    [[nodiscard]] ::std::chrono::time_point<::std::chrono::utc_clock> CreationTime(const C8DynString&) const noexcept override
    {
        return ::std::chrono::time_point<::std::chrono::utc_clock>{};
    }

    [[nodiscard]] ::std::chrono::time_point<::std::chrono::utc_clock> ModifyTime(const C8DynString& path) const noexcept override
    {
        ::std::error_code ec;
        const auto writeTime = ::std::filesystem::last_write_time(path.String(), ec);
        if(ec)
        {
            return ::std::chrono::time_point<::std::chrono::utc_clock>{};
        }
        return ::std::chrono::clock_cast<::std::chrono::utc_clock>(writeTime);
    }
private:
    ::std::shared_ptr<IoUringContext> m_Context;
};

com::ComRef<IFileLoader> CreateIoUringFileLoader(::std::shared_ptr<IoUringContext> context) noexcept
{
    return com::ComRef<IFileLoader>(BasicTauAllocator<AllocationTracking::None>::Instance().AllocateT<IoUringFileLoader>(::std::move(context)));
}

}

#endif
