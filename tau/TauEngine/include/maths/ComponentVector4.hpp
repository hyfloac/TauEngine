#pragma once

#pragma warning(push, 0)
#include <emmintrin.h>
#pragma warning(pop)

#include <NumTypes.hpp>
#include <Alignment.h>

union ALIGN_MSVC(16) Float4
{
    float ALIGN_MSVC(16) arr[4] ALIGN_GCC(16);
    struct ALIGN_MSVC(16) { float x, y, z, w; } ALIGN_GCC(16);
    struct ALIGN_MSVC(16) { float r, g, b, a; } ALIGN_GCC(16);

} ALIGN_GCC(16);

union ALIGN_MSVC(16) ComponentVector4
{
    __m128 vec;
    float ALIGN_MSVC(16) arr[4] ALIGN_GCC(16);
    struct ALIGN_MSVC(16) { float x, y, z, w; } ALIGN_GCC(16);
    struct ALIGN_MSVC(16) { float r, g, b, a; } ALIGN_GCC(16);    
    struct ALIGN_MSVC(16) elems { float x, y, z, w; } ALIGN_GCC(16);

private:
    template<u32 _Index>
    inline float __v() const noexcept
    {
        float ALIGN_MSVC(16) ret[4] ALIGN_GCC(16);
        _mm_store_ps(ret, vec);
        return ret[_Index];
    }

public:
    inline float vx() const noexcept { return __v<0>(); }
    inline float vy() const noexcept { return __v<1>(); }
    inline float vz() const noexcept { return __v<2>(); }
    inline float vw() const noexcept { return __v<3>(); }

    inline Float4 unpack() const noexcept
    {
        Float4 ret = { 0.0f, 0.0f, 0.0f, 0.0f };
        _mm_store_ps(ret.arr, vec);
        return ret;
    }

    ComponentVector4() noexcept = default;

    ComponentVector4(const __m128 _vec) noexcept
        : vec(_vec)
    { }

    ComponentVector4(const float _x, const float _y, const float _z, const float _w) noexcept
        : x(_x), y(_y), z(_z), w(_w)
    { }

    ComponentVector4(const ComponentVector4& copy) noexcept = default;
    ComponentVector4(ComponentVector4&& move) noexcept = default;

    ComponentVector4& operator =(const ComponentVector4& copy) noexcept = default;
    ComponentVector4& operator =(ComponentVector4&& move) noexcept = default;
} ALIGN_GCC(16);

union ALIGN_MSVC(16) ComponentVector4i
{
    __m128i vec;
    struct ALIGN_MSVC(16) { i32 x, y, z, w; } ALIGN_GCC(16);
    struct ALIGN_MSVC(16) { i32 ix, iy, iz, iw; } ALIGN_GCC(16);

    ComponentVector4i() noexcept = default;

    ComponentVector4i(const __m128i _vec) noexcept
        : vec(_vec)
    { }

    ComponentVector4i(const i32 _x, const i32 _y, const i32 _z, const i32 _w) noexcept
        : x(_x), y(_y), z(_z), w(_w)
    { }

    ComponentVector4i(const ComponentVector4i& copy) noexcept = default;
    ComponentVector4i(ComponentVector4i&& move) noexcept = default;

    ComponentVector4i& operator =(const ComponentVector4i& copy) noexcept = default;
    ComponentVector4i& operator =(ComponentVector4i&& move) noexcept = default;
} ALIGN_GCC(16);

typedef ComponentVector4  CompVec4;
typedef ComponentVector4i CompVec4i;

static_assert(sizeof(ComponentVector4)  == 16, "Component Vector4 does not have proper sizing.");
static_assert(sizeof(ComponentVector4i) == 16, "Component Vector4i does not have proper sizing.");
