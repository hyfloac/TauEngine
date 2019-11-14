#include "ArrayListUnitTest.hpp"
#include <cstdio>

int main(int argCount, char* args[]) noexcept
{
    getchar();
    ArrayListUnitTest::addTest();
    ArrayListUnitTest::emplaceTest();
    ArrayListUnitTest::copyTest();
    getchar();
    return 0;
}
