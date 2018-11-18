/**
 * @file
 */
#pragma once
#include <NumTypes.hpp>
#include <DLL.hpp>

/**
 * Tau Unique IDentification.
 * 
 * A knockoff UUID "guaranteed" to be unique within the engine.
 */
class TAU_DLL TUID
{
private:
    u64 _highBits;
    u64 _lowBits;
public:
    static TUID generate() noexcept;
public:
    TUID(u64 highBits, u64 lowBits) noexcept;

    inline u64 highBits() const noexcept { return _highBits; }
    inline u64 lowBits()  const noexcept { return _lowBits;  }

    void toString(char str[38]) const noexcept;
};

TAU_DLL u32 getSeed(TUID tuid) noexcept;
