#pragma once

template<typename _T0, typename _T1, typename _T2>
class Triplet final
{
public:
    _T0 t0;
    _T1 t1;
    _T2 t2;
public:
    Triplet() noexcept = default;

    Triplet(const _T0& t0, const _T1& t1, const _T2& t2) noexcept
        : t0(t0), t1(t1), t2(t2)
    { }

    ~Triplet() noexcept = default;

    Triplet(const Triplet& copy) noexcept = default;
    Triplet(Triplet&& move) noexcept = default;

    Triplet& operator=(const Triplet& copy) noexcept = default;
    Triplet& operator=(Triplet&& move) noexcept = default;
};
