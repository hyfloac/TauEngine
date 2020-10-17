#pragma once

#include "Base.hpp"
#include "Test.generated.hpp"

struct Vector
{
public:
    float x;
    float y;
    float z;
};

TAU_CLASS()
class Test
{
    GENERATED_BODY()
private:
    TAU_PROPERTY(get, set)
    int _time;

    long _date;

    TAU_PROPERTY(get)
    Vector _vec { 3.14f, 32.0f, 0.001f };
public:
    int time() const noexcept { return _time; }
    long date() const noexcept { return _date; }
};
