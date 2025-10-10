/**
 * @file
 */
#pragma once

#include <Safeties.hpp>
#include <NumTypes.hpp>
#include <String.hpp>
#include <TauCOM.hpp>
#include <span>
#include <expected>

namespace tau {

enum class ESeekOrigin
{
    Begin = 0,
    Current,
    End
};

/**
 *   An interface used to represent a generic view of a sequence of
 * bytes.
 *
 *   This can be overloaded to support any filesystem, or other medium
 * of holding files such as compressed archives.
 *
 * This is based on the .NET System.IO.Stream type.
 */
class IStream : public com::IUnknown
{
    DEFAULT_CONSTRUCT_PO(IStream);
    DEFAULT_DESTRUCT_VI(IStream);
    DEFAULT_CM_PO(IStream);
public:
    enum class ErrorCode : i8
    {
        EndOfFile = -1,
        ReadOnly = -2,
        WriteOnly = -3
    };
public:
    /**
     * @return Whether this stream supports reading.
     */
    [[nodiscard]] virtual bool CanRead() const noexcept = 0;

    /**
     * @return Whether this stream supports writing.
     */
    [[nodiscard]] virtual bool CanWrite() const noexcept = 0;

    /**
     * @return Whether this stream supports seeking.
     */
    [[nodiscard]] virtual bool CanSeek() const noexcept = 0;

    /**
     * @return The length of the stream.
     */
    [[nodiscard]] virtual i64 Length() const noexcept = 0;

    /**
     * Gets the current position in the stream.
     *
     *   The stream must support seeking to get the position. Use
     * @link CanSeek @endlink to determine whether this stream supports
     * seeking.
     *
     * @return The current position in the stream.
     */
    [[nodiscard]] virtual i64 Position() const noexcept = 0;

    /**
     * Sets the current position in the stream.
     *
     *   The stream must support seeking to set the position. Use
     * @link CanSeek @endlink to determine whether this stream supports
     * seeking.
     */
    virtual i64 Position(const i64 pos, const ESeekOrigin origin) noexcept = 0;

    virtual ::std::expected<uSys, ErrorCode> Read(void* const buffer, const uSys count) = 0;

    virtual ::std::expected<uSys, ErrorCode> Read(const ::std::span<u8>& buffer)
    {
        return Read(buffer.data(), buffer.size());
    }

    [[nodiscard]] virtual ::std::expected<u8, ErrorCode> ReadByte() = 0;

    virtual void Write(const void* const buffer, const uSys count) = 0;

    virtual void Write(const ::std::span<u8>& buffer)
    {
        Write(buffer.data(), buffer.size());
    }

    virtual void Flush() { }
};

class ITimeoutStream : public com::IUnknown
{
    DEFAULT_CONSTRUCT_PO(ITimeoutStream);
    DEFAULT_DESTRUCT_VI(ITimeoutStream);
    DEFAULT_CM_PO(ITimeoutStream);
public:
    [[nodiscard]] virtual bool CanTimeout() const noexcept = 0;

    [[nodiscard]] virtual i32 ReadTimeout() const noexcept = 0;

    virtual void ReadTimeout(const i32 timeout) const noexcept = 0;

    [[nodiscard]] virtual i32 WriteTimeout() const noexcept = 0;

    virtual void WriteTimeout(const i32 timeout) const noexcept = 0;
};

}

TAU_DECL_UUID(tau::IStream, 0x38F76EBDCD9A4030ull, 0xB36630CCBC9CFB2Full);
TAU_DECL_UUID(tau::ITimeoutStream, 0x93D7E6E4CA864312ull, 0x872D40506D1D5BA1ull);
// TAU_DECL_UUID(, 0xB44C6A79BC634224ull, 0x84521E8111F02CC7ull);
