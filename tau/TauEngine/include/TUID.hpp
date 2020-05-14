/**
 * @file
 */
#pragma once
#include <NumTypes.hpp>
#include <DLL.hpp>

/**
 * Tau Unique Identifier.
 * 
 * A knockoff UUID "guaranteed" to be unique within the engine.
 */
class TAU_DLL TUID
{
private:
    u64 _lowBits;
    u64 _highBits;
public:
    static TUID generate() noexcept;
public:
    TUID(u64 highBits, u64 lowBits) noexcept;

    [[nodiscard]] inline u64 lowBits()  const noexcept { return _lowBits;  }
    [[nodiscard]] inline u64 highBits() const noexcept { return _highBits; }

    void toString(char str[38]) const noexcept;

    [[nodiscard]] inline i32 compareTo(const TUID& tuid) const noexcept
    {
        if(_highBits == tuid._highBits)
        {
            if(_lowBits == tuid._lowBits)
            {
                return 0;
            }

            return _lowBits > tuid._lowBits ? 1 : -1;
        }
        else
        {
            return _highBits > tuid._highBits ? 1 : -1;
        }
    }

#if __has_feature(__cpp_impl_three_way_comparison)
    [[nodiscard]] inline i32 operator <=>(const TUID& tuid) const noexcept { return compareTo(tuid); }
#endif

    [[nodiscard]] inline bool operator ==(const TUID& tuid) const noexcept { return _lowBits == tuid._lowBits && _highBits == tuid._highBits; }
    [[nodiscard]] inline bool operator !=(const TUID& tuid) const noexcept { return !(this->operator==(tuid)); }

    [[nodiscard]] inline bool operator > (const TUID& tuid) const noexcept { return _highBits > tuid._highBits || (_highBits == tuid._highBits && _lowBits > tuid._lowBits); }
    [[nodiscard]] inline bool operator < (const TUID& tuid) const noexcept { return _highBits < tuid._highBits || (_highBits == tuid._highBits && _lowBits < tuid._lowBits); }

    [[nodiscard]] inline bool operator >=(const TUID& tuid) const noexcept { return _highBits > tuid._highBits || (_highBits == tuid._highBits && _lowBits >= tuid._lowBits); }
    [[nodiscard]] inline bool operator <=(const TUID& tuid) const noexcept { return _highBits < tuid._highBits || (_highBits == tuid._highBits && _lowBits <= tuid._lowBits); }
};
