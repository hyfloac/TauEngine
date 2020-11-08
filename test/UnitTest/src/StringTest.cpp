#include "UnitTest.hpp"
#include "StringTest.hpp"
#include <String.hpp>

namespace StringTest {

template<typename _C, uSys _Length>
struct str_array
{
    constexpr const _C* c_str()     const noexcept { return _data; }
    constexpr const _C* data()      const noexcept { return _data; }
    constexpr _C operator[](uSys i) const noexcept { return _data[i]; }
    constexpr const _C* begin()     const noexcept { return _data; }
    constexpr const _C* end()       const noexcept { return _data + _Length; }
    constexpr uSys size()           const noexcept { return _Length; }
    constexpr operator const _C*()  const noexcept { return _data; }

    _C _data[_Length + 1];  // +1 for null-terminator
};

namespace detail {
template<typename _CTo, typename _CFrom>
constexpr _CTo static_cast_ascii(_CFrom x) noexcept
{
    return static_cast<_CTo>(x);
}

template<typename _CTo, typename _CFrom, uSys _Length, uSys... _Chars>
constexpr str_array<_CTo, _Length - 1> do_str_array_cast(const _CFrom(&arr)[_Length], std::index_sequence<_Chars...>) noexcept
{
    return { static_cast_ascii<_CTo>(arr[_Chars])..., 0 };
}
} //namespace detail

template<typename _CTo, typename _CFrom, uSys _Length, typename _Indices = std::make_index_sequence<_Length - 1>>
constexpr str_array<_CTo, _Length - 1> str_array_cast(const _CFrom(&arr)[_Length]) noexcept
{
    return detail::do_str_array_cast<_CTo>(arr, _Indices { });
}

template<typename _C>
void dynStringViewTest() noexcept
{
    static constexpr auto _str0 = str_array_cast<_C>("Hello World!");
    static constexpr auto _str1 = str_array_cast<_C>("Hello");
    static constexpr auto _str2 = str_array_cast<_C>("World");
    static constexpr auto _str3 = str_array_cast<_C>(" ");
    static constexpr auto _str4 = str_array_cast<_C>("!");

    {
        const DynStringT<_C> str(_str0.c_str());
        const DynStringViewT<_C> strView(str, 0, 5);
        UTAssert(strView == _str1.c_str());
        UTAssert(strView != str);
    }

    {
        const DynStringT<_C> str(_str0.c_str());
        const DynStringViewT<_C> strView = DynStringViewT<_C>::create(str, 6, 5);
        UTAssert(strView == _str2.c_str());
        UTAssert(strView != str);
    }

    {
        DynStringT<_C> str(_str1.c_str());
        str = str + _str3.c_str() + _str2.c_str();
        str = str + _str4.c_str();
        const DynStringViewT<_C> strView(str, 0, 5);
        UTAssert(strView == _str1.c_str());
        UTAssert(strView != str);
    }

    {
        DynStringT<_C> str(_str1.c_str());
        str = str + _str3.c_str() + _str2.c_str();
        str = str + _str4.c_str();
        const DynStringViewT<_C> strView = DynStringViewT<_C>::create(str, 6, 5);
        UTAssert(strView == _str2.c_str());
        UTAssert(strView != str);
    }
}

template<typename _C>
void dynStringTest() noexcept
{
    static constexpr auto _str0 = str_array_cast<_C>("Hello");
    static constexpr auto _str1 = str_array_cast<_C>(" World");
    static constexpr auto _str2 = str_array_cast<_C>("!");
    static constexpr auto _str3 = str_array_cast<_C>("Hello World");
    static constexpr auto _str4 = str_array_cast<_C>("Hello World!");

    {
        DynStringT<_C> str0(_str0.c_str());
        const DynStringT<_C> str1 = str0 + _str1.c_str();
        str0 = str1 + _str2.c_str();
        UTAssert(str1 == _str3.c_str());
        UTAssert(str0 == _str4.c_str());
        UTAssert(str0 != str1);
    }
}

void dynStringViewTest() noexcept
{
    UNIT_TEST();
    dynStringViewTest<char>();
    dynStringViewTest<wchar_t>();
}

void dynStringTest() noexcept
{
    UNIT_TEST();
    dynStringTest<char>();
    dynStringTest<wchar_t>();
}

}
