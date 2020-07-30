#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

typedef ::std::size_t uSys;
typedef ::std::int64_t i64;

struct Fraction final
{
    i64 n;
    i64 d;
};

constexpr i64 gcd(i64 n, i64 m) noexcept
{
    i64 remainder = 0;

    while(n != 0)
    {
        remainder = m % n;
        m = n;
        n = remainder;
    }
    
    return m;
}

constexpr Fraction simplify(const Fraction& f) noexcept
{
    const i64 g = gcd(f.n, f.d);
    return { f.n / g, f.d / g };
}

constexpr Fraction add(const Fraction& a, const Fraction& b) noexcept
{
    if(a.d == b.d)
    {
        return { a.n + b.n, a.d };
    }
    else
    {
        const i64 an = a.n * b.d;
        const i64 bn = b.n * a.d;
        return { an + bn, a.d * b.d };
    }
}

constexpr Fraction sub(const Fraction& a, const Fraction& b) noexcept
{
    if(a.d == b.d)
    {
        return { a.n - b.n, a.d };
    }
    else
    {
        const i64 an = a.n * b.d;
        const i64 bn = b.n * a.d;
        return { an - bn, a.d * b.d };
    }
}

constexpr Fraction mul(const Fraction& a, const Fraction& b) noexcept
{
    return { a.n * b.n, a.d * b.d };
}

constexpr Fraction div(const Fraction& a, const Fraction& b) noexcept
{
    return { a.n * b.d, a.d * b.n };
}

constexpr Fraction adds(const Fraction& a, const Fraction& b) noexcept
{
    return simplify(add(a, b));
}

constexpr Fraction subs(const Fraction& a, const Fraction& b) noexcept
{
    return simplify(sub(a, b));
}

constexpr Fraction muls(const Fraction& a, const Fraction& b) noexcept
{
    return simplify(mul(a, b));
}

constexpr Fraction divs(const Fraction& a, const Fraction& b) noexcept
{
    return simplify(div(a, b));
}

constexpr double toDouble(const Fraction& f) noexcept
{
    return static_cast<double>(f.n) / static_cast<double>(f.d);
}

constexpr Fraction _mediantAdd(const Fraction& a, const Fraction& b) noexcept
{
    return { a.n + b.n, a.d + b.d };
}

constexpr Fraction _mediantAdds(const Fraction& a, const Fraction& b) noexcept
{
    return simplify(_mediantAdd(a, b));
}

bool epsilonEquals(const double a, const double b, const double epsilon = 1E-8)
{
    return fabs(a - b) < epsilon; 
}

#ifndef MAX_FRAC_ITER
  #define MAX_FRAC_ITER 65536
#endif

constexpr Fraction fromDouble(const double d) noexcept
{
    const i64 whole = static_cast<i64>(d);
    const double frac = d - static_cast<double>(whole);

    if(frac == 0.0)
    {
        return { whole, 1 };
    }
    
    Fraction low = { 0, 1 };
    Fraction high = { 1, 1 };

    Fraction mid = { 0, 0 };

    for(uSys i = 0; i < MAX_FRAC_ITER; ++i)
    {
        mid = _mediantAdds(low, high);
        const double guess = toDouble(mid);

        if(epsilonEquals(guess, frac))
        {
            const Fraction ret = { mid.n + whole, mid.d };
            return simplify(ret);
        }

        if(guess > frac)
        {
            high = mid;
        }
        else
        {
            low = mid;
        }
    }

    const Fraction ret = { mid.n + whole, mid.d };
    return simplify(ret);
}

int main(int argCount, char* args[])
{
    if(argCount < 2)
    {
        wprintf(L"%S decimal\n", args[0]);
        return 1;
    }

    const double decimal = ::std::strtod(args[1], nullptr);

    const Fraction f = fromDouble(decimal);
    const double decimalEst = toDouble(f);

    wprintf(L"%.8llf = %lld/%lld\n", decimal, f.n, f.d);
    wprintf(L"%.8llf = %.8llf\n", decimal, decimalEst);

    return 0;
}
