#pragma once

enum class InsertMethod
{
    Ignore = 0,
    Replace,
    Greater,
    Lesser
};

enum class IteratorMethod
{
    TopDownLR = 0,
    TopDownRL = 0,
    HighestToLowest,
    LowestToHighest
};
