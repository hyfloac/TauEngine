/**
 * @file
 */
#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <span>
#include <String.hpp>

namespace tau {

class TextReader
{
    DEFAULT_CONSTRUCT_PO(TextReader);
    DEFAULT_DESTRUCT_VI(TextReader);
    DEFAULT_CM_PO(TextReader);
public:
    [[nodiscard]] virtual i32 Peek()
    {
        return -1;
    }

    [[nodiscard]] virtual i32 Read()
    {
        return -1;
    }

    [[nodiscard]] virtual iSys Read(c8* const buffer, const uSys count)
    {
        if(!buffer)
        {
            return -1;
        }

        if(count == 0)
        {
            return 0;
        }

        iSys readCount = 0;

        for(; readCount < static_cast<iSys>(count); ++readCount)
        {
            const i32 ch = Read();
            if(ch < 0)
            {
                break;
            }

            buffer[readCount] = static_cast<c8>(ch);
        }

        return readCount;
    }

    [[nodiscard]] virtual iSys Read(const std::span<c8>& buffer)
    {
        return Read(buffer.data(), buffer.size());
    }

    virtual iSys ReadBlocking(c8* const buffer, const uSys count)
    {
        if(!buffer)
        {
            return -1;
        }

        if(count == 0)
        {
            return 0;
        }

        iSys readCount = 0;
        iSys writeOffset = 0;

        do
        {
            readCount = Read(buffer + writeOffset, count - writeOffset);
            writeOffset += readCount;
        }
        while(readCount > 0 && writeOffset < static_cast<iSys>(count));


        return writeOffset;
    }

    virtual iSys ReadBlocking(const std::span<c8>& buffer)
    {
        return ReadBlocking(buffer.data(), buffer.size());
    }

    [[nodiscard]] virtual C8DynString ReadToEnd()
    {
        c8 buffer[4096];
        C8StringBuilder builder;

        iSys readLength = 0;
        while((readLength = Read(buffer, ::std::size(buffer)) != 0))
        {
            builder.Append(buffer, readLength);
        }

        return builder.ToString();
    }

    [[nodiscard]] virtual C8DynString ReadLine()
    {
        C8StringBuilder builder;

        while(true)
        {
            const i32 ch = Read();

            if(ch < 0)
            {
                break;
            }

            if(ch == '\r' || ch == '\n')
            {
                if(ch == '\r' && Peek() == '\n')
                {
                    (void) Read();
                }

                break;
            }

            builder.Append(static_cast<c32>(ch));
        }

        return builder.ToString();
    }
};

}
