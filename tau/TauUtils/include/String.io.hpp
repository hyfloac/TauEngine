#pragma once
// ReSharper disable CppClangTidyClangDiagnosticUnusedFunction

#include "String.hpp"
#pragma warning(push, 0)
#include <iostream>
#pragma warning(pop)

namespace fmtStr {
inline ::std::ostream& writeFormattedLen(::std::ostream& os, const char* str, ::std::streamsize len) noexcept;
inline ::std::wostream& writeFormattedLen(::std::wostream& os, const wchar_t* str, ::std::streamsize len) noexcept;
}

inline ::std::ostream& operator <<(::std::ostream& os, const ConstExprString& string) noexcept
{
    os << string.c_str();
    return os;
}

inline ::std::ostream& operator <<(::std::ostream& os, const String& string) noexcept
{
    os << string.c_str();
    return os;
}

inline ::std::ostream& operator <<(::std::ostream& os, const StringView& string) noexcept
{
    fmtStr::writeFormattedLen(os, string.c_str(), string.length());
    return os;
}

inline ::std::ostream& operator <<(::std::ostream& os, const DynString& string) noexcept
{
    os << string.c_str();
    return os;
}

inline ::std::ostream& operator <<(::std::ostream& os, const DynStringView& string) noexcept
{
    fmtStr::writeFormattedLen(os, string.c_str(), string.length());
    return os;
}

inline ::std::ostream& operator <<(::std::ostream& os, const StringBuilder& string) noexcept
{
    os << string.c_str();
    return os;
}

inline ::std::wostream& operator <<(::std::wostream& os, const WConstExprString& string) noexcept
{
    os << string.c_str();
    return os;
}

inline ::std::wostream& operator <<(::std::wostream& os, const WString& string) noexcept
{
    os << string.c_str();
    return os;
}

inline ::std::wostream& operator <<(::std::wostream& os, const WStringView& string) noexcept
{
    fmtStr::writeFormattedLen(os, string.c_str(), string.length());
    return os;
}

inline ::std::wostream& operator <<(::std::wostream& os, const WDynString& string) noexcept
{
    os << string.c_str();
    return os;
}

inline ::std::wostream& operator <<(::std::wostream& os, const WDynStringView& string) noexcept
{
    fmtStr::writeFormattedLen(os, string.c_str(), string.length());
    return os;
}

inline ::std::wostream& operator <<(::std::wostream& os, const WStringBuilder& string) noexcept
{
    os << string.c_str();
    return os;
}

inline ::std::ostream& fmtStr::writeFormattedLen(::std::ostream& os, const char* const str, const ::std::streamsize len) noexcept
{
    using _Traits = ::std::char_traits<char>;

    ::std::ios_base::iostate state = ::std::ios_base::goodbit;
    ::std::streamsize pad = os.width() <= 0 || os.width() <= len ? 0 : os.width() - len;

    const ::std::ostream::sentry ok(os);
    if(!ok)
    {
        state |= ::std::ios_base::badbit;
    }
    else
    {
        if((os.flags() & ::std::ios_base::adjustfield) != ::std::ios_base::left)
        {
            for(; 0 < pad; --pad) // pad on left
            { 
                if(_Traits::eq_int_type(_Traits::eof(), os.rdbuf()->sputc(os.fill()))) 
                {
                    state |= ::std::ios_base::badbit; // insertion failed, quit
                    break;
                }
            }
        }

        if(state == ::std::ios_base::goodbit && os.rdbuf()->sputn(str, len) != len)
        {
            state |= ::std::ios_base::badbit;
        }

        if(state == ::std::ios_base::goodbit) 
        {
            for(; 0 < pad; --pad) // pad on right
            {
                if(_Traits::eq_int_type(_Traits::eof(), os.rdbuf()->sputc(os.fill()))) 
                {
                    state |= ::std::ios_base::badbit; // insertion failed, quit
                    break;
                }
            }
        }

        os.width(0);
    }

    os.setstate(state);
    return os;
}

inline ::std::wostream& fmtStr::writeFormattedLen(::std::wostream& os, const wchar_t* const str, const ::std::streamsize len) noexcept
{
    using _Traits = ::std::char_traits<char>;

    ::std::ios_base::iostate state = ::std::ios_base::goodbit;
    ::std::streamsize pad = os.width() <= 0 || os.width() <= len ? 0 : os.width() - len;

    const ::std::wostream::sentry ok(os);
    if(!ok)
    {
        state |= ::std::ios_base::badbit;
    }
    else
    {
        if((os.flags() & ::std::ios_base::adjustfield) != ::std::ios_base::left)
        {
            for(; 0 < pad; --pad) // pad on left
            {
                if(_Traits::eq_int_type(_Traits::eof(), os.rdbuf()->sputc(os.fill())))
                {
                    state |= ::std::ios_base::badbit; // insertion failed, quit
                    break;
                }
            }
        }

        if(state == ::std::ios_base::goodbit && os.rdbuf()->sputn(str, len) != len)
        {
            state |= ::std::ios_base::badbit;
        }

        if(state == ::std::ios_base::goodbit)
        {
            for(; 0 < pad; --pad) // pad on right
            {
                if(_Traits::eq_int_type(_Traits::eof(), os.rdbuf()->sputc(os.fill())))
                {
                    state |= ::std::ios_base::badbit; // insertion failed, quit
                    break;
                }
            }
        }

        os.width(0);
    }

    os.setstate(state);
    return os;
}
