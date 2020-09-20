#include "ArrayListTest.hpp"
#include "AVLTreeTest.hpp"
#include "StringTest.hpp"
#include "RefUnitTest.hpp"
#include "FixedBlockAllocatorTest.hpp"
#include "FreeListAllocatorTest.hpp"
#include "Vector2fTest.hpp"
#include "Vector3fTest.hpp"
#include "Vector4fTest.hpp"
#include "Matrix4x4fTest.hpp"
#include "UnitTest.hpp"
#include "ConPrinter.hpp"
#include <cstdio>

#include "allocator/PageAllocator.hpp"

#define SHOULD_PAUSE 0

#if SHOULD_PAUSE
  #include <conio.h>
  #define PAUSE(_MSG) printf("Press Any Key To %s.\n", (_MSG)); _getch()
#else
  #define PAUSE(_MSG)
#endif

int main(int argCount, char* args[]) noexcept
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
    StringTest::stringWrapTest();
    StringTest::stringViewTest();
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

    printf("\nVector2f Tests:\n\n");
    Vector2fTests::addTest();
    Vector2fTests::subTest();
    Vector2fTests::mulTest();
    Vector2fTests::divTest();

    Vector2fTests::negTest();

    Vector2fTests::magnitudeTest();
    Vector2fTests::normalizeTest();
    Vector2fTests::dotTest();
    printf("Vector2f Tests Finished\n");

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
    Vector4fTests::addTest();
    Vector4fTests::subTest();
    Vector4fTests::mulTest();
    Vector4fTests::divTest();

    Vector4fTests::negTest();

    Vector4fTests::magnitudeTest();
    Vector4fTests::normalizeTest();
    Vector4fTests::dotTest();
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
        
    printf("\nTests Performed: %d\n", UnitTests::testsPerformed());
    printf("Tests Passed: %d\n", UnitTests::testsPassed());
    printf("Tests Failed: %d\n", UnitTests::testsFailed());

    PAUSE("Close");

    return 0;
}
