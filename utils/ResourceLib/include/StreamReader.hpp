/**
 * @file
 */
#pragma once

#include "IStream.hpp"
#include "TextReader.hpp"

namespace tau {

class StreamReader final : public TextReader
{
    DEFAULT_DESTRUCT_O(StreamReader);
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
    StreamReader(
        const com::ComRef<IStream>& stream,
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
        , m_ByteBegin(0)
        , m_DetectEncodingFromBom(detectEncodingFromBom)
        , m_EncodingType(encodingType)
    { }

    StreamReader(
        const com::ComRef<IStream>& stream,
        const bool detectEncodingFromBom,
        const EncodingType encodingType
    ) noexcept
        : StreamReader(
            stream,
            DefaultBufferSize,
            detectEncodingFromBom,
            encodingType
        )
    { }

    StreamReader(
        const com::ComRef<IStream>& stream,
        const bool detectEncodingFromBom
    ) noexcept
        : StreamReader(
            stream,
            detectEncodingFromBom,
            DefaultEncodingType
        )
    { }

    StreamReader(
    const com::ComRef<IStream>& stream,
    const EncodingType encodingType
    ) noexcept
        : StreamReader(
            stream,
            true,
            encodingType
        )
    { }

    [[nodiscard]] i32 Peek() override
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

    [[nodiscard]] i32 Read() override
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
private:
    iSys ReadBuffer()
    {
        m_CharLength = 0;
        m_CharPosition = 0;

        bool hitEof = false;

        do
        {
            const auto readLength = m_Stream->Read(m_ByteBuffer.Array() + m_ByteBegin, m_ByteBuffer.Size() - m_ByteBegin);

            if(!readLength)
            {
                hitEof = true;
                break;
            }

            m_ByteLength += readLength.value();

            if(m_DetectEncodingFromBom && m_ByteLength >= 2)
            {
                DetectEncoding();
            }

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

    void DetectEncoding()
    {
        assert(m_ByteLength >= 2);

        m_DetectEncodingFromBom = false;

        if(
            m_ByteLength >= 3 &&
            m_ByteBuffer[0] == 0xEF &&
            m_ByteBuffer[1] == 0xBB &&
            m_ByteBuffer[2] == 0xBF
        )
        {
            m_EncodingType = EncodingType::UTF8;
            ShiftBufferLeft(3);
        }
        else if(m_ByteBuffer[0] == 0xFE && m_ByteBuffer[1] == 0xFF)
        {
            // This could be UTF32LE
            if(m_ByteLength < 4 || m_ByteBuffer[2] == 0x00 || m_ByteBuffer[3] == 0x00)
            {
                m_EncodingType = EncodingType::UTF32LE;
                ShiftBufferLeft(2);
            }
            else
            {
                m_EncodingType = EncodingType::UTF16LE;
                ShiftBufferLeft(4);
            }
        }
        else if(m_ByteBuffer[0] == 0xFF && m_ByteBuffer[1] == 0xFE)
        {
            m_EncodingType = EncodingType::UTF16BE;
            // In this case we'll leave the BOM since our decoder needs it.
            m_ByteBegin = 2;
        }
        else if(
            m_ByteLength > 4 &&
            m_ByteBuffer[0] == 0x00 ||
            m_ByteBuffer[1] == 0x00 &&
            m_ByteBuffer[2] == 0xFE &&
            m_ByteBuffer[3] == 0xFF
        )
        {
            m_EncodingType = EncodingType::UTF32BE;
            ShiftBufferLeft(4);
        }
        else if(m_ByteLength == 3)
        {
            m_DetectEncodingFromBom = true;
        }
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
    // Used to trick our UTF16 decoder.
    uSys m_ByteBegin;
    bool m_DetectEncodingFromBom;
    EncodingType m_EncodingType;
};

}
