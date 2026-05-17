#include <gtest/gtest.h>

#include "IStream.hpp"
#include "MemoryStream.hpp"

TEST(DynamicMemoryStreamTest, WriteAndReadBack)
{
    tau::DynamicMemoryStream stream;

    constexpr u32 expected = 0xDEADBEEFu;
    stream.WriteType(expected);

    ASSERT_EQ(stream.Length(), static_cast<i64>(sizeof(u32)));

    stream.SetPosition(0);

    u32 actual = 0;
    const i64 bytesRead = stream.ReadType(&actual);

    EXPECT_EQ(bytesRead, static_cast<i64>(sizeof(u32)));
    EXPECT_EQ(actual, expected);
}

TEST(DynamicMemoryStreamTest, ReadAtEndReturnsZero)
{
    tau::DynamicMemoryStream stream;

    u8 buffer[4];
    const i64 bytesRead = stream.ReadBytes(buffer, sizeof(buffer));
    EXPECT_EQ(bytesRead, 0);
}

TEST(DynamicMemoryStreamTest, AdvancePosition)
{
    tau::DynamicMemoryStream stream;

    const u8 payload[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    stream.Write(payload, sizeof(payload));

    stream.SetPosition(0);
    stream.AdvancePosition(3);

    u8 byte = 0;
    const auto result = stream.ReadByte();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 4);
}

TEST(MemoryStreamTest, WriteTypeRoundtrip)
{
    u8 backing[16] = {};
    tau::MemoryStream stream(backing, sizeof(backing));

    struct POD final
    {
        u32 a;
        u16 b;
    };

    constexpr POD expected { 0x12345678u, 0xABCDu };
    stream.WriteType(expected);

    stream.SetPosition(0);

    POD actual {};
    const i64 bytesRead = stream.ReadType(&actual);

    EXPECT_EQ(bytesRead, static_cast<i64>(sizeof(POD)));
    EXPECT_EQ(actual.a, expected.a);
    EXPECT_EQ(actual.b, expected.b);
}
