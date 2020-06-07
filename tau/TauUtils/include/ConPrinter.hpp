#pragma once

#include "NumTypes.hpp"
#include "Objects.hpp"
#include "String.hpp"

template<typename _T>
struct TPrinter final
{
    static i32 print(const _T& t) noexcept { return 0; }
    static i32 print(FILE* const file, const _T& t) noexcept { return 0; }
};

class ConPrinter final
{
    DELETE_CONSTRUCT(ConPrinter);
    DELETE_DESTRUCT(ConPrinter);
    DELETE_CM(ConPrinter);
public:
    static i32 print(const char* str) noexcept { return fputs(str, stdout); }
    static i32 print(const char c) noexcept { return fputc(c, stdout); }
    static i32 print(const c8 c) noexcept { return fputc(c, stdout); }

    static i32 print(FILE* const file, const char* str) noexcept { return fputs(str, file); }
    static i32 print(FILE* const file, const char c) noexcept { return fputc(c, file); }
    static i32 print(FILE* const file, const c8 c) noexcept { return fputc(c, file); }

    static i32 print(const i8  d) noexcept { return fprintf(stdout, "%d", d); }
    static i32 print(const i16 d) noexcept { return fprintf(stdout, "%d", d); }
    static i32 print(const i32 d) noexcept { return fprintf(stdout, "%d", d); }
    static i32 print(const i64 d) noexcept { return fprintf(stdout, "%lld", d); }

    static i32 print(const u8  d) noexcept { return fprintf(stdout, "%u", d); }
    static i32 print(const u16 d) noexcept { return fprintf(stdout, "%u", d); }
    static i32 print(const u32 d) noexcept { return fprintf(stdout, "%u", d); }
    static i32 print(const u64 d) noexcept { return fprintf(stdout, "%llu", d); }

    static i32 print(const f32 f) noexcept { return fprintf(stdout, "%f", f); }
    static i32 print(const f64 f) noexcept { return fprintf(stdout, "%f", f); }

    static i32 print(FILE* const file, const i8  d) noexcept { return fprintf(file, "%d", d); }
    static i32 print(FILE* const file, const i16 d) noexcept { return fprintf(file, "%d", d); }
    static i32 print(FILE* const file, const i32 d) noexcept { return fprintf(file, "%d", d); }
    static i32 print(FILE* const file, const i64 d) noexcept { return fprintf(file, "%lld", d); }

    static i32 print(FILE* const file, const u8  d) noexcept { return fprintf(file, "%u", d); }
    static i32 print(FILE* const file, const u16 d) noexcept { return fprintf(file, "%u", d); }
    static i32 print(FILE* const file, const u32 d) noexcept { return fprintf(file, "%u", d); }
    static i32 print(FILE* const file, const u64 d) noexcept { return fprintf(file, "%llu", d); }

    static i32 print(FILE* const file, const f32 f) noexcept { return fprintf(file, "%f", f); }
    static i32 print(FILE* const file, const f64 f) noexcept { return fprintf(file, "%f", f); }

    template<typename _T>
    static i32 print(const _T* const p) noexcept { return fprintf(stdout, "%p", p); }
    template<typename _T>
    static i32 print(FILE* const file, const _T* const p) noexcept { return fprintf(file, "%p", p); }

    static i32 print(const ConstExprString& str) noexcept { return fputs(str.c_str(), stdout); }
    static i32 print(const String& str) noexcept { return fputs(str.c_str(), stdout); }
    static i32 print(const StringView& str) noexcept { return fprintf(stdout, "%.*s", static_cast<int>(str.length()), str.c_str()); }
    static i32 print(const DynString& str) noexcept { return fputs(str.c_str(), stdout); }
    static i32 print(const DynStringView& str) noexcept { return fprintf(stdout, "%.*s", static_cast<int>(str.length()), str.c_str()); }

    static i32 print(FILE* const file, const ConstExprString& str) noexcept { return fputs(str.c_str(), file); }
    static i32 print(FILE* const file, const String& str) noexcept { return fputs(str.c_str(), file); }
    static i32 print(FILE* const file, const StringView& str) noexcept { return fprintf(file, "%.*s", static_cast<int>(str.length()), str.c_str()); }
    static i32 print(FILE* const file, const DynString& str) noexcept { return fputs(str.c_str(), file); }
    static i32 print(FILE* const file, const DynStringView& str) noexcept { return fprintf(file, "%.*s", static_cast<int>(str.length()), str.c_str()); }

    template<typename _T>
    static i32 print(const _T& t) noexcept { return TPrinter<_T>::print(t); }
    template<typename _T>
    static i32 print(FILE* const file, const _T& t) noexcept { return TPrinter<_T>::print(file, t); }

    template<typename _CurrArg, typename... _Args>
    static i32 print(const char* fmt, _CurrArg currArg, _Args... args) noexcept
    {
        i32 count = 0;
        for(uSys i = 0; fmt[i]; ++i)
        {
            if(fmt[i] == '%')
            {
                if(fmt[i + 1] == '%')
                {
                    fputc('%', stdout);
                    ++count;
                    ++i;
                }
                else
                {
                    count += print(currArg);
                    count += print(fmt, args);
                }
            }
            else
            {
                fputc(fmt[i], stdout);
                ++count;
            }
        }
        return count;
    }

    template<typename _CurrArg, typename... _Args>
    static i32 print(FILE* const file, const char* fmt, _CurrArg currArg, _Args... args) noexcept
    {
        i32 count = 0;
        for(uSys i = 0; fmt[i]; ++i)
        {
            if(fmt[i] == '%')
            {
                if(fmt[i + 1] == '%')
                {
                    fputc('%', file);
                    ++count;
                    ++i;
                }
                else
                {
                    count += print(file, currArg);
                    count += print(file, fmt, args);
                }
            }
            else
            {
                fputc(fmt[i], file);
                ++count;
            }
        }
        return count;
    }
};
