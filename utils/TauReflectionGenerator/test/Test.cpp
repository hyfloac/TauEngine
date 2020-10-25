#include "Test.hpp"
#include <iostream>

int main()
{
    Test test;

    std::cout << "Test::time " << test.time() << std::endl;
    std::cout << "Test::date " << test.date() << std::endl;
    std::cout << std::endl;

    int** time = Test::GetStaticClass().getProperty<int*>(&test, "_time");
    long* date = Test::GetStaticClass().getProperty<long>(&test, "_date");
    Vector* vec = test.getClass().getProperty<Vector>(&test, "_vec");
    const Vector* cvec = test.getClass().getProperty<Vector>(const_cast<const Test*>(&test), "_vec");

    std::cout << "&Test::time " << time << std::endl;
    std::cout << "&Test::date " << date << std::endl;
    std::cout << "&Test::vec  " << vec  << std::endl;
    std::cout << "&Test::cvec " << cvec << std::endl;

    std::cout << std::endl;

    **time = 234;
    std::cout << "Test::time " << test.time() << std::endl;
    std::cout << "Test::date " << test.date() << std::endl;
    std::cout << "Test::vec (" << cvec->x << ", " << cvec->y << ", " << cvec->z << ")" << std::endl;

    std::cout << std::endl;

    Foo foo;

    int* bar = foo.getClass().getProperty<int>(&foo, "_bar");
    Foo::GetStaticClass().setProperty<float>(&foo, "_baz", 2.718f);

    *bar = 128;

    std::cout << "&Foo::bar " << bar << std::endl;

    std::cout << std::endl;

    std::cout << "Foo::bar " << foo.bar() << std::endl;
    std::cout << "Foo::baz " << foo.baz() << std::endl;

    return 0;
}
