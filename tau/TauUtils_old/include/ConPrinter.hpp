#pragma once

#include "NumTypes.hpp"
#include "Objects.hpp"
#include "String.hpp"

template<typename _T>
struct TPrinter final
{
    static i32 print(const _T& t) noexcept { return 0; }
    static i32 printW(const _T& t) noexcept { return 0; }
    static i32 print(FILE* const file, const _T& t) noexcept { return 0; }
    static i32 printW(FILE* const file, const _T& t) noexcept { return 0; }
};

class ConPrinter final
{
    DELETE_CONSTRUCT(ConPrinter);
    DELETE_DESTRUCT(ConPrinter);
    DELETE_CM(ConPrinter);
public:
    static i32 print(const char* str)    noexcept { return fputs(str, stdout);  }
    static i32 print(const char c)       noexcept { return fputc(c, stdout);    }
    static i32 print(const wchar_t* str) noexcept { return fputws(str, stdout); }
    static i32 print(const wchar_t c)    noexcept { return fputwc(c, stdout);   }
    
    static i32 printW(const char* str)    noexcept { return fwprintf(stdout, L"%hs", str);  }
    static i32 printW(const char c)       noexcept { return fputwc(c, stdout);   }
    static i32 printW(const wchar_t* str) noexcept { return fputws(str, stdout); }
    static i32 printW(const wchar_t c)    noexcept { return fputwc(c, stdout);   }

    static i32 print(FILE* const file, const char* str)    noexcept { return fputs(str, file);  }
    static i32 print(FILE* const file, const char c)       noexcept { return fputc(c, file);    }
    static i32 print(FILE* const file, const wchar_t* str) noexcept { return fputws(str, file); }
    static i32 print(FILE* const file, const wchar_t c)    noexcept { return fputwc(c, file);   }
    
    static i32 printW(FILE* const file, const char* str)    noexcept { return fwprintf(file, L"%hs", str);  }
    static i32 printW(FILE* const file, const char c)       noexcept { return fputwc(c, file);   }
    static i32 printW(FILE* const file, const wchar_t* str) noexcept { return fputws(str, file); }
    static i32 printW(FILE* const file, const wchar_t c)    noexcept { return fputwc(c, file);   }

    static i32 print(const i8  d) noexcept { return fprintf(stdout, "%d", d); }
    static i32 print(const i16 d) noexcept { return fprintf(stdout, "%d", d); }
    static i32 print(const i32 d) noexcept { return fprintf(stdout, "%d", d); }
    static i32 print(const i64 d) noexcept { return fprintf(stdout, "%lld", d); }

    static i32 printW(const i8  d) noexcept { return fwprintf(stdout, L"%d", d); }
    static i32 printW(const i16 d) noexcept { return fwprintf(stdout, L"%d", d); }
    static i32 printW(const i32 d) noexcept { return fwprintf(stdout, L"%d", d); }
    static i32 printW(const i64 d) noexcept { return fwprintf(stdout, L"%lld", d); }

    static i32 print(const u8  d) noexcept { return fprintf(stdout, "%u", d); }
    static i32 print(const u16 d) noexcept { return fprintf(stdout, "%u", d); }
    static i32 print(const u32 d) noexcept { return fprintf(stdout, "%u", d); }
    static i32 print(const u64 d) noexcept { return fprintf(stdout, "%llu", d); }

    static i32 printW(const u8  d) noexcept { return fwprintf(stdout, L"%u", d); }
    static i32 printW(const u16 d) noexcept { return fwprintf(stdout, L"%u", d); }
    static i32 printW(const u32 d) noexcept { return fwprintf(stdout, L"%u", d); }
    static i32 printW(const u64 d) noexcept { return fwprintf(stdout, L"%llu", d); }

    static i32 print(const f32 f) noexcept { return fprintf(stdout, "%f", f); }
    static i32 print(const f64 f) noexcept { return fprintf(stdout, "%f", f); }

    static i32 printW(const f32 f) noexcept { return fwprintf(stdout, L"%f", f); }
    static i32 printW(const f64 f) noexcept { return fwprintf(stdout, L"%f", f); }

    static i32 print(FILE* const file, const i8  d) noexcept { return fprintf(file, "%d", d); }
    static i32 print(FILE* const file, const i16 d) noexcept { return fprintf(file, "%d", d); }
    static i32 print(FILE* const file, const i32 d) noexcept { return fprintf(file, "%d", d); }
    static i32 print(FILE* const file, const i64 d) noexcept { return fprintf(file, "%lld", d); }

    static i32 printW(FILE* const file, const i8  d) noexcept { return fwprintf(file, L"%d", d); }
    static i32 printW(FILE* const file, const i16 d) noexcept { return fwprintf(file, L"%d", d); }
    static i32 printW(FILE* const file, const i32 d) noexcept { return fwprintf(file, L"%d", d); }
    static i32 printW(FILE* const file, const i64 d) noexcept { return fwprintf(file, L"%lld", d); }

    static i32 print(FILE* const file, const u8  d) noexcept { return fprintf(file, "%u", d); }
    static i32 print(FILE* const file, const u16 d) noexcept { return fprintf(file, "%u", d); }
    static i32 print(FILE* const file, const u32 d) noexcept { return fprintf(file, "%u", d); }
    static i32 print(FILE* const file, const u64 d) noexcept { return fprintf(file, "%llu", d); }

    static i32 printW(FILE* const file, const u8  d) noexcept { return fwprintf(file, L"%u", d); }
    static i32 printW(FILE* const file, const u16 d) noexcept { return fwprintf(file, L"%u", d); }
    static i32 printW(FILE* const file, const u32 d) noexcept { return fwprintf(file, L"%u", d); }
    static i32 printW(FILE* const file, const u64 d) noexcept { return fwprintf(file, L"%llu", d); }

    static i32 print(FILE* const file, const f32 f) noexcept { return fprintf(file, "%f", f); }
    static i32 print(FILE* const file, const f64 f) noexcept { return fprintf(file, "%f", f); }

    static i32 printW(FILE* const file, const f32 f) noexcept { return fwprintf(file, L"%f", f); }
    static i32 printW(FILE* const file, const f64 f) noexcept { return fwprintf(file, L"%f", f); }

    template<typename _T>
    static i32 print(const _T* const p) noexcept { return fprintf(stdout, "%p", p); }
    template<typename _T>
    static i32 printW(const _T* const p) noexcept { return fwprintf(stdout, L"%p", p); }

    template<typename _T>
    static i32 print(FILE* const file, const _T* const p) noexcept { return fprintf(file, "%p", p); }
    template<typename _T>
    static i32 printW(FILE* const file, const _T* const p) noexcept { return fwprintf(file, L"%p", p); }

    static i32 print(const ConstExprString& str) noexcept { return fputs(str.c_str(), stdout); }
    static i32 print(const DynString& str) noexcept { return fputs(str.c_str(), stdout); }
    static i32 print(const DynStringView& str) noexcept { return fprintf(stdout, "%.*s", static_cast<int>(str.length()), str.c_str()); }

    static i32 print(const WConstExprString& str) noexcept { return fputws(str.c_str(), stdout); }
    static i32 print(const WDynString& str) noexcept { return fputws(str.c_str(), stdout); }
    static i32 print(const WDynStringView& str) noexcept { return fwprintf(stdout, L"%.*s", static_cast<int>(str.length()), str.c_str()); }

    static i32 printW(const WConstExprString& str) noexcept { return fputws(str.c_str(), stdout); }
    static i32 printW(const WDynString& str) noexcept { return fputws(str.c_str(), stdout); }
    static i32 printW(const WDynStringView& str) noexcept { return fwprintf(stdout, L"%.*s", static_cast<int>(str.length()), str.c_str()); }

    static i32 print(FILE* const file, const ConstExprString& str) noexcept { return fputs(str.c_str(), file); }
    static i32 print(FILE* const file, const DynString& str) noexcept { return fputs(str.c_str(), file); }
    static i32 print(FILE* const file, const DynStringView& str) noexcept { return fprintf(file, "%.*s", static_cast<int>(str.length()), str.c_str()); }

    static i32 print(FILE* const file, const WConstExprString& str) noexcept { return fputws(str.c_str(), file); }
    static i32 print(FILE* const file, const WDynString& str) noexcept { return fputws(str.c_str(), file); }
    static i32 print(FILE* const file, const WDynStringView& str) noexcept { return fwprintf(file, L"%.*s", static_cast<int>(str.length()), str.c_str()); }

    static i32 printW(FILE* const file, const WConstExprString& str) noexcept { return fputws(str.c_str(), file); }
    static i32 printW(FILE* const file, const WDynString& str) noexcept { return fputws(str.c_str(), file); }
    static i32 printW(FILE* const file, const WDynStringView& str) noexcept { return fwprintf(file, L"%.*s", static_cast<int>(str.length()), str.c_str()); }

    template<typename _T>
    static i32 print(const _T& t) noexcept { return TPrinter<_T>::print(t); }
    template<typename _T>
    static i32 printW(const _T& t) noexcept { return TPrinter<_T>::printW(t); }

    template<typename _T>
    static i32 print(FILE* const file, const _T& t) noexcept { return TPrinter<_T>::print(file, t); }
    template<typename _T>
    static i32 printW(FILE* const file, const _T& t) noexcept { return TPrinter<_T>::printW(file, t); }

    template<typename _CurrArg, typename... _Args>
    static i32 print(const char* fmt, _CurrArg currArg, _Args... args) noexcept
    {
        i32 count = 0;
        for(uSys i = 0; fmt[i]; ++i)
        {
            if(fmt[i] == '%')
            {
                count += print(currArg);
                count += print(fmt + i + 1, args...);
                break;
            }
            else if(fmt[i] == '\'')
            {
                if(fmt[i + 1] == '\'')
                {
                    fputc('\'', stdout);
                    ++count;
                    ++i;
                }
                else if(fmt[i + 1] == '%')
                {
                    if(fmt[i + 2] == '\'')
                    {
                        fputc('%', stdout);
                        ++count;
                        i += 2;
                    }
                }
                else
                {
                    return count;
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
    static i32 printW(const wchar_t* fmt, _CurrArg currArg, _Args... args) noexcept
    {
        i32 count = 0;
        for(uSys i = 0; fmt[i]; ++i)
        {
            if(fmt[i] == L'%')
            {
                count += printW(currArg);
                count += printW(fmt + i + 1, args...);
                break;
            }
            else if(fmt[i] == L'\'')
            {
                if(fmt[i + 1] == L'\'')
                {
                    fputwc(L'\'', stdout);
                    ++count;
                    ++i;
                }
                else if(fmt[i + 1] == L'%')
                {
                    if(fmt[i + 2] == L'\'')
                    {
                        fputwc(L'%', stdout);
                        ++count;
                        i += 2;
                    }
                }
                else
                {
                    return count;
                }
            }
            else
            {
                fputwc(fmt[i], stdout);
                ++count;
            }
        }
        return count;
    }
    
    template<typename _CurrArg, typename... _Args>
    static i32 print(const wchar_t* fmt, _CurrArg currArg, _Args... args) noexcept
    { return printW(fmt, currArg, args...); }

    template<typename _CurrArg, typename... _Args>
    static i32 print(FILE* const file, const char* fmt, _CurrArg currArg, _Args... args) noexcept
    {
        i32 count = 0;
        for(uSys i = 0; fmt[i]; ++i)
        {
            if(fmt[i] == '%')
            {
                count += print(file, currArg);
                count += print(file, fmt + i + 1, args...);
                break;
            }
            else if(fmt[i] == '\'')
            {
                if(fmt[i + 1] == '\'')
                {
                    fputc('\'', file);
                    ++count;
                    ++i;
                }
                else if(fmt[i + 1] == '%')
                {
                    if(fmt[i + 2] == '\'')
                    {
                        fputc('%', file);
                        ++count;
                        i += 2;
                    }
                }
                else
                {
                    return count;
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

    template<typename _CurrArg, typename... _Args>
    static i32 printW(FILE* const file, const wchar_t* fmt, _CurrArg currArg, _Args... args) noexcept
    {
        i32 count = 0;
        for(uSys i = 0; fmt[i]; ++i)
        {
            if(fmt[i] == L'%')
            {
                count += printW(file, currArg);
                count += printW(file, fmt + i + 1, args...);
                break;
            }
            else if(fmt[i] == L'\'')
            {
                if(fmt[i + 1] == L'\'')
                {
                    fputwc(L'\'', stdout);
                    ++count;
                    ++i;
                }
                else if(fmt[i + 1] == L'%')
                {
                    if(fmt[i + 2] == L'\'')
                    {
                        fputwc(L'%', stdout);
                        ++count;
                        i += 2;
                    }
                }
                else
                {
                    return count;
                }
            }
            else
            {
                fputwc(fmt[i], file);
                ++count;
            }
        }
        return count;
    }
    
    template<typename _CurrArg, typename... _Args>
    static i32 print(FILE* const file, const wchar_t* fmt, _CurrArg currArg, _Args... args) noexcept
    { return printW(file, fmt, currArg, args...); }
};
