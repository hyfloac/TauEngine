#include "ArrayListUnitTest.hpp"
#include "UnitTest.hpp"
#include <ArrayList.hpp>

namespace ArrayListUnitTest {

struct TestStruct final
{
    std::size_t x;
    int y;

    TestStruct(int x, int y) noexcept
        : x(x), y(y)
    { }
};

void addTest() noexcept
{
    UNIT_TEST();
    ArrayList<TestStruct> list;

    for(int i = 0; i < 65000; ++i)
    {
        list.add(TestStruct(i, i << 1));
    }

    for(std::size_t i = 0; i < list.count(); ++i)
    {
        const TestStruct& elem = list[i];
        if(elem.x != i)
        {
            printf("Match failed: %zu\n", i);
        }
    }
}

void emplaceTest() noexcept
{
    UNIT_TEST();
    ArrayList<TestStruct> list;

    for(int i = 0; i < 1000; ++i)
    {
        list.emplace(i, i << 1);
    }

    for(std::size_t i = 0; i < list.count(); ++i)
    {
        const TestStruct& elem = list[i];
        if(elem.x != i)
        {
            printf("Match failed: %zu\n", i);
        }
    }
}

void copyTest() noexcept
{
    UNIT_TEST();
    ArrayList<TestStruct> list;

    for(int i = 0; i < 1000; ++i)
    {
        list.add(TestStruct(i, i << 1));
    }

    ArrayList<TestStruct> copy(list);

    for(std::size_t i = 0; i < list.count(); ++i)
    {
        const TestStruct& elem = copy[i];
        if(elem.x != i)
        {
            printf("Match failed: %zu\n", i);
        }
    }
}
}
