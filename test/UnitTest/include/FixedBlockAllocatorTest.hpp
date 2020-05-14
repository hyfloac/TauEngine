#pragma once


namespace FixedBlockAllocatorUnitTest {
void arenaAllocationValidityTest() noexcept;
void arenaMacroAllocateTest() noexcept;
void arenaMaxPageExceedTest() noexcept;
void arenaCountTest() noexcept;
void arenaMultipleDeleteTest() noexcept;

void allocationValidityTest() noexcept;
void macroAllocateTest() noexcept;
void maxPageExceedTest() noexcept;
void countTest() noexcept;
void multipleDeleteTest() noexcept;
}
