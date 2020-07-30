#pragma once

namespace RefPtrTest {
void refCountTest() noexcept;
void refRewriteTest() noexcept;
void refTmpTest() noexcept;

void nullableRefCountTest() noexcept;
void nullableRefRewriteTest() noexcept;
void nullableRefTmpTest() noexcept;
void nullableRefNullSetTest() noexcept;

void strongRefCountTest() noexcept;
void strongRefRewriteTest() noexcept;
void strongRefTmpTest() noexcept;

void nullableStrongRefCountTest() noexcept;
void nullableStrongRefRewriteTest() noexcept;
void nullableStrongRefTmpTest() noexcept;
void nullableStrongRefNullSetTest() noexcept;

void refResetTest() noexcept;
void nullableRefResetTest() noexcept;
void strongRefResetTest() noexcept;
void nullableStrongRefResetTest() noexcept;
}
