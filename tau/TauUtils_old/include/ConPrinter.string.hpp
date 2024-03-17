#pragma once

#include "ConPrinter.hpp"
#include <string>

template<>
struct TPrinter<::std::string> final
{
    static i32 print(const ::std::string& t) noexcept { return ConPrinter::print(t.c_str()); }
    static i32 printW(const ::std::string& t) noexcept { return wprintf(L"%hs", t.c_str()); }
    static i32 print(FILE* const file, const ::std::string& t) noexcept { return ConPrinter::print(t.c_str()); }
    static i32 printW(FILE* const file, const ::std::string& t) noexcept { return fwprintf(file, L"%hs", t.c_str()); }
};

template<>
struct TPrinter<::std::wstring> final
{
    static i32 print(const ::std::wstring& t) noexcept { return printf("%ls", t.c_str()); }
    static i32 printW(const ::std::wstring& t) noexcept { return wprintf(L"%s", t.c_str()); }
    static i32 print(FILE* const file, const ::std::wstring& t) noexcept { return fprintf(file, "%ls", t.c_str()); }
    static i32 printW(FILE* const file, const ::std::wstring& t) noexcept { return fwprintf(file, L"%s", t.c_str()); }
};
