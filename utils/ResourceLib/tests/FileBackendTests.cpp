#include <gtest/gtest.h>

#if !defined(_WIN32)

#include "IFile.hpp"
#include "PosixFile.hpp"
#include "MmapFile.hpp"
#include "PosixMmapFile.hpp"

#include <cstdio>
#include <filesystem>
#include <string>

namespace {

class TempFile final
{
public:
    TempFile() noexcept
    {
        char tmpl[] = "/tmp/tau_filebackend_XXXXXX";
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

}

TEST(PosixFileTest, WriteThenReadBack)
{
    TempFile temp;
    ASSERT_FALSE(temp.Path().empty());

    auto loader = tau::CreatePosixFileLoader();
    ASSERT_TRUE(loader);

    // Write phase
    {
        tau::com::ComRef<tau::IFileStream> stream(loader->Load(temp.PathAsC8(), tau::FileProps::WriteNew));
        ASSERT_TRUE(stream);
        EXPECT_TRUE(stream->CanWrite());

        constexpr u32 payloadA = 0xCAFEF00Du;
        constexpr u16 payloadB = 0xBEEFu;
        stream->WriteType(payloadA);
        stream->WriteType(payloadB);
    }

    // Read phase
    {
        tau::com::ComRef<tau::IFileStream> stream(loader->Load(temp.PathAsC8(), tau::FileProps::Read));
        ASSERT_TRUE(stream);
        EXPECT_TRUE(stream->CanRead());
        EXPECT_EQ(stream->Length(), static_cast<i64>(sizeof(u32) + sizeof(u16)));

        u32 a = 0;
        u16 b = 0;
        EXPECT_EQ(stream->ReadType(&a), static_cast<i64>(sizeof(u32)));
        EXPECT_EQ(stream->ReadType(&b), static_cast<i64>(sizeof(u16)));

        EXPECT_EQ(a, 0xCAFEF00Du);
        EXPECT_EQ(b, 0xBEEFu);
    }
}

TEST(PosixFileTest, SeekFromEnd)
{
    TempFile temp;
    const std::uint8_t payload[] = { 10, 20, 30, 40, 50, 60, 70, 80 };
    WriteRawFile(temp.Path(), payload, sizeof(payload));

    auto loader = tau::CreatePosixFileLoader();
    tau::com::ComRef<tau::IFileStream> stream(loader->Load(temp.PathAsC8(), tau::FileProps::Read));
    ASSERT_TRUE(stream);

    stream->Position(-3, tau::ESeekOrigin::End);
    u8 byte = 0;
    const auto result = stream->ReadByte();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 60);
}

TEST(MmapFileTest, ReadBackAndIMmapAccess)
{
    TempFile temp;
    const std::uint8_t payload[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    WriteRawFile(temp.Path(), payload, sizeof(payload));

    auto loader = tau::CreatePosixMmapFileLoader();
    ASSERT_TRUE(loader);

    tau::com::ComRef<tau::IFileStream> stream(loader->Load(temp.PathAsC8(), tau::FileProps::Read));
    ASSERT_TRUE(stream);

    EXPECT_TRUE(stream->CanRead());
    EXPECT_FALSE(stream->CanWrite());
    EXPECT_EQ(stream->Length(), static_cast<i64>(sizeof(payload)));

    u8 buffer[sizeof(payload)] = {};
    const auto result = stream->Read(buffer, sizeof(buffer));
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), sizeof(payload));
    EXPECT_EQ(0, std::memcmp(buffer, payload, sizeof(payload)));

    // The IMmapFile interface should also expose the raw mapping.
    tau::com::ComRef<tau::IMmapFile> mapped;
    EXPECT_EQ(stream->QueryInterface<tau::IMmapFile>(mapped.Load()), tau::com::RC_Success);
    ASSERT_TRUE(mapped);
    EXPECT_EQ(mapped->MappedSize(), sizeof(payload));
    EXPECT_EQ(0, std::memcmp(mapped->MappedData(), payload, sizeof(payload)));
}

TEST(MmapFileTest, WriteIsRejected)
{
    TempFile temp;
    const std::uint8_t payload[] = { 1 };
    WriteRawFile(temp.Path(), payload, sizeof(payload));

    auto loader = tau::CreatePosixMmapFileLoader();
    EXPECT_EQ(loader->Load(temp.PathAsC8(), tau::FileProps::WriteNew), nullptr);
}

#endif
