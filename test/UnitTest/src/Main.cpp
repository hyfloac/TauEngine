#include "ArrayListTest.hpp"
#include "AVLTreeTest.hpp"
#include "StreamedAVLTreeTest.hpp"
#include "StringTest.hpp"
#include "RefUnitTest.hpp"
#include "FixedBlockAllocatorTest.hpp"
#include "FreeListAllocatorTest.hpp"
#include "Vector2fTest.hpp"
#include "Vector3fTest.hpp"
#include "Vector4fTest.hpp"
#include "Matrix4x4fTest.hpp"
#include "SlabAllocatorTest.hpp"
#include "MathTest.hpp"
#include "UnitTest.hpp"
#include "ConPrinter.hpp"
#include "MemoryFileTest.hpp"
#include <cstdio>

#include "allocator/PageAllocator.hpp"

#define SHOULD_PAUSE 0

#if SHOULD_PAUSE
  #include <conio.h>
  #define PAUSE(_MSG) printf("Press Any Key To %s.\n", (_MSG)); _getch()
#else
  #define PAUSE(_MSG)
#endif

int main(int argCount, char* args[])
{
    PageAllocator::init();

    PAUSE("Start");

    printf("\nArrayList Tests:\n\n");
    ArrayListUnitTest::addTest();
    ArrayListUnitTest::emplaceTest();
    ArrayListUnitTest::copyTest();
    printf("ArrayList Tests Finished\n");

    PAUSE("Continue");

    printf("\nString Tests:\n\n");
    StringTest::dynStringViewTest();
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

    PAUSE("Continue");

    printf("\nFree List Allocator Tests:\n\n");
    FreeListAllocatorTest::resetTest();
    FreeListAllocatorTest::destructTest();

    FreeListAllocatorTest::difTypeResetTest();
    FreeListAllocatorTest::difTypeDestructTest();

    FreeListAllocatorTest::difPtrResetTest();
    FreeListAllocatorTest::difPtrDestructTest();

    FreeListAllocatorTest::difPtrTypeResetTest();
    FreeListAllocatorTest::difPtrTypeDestructTest();
    printf("Free List Allocator Tests Finished\n");

    PAUSE("Continue");

    Vector2fTests::runTests();

    PAUSE("Continue");

    printf("\nVector3f Tests:\n\n");
    Vector3fTests::addTest();
    Vector3fTests::subTest();
    Vector3fTests::mulTest();
    Vector3fTests::divTest();

    Vector3fTests::negTest();

    Vector3fTests::magnitudeTest();
    Vector3fTests::normalizeTest();
    Vector3fTests::dotTest();
    Vector3fTests::crossTest();
    printf("Vector3f Tests Finished\n");

    PAUSE("Continue");

    printf("\nVector4f Tests:\n\n");
    Vector4fTests::runTests();
    printf("Vector4f Tests Finished\n");

    PAUSE("Continue");

    printf("\nMatrix4x4f Tests:\n\n");
    Matrix4x4fTests::addTest();
    Matrix4x4fTests::subTest();
    Matrix4x4fTests::mulTest();

    Matrix4x4fTests::addScalarTest();
    Matrix4x4fTests::subScalarTest();
    Matrix4x4fTests::mulScalarTest();
    Matrix4x4fTests::divScalarTest();

    Matrix4x4fTests::mulVecTest();
    printf("Matrix4x4f Tests Finished\n");

    PAUSE("Continue");

    printf("\nAVL Tree Tests:\n\n");
    AVLTreeUnitTest::insertIgnoreTest();
    AVLTreeUnitTest::insertDuplicateIgnoreTest();
    AVLTreeUnitTest::emplaceIgnoreTest();
    AVLTreeUnitTest::emplaceDuplicateIgnoreTest();
    AVLTreeUnitTest::findIgnoreTest();
    AVLTreeUnitTest::findDuplicateIgnoreTest();
    AVLTreeUnitTest::removeIgnoreTest();
    AVLTreeUnitTest::removeDuplicateIgnoreTest();
    
    AVLTreeUnitTest::insertReplaceTest();
    AVLTreeUnitTest::insertDuplicateReplaceTest();
    AVLTreeUnitTest::emplaceReplaceTest();
    AVLTreeUnitTest::emplaceDuplicateReplaceTest();
    AVLTreeUnitTest::findReplaceTest();
    AVLTreeUnitTest::findDuplicateReplaceTest();
    AVLTreeUnitTest::removeReplaceTest();
    AVLTreeUnitTest::removeDuplicateReplaceTest();
    
    AVLTreeUnitTest::insertGreaterTest();
    AVLTreeUnitTest::insertDuplicateGreaterTest();
    AVLTreeUnitTest::emplaceGreaterTest();
    AVLTreeUnitTest::emplaceDuplicateGreaterTest();
    AVLTreeUnitTest::findGreaterTest();
    AVLTreeUnitTest::findDuplicateGreaterTest();
    AVLTreeUnitTest::removeGreaterTest();
    AVLTreeUnitTest::removeDuplicateGreaterTest();
    
    AVLTreeUnitTest::insertLesserTest();
    AVLTreeUnitTest::insertDuplicateLesserTest();
    AVLTreeUnitTest::emplaceLesserTest();
    AVLTreeUnitTest::emplaceDuplicateLesserTest();
    AVLTreeUnitTest::findLesserTest();
    AVLTreeUnitTest::findDuplicateLesserTest();
    AVLTreeUnitTest::removeLesserTest();
    AVLTreeUnitTest::removeDuplicateLesserTest();
    printf("AVL Tree Tests Finished\n");

    PAUSE("Continue");

    printf("\nStreamed AVL Tree Tests:\n\n");
    StreamedAVLTreeUnitTest::insertIgnoreTest();
    StreamedAVLTreeUnitTest::insertDuplicateIgnoreTest();
    StreamedAVLTreeUnitTest::emplaceIgnoreTest();
    StreamedAVLTreeUnitTest::emplaceDuplicateIgnoreTest();
    StreamedAVLTreeUnitTest::findIgnoreTest();
    StreamedAVLTreeUnitTest::findDuplicateIgnoreTest();
    StreamedAVLTreeUnitTest::removeIgnoreTest();
    StreamedAVLTreeUnitTest::removeDuplicateIgnoreTest();
    
    StreamedAVLTreeUnitTest::insertReplaceTest();
    StreamedAVLTreeUnitTest::insertDuplicateReplaceTest();
    StreamedAVLTreeUnitTest::emplaceReplaceTest();
    StreamedAVLTreeUnitTest::emplaceDuplicateReplaceTest();
    StreamedAVLTreeUnitTest::findReplaceTest();
    StreamedAVLTreeUnitTest::findDuplicateReplaceTest();
    StreamedAVLTreeUnitTest::removeReplaceTest();
    StreamedAVLTreeUnitTest::removeDuplicateReplaceTest();
    
    StreamedAVLTreeUnitTest::insertGreaterTest();
    StreamedAVLTreeUnitTest::insertDuplicateGreaterTest();
    StreamedAVLTreeUnitTest::emplaceGreaterTest();
    StreamedAVLTreeUnitTest::emplaceDuplicateGreaterTest();
    StreamedAVLTreeUnitTest::findGreaterTest();
    StreamedAVLTreeUnitTest::findDuplicateGreaterTest();
    StreamedAVLTreeUnitTest::removeGreaterTest();
    StreamedAVLTreeUnitTest::removeDuplicateGreaterTest();
    
    StreamedAVLTreeUnitTest::insertLesserTest();
    StreamedAVLTreeUnitTest::insertDuplicateLesserTest();
    StreamedAVLTreeUnitTest::emplaceLesserTest();
    StreamedAVLTreeUnitTest::emplaceDuplicateLesserTest();
    StreamedAVLTreeUnitTest::findLesserTest();
    StreamedAVLTreeUnitTest::findDuplicateLesserTest();
    StreamedAVLTreeUnitTest::removeLesserTest();
    StreamedAVLTreeUnitTest::removeDuplicateLesserTest();
    printf("Streamed AVL Tree Tests Finished\n");
        
    PAUSE("Continue");

    printf("\nSlab Allocator Tests:\n\n");
    SlabAllocatorUnitTest::runTests();
    printf("Slab Allocator Tests Finished\n");
        
    PAUSE("Continue");

    printf("\nMath Tests:\n\n");
    MathTest::runTests();
    printf("Math Tests Finished\n");
        
    PAUSE("Continue");

    printf("\nMemory File Tests:\n\n");
    MemoryFileTest::runTests();
    printf("Memory File Tests Finished\n");

    printf("\nTests Performed: %d\n", UnitTests::testsPerformed());
    printf("Tests Passed: %d\n", UnitTests::testsPassed());
    printf("Tests Failed: %d\n", UnitTests::testsFailed());

    PAUSE("Close");

    return 0;
}
