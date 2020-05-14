#include "ArrayListUnitTest.hpp"
#include "StringTest.hpp"
#include "RefUnitTest.hpp"
#include "FixedBlockAllocatorTest.hpp"
#include "UnitTest.hpp"
#include <cstdio>

#define SHOULD_PAUSE 0

#if SHOULD_PAUSE
  #include <conio.h>
  #define PAUSE(_MSG) printf("Press Any Key To %s.\n", (_MSG)); _getch()
#else
  #define PAUSE(_MSG)
#endif

int main(int argCount, char* args[]) noexcept
{
    PAUSE("Start");

    printf("\nArrayList Tests:\n\n");
    ArrayListUnitTest::addTest();
    ArrayListUnitTest::emplaceTest();
    ArrayListUnitTest::copyTest();
    printf("ArrayList Tests Finished\n");

    PAUSE("Continue");

    printf("\nString Tests:\n\n");
    StringTest::stringWrapTest();
    StringTest::stringViewTest();
    StringTest::dynStringTest();
    printf("String Tests Finished\n");

    PAUSE("Continue");

    printf("\nReference Counting Pointer Tests:\n\n");
    RefPtrTest::refCountTest();
    RefPtrTest::refRewriteTest();
    RefPtrTest::refTmpTest();
    RefPtrTest::nullableRefCountTest();
    RefPtrTest::nullableRefRewriteTest();
    RefPtrTest::nullableRefTmpTest();
    RefPtrTest::nullableRefNullSetTest();

    RefPtrTest::strongRefCountTest();
    RefPtrTest::strongRefRewriteTest();
    RefPtrTest::strongRefTmpTest();
    RefPtrTest::nullableStrongRefCountTest();
    RefPtrTest::nullableStrongRefRewriteTest();
    RefPtrTest::nullableStrongRefTmpTest();
    RefPtrTest::nullableStrongRefNullSetTest();

    RefPtrTest::refResetTest();
    RefPtrTest::nullableRefResetTest();
    RefPtrTest::strongRefResetTest();
    RefPtrTest::nullableStrongRefResetTest();

    RefPtrTest::refCrossAssignmentTest();
    RefPtrTest::strongRefCrossAssignmentTest();
    printf("Reference Counting Pointer Tests Finished\n");

    PAUSE("Continue");

    printf("\nFixed Block Allocator Tests:\n\n");
    FixedBlockAllocatorUnitTest::arenaAllocationValidityTest();
    FixedBlockAllocatorUnitTest::arenaMacroAllocateTest();
    FixedBlockAllocatorUnitTest::arenaMaxPageExceedTest();
    FixedBlockAllocatorUnitTest::arenaCountTest();
    FixedBlockAllocatorUnitTest::arenaMultipleDeleteTest();

    FixedBlockAllocatorUnitTest::allocationValidityTest();
    FixedBlockAllocatorUnitTest::macroAllocateTest();
    FixedBlockAllocatorUnitTest::maxPageExceedTest();
    FixedBlockAllocatorUnitTest::countTest();
    FixedBlockAllocatorUnitTest::multipleDeleteTest();
    printf("Fixed Block Allocator Tests Finished\n");

    printf("\nTests Performed: %d\n", UnitTests::testsPerformed());
    printf("Tests Passed: %d\n", UnitTests::testsPassed());
    printf("Tests Failed: %d\n", UnitTests::testsFailed());

    PAUSE("Close");

    return 0;
}
