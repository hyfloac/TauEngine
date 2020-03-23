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
    u64 _lowBits;
    u64 _highBits;
public:
    static TUID generate() noexcept;
public:
    TUID(const u64 highBits, const u64 lowBits) noexcept;

    [[nodiscard]] inline u64 lowBits()  const noexcept { return _lowBits;  }
    [[nodiscard]] inline u64 highBits() const noexcept { return _highBits; }

    void toString(char str[38]) const noexcept;
};
