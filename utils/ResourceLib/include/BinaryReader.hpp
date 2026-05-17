/**
 * @file
 */
#pragma once

#include "IStream.hpp"
#include <DynArray.hpp>

namespace tau {


class BinaryReader final
{
    DEFAULT_DESTRUCT(BinaryReader);
public:
    static constexpr uSys DefaultBufferSize = 1024;
    static constexpr uSys MinBufferSize = 128;

    enum class EncodingType : u8
    {
        UTF8 = 0,
        UTF16LE,
        UTF16BE,
        UTF32LE,
        UTF32BE
    };

    static constexpr EncodingType DefaultEncodingType  = EncodingType::UTF8;
public:
    BinaryReader(
        IStream* const stream,
        const uSys bufferSize = DefaultBufferSize,
        const bool detectEncodingFromBom = true,
        const EncodingType encodingType = DefaultEncodingType
    ) noexcept
        : m_Stream(stream)
        , m_ByteBuffer(bufferSize)
        , m_CharBuffer(bufferSize + 1)
        , m_CharPosition(0)
        , m_CharLength(0)
        , m_ByteLength(0)
        , m_BytePosition(0)
        , m_EncodingType(encodingType)
    {
        stream->AddReference();
    }

    BinaryReader(
        IStream* const stream,
        const bool detectEncodingFromBom,
        const EncodingType encodingType
    ) noexcept
        : BinaryReader(
            stream,
            DefaultBufferSize,
            detectEncodingFromBom,
            encodingType
        )
    { }

    BinaryReader(
        IStream* const stream,
        const bool detectEncodingFromBom
    ) noexcept
        : BinaryReader(
            stream,
            detectEncodingFromBom,
            DefaultEncodingType
        )
    { }

    BinaryReader(
        IStream* const stream,
        const EncodingType encodingType
    ) noexcept
        : BinaryReader(
            stream,
            true,
            encodingType
        )
    { }

    [[nodiscard]] i32 Peek()
    {
        if(m_CharPosition >= m_CharLength)
        {
            if(ReadBuffer() == 0)
            {
                return -1;
            }
        }

        return m_CharBuffer[m_CharPosition];
    }

    [[nodiscard]] i32 Read()
    {
        if(m_CharPosition >= m_CharLength)
        {
            if(ReadBuffer() == 0)
            {
                return -1;
            }
        }

        const i32 ret = m_CharBuffer[m_CharPosition];
        ++m_CharPosition;
        return ret;
    }

    [[nodiscard]] ::std::expected<c32, IStream::ErrorCode> ReadChar()
    {
        const i32 c0 = Read();

        if(c0 < 0)
        {
            return ::std::unexpected(IStream::ErrorCode::EndOfFile);
        }

        if((c0 & 0x80) == 0x00)
        {
            return static_cast<c32>(c0);
        }

        i32 count;

        if((c0 & 0xE0) == 0xC0) // U+0080 - U+07FF
        {
            count = 1;
        }
        else if((c0 & 0xF0) == 0xE0) // U+0800 - U+FFFF
        {
            count = 2;
        }
        else if((c0 & 0xF0) == 0xF0) // U+10000 - U+1FFFFF
        {
            count = 3;
        }
        else if((c0 & 0xF8) == 0xF0) // U+10000 - U+10FFFF
        {
            count = 3;
        }
        else
        {
            return static_cast<c32>(-1);
        }



        ::tau::string::utf8::DecodeCodePointForward()
    }
private:
    iSys ReadBuffer()
    {
        m_CharLength = 0;
        m_CharPosition = 0;

        bool hitEof = false;

        do
        {
            const auto readLength = m_Stream->Read(m_ByteBuffer.Array(), m_ByteBuffer.Size());

            if(!readLength)
            {
                hitEof = true;
                break;
            }

            m_ByteLength += readLength.value();

            m_CharLength = DecodeChars();
        }
        while(m_CharLength == 0);

        if(hitEof)
        {
            m_CharLength = DecodeChars();
            m_BytePosition = 0;
            m_ByteLength = 0;
        }

        return static_cast<iSys>(m_CharLength);
    }

    void ShiftBufferLeft(const uSys count)
    {
        (void) ::std::memmove(m_ByteBuffer.Array(), m_ByteBuffer.Array() + count, m_ByteBuffer.Size() - count);
    }

    [[nodiscard]] iSys DecodeChars() noexcept
    {
        assert(m_EncodingType != EncodingType::UTF32BE);

        switch(m_EncodingType)
        {
            case EncodingType::UTF8:
                (void) ::std::memcpy(m_CharBuffer.Array(), m_ByteBuffer.Array(), m_ByteLength);
                return m_ByteLength;
            case EncodingType::UTF16LE:
            case EncodingType::UTF16BE:
                return string::utf8_16::Transform(
                    reinterpret_cast<const c16*>(m_ByteBuffer.Array()),
                    m_CharBuffer.Array(),
                    m_ByteLength / sizeof(c16),
                    m_CharBuffer.Size(),
                    true
                );
            case EncodingType::UTF32LE:
                return string::utf8::Transform(
                    reinterpret_cast<const c32*>(m_ByteBuffer.Array()),
                    m_CharBuffer.Array(),
                    m_ByteLength / sizeof(c32),
                    m_CharBuffer.Size()
                );
            case EncodingType::UTF32BE:
                break;
            default:
                break;
        }

        return -1;
    }
private:
    com::ComRef<IStream> m_Stream;
    DynArray<u8> m_ByteBuffer;
    DynArray<c8> m_CharBuffer;
    uSys m_CharPosition;
    uSys m_CharLength;
    uSys m_ByteLength;
    uSys m_BytePosition;
    EncodingType m_EncodingType;
};

}
