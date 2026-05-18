/**
 * @file
 *
 * Convenience helpers that work on top of @c IStream without
 * extending its COM interface.
 */
#pragma once

#include <DynArray.hpp>
#include <NumTypes.hpp>
#include <String.hpp>

#include "IStream.hpp"

namespace tau {

/**
 *   Reads the entire contents of @p stream into a freshly
 * allocated @c RefDynArray<u8>.
 *
 *   The stream must support @link IStream::Length @endlink. On
 * any failure (unknown length, partial read, read error) an
 * empty array is returned.
 */
inline RefDynArray<u8> ReadAll(IStream* const stream) noexcept
{
    const i64 length = stream->Length();
    if(length <= 0)
    {
        return RefDynArray<u8>(0);
    }

    RefDynArray<u8> buffer(static_cast<uSys>(length));
    const auto result = stream->Read(buffer.Array(), static_cast<uSys>(length));
    if(!result || result.value() != static_cast<uSys>(length))
    {
        return RefDynArray<u8>(0);
    }
    return buffer;
}

}
