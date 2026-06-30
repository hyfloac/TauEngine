/**
 * @file
 */
#pragma once

#include <cstdint>
#include <cassert>
#include <cstring>
#include <new>
#include <bit>
#include <NumTypes.hpp>

namespace tau::graphics {

#ifndef PUSH_ASSERT
    #if !defined(TAU_PRODUCTION)
        #define PUSH_ASSERT(...) assert(__VA_ARGS__)
    #else
        #define PUSH_ASSERT(...)
    #endif
#endif

#if !defined(TAU_PRODUCTION)
    #define PUSH_CHECK(Size) PUSH_ASSERT(m_WrittenLength + Size <= m_AllocatedLength)
#else
    #define PUSH_CHECK(Size)
#endif

class PushBuffer final
{
public:
    PushBuffer(
        std::nullptr_t
    ) noexcept
        : m_AllocatedLength(0)
        , m_WrittenLength(0)
        , m_Buffer(nullptr)
    { }

    PushBuffer(
        const u32 initialCapacity = 16
    ) noexcept
        : m_AllocatedLength(initialCapacity)
        , m_WrittenLength(0)
        , m_Buffer(new(::std::nothrow) u32[m_AllocatedLength])
    { }

    ~PushBuffer() noexcept
    {
        delete[] m_Buffer;
        m_Buffer = nullptr;
    }

    PushBuffer(const PushBuffer& copy) noexcept
        : m_AllocatedLength(copy.m_AllocatedLength)
        , m_WrittenLength(copy.m_WrittenLength)
        , m_Buffer(new(::std::nothrow) u32[m_AllocatedLength])
    {
        PUSH_ASSERT(m_Buffer);

        (void) ::std::memcpy(m_Buffer, copy.m_Buffer, m_WrittenLength * sizeof(*m_Buffer));
    }

    PushBuffer(PushBuffer&& move) noexcept
        : m_AllocatedLength(move.m_AllocatedLength)
        , m_WrittenLength(move.m_WrittenLength)
        , m_Buffer(move.m_Buffer)
    {
        move.m_AllocatedLength = 0;
        move.m_Buffer = nullptr;
    }

    PushBuffer& operator =(const PushBuffer& copy) noexcept
    {
        if(this == &copy)
        {
            return *this;
        }

        delete[] m_Buffer;

        m_AllocatedLength = copy.m_AllocatedLength;
        m_WrittenLength = copy.m_WrittenLength;
        m_Buffer = new(::std::nothrow) u32[m_AllocatedLength];

        PUSH_ASSERT(m_Buffer);

        (void) ::std::memcpy(m_Buffer, copy.m_Buffer, m_WrittenLength * sizeof(*m_Buffer));

        return *this;
    }

    PushBuffer& operator =(PushBuffer&& move) noexcept
    {
        if(this == &move)
        {
            return *this;
        }

        delete[] m_Buffer;

        m_AllocatedLength = move.m_AllocatedLength;
        m_WrittenLength = move.m_WrittenLength;
        m_Buffer = move.m_Buffer;

        move.m_AllocatedLength = 0;
        move.m_Buffer = nullptr;

        return *this;
    }

    [[nodiscard]] u32 WrittenLength() const noexcept { return m_WrittenLength; }
    [[nodiscard]] const u32* Buffer() const noexcept { return m_Buffer; }

    void Reset() noexcept
    {
        m_WrittenLength = 0;
    }

    void Push(
        const u32 value
    ) noexcept
    {
        PUSH_CHECK(1);

        m_Buffer[m_WrittenLength] = value;
        ++m_WrittenLength;
    }

    void Push(
        const u32 value0,
        const u32 value1
    ) noexcept
    {
        PUSH_CHECK(2);

        m_Buffer[m_WrittenLength + 0] = value0;
        m_Buffer[m_WrittenLength + 1] = value1;
        m_WrittenLength += 2;
    }

    void Push(
        const u32 value0,
        const u32 value1,
        const u32 value2
    ) noexcept
    {
        PUSH_CHECK(3);

        m_Buffer[m_WrittenLength + 0] = value0;
        m_Buffer[m_WrittenLength + 1] = value1;
        m_Buffer[m_WrittenLength + 2] = value2;
        m_WrittenLength += 3;
    }

    void Push(
        const u32 value0,
        const u32 value1,
        const u32 value2,
        const u32 value3
    ) noexcept
    {
        PUSH_CHECK(4);

        m_Buffer[m_WrittenLength + 0] = value0;
        m_Buffer[m_WrittenLength + 1] = value1;
        m_Buffer[m_WrittenLength + 2] = value2;
        m_Buffer[m_WrittenLength + 3] = value3;
        m_WrittenLength += 4;
    }

    template<typename T>
    void PushT(
        const T value
    ) noexcept
    {
        if constexpr(sizeof(value) == sizeof(u32))
        {
            Push(::std::bit_cast<u32>(value));
        }
        else if constexpr(sizeof(value) == sizeof(u64))
        {
            u32 words[2];
            (void) ::std::memcpy(words, &value, sizeof(words));
            Push(words[0], words[1]);
        }
        else if constexpr(sizeof(value) == sizeof(u16))
        {
            const u16 u16Val = ::std::bit_cast<u16>(value);
            Push(static_cast<u32>(u16Val));
        }
        else if constexpr(sizeof(value) == sizeof(u8))
        {
            const u8 u8Val = ::std::bit_cast<u8>(value);
            Push(static_cast<u32>(u8Val));
        }
    }

    template<typename T0, typename T1>
    void PushT(
        const T0 value0,
        const T1 value1
    ) noexcept
    {
        if constexpr(
            sizeof(value0) == sizeof(u32) &&
            sizeof(value1) == sizeof(u32)
        )
        {
            Push(
                ::std::bit_cast<u32>(value0),
                ::std::bit_cast<u32>(value1)
            );
        }
        else
        {
            PushT(value0);
            PushT(value1);
        }
    }

    template<typename T0, typename T1, typename T2>
    void PushT(
        const T0 value0,
        const T1 value1,
        const T2 value2
    ) noexcept
    {
        if constexpr(
            sizeof(value0) == sizeof(u32) &&
            sizeof(value1) == sizeof(u32) &&
            sizeof(value2) == sizeof(u32)
        )
        {
            Push(
                ::std::bit_cast<u32>(value0),
                ::std::bit_cast<u32>(value1),
                ::std::bit_cast<u32>(value2)
            );
        }
        else
        {
            PushT(value0, value1);
            PushT(value2);
        }
    }

    template<typename T0, typename T1, typename T2, typename T3>
    void PushT(
        const T0 value0,
        const T1 value1,
        const T2 value2,
        const T3 value3
    ) noexcept
    {
        if constexpr(
            sizeof(value0) == sizeof(u32) &&
            sizeof(value1) == sizeof(u32) &&
            sizeof(value2) == sizeof(u32) &&
            sizeof(value3) == sizeof(u32)
        )
        {
            Push(
                ::std::bit_cast<u32>(value0),
                ::std::bit_cast<u32>(value1),
                ::std::bit_cast<u32>(value2),
                ::std::bit_cast<u32>(value3)
            );
        }
        else
        {
            PushT(value0, value1);
            PushT(value2, value3);
        }
    }

    void ReserveWords(
        const u32 reserveCount
    ) noexcept
    {
        if(m_WrittenLength + reserveCount <= m_AllocatedLength)
        {
            return;
        }

        u32 newLength = m_AllocatedLength + (m_AllocatedLength >> 1);

        if(newLength < m_AllocatedLength + reserveCount)
        {
            const u32 minLength = m_AllocatedLength + reserveCount;
            newLength = minLength + (minLength >> 1);
        }

        u32* newBuffer = new(::std::nothrow) u32[newLength];

        PUSH_ASSERT(newBuffer);

        (void) ::std::memcpy(newBuffer, m_Buffer, m_WrittenLength * sizeof(*m_Buffer));

        delete[] m_Buffer;
        m_Buffer = newBuffer;
        m_AllocatedLength = newLength;
    }
private:
    u32 m_AllocatedLength;
    u32 m_WrittenLength;
    u32* m_Buffer;
};

#undef PUSH_CHECK

}
