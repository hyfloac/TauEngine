#include <gtest/gtest.h>

#if !defined(_WIN32)

#include "IFile.hpp"
#include "IAsyncStream.hpp"
#include "IoUringFile.hpp"

#include <stdexec/execution.hpp>

#include <atomic>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <memory>
#include <semaphore>
#include <string>
#include <sys/stat.h>
#include <thread>
#include <unistd.h>

namespace {

class TempFile final
{
public:
    TempFile() noexcept
    {
        char tmpl[] = "/tmp/tau_iouring_XXXXXX";
        const int fd = ::mkstemp(tmpl);
        if(fd >= 0)
        {
            ::close(fd);
            m_Path = tmpl;
        }
    }

    ~TempFile() noexcept
    {
        if(!m_Path.empty())
        {
            std::error_code ec;
            std::filesystem::remove(m_Path, ec);
        }
    }

    TempFile(const TempFile&) = delete;
    TempFile& operator=(const TempFile&) = delete;

    [[nodiscard]] const std::string& Path() const noexcept { return m_Path; }
    [[nodiscard]] C8DynString PathAsC8() const noexcept
    {
        return C8DynString(reinterpret_cast<const c8*>(m_Path.c_str()));
    }
private:
    std::string m_Path;
};

void WriteRawFile(const std::string& path, const void* const data, const std::size_t length)
{
    FILE* const f = std::fopen(path.c_str(), "wb");
    ASSERT_NE(f, nullptr);
    if(length > 0)
    {
        ASSERT_EQ(std::fwrite(data, 1, length, f), length);
    }
    std::fclose(f);
}

/// Minimal receiver that captures the outcome (value / stopped / error)
/// from an AsyncIoSender. The receiver's environment exposes a custom
/// stop_token so we can exercise the cancellation paths that
/// sync_wait's default token doesn't expose.
struct CancellableReceiver final
{
    using receiver_concept = stdexec::receiver_t;

    enum class OutcomeKind { Pending, Value, Stopped, Error };

    struct Env
    {
        stdexec::inplace_stop_token Token;
        stdexec::inplace_stop_token query(stdexec::get_stop_token_t) const noexcept { return Token; }
    };

    Env Environment;
    std::atomic<OutcomeKind>* Outcome;
    tau::AsyncIoResult* Value;
    std::binary_semaphore* Done;

    void set_value(tau::AsyncIoResult r) && noexcept
    {
        if(Value) *Value = r;
        Outcome->store(OutcomeKind::Value, std::memory_order_release);
        Done->release();
    }

    void set_stopped() && noexcept
    {
        Outcome->store(OutcomeKind::Stopped, std::memory_order_release);
        Done->release();
    }

    void set_error(std::exception_ptr) && noexcept
    {
        Outcome->store(OutcomeKind::Error, std::memory_order_release);
        Done->release();
    }

    Env get_env() const noexcept { return Environment; }
};

}

TEST(IoUringFileTest, SyncReadAfterWrite)
{
    auto ctx = ::std::make_shared<tau::IoUringContext>();
    auto loader = tau::CreateIoUringFileLoader(ctx);
    ASSERT_TRUE(loader);

    TempFile temp;

    // Write
    {
        tau::com::ComRef<tau::IFileStream> stream(loader->Load(temp.PathAsC8(), tau::FileProps::WriteNew));
        ASSERT_TRUE(stream);
        constexpr u64 payload = 0x0123456789ABCDEFull;
        stream->WriteType(payload);
    }

    // Read back
    {
        tau::com::ComRef<tau::IFileStream> stream(loader->Load(temp.PathAsC8(), tau::FileProps::Read));
        ASSERT_TRUE(stream);
        u64 actual = 0;
        EXPECT_EQ(stream->ReadType(&actual), static_cast<i64>(sizeof(u64)));
        EXPECT_EQ(actual, 0x0123456789ABCDEFull);
    }
}

TEST(IoUringFileTest, AsyncReadViaStdexec)
{
    auto ctx = ::std::make_shared<tau::IoUringContext>();
    auto loader = tau::CreateIoUringFileLoader(ctx);

    TempFile temp;
    const std::uint8_t payload[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE, 0xF0, 0x0D };
    WriteRawFile(temp.Path(), payload, sizeof(payload));

    tau::com::ComRef<tau::IAsyncStream> stream;
    {
        tau::com::ComRef<tau::IFileStream> file(loader->Load(temp.PathAsC8(), tau::FileProps::Read));
        ASSERT_TRUE(file);
        ASSERT_EQ(file->QueryInterface<tau::IAsyncStream>(stream.Load()), tau::com::RC_Success);
    }
    ASSERT_TRUE(stream);

    std::uint8_t buffer[sizeof(payload)] = {};
    auto sender = stream->ReadAsync(buffer, sizeof(buffer));

    const auto result = stdexec::sync_wait(std::move(sender));
    ASSERT_TRUE(result.has_value());
    const auto& [io] = result.value();
    ASSERT_TRUE(io.has_value()) << "AsyncIoResult held an error";
    EXPECT_EQ(io.value(), sizeof(payload));
    EXPECT_EQ(0, std::memcmp(buffer, payload, sizeof(payload)));
}

TEST(IoUringFileTest, AsyncWriteAndReadBack)
{
    auto ctx = ::std::make_shared<tau::IoUringContext>();
    auto loader = tau::CreateIoUringFileLoader(ctx);

    TempFile temp;

    // Async write
    {
        tau::com::ComRef<tau::IFileStream> file(loader->Load(temp.PathAsC8(), tau::FileProps::WriteNew));
        ASSERT_TRUE(file);

        tau::com::ComRef<tau::IAsyncStream> stream;
        ASSERT_EQ(file->QueryInterface<tau::IAsyncStream>(stream.Load()), tau::com::RC_Success);

        const std::uint8_t payload[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
        auto sender = stream->WriteAsync(payload, sizeof(payload));
        const auto result = stdexec::sync_wait(std::move(sender));
        ASSERT_TRUE(result.has_value());
        const auto& [io] = result.value();
        ASSERT_TRUE(io.has_value());
        EXPECT_EQ(io.value(), sizeof(payload));
    }

    // Sync read back through a separate file handle on the same context
    {
        tau::com::ComRef<tau::IFileStream> file(loader->Load(temp.PathAsC8(), tau::FileProps::Read));
        ASSERT_TRUE(file);
        EXPECT_EQ(file->Length(), 12);

        std::uint8_t buffer[12] = {};
        const auto r = file->Read(buffer, sizeof(buffer));
        ASSERT_TRUE(r.has_value());
        EXPECT_EQ(r.value(), 12u);
        const std::uint8_t expected[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
        EXPECT_EQ(0, std::memcmp(buffer, expected, sizeof(expected)));
    }
}

TEST(IoUringFileTest, ChainedAsyncReadsViaThen)
{
    auto ctx = ::std::make_shared<tau::IoUringContext>();
    auto loader = tau::CreateIoUringFileLoader(ctx);

    TempFile temp;
    const std::uint8_t payload[] = { 10, 20, 30, 40, 50, 60, 70, 80 };
    WriteRawFile(temp.Path(), payload, sizeof(payload));

    tau::com::ComRef<tau::IFileStream> file(loader->Load(temp.PathAsC8(), tau::FileProps::Read));
    tau::com::ComRef<tau::IAsyncStream> stream;
    ASSERT_EQ(file->QueryInterface<tau::IAsyncStream>(stream.Load()), tau::com::RC_Success);

    // Two non-overlapping reads in a single pipeline. m_Position
    // advances synchronously at submission time so the second
    // ReadAsync sees the next region.
    std::uint8_t buf0[4] = {};
    std::uint8_t buf1[4] = {};

    auto pipeline = stdexec::when_all(
        stream->ReadAsync(buf0, sizeof(buf0)),
        stream->ReadAsync(buf1, sizeof(buf1))
    );

    const auto result = stdexec::sync_wait(std::move(pipeline));
    ASSERT_TRUE(result.has_value());

    const auto& [io0, io1] = result.value();
    ASSERT_TRUE(io0.has_value());
    ASSERT_TRUE(io1.has_value());
    EXPECT_EQ(io0.value(), 4u);
    EXPECT_EQ(io1.value(), 4u);

    EXPECT_EQ(0, std::memcmp(buf0, payload + 0, 4));
    EXPECT_EQ(0, std::memcmp(buf1, payload + 4, 4));
}

// Pre-cancelled stop_token: the callback fires synchronously inside
// start(), before the SQE is submitted. Exercises the
// NotStarted -> Cancelling -> set_stopped path with no kernel work.
TEST(IoUringFileTest, PreCancelledStopTokenSkipsSubmit)
{
    auto ctx = ::std::make_shared<tau::IoUringContext>();
    auto loader = tau::CreateIoUringFileLoader(ctx);

    TempFile temp;
    const std::uint8_t payload[] = { 1, 2, 3, 4 };
    WriteRawFile(temp.Path(), payload, sizeof(payload));

    tau::com::ComRef<tau::IFileStream> file(loader->Load(temp.PathAsC8(), tau::FileProps::Read));
    tau::com::ComRef<tau::IAsyncStream> stream;
    ASSERT_EQ(file->QueryInterface<tau::IAsyncStream>(stream.Load()), tau::com::RC_Success);

    stdexec::inplace_stop_source source;
    source.request_stop();   // cancel before the op even starts

    std::atomic<CancellableReceiver::OutcomeKind> outcome { CancellableReceiver::OutcomeKind::Pending };
    std::binary_semaphore done { 0 };
    tau::AsyncIoResult value { 0 };

    std::uint8_t buffer[4] = {};
    auto op = stdexec::connect(
        stream->ReadAsync(buffer, sizeof(buffer)),
        CancellableReceiver{{source.get_token()}, &outcome, &value, &done}
    );
    stdexec::start(op);
    done.acquire();

    EXPECT_EQ(outcome.load(), CancellableReceiver::OutcomeKind::Stopped);
    // Buffer should not have been touched.
    for(const std::uint8_t b : buffer)
    {
        EXPECT_EQ(b, 0);
    }
}

// Mid-flight cancel: read from a FIFO with no writer. The read blocks
// in the kernel until either data arrives or the cancel SQE intercepts
// it. We trigger the stop after submission, exercising
// InFlight -> Cancelling -> cancel SQE -> set_stopped.
TEST(IoUringFileTest, MidFlightCancelStopsBlockingRead)
{
    auto ctx = ::std::make_shared<tau::IoUringContext>();
    auto loader = tau::CreateIoUringFileLoader(ctx);

    char tmplBuf[] = "/tmp/tau_iouring_fifo_XXXXXX";
    ASSERT_NE(::mkdtemp(tmplBuf), nullptr);
    const std::string fifoPath = std::string(tmplBuf) + "/fifo";
    ASSERT_EQ(::mkfifo(fifoPath.c_str(), 0644), 0);

    // Open the write end with O_RDWR so opening the read end doesn't
    // block; we just never write to it.
    const int writeFd = ::open(fifoPath.c_str(), O_RDWR);
    ASSERT_GE(writeFd, 0);

    const C8DynString fifoPathC8 { reinterpret_cast<const c8*>(fifoPath.c_str()) };
    tau::com::ComRef<tau::IFileStream> file(loader->Load(fifoPathC8, tau::FileProps::Read));
    ASSERT_TRUE(file);
    tau::com::ComRef<tau::IAsyncStream> stream;
    ASSERT_EQ(file->QueryInterface<tau::IAsyncStream>(stream.Load()), tau::com::RC_Success);

    stdexec::inplace_stop_source source;

    std::atomic<CancellableReceiver::OutcomeKind> outcome { CancellableReceiver::OutcomeKind::Pending };
    std::binary_semaphore done { 0 };
    tau::AsyncIoResult value { 0 };

    std::uint8_t buffer[16] = {};
    auto op = stdexec::connect(
        stream->ReadAsync(buffer, sizeof(buffer)),
        CancellableReceiver{{source.get_token()}, &outcome, &value, &done}
    );
    stdexec::start(op);

    // Op is now in flight, blocked on the empty FIFO. Trigger cancel
    // from another thread to exercise the InFlight -> Cancelling path.
    std::thread canceller([&]() { source.request_stop(); });

    done.acquire();
    canceller.join();

    EXPECT_EQ(outcome.load(), CancellableReceiver::OutcomeKind::Stopped);

    ::close(writeFd);
    (void) std::filesystem::remove(fifoPath);
    (void) std::filesystem::remove(tmplBuf);
}

#endif
