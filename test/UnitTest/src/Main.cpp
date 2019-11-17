#include "ArrayListUnitTest.hpp"
#include "StringTest.hpp"
#include <cstdio>
#include <conio.h>

int main(int argCount, char* args[]) noexcept
{
    printf("Press Any Key To Start.\n");
    _getch();
    printf("\nArrayList Tests:\n");
    ArrayListUnitTest::addTest();
    ArrayListUnitTest::emplaceTest();
    ArrayListUnitTest::copyTest();
    printf("ArrayList Tests Finished\n");
    printf("\nPress Any Key To Continue\n");
    _getch();
    printf("\nString Tests:\n");
    StringTest::stringWrapTest();
    StringTest::stringViewTest();
    StringTest::dynStringTest();
    printf("String Tests Finished\n");
    printf("Press Any Key To Close.\n");
    _getch();
    return 0;
}
