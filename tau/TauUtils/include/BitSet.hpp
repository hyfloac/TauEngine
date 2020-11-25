#pragma once

#include <climits>
#include <DynArray.hpp>

class BitSet final
{
public:
    class BitRef final
    {
        DEFAULT_DESTRUCT(BitRef);
        DEFAULT_CM_PU(BitRef);
    private:
        uSys& _word;
        uSys _bitMask;
    public:
        BitRef(uSys& word, const uSys bitMask) noexcept
            : _word(word)
            , _bitMask(bitMask)
        { }

        [[nodiscard]] operator bool() const noexcept
        { return _word & _bitMask; }

        BitRef& operator=(const bool val) noexcept
        {
            if(val)
            { _word |= _bitMask; }
            else
            { _word &= ~_bitMask; }

            return *this;
        }
    };
public:
    static constexpr uSys BIT_COUNT = sizeof(uSys) * CHAR_BIT;
private:
    uSys _bitCount;
    DynArray<uSys> _bits;
public:
    BitSet(const uSys bitCount, const bool initialValue = false) noexcept
        : _bitCount(bitCount)
        , _bits((bitCount / BIT_COUNT) + 1)
    { ::std::memset(_bits.arr(), initialValue ? 0xFF : 0x00, _bits.count() * sizeof(uSys)); }

    [[nodiscard]] uSys bitCount() const noexcept { return _bitCount; }

    [[nodiscard]] bool operator[](const uSys index) const noexcept
    {
        const uSys wordIndex = index / BIT_COUNT;
        const uSys bitIndex = index % BIT_COUNT;
        const uSys invBitIndex = BIT_COUNT - bitIndex;

        const uSys word = _bits[wordIndex];
        const bool ret = word & (1 << invBitIndex);

        return ret;
    }

    [[nodiscard]] BitRef operator[](const uSys index) noexcept
    {
        const uSys wordIndex = index / BIT_COUNT;
        const uSys bitIndex = index % BIT_COUNT;
        const uSys invBitIndex = BIT_COUNT - bitIndex;

        return BitRef(_bits[wordIndex], 1 << invBitIndex);
    }

    [[nodiscard]] bool at(const uSys index) const noexcept
    {
        const uSys wordIndex = index / BIT_COUNT;
        const uSys bitIndex = index % BIT_COUNT;
        const uSys invBitIndex = BIT_COUNT - bitIndex;

        if(wordIndex >= _bits.count())
        { return false; }

        const uSys word = _bits[wordIndex];
        const bool ret = word & (1 << invBitIndex);

        return ret;
    }
};
