#pragma once
// ReSharper disable CppClangTidyClangDiagnosticUnusedFunction

#pragma warning(push, 0)
#include <cctype>
#include <cstring>
#pragma warning(pop)

#include "NumTypes.hpp"
#include "Safeties.hpp"
#include "Objects.hpp"
#include "TUMaths.hpp"
#include "Utils.hpp"

#define STR_SWITCH(_PARAM, _BLOCK, _DEFAULT_BLOCK) \
{ \
    const auto& _tmp__strSwitch_ = _PARAM; \
    bool _switch_defaulted_ = true; \
    switch(_tmp__strSwitch_.hashCode()) _BLOCK \
    if(_switch_defaulted_) _DEFAULT_BLOCK \
}

#define STR_CASE(_CASE, _BLOCK) \
case ConstExprString(_CASE).hashCode(): \
    if(ConstExprString(_CASE).equals(_tmp__strSwitch_.c_str())) { \
        _switch_defaulted_ = false; \
        _BLOCK \
    } else { break; }

static inline bool equalsIgnoreCase(const char* RESTRICT lhs, const char* RESTRICT rhs) noexcept
{
    while(*lhs != '\0' && *rhs != '\0')
    {
        if(tolower(*lhs) != tolower(*rhs))
        { return false; }
        ++lhs;
        ++rhs;
    }

    return *lhs == '\0' && *rhs == '\0';
}

static inline void toLower(char* str) noexcept
{
    while(*str != '\0')
    {
        *str = static_cast<char>(tolower(*str));
        ++str;
    }
}

static inline void toLower(const char* str, char* store) noexcept
{
    while(*str != '\0')
    {
        *store = static_cast<char>(tolower(*str));
        ++str;
        ++store;
    }
}

static inline void toUpper(char* str) noexcept
{
    while(*str != '\0')
    {
        *str = static_cast<char>(toupper(*str));
        ++str;
    }
}

static inline void toUpper(const char* str, char* store) noexcept
{
    while(*str != '\0')
    {
        *store = static_cast<char>(toupper(*str));
        ++str;
        ++store;
    }
}

static inline uSys findHashCode(NonNull const char* str) noexcept
{
    uSys hash = 0;
    for(uSys i = 0; str[i]; ++i)
    {
        hash = 31u * hash + static_cast<uSys>(str[i]);
    }
    return hash;
}

static inline uSys findHashCode(NonNull const char* str, const uSys len) noexcept
{
    uSys hash = 0;
    for(uSys i = 0; i < len; ++i)
    {
        hash = 31u * hash + static_cast<uSys>(str[i]);
    }
    return hash;
}

namespace cexpr
{
    template<uSys _Len>
    constexpr inline uSys strlen(const char(&str)[_Len]) noexcept { return _Len - 1; }

    template<uSys _Len>
    constexpr inline uSys strlen(const wchar_t(&str)[_Len]) noexcept { return _Len - 1; }

    template<uSys _Len>
    constexpr inline uSys strlen(const char16_t(&str)[_Len]) noexcept { return _Len - 1; }

    template<uSys _Len>
    constexpr inline uSys strlen(const char32_t(&str)[_Len]) noexcept { return _Len - 1; }

    template<typename _Type, uSys _Len>
    constexpr inline uSys arrlen(const _Type(&arr)[_Len]) noexcept { return _Len - 1; }

    template<uSys _Len>
    constexpr bool streq(const char(&RESTRICT lhs)[_Len], const char(&RESTRICT rhs)[_Len])
    {
        while(*lhs != '\0' && *rhs != '\0')
        {
            if(*lhs != *rhs)
            { return false; }
            ++lhs;
            ++rhs;
        }

        return *lhs == '\0' && *rhs == '\0';
    }

    template<std::size_t _Len>
    constexpr bool streqIgnoreCase(const char(&RESTRICT lhs)[_Len], const char(&RESTRICT rhs)[_Len])
    {
        while(*lhs != '\0' && *rhs != '\0')
        {
            if(tolower(*lhs) != tolower(*rhs))
            { return false; }
            ++lhs;
            ++rhs;
        }

        return *lhs == '\0' && *rhs == '\0';
    }

    template<uSys _Len>
    constexpr uSys findHashCode(NonNull const char(&str)[_Len]) noexcept
    {
        uSys hash = 0;
        for(uSys i = 0; str[i]; ++i)
        {
            hash = 31u * hash + static_cast<uSys>(str[i]);
        }
        return hash;
    }
}

class String;
class StringView;
class DynString;
class DynStringView;
class StringBuilder;

class StringIterator final
{
    DEFAULT_DESTRUCT(StringIterator);
    DEFAULT_COPY(StringIterator);
private:
    const char* _string;
    uSys _length;
    uSys _index;
public:
    inline StringIterator(const char* const string, const uSys length, const uSys index)
        : _string(string)
        , _length(length)
        , _index(index)
    { }

    inline StringIterator& operator++() noexcept
    {
        if(_index < _length - 1)
        { ++_index; }
        return *this;
    }

    inline StringIterator& operator--() noexcept
    {
        if(_index > 0)
        { --_index; }
        return *this;
    }

    [[nodiscard]] inline StringIterator operator++(int) noexcept
    {
        const StringIterator copy(*this);
        this->operator++();
        return copy;
    }

    [[nodiscard]] inline StringIterator operator--(int) noexcept
    {
        const StringIterator copy(*this);
        this->operator--();
        return copy;
    }

    [[nodiscard]] inline char operator*() const noexcept
    { return _string[_index]; }

    [[nodiscard]] inline bool operator ==(const StringIterator& other) const noexcept
    { return _index == other._index; }
    [[nodiscard]] inline bool operator !=(const StringIterator& other) const noexcept
    { return _index != other._index; }
};

class ConstExprString final
{
    DEFAULT_DESTRUCT(ConstExprString);
private:
    const char* _string;
    uSys _length;
    uSys _hash;
public:
    template<uSys _Len>
    inline constexpr ConstExprString(const char(&str)[_Len]) noexcept
        : _string(str)
        , _length(_Len - 1)
        , _hash(cexpr::findHashCode(str))
    { }

    inline constexpr ConstExprString(const ConstExprString& copy) noexcept = default;
    inline constexpr ConstExprString(ConstExprString&& move) noexcept = default;
    inline constexpr ConstExprString& operator=(const ConstExprString& copy) noexcept = default;
    inline constexpr ConstExprString& operator=(ConstExprString&& move) noexcept = default;

    template<uSys _Len>
    inline constexpr ConstExprString& operator=(const char(&str)[_Len]) noexcept
    {
        _string = str;
        _length = _Len;
        _hash = cexpr::findHashCode(str);

        return *this;
    }

    [[nodiscard]] inline constexpr const char* c_str() const noexcept { return _string; }
    [[nodiscard]] inline constexpr uSys length() const noexcept { return _length; }
    [[nodiscard]] inline constexpr uSys operator()() const noexcept { return _hash; }
    [[nodiscard]] inline constexpr uSys hashCode() const noexcept { return _hash; }

    template<uSys _Len>
    [[nodiscard]] inline constexpr bool equals(const char(&str)[_Len]) const noexcept
    {
        if(_string == str) { return true; }
        if(_length != _Len) { return false; }
        return ::std::strcmp(this->_string, str) == 0;
    }

    [[nodiscard]] inline bool equals(const char* str) const noexcept
    {
        if(_string == str) { return true; }
        return ::std::strcmp(this->_string, str) == 0;
    }

    [[nodiscard]] inline char operator [](const uSys index) const noexcept
    { return c_str()[index]; }

    [[nodiscard]] inline char at(const uSys index) const noexcept
    {
        if(index >= _length) { return '\0'; }
        return c_str()[index];
    }
};

class String final
{
    DEFAULT_DESTRUCT(String);
    DEFAULT_COPY(String);
private:
    const char* _string;
    uSys _length;
    uSys _hash;
public:
    inline String(const NotNull<const char>& string) noexcept
        : _string(string)
        , _length(::std::strlen(string))
        , _hash(findHashCode(string))
    { }

    inline String(const char* string) noexcept
        : _string(string)
        , _length(::std::strlen(string))
        , _hash(findHashCode(string))
    { Ensure(string != null); }

    template<uSys _Len>
    inline constexpr String(const char(&str)[_Len]) noexcept
        : _string(str)
        , _length(_Len - 1)
        , _hash(cexpr::findHashCode(str))
    { }

    inline String& operator=(const char* str) noexcept
    {
        if(!str)
        { return *this; }

        _string = str;
        _length = ::std::strlen(str);
        _hash = findHashCode(str);

        return *this;
    }

    inline String& operator=(const NotNull<const char>& str) noexcept
    {
        _string = str;
        _length = ::std::strlen(str);
        _hash = findHashCode(str);

        return *this;
    }

    template<uSys _Len>
    inline constexpr String& operator=(const char(&str)[_Len]) noexcept
    {
        _string = str;
        _length = _Len - 1;
        _hash = cexpr::findHashCode(str);

        return *this;
    }

    [[nodiscard]] inline NonNull const char* c_str() const noexcept { return _string; }
    [[nodiscard]] inline uSys length() const noexcept { return _length; }

    [[nodiscard]] inline operator const char*() const noexcept { return _string; }
    [[nodiscard]] inline uSys operator()() const noexcept { return _hash; }

    [[nodiscard]] inline uSys hashCode() const noexcept { return _hash; }

    [[nodiscard]] inline bool equals(const String& other) const noexcept
    {
        if(this == &other || _string == other._string) { return true; }
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return strcmp(this->_string, other._string) == 0;
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const StringView& other) const noexcept;
    [[nodiscard]] inline bool equals(const DynStringView& other) const noexcept;
    [[nodiscard]] inline bool equals(const DynString& other) const noexcept;

    [[nodiscard]] inline bool equals(const char* str) const noexcept
    {
        if(_string == str) { return true; }
        return strcmp(this->_string, str) == 0;
    }

    [[nodiscard]] inline i32 compareTo(const String& other) const noexcept
    {
        if(this == &other || _string == other._string) { return 0; }
        return strcmp(this->_string, other._string);
    }

    [[nodiscard]] inline i32 compareTo(const StringView& other) const noexcept;
    [[nodiscard]] inline i32 compareTo(const DynString& other) const noexcept;
    [[nodiscard]] inline i32 compareTo(const DynStringView& other) const noexcept;

    [[nodiscard]] inline i32 compareTo(const char* str) const noexcept
    {
        if(_string == str) { return 0; }
        return strcmp(this->_string, str);
    }

    [[nodiscard]] inline StringIterator begin() const noexcept { return StringIterator(_string, _length, 0); }
    [[nodiscard]] inline StringIterator   end() const noexcept { return StringIterator(_string, _length, _length - 1); }

#if __has_feature(__cpp_impl_three_way_comparison)
    [[nodiscard]] inline i32 operator <=>(const String& other) const noexcept { return compareTo(other); }
    [[nodiscard]] inline i32 operator <=>(const StringView& other) const noexcept { return compareTo(other); }
    [[nodiscard]] inline i32 operator <=>(const DynString& other) const noexcept { return compareTo(other); }
    [[nodiscard]] inline i32 operator <=>(const DynStringView& other) const noexcept { return compareTo(other); }
    [[nodiscard]] inline i32 operator <=>(const char* str) const noexcept { return compareTo(str); }
#endif

    [[nodiscard]] inline bool operator ==(const String& other) const noexcept { return  equals(other); }
    [[nodiscard]] inline bool operator !=(const String& other) const noexcept { return !equals(other); }
    [[nodiscard]] inline bool operator < (const String& other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] inline bool operator > (const String& other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] inline bool operator <=(const String& other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] inline bool operator >=(const String& other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] inline bool operator ==(const StringView& other) const noexcept { return  equals(other); }
    [[nodiscard]] inline bool operator !=(const StringView& other) const noexcept { return !equals(other); }
    [[nodiscard]] inline bool operator < (const StringView& other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] inline bool operator > (const StringView& other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] inline bool operator <=(const StringView& other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] inline bool operator >=(const StringView& other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] inline bool operator ==(const DynString& other) const noexcept { return  equals(other); }
    [[nodiscard]] inline bool operator !=(const DynString& other) const noexcept { return !equals(other); }
    [[nodiscard]] inline bool operator < (const DynString& other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] inline bool operator > (const DynString& other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] inline bool operator <=(const DynString& other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] inline bool operator >=(const DynString& other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] inline bool operator ==(const DynStringView& other) const noexcept { return  equals(other); }
    [[nodiscard]] inline bool operator !=(const DynStringView& other) const noexcept { return !equals(other); }
    [[nodiscard]] inline bool operator < (const DynStringView& other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] inline bool operator > (const DynStringView& other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] inline bool operator <=(const DynStringView& other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] inline bool operator >=(const DynStringView& other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] inline bool operator ==(const char* str) const noexcept { return  equals(str); }
    [[nodiscard]] inline bool operator !=(const char* str) const noexcept { return !equals(str); }
    [[nodiscard]] inline bool operator < (const char* str) const noexcept { return compareTo(str) <  0; }
    [[nodiscard]] inline bool operator > (const char* str) const noexcept { return compareTo(str) >  0; }
    [[nodiscard]] inline bool operator <=(const char* str) const noexcept { return compareTo(str) <= 0; }
    [[nodiscard]] inline bool operator >=(const char* str) const noexcept { return compareTo(str) >= 0; }

    [[nodiscard]] inline char operator [](const uSys index) const noexcept
    { return c_str()[index]; }

    [[nodiscard]] inline char at(const uSys index) const noexcept
    {
        if(index >= _length) { return '\0'; }
        return c_str()[index];
    }
private:
    friend class StringView;
    friend class DynString;
    friend class DynStringView;
    friend class StringBuilder;
};

class StringView final
{
    DEFAULT_DESTRUCT(StringView);
    DEFAULT_COPY(StringView);
private:
    const char* _string;
    uSys _length;
    uSys _hash;
private:
    inline StringView(const uSys begin, const uSys length, const String& str) noexcept
        : _string(str._string + begin)
        , _length(length)
        , _hash(findHashCode(_string, length))
    { }
public:
    inline StringView(const String& str, const uSys begin, const uSys end) noexcept
        : _string(str._string + begin)
        , _length(end - begin)
        , _hash(findHashCode(_string, _length))
    { }

    [[nodiscard]] static inline StringView create(const String & str, const uSys begin, const uSys length) noexcept
    { return StringView(begin, length, str); }

    inline StringView& reset(const String& str, const uSys begin, const uSys end) noexcept
    {
        if(end > str._length)
        { return *this; }

        _string = str._string + begin;
        _length = end - begin;
        _hash = findHashCode(_string, _length);

        return *this;
    }

    inline StringView& resetLen(const String& str, const uSys begin, const uSys length) noexcept
    {
        if(begin + length > str._length)
        { return *this; }

        _string = str._string + begin;
        _length = length;
        _hash = findHashCode(_string, _length);

        return *this;
    }

    [[nodiscard]] inline const char* c_str() const noexcept { return _string; }
    [[nodiscard]] inline uSys length() const noexcept { return _length; }
    [[nodiscard]] inline uSys hashCode() const noexcept { return _hash; }

    [[nodiscard]] inline StringIterator begin() const noexcept { return StringIterator(_string, _length, 0); }
    [[nodiscard]] inline StringIterator   end() const noexcept { return StringIterator(_string, _length, _length - 1); }

    [[nodiscard]] inline bool equals(const String& other) const noexcept
    {
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return ::std::strncmp(this->_string, other._string, this->_length) == 0;
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const StringView& other) const noexcept
    {
        if(this == &other) { return true; }
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return ::std::strncmp(this->_string, other._string, this->_length) == 0;
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const DynString& other) const noexcept;
    [[nodiscard]] inline bool equals(const DynStringView& other) const noexcept;

    [[nodiscard]] inline bool equals(const char* str) const noexcept
    {
        return ::std::strncmp(this->_string, str, this->_length) == 0;
    }

    [[nodiscard]] inline i32 compareTo(const StringView& other) const noexcept
    {
        if(this == &other) { return 0; }
        return ::std::strncmp(this->_string, other._string, minT(this->_length, other._length));
    }

    [[nodiscard]] inline i32 compareTo(const String& other) const noexcept
    {
        return ::std::strncmp(this->_string, other._string, minT(this->_length, other._length));
    }

    [[nodiscard]] inline i32 compareTo(const DynStringView& other) const noexcept;
    [[nodiscard]] inline i32 compareTo(const DynString& other) const noexcept;
    [[nodiscard]] inline i32 compareTo(const char* const str) const noexcept
    {
        return ::std::strncmp(this->_string, str, this->_length);
    }

#if __has_feature(__cpp_impl_three_way_comparison)
    [[nodiscard]] inline i32 operator <=>(const String& other) const noexcept { return compareTo(other); }
    [[nodiscard]] inline i32 operator <=>(const StringView& other) const noexcept { return compareTo(other); }
    [[nodiscard]] inline i32 operator <=>(const DynString& other) const noexcept { return compareTo(other); }
    [[nodiscard]] inline i32 operator <=>(const DynStringView& other) const noexcept { return compareTo(other); }
    [[nodiscard]] inline i32 operator <=>(const char* other) const noexcept { return compareTo(other); }
#endif

    [[nodiscard]] inline bool operator ==(const String& str) const noexcept { return  equals(str); }
    [[nodiscard]] inline bool operator !=(const String& str) const noexcept { return !equals(str); }
    [[nodiscard]] inline bool operator < (const String& str) const noexcept { return compareTo(str) <  0; }
    [[nodiscard]] inline bool operator > (const String& str) const noexcept { return compareTo(str) >  0; }
    [[nodiscard]] inline bool operator <=(const String& str) const noexcept { return compareTo(str) <= 0; }
    [[nodiscard]] inline bool operator >=(const String& str) const noexcept { return compareTo(str) >= 0; }

    [[nodiscard]] inline bool operator ==(const StringView& str) const noexcept { return  equals(str); }
    [[nodiscard]] inline bool operator !=(const StringView& str) const noexcept { return !equals(str); }
    [[nodiscard]] inline bool operator < (const StringView& str) const noexcept { return compareTo(str) <  0; }
    [[nodiscard]] inline bool operator > (const StringView& str) const noexcept { return compareTo(str) >  0; }
    [[nodiscard]] inline bool operator <=(const StringView& str) const noexcept { return compareTo(str) <= 0; }
    [[nodiscard]] inline bool operator >=(const StringView& str) const noexcept { return compareTo(str) >= 0; }

    [[nodiscard]] inline bool operator ==(const DynString& str) const noexcept { return  equals(str); }
    [[nodiscard]] inline bool operator !=(const DynString& str) const noexcept { return !equals(str); }
    [[nodiscard]] inline bool operator < (const DynString& str) const noexcept { return compareTo(str) <  0; }
    [[nodiscard]] inline bool operator > (const DynString& str) const noexcept { return compareTo(str) >  0; }
    [[nodiscard]] inline bool operator <=(const DynString& str) const noexcept { return compareTo(str) <= 0; }
    [[nodiscard]] inline bool operator >=(const DynString& str) const noexcept { return compareTo(str) >= 0; }

    [[nodiscard]] inline bool operator ==(const DynStringView& str) const noexcept { return  equals(str); }
    [[nodiscard]] inline bool operator !=(const DynStringView& str) const noexcept { return !equals(str); }
    [[nodiscard]] inline bool operator < (const DynStringView& str) const noexcept { return compareTo(str) <  0; }
    [[nodiscard]] inline bool operator > (const DynStringView& str) const noexcept { return compareTo(str) >  0; }
    [[nodiscard]] inline bool operator <=(const DynStringView& str) const noexcept { return compareTo(str) <= 0; }
    [[nodiscard]] inline bool operator >=(const DynStringView& str) const noexcept { return compareTo(str) >= 0; }

    [[nodiscard]] inline bool operator ==(const char* str) const noexcept { return  equals(str); }
    [[nodiscard]] inline bool operator !=(const char* str) const noexcept { return !equals(str); }
    [[nodiscard]] inline bool operator < (const char* str) const noexcept { return compareTo(str) <  0; }
    [[nodiscard]] inline bool operator > (const char* str) const noexcept { return compareTo(str) >  0; }
    [[nodiscard]] inline bool operator <=(const char* str) const noexcept { return compareTo(str) <= 0; }
    [[nodiscard]] inline bool operator >=(const char* str) const noexcept { return compareTo(str) >= 0; }

    [[nodiscard]] inline char operator [](const uSys index) const noexcept
    { return c_str()[index]; }

    [[nodiscard]] inline char at(const uSys index) const noexcept
    {
        if(index >= _length) { return '\0'; }
        return c_str()[index];
    }
private:
    friend class String;
    friend class DynString;
    friend class DynStringView;
    friend class StringBuilder;
};

class DynString final
{
public:
    static DynString passControl(const char* str) noexcept
    {
        const uSys len = ::std::strlen(str);
        if(len < 16)
        {
            DynString tmp(str);
            delete str;
            return tmp;
        }
        return DynString(str, len);
    }
private:
    union
    {
        struct
        {
            const char* string;
            uSys* refCount;
        } _largeString;
        char _stackString[16];
    };
    uSys _length;
    uSys _hash;
private:
    DynString(const char* const string, const uSys length) noexcept
        : _largeString{ string, new(::std::nothrow) uSys(1) }
        , _length(length)
        , _hash(findHashCode(string))
    { }

    DynString(const uSys length) noexcept
        : _stackString{}
        , _length(length)
        , _hash(0)
    { }
    
    DynString(const uSys length, const char* const string) noexcept
        : _largeString{ null, null }
        , _length(length)
        , _hash(0)
    {
        ::std::memcpy(_stackString, string, length);
        _stackString[length] = '\0';
        _hash = findHashCode(_stackString);
    }
public:
    inline DynString() noexcept
        : _largeString{ null, null}
        , _length(0)
        , _hash(0)
    { }

    inline DynString(const NotNull<const char>& string) noexcept
        : _largeString{ null, null }
        , _length(::std::strlen(string))
        , _hash(findHashCode(string))
    {
        if(_length >= 16)
        {
            _largeString.refCount = new(::std::nothrow) uSys(1);
            char* str = new(::std::nothrow) char[_length + 1];
            ::std::memcpy(str, string, _length + 1);
            _largeString.string = str;
        }
        else
        { ::std::memcpy(_stackString, string, _length + 1); }
    }

    inline DynString(const char* const string) noexcept
        : _largeString{ null, null }
        , _length(::std::strlen(string))
        , _hash(findHashCode(string))
    {
        Ensure(string != null);
        if(_length >= 16)
        {
            _largeString.refCount = new(::std::nothrow) uSys(1);
            char* str = new(::std::nothrow) char[_length + 1];
            ::std::memcpy(str, string, _length + 1);
            _largeString.string = str;
        }
        else
        { ::std::memcpy(_stackString, string, _length + 1); }
    }

    /**
     * Enabled only if _Len is less than 16.
     *
     *   This is because new cannot (assuming a well formed
     * compiler) be called from a constexpr function.
     */
    template<uSys _Len, ::std::enable_if_t<_Len <= 16, int> = 0>
    inline constexpr DynString(const char(&str)[_Len]) noexcept
        : _largeString{ null, null }
        , _length(_Len - 1)
        , _hash(cexpr::findHashCode(str))
    { ::std::memcpy(_stackString, str, _Len); }

    inline ~DynString() noexcept
    {
        if(_length >= 16 && _largeString.string && --(*_largeString.refCount) == 0)
        {
            delete[] _largeString.string;
            delete _largeString.refCount;
        }
    }

    inline DynString(const DynString& copy) noexcept
        : _length(copy._length)
        , _hash(copy._hash)
    {
        if(_length >= 16)
        {
            _largeString.string = copy._largeString.string;
            _largeString.refCount = copy._largeString.refCount;
            ++(*_largeString.refCount);
        }
        else
        { ::std::memcpy(_stackString, copy._stackString, 16); }
    }

    inline DynString(DynString&& move) noexcept
        : _length(move._length)
        , _hash(move._hash)
    {
        if(_length >= 16)
        {
            _largeString.string = move._largeString.string;
            _largeString.refCount = move._largeString.refCount;
            move._largeString.string = null;
        }
        else
        { ::std::memcpy(_stackString, move._stackString, 16); }
    }

    inline DynString& operator=(const DynString& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(copy._length >= 16)
        {
            if(_length >= 16 && --(*_largeString.refCount) == 0)
            {
                delete[] _largeString.string;
                delete _largeString.refCount;
            }

            _largeString.string = copy._largeString.string;
            _largeString.refCount = copy._largeString.refCount;
            ++(*_largeString.refCount);
        }
        else
        { ::std::memcpy(_stackString, copy._stackString, 16); }

        _length = copy._length;
        _hash = copy._hash;

        return *this;
    }

    inline DynString& operator=(DynString&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(move._length >= 16)
        {
            if(_length >= 16 && --(*_largeString.refCount) == 0)
            {
                delete[] _largeString.string;
                delete _largeString.refCount;
            }

            _largeString.string = move._largeString.string;
            _largeString.refCount = move._largeString.refCount;
            move._largeString.string = null;
        }
        else
        { ::std::memcpy(_stackString, move._stackString, 16); }

        _length = move._length;
        _hash = move._hash;

        return *this;
    }

    inline DynString& operator=(const NotNull<const char>& string) noexcept
    {
        /**
         * _largeString.string and _stackString store the same address.
         */
        if(string == _largeString.string)
        { return *this; }

        const uSys length = ::std::strlen(string);

        if(_length >= 16 && length >= 16 && *_largeString.refCount == 1)
        {
            delete[] _largeString.string;
            _length = length;

            char* str = new(::std::nothrow) char[_length + 1];
            ::std::memcpy(str, string, _length + 1);
            _largeString.string = str;
        }
        else
        {
            if(_length >= 16 && --(*_largeString.refCount) == 0)
            {
                delete[] _largeString.string;
                delete _largeString.refCount;
            }

            _length = length;
            if(_length >= 16)
            {
                _largeString.refCount = new(::std::nothrow) uSys(1);
                char* str = new(::std::nothrow) char[_length + 1];
                ::std::memcpy(str, string, _length + 1);
                _largeString.string = str;
            }
            else
            { ::std::memcpy(_stackString, string, _length + 1); }
        }

        return *this;
    }

    inline DynString& operator=(const char* const string) noexcept
    {
        if(!string)
        { return *this; }

        /**
         * _largeString.string and _stackString store the same address.
         */
        if(string == _largeString.string)
        { return *this; }

        const uSys length = ::std::strlen(string);

        if(_length >= 16 && length >= 16 && *_largeString.refCount == 1)
        {
            delete[] _largeString.string;
            _length = length;

            char* str = new(::std::nothrow) char[_length + 1];
            ::std::memcpy(str, string, _length + 1);
            _largeString.string = str;
        }
        else
        {
            if(_length >= 16 && --(*_largeString.refCount) == 0)
            {
                delete[] _largeString.string;
                delete _largeString.refCount;
            }

            _length = length;
            if(_length >= 16)
            {
                _largeString.refCount = new(::std::nothrow) uSys(1);
                char* str = new(::std::nothrow) char[_length + 1];
                ::std::memcpy(str, string, _length + 1);
                _largeString.string = str;
            }
            else
            { ::std::memcpy(_stackString, string, _length + 1); }
        }

        return *this;
    }

    [[nodiscard]] inline NonNull const char* c_str() const noexcept { return _length >= 16 ? _largeString.string : _stackString; }
    [[nodiscard]] inline uSys length() const noexcept { return _length; }

    inline operator const char*() const noexcept { return _length >= 16 ? _largeString.string : _stackString; }
    [[nodiscard]] inline uSys operator()() const noexcept { return _hash; }

    [[nodiscard]] inline uSys hashCode() const noexcept { return _hash; }

    [[nodiscard]] inline bool equals(const String& other) const noexcept
    {
        if(this->_length == other._length && this->_hash == other._hash)
        {
            if(this->_length >= 16)
            {
                if(this->_largeString.string == other._string) { return true; }
                return ::std::strcmp(this->_largeString.string, other._string) == 0;
            }
            else
            { return ::std::strcmp(this->_stackString, other._string) == 0; }
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const StringView& other) const noexcept
    {
        if(this->_length == other._length && this->_hash == other._hash)
        {
            if(this->_length >= 16)
            { return ::std::strncmp(this->_largeString.string, other._string, other._length) == 0; }
            else
            { return ::std::strncmp(this->_stackString, other._string, other._length) == 0; }
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const DynString& other) const noexcept
    {
        if(this == &other) { return true; }
        if(this->_length == other._length && this->_hash == other._hash)
        {
            if(this->_length >= 16)
            {
                if(this->_largeString.string == other._largeString.string)
                { return true; }
                return ::std::strcmp(this->_largeString.string, other._largeString.string) == 0;
            }
            else
            { return ::std::strcmp(this->_stackString, other._stackString) == 0; }
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const DynStringView& other) const noexcept;

    [[nodiscard]] inline bool equals(const char* const str) const noexcept
    {
        if(this->_length >= 16)
        {
            if(this->_largeString.string == str) { return true; }
            return ::std::strcmp(this->_largeString.string, str) == 0;
        }
        else
        {
            if(this->_stackString == str) { return true; }
            return ::std::strcmp(this->_stackString, str) == 0;
        }
    }

    [[nodiscard]] inline i32 compareTo(const String& other) const noexcept
    {
        if(this->_length >= 16)
        {
            if(this->_largeString.string == other._string) { return true; }
            return ::std::strcmp(this->_largeString.string, other._string);
        }
        else
        {
            if(this->_stackString == other._string) { return true; }
            return ::std::strcmp(this->_stackString, other._string);
        }
    }

    [[nodiscard]] inline i32 compareTo(const StringView& other) const noexcept
    {
        if(this->_length >= 16)
        {
            if(this->_largeString.string == other._string) { return true; }
            return ::std::strncmp(this->_largeString.string, other._string, other._length);
        }
        else
        {
            if(this->_stackString == other._string) { return true; }
            return ::std::strncmp(this->_stackString, other._string, other._length);
        }
    }

    [[nodiscard]] inline i32 compareTo(const DynString& other) const noexcept
    {
        if(this->_length >= 16)
        {
            if(this->_largeString.string == other._largeString.string) { return true; }
            return ::std::strcmp(this->_largeString.string, other._largeString.string);
        }
        else
        {
            if(this->_stackString == other._stackString) { return true; }
            return ::std::strcmp(this->_stackString, other._stackString);
        }
    }

    [[nodiscard]] inline i32 compareTo(const DynStringView& other) const noexcept;

    [[nodiscard]] inline i32 compareTo(const char* const str) const noexcept
    {
        if(this->_length >= 16)
        {
            if(this->_largeString.string == str) { return true; }
            return strcmp(this->_largeString.string, str);
        }
        else
        {
            if(this->_stackString == str) { return true; }
            return strcmp(this->_stackString, str);
        }
    }

    [[nodiscard]] inline StringIterator begin() const noexcept { return StringIterator(_length >= 16 ? _largeString.string : _stackString, _length, 0); }
    [[nodiscard]] inline StringIterator   end() const noexcept { return StringIterator(_length >= 16 ? _largeString.string : _stackString, _length, _length - 1); }

#define CONCAT(_OTHER_LEN, _OTHER_STR) \
    const uSys newLen = _length + (_OTHER_LEN);                             \
    if(newLen >= 16) {                                                      \
        char* newStr = new(::std::nothrow) char[newLen + 1];                \
        newStr[newLen] = '\0';                                              \
        ::std::memcpy(newStr, _length >= 16 ? _largeString.string : _stackString, _length); \
        ::std::memcpy(newStr + _length, (_OTHER_STR), (_OTHER_LEN));            \
        return DynString(newStr, newLen);                                   \
    } else {                                                                \
        DynString tmp(newLen);                                              \
        tmp._stackString[newLen] = '\0';                                    \
        ::std::memcpy(tmp._stackString, _stackString, _length);             \
        ::std::memcpy(tmp._stackString + _length, (_OTHER_STR), (_OTHER_LEN));  \
        tmp._hash = findHashCode(tmp._stackString);                         \
        return tmp; \
    }

    [[nodiscard]] inline DynString concat(const String& other) const noexcept
    { CONCAT(other._length, other._string); }

    [[nodiscard]] inline DynString concat(const StringView& other) const noexcept
    { CONCAT(other._length, other._string); }

    [[nodiscard]] inline DynString concat(const DynString& other) const noexcept
    { CONCAT(other._length, other._length >= 16 ? other._largeString.string : other._stackString); }

    [[nodiscard]] inline DynString concat(const DynStringView& other) const noexcept;

    [[nodiscard]] inline DynString concat(const NotNull<const char>& other) const noexcept
    {
        const uSys otherLen = strlen(other);
        CONCAT(otherLen, other);
    }

    [[nodiscard]] inline DynString concat(const char* const other) const noexcept
    {
        const uSys otherLen = strlen(other);
        CONCAT(otherLen, other);
    }

    [[nodiscard]] inline DynString subString([[tau::inclusive]] const uSys begin, [[tau::exclusive]] const uSys end) const noexcept
    {
        if(begin >= end || end > _length)
        { return DynString(""); }

        const uSys length = end - begin;

        if(length >= 16)
        {
            char* sub = new(::std::nothrow) char[length + 1];
            sub[length] = '\0';
            ::std::memcpy(sub, c_str() + begin, length);
            return DynString(sub, length);
        }

        return DynString(length, c_str() + begin);
    }

    [[nodiscard]] inline DynString subStringLen([[tau::inclusive]] const uSys begin, const uSys length) const noexcept
    {
        if(begin + length > _length)
        { return DynString(""); }

        if(length >= 16)
        {
            char* sub = new(::std::nothrow) char[length + 1];
            sub[length] = '\0';
            ::std::memcpy(sub, c_str() + begin, length);
            return DynString(sub, length);
        }

        return DynString(length, c_str() + begin);
    }

    [[nodiscard]] inline DynString subString([[tau::inclusive]] const uSys from) const noexcept
    {
        if(from > _length)
        { return DynString(""); }

        const uSys length = _length - from;

        if(length >= 16)
        {
            char* sub = new(::std::nothrow) char[length + 1];
            sub[length] = '\0';
            ::std::memcpy(sub, c_str() + from, length);
            return DynString(sub, length);
        }

        return DynString(length, c_str() + from);
    }

#if __has_feature(__cpp_impl_three_way_comparison)
    [[nodiscard]] inline i32 operator <=>(const String& other) const noexcept { return compareTo(other); }
    [[nodiscard]] inline i32 operator <=>(const StringView& other) const noexcept { return compareTo(other); }
    [[nodiscard]] inline i32 operator <=>(const DynString& other) const noexcept { return compareTo(other); }
    [[nodiscard]] inline i32 operator <=>(const DynStringView& other) const noexcept { return compareTo(other); }
    [[nodiscard]] inline i32 operator <=>(const char* const other) const noexcept { return compareTo(other); }
#endif

    [[nodiscard]] inline bool operator ==(const String& other) const noexcept { return  equals(other); }
    [[nodiscard]] inline bool operator !=(const String& other) const noexcept { return !equals(other); }
    [[nodiscard]] inline bool operator < (const String& other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] inline bool operator > (const String& other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] inline bool operator <=(const String& other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] inline bool operator >=(const String& other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] inline bool operator ==(const StringView& other) const noexcept { return  equals(other); }
    [[nodiscard]] inline bool operator !=(const StringView& other) const noexcept { return !equals(other); }
    [[nodiscard]] inline bool operator < (const StringView& other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] inline bool operator > (const StringView& other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] inline bool operator <=(const StringView& other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] inline bool operator >=(const StringView& other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] inline bool operator ==(const DynString& other) const noexcept { return  equals(other); }
    [[nodiscard]] inline bool operator !=(const DynString& other) const noexcept { return !equals(other); }
    [[nodiscard]] inline bool operator < (const DynString& other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] inline bool operator > (const DynString& other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] inline bool operator <=(const DynString& other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] inline bool operator >=(const DynString& other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] inline bool operator ==(const DynStringView& other) const noexcept { return  equals(other); }
    [[nodiscard]] inline bool operator !=(const DynStringView& other) const noexcept { return !equals(other); }
    [[nodiscard]] inline bool operator < (const DynStringView& other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] inline bool operator > (const DynStringView& other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] inline bool operator <=(const DynStringView& other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] inline bool operator >=(const DynStringView& other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] inline bool operator ==(const char* const other) const noexcept { return  equals(other); }
    [[nodiscard]] inline bool operator !=(const char* const other) const noexcept { return !equals(other); }
    [[nodiscard]] inline bool operator < (const char* const other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] inline bool operator > (const char* const other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] inline bool operator <=(const char* const other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] inline bool operator >=(const char* const other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] inline DynString operator +(const String&        other) const noexcept { return concat(other); }
    [[nodiscard]] inline DynString operator +(const StringView&    other) const noexcept { return concat(other); }
    [[nodiscard]] inline DynString operator +(const DynString&     other) const noexcept { return concat(other); }
    [[nodiscard]] inline DynString operator +(const DynStringView& other) const noexcept { return concat(other); }
    [[nodiscard]] inline DynString operator +(const char* const    other) const noexcept { return concat(other); }
    [[nodiscard]] inline DynString operator +(const NotNull<const char>& other) const noexcept { return concat(other); }

    [[nodiscard]] inline char operator [](const uSys index) const noexcept
    { return c_str()[index]; }

    [[nodiscard]] inline char at(const uSys index) const noexcept
    {
        if(index >= _length) { return '\0'; }
        return c_str()[index];
    }
private:
    friend class String;
    friend class StringView;
    friend class DynStringView;
    friend class StringBuilder;
};

class DynStringView final
{
private:
    const char* _string;
    uSys* _refCount;
    uSys  _length;
    uSys  _hash;
private:
    inline DynStringView(const uSys begin, const uSys length, const DynString& str) noexcept
        : _string(str.c_str() + begin)
        , _refCount(null)
        , _length(length)
        , _hash(findHashCode(_string, length))
    {
        if(str._length >= 16)
        {
            _refCount = str._largeString.refCount;
            ++(*_refCount);
        }
    }

    inline DynStringView(const uSys begin, const uSys length, const DynStringView& str) noexcept
        : _string(str.c_str() + begin)
        , _refCount(str._refCount)
        , _length(length)
        , _hash(findHashCode(_string, length))
    {
        if(_refCount) { ++(*_refCount); }
    }
public:
    inline DynStringView(const DynString& str, const uSys begin, const uSys end) noexcept
        : _string(str.c_str() + begin)
        , _refCount(null)
        , _length(end - begin)
        , _hash(findHashCode(_string, _length))
    {
        if(str._length >= 16)
        {
            _refCount = str._largeString.refCount;
            ++(*_refCount);
        }
    }

    inline DynStringView(const DynStringView& str, const uSys begin, const uSys end) noexcept
        : _string(str.c_str() + begin)
        , _refCount(str._refCount)
        , _length(end - begin)
        , _hash(findHashCode(_string, _length))
    {
        if(_refCount) { ++(*_refCount); }
    }

    inline DynStringView(DynString&& str, uSys begin, uSys end) noexcept = delete;
    inline DynStringView(DynStringView&& str, uSys begin, uSys end) noexcept = delete;

    inline ~DynStringView() noexcept
    {
        if(_refCount && --(*_refCount) == 0)
        {
            delete[] _string;
            delete _refCount;
        }
    }

    inline DynStringView(const DynStringView& copy) noexcept
        : _string(copy._string)
        , _refCount(copy._refCount)
        , _length(copy._length)
        , _hash(copy._hash)
    {
        if(_refCount) { ++(*_refCount); }
    }

    inline DynStringView(DynStringView&& move) noexcept
        : _string(move._string)
        , _refCount(move._refCount)
        , _length(move._length)
        , _hash(move._hash)
    { move._refCount = null; }

    inline DynStringView& operator=(const DynStringView& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(_refCount && --(*_refCount) == 0)
        {
            delete[] _string;
            delete _refCount;
        }

        _string = copy._string;
        _refCount = copy._refCount;
        _length = copy._length;
        _hash = copy._hash;

        if(_refCount) { ++(*_refCount); }

        return *this;
    }

    inline DynStringView& operator=(DynStringView&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(_refCount && --(*_refCount) == 0)
        {
            delete[] _string;
            delete _refCount;
        }

        _string = move._string;
        _refCount = move._refCount;
        _length = move._length;
        _hash = move._hash;

        move._refCount = null;

        return *this;
    }

    [[nodiscard]] static inline DynStringView create(const DynString& str, const uSys begin, const uSys length) noexcept
    { return DynStringView(begin, length, str); }

    [[nodiscard]] static inline DynStringView create(const DynStringView& str, const uSys begin, const uSys length) noexcept
    { return DynStringView(begin, length, str); }

    [[nodiscard]] static inline DynStringView create(DynString&& str, uSys begin, uSys length) noexcept = delete;
    [[nodiscard]] static inline DynStringView create(DynStringView&& str, uSys begin, uSys length) noexcept = delete;

    DynStringView& reset(const DynString& str, const uSys begin, const uSys end) noexcept
    {
        if(end > str._length)
        { return *this; }

        if(_refCount && --(*_refCount) == 0)
        {
            delete[] _string;
            delete _refCount;
        }

        _string = str.c_str() + begin;
        if(str._length >= 16)
        {
            _refCount = str._largeString.refCount;
            ++(*_refCount);
        }
        else
        {
            _refCount = null;
        }
        _length = end - begin;
        _hash = findHashCode(_string, _length);

        return *this;
    }

    DynStringView& resetLen(const DynString& str, const uSys begin, const uSys length) noexcept
    {
        if(begin + length > str._length)
        { return *this; }

        if(_refCount && --(*_refCount) == 0)
        {
            delete[] _string;
            delete _refCount;
        }

        _string = str.c_str() + begin;
        if(str._length >= 16)
        {
            _refCount = str._largeString.refCount;
            ++(*_refCount);
        }
        else
        {
            _refCount = null;
        }
        _length = length;
        _hash = findHashCode(_string, _length);

        return *this;
    }

    DynStringView& reset(DynString&& str, uSys begin, uSys end) noexcept = delete;
    DynStringView& resetLen(DynString&& str, uSys begin, uSys length) noexcept = delete;

    [[nodiscard]] inline const char* c_str() const noexcept { return _string; }
    [[nodiscard]] inline uSys length() const noexcept { return _length; }
    [[nodiscard]] inline uSys hashCode() const noexcept { return _hash; }

    [[nodiscard]] inline StringIterator begin() const noexcept { return StringIterator(_string, _length, 0); }
    [[nodiscard]] inline StringIterator   end() const noexcept { return StringIterator(_string, _length, _length - 1); }

    [[nodiscard]] inline bool equals(const String& other) const noexcept
    {
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return ::std::strncmp(this->_string, other._string, this->_length) == 0;
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const StringView& other) const noexcept
    {
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return ::std::strncmp(this->_string, other._string, this->_length) == 0;
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const DynString& other) const noexcept
    {
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return ::std::strncmp(this->_string, other.c_str(), this->_length) == 0;
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const DynStringView& other) const noexcept
    {
        if(this == &other) { return true; }
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return ::std::strncmp(this->_string, other._string, this->_length) == 0;
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const char* str) const noexcept
    {
        return ::std::strncmp(this->_string, str, this->_length) == 0;
    }

    [[nodiscard]] inline i32 compareTo(const StringView& other) const noexcept
    {
        return ::std::strncmp(this->_string, other._string, minT(this->_length, other._length));
    }

    [[nodiscard]] inline i32 compareTo(const String& other) const noexcept
    {
        return ::std::strncmp(this->_string, other._string, minT(this->_length, other._length));
    }

    [[nodiscard]] inline i32 compareTo(const DynStringView& other) const noexcept
    {
        if(this == &other) { return 0; }
        return ::std::strncmp(this->_string, other._string, minT(this->_length, other._length));
    }

    [[nodiscard]] inline i32 compareTo(const DynString& other) const noexcept
    {
        return ::std::strncmp(this->_string, other.c_str(), minT(this->_length, other._length));
    }

    [[nodiscard]] inline i32 compareTo(const char* str) const noexcept
    {
        return ::std::strncmp(this->_string, str, this->_length);
    }

#define CONCAT_VIEW(_OTHER_LEN, _OTHER_STR) \
    const uSys newLen = _length + (_OTHER_LEN);                                 \
    if(newLen >= 16) {                                                          \
        char* newStr = new(::std::nothrow) char[newLen + 1];                    \
        newStr[newLen] = '\0';                                                  \
        ::std::memcpy(newStr, _string, _length);                                \
        ::std::memcpy(newStr + _length, (_OTHER_STR), (_OTHER_LEN));            \
        return DynString(newStr, newLen);                                       \
    } else {                                                                    \
        DynString tmp(newLen);                                                  \
        tmp._stackString[newLen] = '\0';                                        \
        ::std::memcpy(tmp._stackString, _string, _length);                      \
        ::std::memcpy(tmp._stackString + _length, (_OTHER_STR), (_OTHER_LEN));  \
        tmp._hash = findHashCode(tmp._stackString);                             \
        return tmp;                                                             \
    }

    [[nodiscard]] inline DynString concat(const String& other) const noexcept
    { CONCAT_VIEW(other._length, other._string); }

    [[nodiscard]] inline DynString concat(const StringView& other) const noexcept
    { CONCAT_VIEW(other._length, other._string); }

    [[nodiscard]] inline DynString concat(const DynString& other) const noexcept
    { CONCAT_VIEW(other._length, other._length >= 16 ? other._largeString.string : other._stackString); }

    [[nodiscard]] inline DynString concat(const DynStringView& other) const noexcept
    { CONCAT_VIEW(other._length, other._string); }

    [[nodiscard]] inline DynString concat(const NotNull<const char>& other) const noexcept
    {
        const uSys otherLen = strlen(other);
        CONCAT_VIEW(otherLen, other);
    }

    [[nodiscard]] inline DynString concat(const char* const other) const noexcept
    {
        const uSys otherLen = strlen(other);
        CONCAT_VIEW(otherLen, other);
    }

    [[nodiscard]] inline DynString subString([[tau::inclusive]] const uSys begin, [[tau::exclusive]] const uSys end) const noexcept
    {
        if(begin >= end || end > _length)
        { return DynString(""); }

        const uSys length = end - begin;

        if(length >= 16)
        {
            char* sub = new(::std::nothrow) char[length + 1];
            sub[length] = '\0';
            ::std::memcpy(sub, c_str() + begin, length);
            return DynString(sub, length);
        }

        return DynString(length, c_str() + begin);
    }

    [[nodiscard]] inline DynString subStringLen([[tau::inclusive]] const uSys begin, const uSys length) const noexcept
    {
        if(begin + length > _length)
        { return DynString(""); }

        if(length >= 16)
        {
            char* sub = new(::std::nothrow) char[length + 1];
            sub[length] = '\0';
            ::std::memcpy(sub, c_str() + begin, length);
            return DynString(sub, length);
        }

        return DynString(length, c_str() + begin);
    }

    [[nodiscard]] inline DynString subString([[tau::inclusive]] const uSys from) const noexcept
    {
        if(from > _length)
        { return DynString(""); }

        const uSys length = _length - from;

        if(length >= 16)
        {
            char* sub = new(::std::nothrow) char[length + 1];
            sub[length] = '\0';
            ::std::memcpy(sub, c_str() + from, length);
            return DynString(sub, length);
        }

        return DynString(length, c_str() + from);
    }

#if __has_feature(__cpp_impl_three_way_comparison)
    [[nodiscard]] inline i32 operator <=>(const String& other) const noexcept { return compareTo(other); }
    [[nodiscard]] inline i32 operator <=>(const StringView& other) const noexcept { return compareTo(other); }
    [[nodiscard]] inline i32 operator <=>(const DynString& other) const noexcept { return compareTo(other); }
    [[nodiscard]] inline i32 operator <=>(const DynStringView& other) const noexcept { return compareTo(other); }
    [[nodiscard]] inline i32 operator <=>(const char* other) const noexcept { return compareTo(other); }
#endif

    [[nodiscard]] inline bool operator ==(const String& str) const noexcept { return  equals(str); }
    [[nodiscard]] inline bool operator !=(const String& str) const noexcept { return !equals(str); }
    [[nodiscard]] inline bool operator < (const String& str) const noexcept { return compareTo(str) < 0; }
    [[nodiscard]] inline bool operator > (const String& str) const noexcept { return compareTo(str) > 0; }
    [[nodiscard]] inline bool operator <=(const String& str) const noexcept { return compareTo(str) <= 0; }
    [[nodiscard]] inline bool operator >=(const String& str) const noexcept { return compareTo(str) >= 0; }

    [[nodiscard]] inline bool operator ==(const StringView& str) const noexcept { return  equals(str); }
    [[nodiscard]] inline bool operator !=(const StringView& str) const noexcept { return !equals(str); }
    [[nodiscard]] inline bool operator < (const StringView& str) const noexcept { return compareTo(str) < 0; }
    [[nodiscard]] inline bool operator > (const StringView& str) const noexcept { return compareTo(str) > 0; }
    [[nodiscard]] inline bool operator <=(const StringView& str) const noexcept { return compareTo(str) <= 0; }
    [[nodiscard]] inline bool operator >=(const StringView& str) const noexcept { return compareTo(str) >= 0; }

    [[nodiscard]] inline bool operator ==(const DynString& str) const noexcept { return  equals(str); }
    [[nodiscard]] inline bool operator !=(const DynString& str) const noexcept { return !equals(str); }
    [[nodiscard]] inline bool operator < (const DynString& str) const noexcept { return compareTo(str) < 0; }
    [[nodiscard]] inline bool operator > (const DynString& str) const noexcept { return compareTo(str) > 0; }
    [[nodiscard]] inline bool operator <=(const DynString& str) const noexcept { return compareTo(str) <= 0; }
    [[nodiscard]] inline bool operator >=(const DynString& str) const noexcept { return compareTo(str) >= 0; }

    [[nodiscard]] inline bool operator ==(const DynStringView& str) const noexcept { return  equals(str); }
    [[nodiscard]] inline bool operator !=(const DynStringView& str) const noexcept { return !equals(str); }
    [[nodiscard]] inline bool operator < (const DynStringView& str) const noexcept { return compareTo(str) < 0; }
    [[nodiscard]] inline bool operator > (const DynStringView& str) const noexcept { return compareTo(str) > 0; }
    [[nodiscard]] inline bool operator <=(const DynStringView& str) const noexcept { return compareTo(str) <= 0; }
    [[nodiscard]] inline bool operator >=(const DynStringView& str) const noexcept { return compareTo(str) >= 0; }

    [[nodiscard]] inline bool operator ==(const char* str) const noexcept { return  equals(str); }
    [[nodiscard]] inline bool operator !=(const char* str) const noexcept { return !equals(str); }
    [[nodiscard]] inline bool operator < (const char* str) const noexcept { return compareTo(str) < 0; }
    [[nodiscard]] inline bool operator > (const char* str) const noexcept { return compareTo(str) > 0; }
    [[nodiscard]] inline bool operator <=(const char* str) const noexcept { return compareTo(str) <= 0; }
    [[nodiscard]] inline bool operator >=(const char* str) const noexcept { return compareTo(str) >= 0; }

    [[nodiscard]] inline char operator [](const uSys index) const noexcept
    { return c_str()[index]; }

    [[nodiscard]] inline char at(const uSys index) const noexcept
    {
        if(index >= _length) { return '\0'; }
        return c_str()[index];
    }
private:
    friend class String;
    friend class StringView;
    friend class DynString;
    friend class StringBuilder;
};

[[nodiscard]] inline DynString DynString::concat(const DynStringView& other) const noexcept
{
    CONCAT(other._length, other._string);
}

class StringBuilder final
{
private:
    char* _string;
    uSys _length;
    uSys _size;
public:
    inline StringBuilder() noexcept
        : _string(new(::std::nothrow) char[64])
        , _length(0)
        , _size(64)
    { }

    inline StringBuilder(const ::std::size_t initialSize) noexcept
        : _string(new(::std::nothrow) char[initialSize])
        , _length(0)
        , _size(initialSize)
    { Ensure(initialSize != 0); }

    inline StringBuilder(const NotNull<const char>& string) noexcept
        : _string(null)
        , _length(::std::strlen(string))
        , _size(_length + 64)
    {
        char* str = new(::std::nothrow) char[_size];
        ::std::memcpy(str, string, _length + 1);
        _string = str;
    }

    inline StringBuilder(const char* const string) noexcept
        : _string(null)
        , _length(::std::strlen(string))
        , _size(_length + 64)
    {
        Ensure(string != null);
        char* str = new(::std::nothrow) char[_size];
        ::std::memcpy(str, string, _length + 1);
        _string = str;
    }

    inline ~StringBuilder() noexcept
    { delete[] _string; }

    inline StringBuilder(const StringBuilder& copy) noexcept
        : _string(new(::std::nothrow) char[copy._size])
        , _length(copy._length)
        , _size(copy._size)
    { ::std::memcpy(_string, copy._string, _size); }

    inline StringBuilder(StringBuilder&& move) noexcept
        : _string(move._string)
        , _length(move._length)
        , _size(move._size)
    { move._string = null; }

    inline StringBuilder& operator =(const StringBuilder& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        delete[] _string;

        _string = new(::std::nothrow) char[copy._size];
        _length = copy._length;
        _size = copy._size;

        ::std::memcpy(_string, copy._string, _size);

        return *this;
    }

    inline StringBuilder& operator =(StringBuilder&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        delete[] _string;

        _string = move._string;
        _length = move._length;
        _size = move._size;

        move._string = null;

        return *this;
    }

    [[nodiscard]] const char* c_str() const noexcept { return _string; }
    [[nodiscard]] uSys length() const noexcept { return _length; }
    [[nodiscard]] uSys size() const noexcept { return _size; }

    [[nodiscard]] inline bool equals(const StringBuilder& other) const noexcept
    {
        if(this == &other || _string == other._string) { return true; }
        if(this->_length == other._length)
        {
            return ::std::strcmp(this->_string, other._string) == 0;
        }
        return false;
    }

    [[nodiscard]] inline i32 compareTo(const StringBuilder& other) const noexcept
    {
        if(this == &other || _string == other._string) { return 0; }
        return ::std::strcmp(this->_string, other._string);
    }

    [[nodiscard]] StringIterator begin() const noexcept { return StringIterator(_string, _length, 0); }
    [[nodiscard]] StringIterator   end() const noexcept { return StringIterator(_string, _length, _length - 1); }

    inline StringBuilder& append(const String& string) noexcept
    {
        append(string._string, string._length);
        return *this;
    }

    inline StringBuilder& append(const StringView& string) noexcept
    {
        append(string._string, string._length);
        return *this;
    }

    inline StringBuilder& append(const DynString& string) noexcept
    {
        append(string.c_str(), string._length);
        return *this;
    }

    inline StringBuilder& append(const DynStringView& string) noexcept
    {
        append(string._string, string._length);
        return *this;
    }

    inline StringBuilder& append(const StringBuilder& string) noexcept
    {
        append(string._string, string._length);
        return *this;
    }

    inline StringBuilder& append(const NotNull<const char>& string) noexcept
    {
        append(string, strlen(string));
        return *this;
    }

    inline StringBuilder& append(const char* string) noexcept
    {
        append(string, strlen(string));
        return *this;
    }

    inline StringBuilder& append(const char c) noexcept
    {
        char string[2] = { c, '\0' };
        append(string, 1);
        return *this;
    }

    inline StringBuilder& reset() noexcept
    {
        _length = 0;
        _string[0] = '\0';
        return *this;
    }

    inline StringBuilder& reset(const uSys newSize) noexcept
    {
        if(newSize == 0) { return *this; }
        delete[] _string;
        _string = new(::std::nothrow) char[newSize];
        _size = newSize;
        _length = 0;
        _string[0] = '\0';
        return *this;
    }

    inline StringBuilder& resetIf(const uSys maxSize, const uSys newSize) noexcept
    {
        if(newSize == 0) { return *this; }
        if(maxSize <= newSize) { return *this; }
        if(_size >= maxSize)
        {
            delete[] _string;
            _string = new(::std::nothrow) char[newSize];
            _size = newSize;
        }
        _length = 0;
        _string[0] = '\0';
        return *this;
    }

    inline StringBuilder& backspace() noexcept
    {
        if(_length > 0)
        {
            --_length;
            _string[_length] = '\0';
        }
        return *this;
    }

    inline StringBuilder& backspace(const uSys count) noexcept
    {
        if(_length > 0)
        {
            const uSys newLen = _length - count;
            if(newLen > _length)
            { return *this; }
            _length = newLen;
            _string[_length] = '\0';
        }
        return *this;
    }

#if __has_feature(__cpp_impl_three_way_comparison)
    [[nodiscard]] inline i32 operator <=> (const StringBuilder& other) const noexcept { return compareTo(other); }
#endif

    [[nodiscard]] inline bool operator ==(const StringBuilder& other) const noexcept { return  equals(other); }
    [[nodiscard]] inline bool operator !=(const StringBuilder& other) const noexcept { return !equals(other); }
    [[nodiscard]] inline bool operator < (const StringBuilder& other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] inline bool operator > (const StringBuilder& other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] inline bool operator <=(const StringBuilder& other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] inline bool operator >=(const StringBuilder& other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] inline StringBuilder& operator +(const String& other) noexcept { return append(other); }
    [[nodiscard]] inline StringBuilder& operator +(const StringView& other) noexcept { return append(other); }
    [[nodiscard]] inline StringBuilder& operator +(const DynString& other) noexcept { return append(other); }
    [[nodiscard]] inline StringBuilder& operator +(const DynStringView& other) noexcept { return append(other); }
    [[nodiscard]] inline StringBuilder& operator +(const StringBuilder& other) noexcept { return append(other); }
    [[nodiscard]] inline StringBuilder& operator +(const char* other) noexcept { return append(other); }
    [[nodiscard]] inline StringBuilder& operator +(const NotNull<const char>& other) noexcept { return append(other); }

    inline StringBuilder& operator +=(const String& other) noexcept { return append(other); }
    inline StringBuilder& operator +=(const StringView& other) noexcept { return append(other); }
    inline StringBuilder& operator +=(const DynString& other) noexcept { return append(other); }
    inline StringBuilder& operator +=(const DynStringView& other) noexcept { return append(other); }
    inline StringBuilder& operator +=(const StringBuilder& other) noexcept { return append(other); }
    inline StringBuilder& operator +=(const char* other) noexcept { return append(other); }
    inline StringBuilder& operator +=(const NotNull<const char>& other) noexcept { return append(other); }

    [[nodiscard]] inline DynString toString() const noexcept { return DynString(_string); }

    [[nodiscard]] inline char operator[](const uSys index) const noexcept
    { return _string[index]; }

    [[nodiscard]] inline char at(const uSys index) const noexcept
    {
        if(index >= _length) { return '\0'; }
        return _string[index];
    }
private:
    void append(const char* const string, const std::size_t length) noexcept
    {
        const uSys newLen = _length + length;
        if(newLen >= _size)
        {
            const uSys newSize = newLen + (newLen >> 1);
            char* newStr = new(::std::nothrow) char[newSize];
            ::std::memcpy(newStr, _string, _length + 1);
            _string = newStr;
            _size = newSize;
        }
        ::std::memcpy(_string + _length, string, length);
        _length += length;
        _string[_length] = '\0';
    }
};

namespace std
{
template<>
struct hash<ConstExprString>
{
    inline uSys operator()(const ConstExprString& str) const noexcept
    { return str.hashCode(); }
};

template<>
struct hash<String>
{
    inline uSys operator()(const String& str) const noexcept
    { return str.hashCode(); }
};

template<>
struct hash<StringView>
{
    inline uSys operator()(const StringView& str) const noexcept
    { return str.hashCode(); }
};

template<>
struct hash<DynString>
{
    inline uSys operator()(const DynString& str) const noexcept
    { return str.hashCode(); }
};

template<>
struct hash<DynStringView>
{
    inline uSys operator()(const DynStringView& str) const noexcept
    { return str.hashCode(); }
};
}

[[nodiscard]] inline bool String::equals(const StringView& other) const noexcept
{
    if(this->_length == other._length && this->_hash == other._hash)
    {
        return ::std::strcmp(this->_string, other._string) == 0;
    }
    return false;
}

[[nodiscard]] inline bool String::equals(const DynString& other) const noexcept
{
    if(_string == other.c_str()) { return true; }
    if(this->_length == other._length && this->_hash == other._hash)
    {
        return ::std::strcmp(this->_string, other.c_str()) == 0;
    }
    return false;
}

[[nodiscard]] inline bool String::equals(const DynStringView& other) const noexcept
{
    if(this->_length == other._length && this->_hash == other._hash)
    {
        return ::std::strcmp(this->_string, other._string) == 0;
    }
    return false;
}

[[nodiscard]] inline i32 String::compareTo(const StringView& other) const noexcept
{
    return ::std::strncmp(this->_string, other._string, minT(this->_length, other._length));
}

[[nodiscard]] inline i32 String::compareTo(const DynString& other) const noexcept
{
    if(_string == other.c_str()) { return 0; }
    return ::std::strcmp(this->_string, other.c_str());
}

[[nodiscard]] inline i32 String::compareTo(const DynStringView& other) const noexcept
{
    return ::std::strncmp(this->_string, other._string, other._length);
}

[[nodiscard]] inline bool StringView::equals(const DynString& other) const noexcept
{
    if(this->_length == other._length && this->_hash == other._hash)
    {
        return ::std::strncmp(this->_string, other.c_str(), this->_length) == 0;
    }
    return false;
}

[[nodiscard]] inline bool StringView::equals(const DynStringView& other) const noexcept
{
    if(this->_length == other._length && this->_hash == other._hash)
    {
        return ::std::strncmp(this->_string, other._string, minT(this->_length, other._length)) == 0;
    }
    return false;
}

[[nodiscard]] inline i32 StringView::compareTo(const DynString& other) const noexcept
{
    return ::std::strncmp(this->_string, other.c_str(), other._length);
}

[[nodiscard]] inline i32 StringView::compareTo(const DynStringView& other) const noexcept
{
    return ::std::strncmp(this->_string, other._string, minT(this->_length, other._length));
}

[[nodiscard]] inline bool DynString::equals(const DynStringView& other) const noexcept
{
    return ::std::strncmp(this->c_str(), other._string, other._length) == 0;
}

[[nodiscard]] inline i32 DynString::compareTo(const DynStringView& other) const noexcept
{
    return ::std::strncmp(this->c_str(), other._string, other._length);
}
