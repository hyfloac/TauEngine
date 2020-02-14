#pragma once

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
        : _string(string), _length(length), _index(index)
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
private:
    const char* _string;
    uSys _hash;
public:
    template<uSys _Len>
    inline constexpr ConstExprString(const char(&str)[_Len]) noexcept
        : _string(str), _hash(cexpr::findHashCode(str))
    { }

    [[nodiscard]] inline constexpr const char* c_str() const noexcept { return _string; }
    [[nodiscard]] inline constexpr uSys hashCode() const noexcept { return _hash; }

    template<uSys _Len>
    [[nodiscard]] inline constexpr bool equals(const char(&str)[_Len]) const noexcept
    {
        if(_string == str) { return true; }
        return strcmp(this->_string, str) == 0;
    }

    [[nodiscard]] inline bool equals(const char* str) const noexcept
    {
        if(_string == str) { return true; }
        return strcmp(this->_string, str) == 0;
    }
};

class String final
{
private:
    const char* _string;
    uSys _length;
    uSys _hash;
public:
    inline String(const NotNull<const char>& string) noexcept
        : _string(string), _length(strlen(string)), _hash(findHashCode(string))
    { }

    inline String(const char* string) noexcept
        : _string(string), _length(strlen(string)), _hash(findHashCode(string))
    { Ensure(string != nullptr); }

    inline ~String() noexcept = default;

    inline String(const String& copy) noexcept = default;
    inline String(String&& move) noexcept = default;

    [[nodiscard]] inline String& operator =(const String& copy) noexcept = default;
    [[nodiscard]] inline String& operator =(String&& move) noexcept = default;

    [[nodiscard]] inline String& operator =(const char* str) noexcept
    {
        if(str)
        {
            _string = str;
            _length = strlen(str);
            _hash = findHashCode(str);
        }
        return *this;
    }

    [[nodiscard]] inline String& operator =(const NotNull<const char>& str) noexcept
    {
        _string = str;
        _length = strlen(str);
        _hash = findHashCode(str);
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
private:
    friend class StringView;
    friend class DynString;
    friend class DynStringView;
    friend class StringBuilder;
};

class StringView final
{
private:
    const char*   _string;
    uSys _length;
    uSys _hash;
private:
    inline StringView(const uSys begin, const uSys length, const String& str) noexcept
        : _string(str._string + begin), _length(length), _hash(findHashCode(_string, length))
    { }
public:
    inline StringView(const String& str, const uSys begin, const uSys end) noexcept
        : _string(str._string + begin), _length(end - begin), _hash(findHashCode(_string, _length))
    { }

    [[nodiscard]] static inline StringView create(const String& str, const uSys begin, const uSys length) noexcept
    { return StringView(begin, length, str); }

    [[nodiscard]] inline const char* c_str() const noexcept { return _string; }
    [[nodiscard]] inline uSys length() const noexcept { return _length; }
    [[nodiscard]] inline uSys hashCode() const noexcept { return _hash; }

    [[nodiscard]] inline StringIterator begin() const noexcept { return StringIterator(_string, _length, 0); }
    [[nodiscard]] inline StringIterator   end() const noexcept { return StringIterator(_string, _length, _length - 1); }

    [[nodiscard]] inline bool equals(const String& other) const noexcept
    {
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return strncmp(this->_string, other._string, this->_length) == 0;
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const StringView& other) const noexcept
    {
        if(this == &other) { return true; }
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return strncmp(this->_string, other._string, this->_length) == 0;
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const DynString& other) const noexcept;
    [[nodiscard]] inline bool equals(const DynStringView& other) const noexcept;

    [[nodiscard]] inline bool equals(const char* str) const noexcept
    {
        return strncmp(this->_string, str, this->_length) == 0;
    }

    [[nodiscard]] inline i32 compareTo(const StringView& other) const noexcept
    {
        if(this == &other) { return 0; }
        return strncmp(this->_string, other._string, minT(this->_length, other._length));
    }

    [[nodiscard]] inline i32 compareTo(const String& other) const noexcept
    {
        return strncmp(this->_string, other._string, minT(this->_length, other._length));
    }

    [[nodiscard]] inline i32 compareTo(const DynStringView& other) const noexcept;
    [[nodiscard]] inline i32 compareTo(const DynString& other) const noexcept;
    [[nodiscard]] inline i32 compareTo(const char* str) const noexcept
    {
        return strncmp(this->_string, str, this->_length);
    }

#if __has_feature(__cpp_impl_three_way_comparison)
    inline i32 operator <=>(const String& other) const noexcept { return compareTo(other); }
    inline i32 operator <=>(const StringView& other) const noexcept { return compareTo(other); }
    inline i32 operator <=>(const DynString& other) const noexcept { return compareTo(other); }
    inline i32 operator <=>(const DynStringView& other) const noexcept { return compareTo(other); }
    inline i32 operator <=>(const char* other) const noexcept { return compareTo(other); }
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
        return DynString(str, ::std::strlen(str));
    }
private:
    const char* _string;
    uSys* _refCount;
    uSys  _length;
    uSys  _hash;
private:
    DynString(const char* string, uSys length) noexcept
        : _string(string), _refCount(new(::std::nothrow) uSys(1)), _length(length), _hash(findHashCode(string))
    { }
public:
    inline DynString(const NotNull<const char>& string) noexcept
        : _string(nullptr), _refCount(new(::std::nothrow) uSys(1)), _length(strlen(string)), _hash(findHashCode(string))
    {
        char* str = new(::std::nothrow) char[_length + 1];
        memcpy(str, string, _length + 1);
        _string = str;
    }

    inline DynString(const char* string) noexcept
        : _string(nullptr), _refCount(new(::std::nothrow) uSys(1)), _length(strlen(string)), _hash(findHashCode(string))
    {
        Ensure(string != nullptr);
        char* str = new(::std::nothrow) char[_length + 1];
        memcpy(str, string, _length + 1);
        _string = str;
    }

    inline ~DynString() noexcept
    {
        if(--(*_refCount) == 0)
        {
            delete[] _string;
            delete _refCount;
        }
    }

    inline DynString(const DynString& copy) noexcept
        : _string(copy._string), _refCount(copy._refCount), _length(copy._length), _hash(copy._hash)
    { ++(*_refCount); }

    inline DynString(DynString&& move) noexcept
        : _string(move._string), _refCount(move._refCount), _length(move._length), _hash(move._hash)
    { ++(*_refCount); }

    inline DynString& operator=(const DynString& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(--(*_refCount) == 0)
        {
            delete[] _string;
            delete _refCount;
        }

        _string = copy._string;
        _refCount = copy._refCount;
        _length = copy._length;
        _hash = copy._hash;

        ++(*_refCount);

        return *this;
    }

    inline DynString& operator=(DynString&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(--(*_refCount) == 0)
        {
            delete[] _string;
            delete _refCount;
        }

        _string = move._string;
        _refCount = move._refCount;
        _length = move._length;
        _hash = move._hash;

        ++(*_refCount);

        return *this;
    }

    [[nodiscard]] inline NonNull const char* c_str() const noexcept { return _string; }
    [[nodiscard]] inline uSys length() const noexcept { return _length; }

    inline operator const char*() const noexcept { return _string; }
    [[nodiscard]] inline uSys operator()() const noexcept { return _hash; }

    [[nodiscard]] inline uSys hashCode() const noexcept { return _hash; }

    [[nodiscard]] inline bool equals(const String& other) const noexcept
    {
        if(_string == other._string) { return true; }
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return strcmp(this->_string, other._string) == 0;
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const StringView& other) const noexcept
    {
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return strncmp(this->_string, other._string, other._length) == 0;
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const DynString& other) const noexcept
    {
        if(this == &other || this->_string == other._string) { return true; }
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return strcmp(this->_string, other._string) == 0;
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const DynStringView& other) const noexcept;

    [[nodiscard]] inline bool equals(const char* str) const noexcept
    {
        if(this->_string == str) { return true; }
        return strcmp(this->_string, str) == 0;
    }

    [[nodiscard]] inline i32 compareTo(const String& other) const noexcept
    {
        if(this->_string == other._string) { return 0; }
        return strcmp(this->_string, other._string);
    }

    [[nodiscard]] inline i32 compareTo(const StringView& other) const noexcept
    {
        if(this->_string == other._string) { return 0; }
        return strncmp(this->_string, other._string, other._length);
    }

    [[nodiscard]] inline i32 compareTo(const DynString& other) const noexcept
    {
        if(this == &other || this->_string == other._string) { return 0; }
        return strcmp(this->_string, other._string);
    }

    [[nodiscard]] inline i32 compareTo(const DynStringView& other) const noexcept;

    [[nodiscard]] inline i32 compareTo(const char* str) const noexcept
    {
        if(this->_string == str) { return 0; }
        return strcmp(this->_string, str);
    }

    [[nodiscard]] inline StringIterator begin() const noexcept { return StringIterator(_string, _length, 0); }
    [[nodiscard]] inline StringIterator   end() const noexcept { return StringIterator(_string, _length, _length - 1); }

#define CONCAT(__OTHER_LEN, __OTHER_STR) const uSys newLen = this->_length + (__OTHER_LEN); \
                                         char* newStr = new(::std::nothrow) char[newLen + 1]; \
                                         newStr[newLen] = '\0'; \
                                         memcpy(newStr, this->_string, this->_length); \
                                         memcpy(newStr + this->_length, __OTHER_STR, __OTHER_LEN)

    [[nodiscard]] inline DynString concat(const String& other) const noexcept
    {
        CONCAT(other._length, other._string);
        return DynString(newStr, newLen);
    }

    [[nodiscard]] inline DynString concat(const StringView& other) const noexcept
    {
        CONCAT(other._length, other._string);
        return DynString(newStr, newLen);
    }

    [[nodiscard]] inline DynString concat(const DynString& other) const noexcept
    {
        CONCAT(other._length, other._string);
        return DynString(newStr, newLen);
    }

    [[nodiscard]] inline DynString concat(const DynStringView& other) const noexcept;

    [[nodiscard]] inline DynString concat(const NotNull<const char>& other) const noexcept
    {
        const uSys otherLen = strlen(other);
        CONCAT(otherLen, other);
        return DynString(newStr, newLen);
    }

    [[nodiscard]] inline DynString concat(const char* other) const noexcept
    {
        const uSys otherLen = strlen(other);
        CONCAT(otherLen, other);
        return DynString(newStr, newLen);
    }

#if __has_feature(__cpp_impl_three_way_comparison)
    inline i32 operator <=> (const String& other) const noexcept { return compareTo(other); }
    inline i32 operator <=> (const StringView& other) const noexcept { return compareTo(other); }
    inline i32 operator <=> (const DynString& other) const noexcept { return compareTo(other); }
    inline i32 operator <=> (const DynStringView& other) const noexcept { return compareTo(other); }
    inline i32 operator <=> (const char* other) const noexcept { return compareTo(other); }
#endif

    inline bool operator ==(const String& other) const noexcept { return  equals(other); }
    inline bool operator !=(const String& other) const noexcept { return !equals(other); }
    inline bool operator < (const String& other) const noexcept { return compareTo(other) <  0; }
    inline bool operator > (const String& other) const noexcept { return compareTo(other) >  0; }
    inline bool operator <=(const String& other) const noexcept { return compareTo(other) <= 0; }
    inline bool operator >=(const String& other) const noexcept { return compareTo(other) >= 0; }

    inline bool operator ==(const StringView& other) const noexcept { return  equals(other); }
    inline bool operator !=(const StringView& other) const noexcept { return !equals(other); }
    inline bool operator < (const StringView& other) const noexcept { return compareTo(other) <  0; }
    inline bool operator > (const StringView& other) const noexcept { return compareTo(other) >  0; }
    inline bool operator <=(const StringView& other) const noexcept { return compareTo(other) <= 0; }
    inline bool operator >=(const StringView& other) const noexcept { return compareTo(other) >= 0; }

    inline bool operator ==(const DynString& other) const noexcept { return  equals(other); }
    inline bool operator !=(const DynString& other) const noexcept { return !equals(other); }
    inline bool operator < (const DynString& other) const noexcept { return compareTo(other) <  0; }
    inline bool operator > (const DynString& other) const noexcept { return compareTo(other) >  0; }
    inline bool operator <=(const DynString& other) const noexcept { return compareTo(other) <= 0; }
    inline bool operator >=(const DynString& other) const noexcept { return compareTo(other) >= 0; }

    inline bool operator ==(const DynStringView& other) const noexcept { return  equals(other); }
    inline bool operator !=(const DynStringView& other) const noexcept { return !equals(other); }
    inline bool operator < (const DynStringView& other) const noexcept { return compareTo(other) <  0; }
    inline bool operator > (const DynStringView& other) const noexcept { return compareTo(other) >  0; }
    inline bool operator <=(const DynStringView& other) const noexcept { return compareTo(other) <= 0; }
    inline bool operator >=(const DynStringView& other) const noexcept { return compareTo(other) >= 0; }

    inline bool operator ==(const char* other) const noexcept { return  equals(other); }
    inline bool operator !=(const char* other) const noexcept { return !equals(other); }
    inline bool operator < (const char* other) const noexcept { return compareTo(other) < 0; }
    inline bool operator > (const char* other) const noexcept { return compareTo(other) > 0; }
    inline bool operator <=(const char* other) const noexcept { return compareTo(other) <= 0; }
    inline bool operator >=(const char* other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] inline DynString operator +(const String&        other) const noexcept { return concat(other); }
    [[nodiscard]] inline DynString operator +(const StringView&    other) const noexcept { return concat(other); }
    [[nodiscard]] inline DynString operator +(const DynString&     other) const noexcept { return concat(other); }
    [[nodiscard]] inline DynString operator +(const DynStringView& other) const noexcept { return concat(other); }
    [[nodiscard]] inline DynString operator +(const char*          other) const noexcept { return concat(other); }
    [[nodiscard]] inline DynString operator +(const NotNull<const char>& other) const noexcept { return concat(other); }

    [[nodiscard]] inline char operator [](uSys index) const noexcept
    { return _string[index]; }

    [[nodiscard]] inline char at(uSys index) const noexcept
    {
        if(index >= _length) { return '\0'; }
        return _string[index];
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
    const char*    _string;
    uSys* _refCount;
    uSys  _length;
    uSys  _hash;
private:
    inline DynStringView(const uSys begin, const uSys length, const DynString& str) noexcept
        : _string(str._string + begin), _refCount(str._refCount), _length(length), _hash(findHashCode(_string, length))
    { ++(*_refCount); }
public:
    inline DynStringView(const DynString& str, const uSys begin, const uSys end) noexcept
        : _string(str._string + begin), _refCount(str._refCount), _length(end - begin), _hash(findHashCode(_string, _length))
    { ++(*_refCount); }

    inline ~DynStringView() noexcept
    {
        if(--(*_refCount) == 0)
        {
            delete[] _string;
            delete _refCount;
        }
    }

    inline DynStringView(const DynStringView& copy) noexcept
        : _string(copy._string), _refCount(copy._refCount), _length(copy._length), _hash(copy._hash)
    { ++(*_refCount); }

    inline DynStringView(DynStringView&& move) noexcept
        : _string(move._string), _refCount(move._refCount), _length(move._length), _hash(move._hash)
    { ++(*_refCount); }

    inline DynStringView& operator=(const DynStringView& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(--(*_refCount) == 0)
        {
            delete[] _string;
            delete _refCount;
        }

        _string = copy._string;
        _refCount = copy._refCount;
        _length = copy._length;
        _hash = copy._hash;

        ++(*_refCount);

        return *this;
    }

    inline DynStringView& operator=(DynStringView&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(--(*_refCount) == 0)
        {
            delete[] _string;
            delete _refCount;
        }

        _string = move._string;
        _refCount = move._refCount;
        _length = move._length;
        _hash = move._hash;

        ++(*_refCount);

        return *this;
    }

    [[nodiscard]] static inline DynStringView create(const DynString& str, const uSys begin, const uSys length) noexcept
    { return DynStringView(begin, length, str); }

    [[nodiscard]] inline const char* c_str() const noexcept { return _string; }
    [[nodiscard]] inline uSys length() const noexcept { return _length; }
    [[nodiscard]] inline uSys hashCode() const noexcept { return _hash; }

    [[nodiscard]] inline StringIterator begin() const noexcept { return StringIterator(_string, _length, 0); }
    [[nodiscard]] inline StringIterator   end() const noexcept { return StringIterator(_string, _length, _length - 1); }

    [[nodiscard]] inline bool equals(const String& other) const noexcept
    {
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return strncmp(this->_string, other._string, this->_length) == 0;
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const StringView& other) const noexcept
    {
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return strncmp(this->_string, other._string, this->_length) == 0;
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const DynString& other) const noexcept
    {
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return strncmp(this->_string, other._string, this->_length) == 0;
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const DynStringView& other) const noexcept
    {
        if(this == &other) { return true; }
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return strncmp(this->_string, other._string, this->_length) == 0;
        }
        return false;
    }

    [[nodiscard]] inline bool equals(const char* str) const noexcept
    {
        return strncmp(this->_string, str, this->_length) == 0;
    }

    [[nodiscard]] inline i32 compareTo(const StringView& other) const noexcept
    {
        return strncmp(this->_string, other._string, minT(this->_length, other._length));
    }

    [[nodiscard]] inline i32 compareTo(const String& other) const noexcept
    {
        return strncmp(this->_string, other._string, minT(this->_length, other._length));
    }

    [[nodiscard]] inline i32 compareTo(const DynStringView& other) const noexcept
    {
        if(this == &other) { return 0; }
        return strncmp(this->_string, other._string, minT(this->_length, other._length));
    }

    [[nodiscard]] inline i32 compareTo(const DynString& other) const noexcept
    {
        return strncmp(this->_string, other._string, minT(this->_length, other._length));
    }

    [[nodiscard]] inline i32 compareTo(const char* str) const noexcept
    {
        return strncmp(this->_string, str, this->_length);
    }

#if __has_feature(__cpp_impl_three_way_comparison)
    inline i32 operator <=>(const String& other) const noexcept { return compareTo(other); }
    inline i32 operator <=>(const StringView& other) const noexcept { return compareTo(other); }
    inline i32 operator <=>(const DynString& other) const noexcept { return compareTo(other); }
    inline i32 operator <=>(const DynStringView& other) const noexcept { return compareTo(other); }
    inline i32 operator <=>(const char* other) const noexcept { return compareTo(other); }
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
private:
    friend class String;
    friend class StringView;
    friend class DynString;
    friend class StringBuilder;
};

[[nodiscard]] inline DynString DynString::concat(const DynStringView& other) const noexcept
{
    CONCAT(other._length, other._string);
    return DynString(newStr, newLen);
}

class StringBuilder final
{
private:
    char* _string;
    uSys _length;
    uSys _size;
public:
    inline StringBuilder() noexcept
        : _string(new(::std::nothrow) char[64]), _length(0), _size(64)
    { }

    inline StringBuilder(std::size_t initialSize) noexcept
        : _string(new(::std::nothrow) char[initialSize]), _length(0), _size(initialSize)
    { Ensure(initialSize != 0); }

    inline StringBuilder(const NotNull<const char>& string) noexcept
        : _string(nullptr), _length(strlen(string)), _size(_length + 64)
    {
        char* str = new(::std::nothrow) char[_size];
        memcpy(str, string, _length + 1);
        _string = str;
    }

    inline StringBuilder(const char* string) noexcept
        : _string(nullptr), _length(strlen(string)), _size(_length + 64)
    {
        Ensure(string != nullptr);
        char* str = new(::std::nothrow) char[_size];
        memcpy(str, string, _length + 1);
        _string = str;
    }

    inline ~StringBuilder() noexcept
    { delete[] _string; }

    inline StringBuilder(const StringBuilder& copy) noexcept
        : _string(new(::std::nothrow) char[copy._size]), _length(copy._length), _size(copy._size)
    {
        memcpy(_string, copy._string, _size);
    }

    inline StringBuilder(StringBuilder&& move) noexcept
        : _string(new(::std::nothrow) char[move._size]), _length(move._length), _size(move._size)
    {
        memcpy(_string, move._string, _size);
    }

    inline StringBuilder& operator =(const StringBuilder& copy) noexcept
    {
        _string = new(::std::nothrow) char[copy._size];
        _length = copy._length;
        _size = copy._size;

        memcpy(_string, copy._string, _size);

        return *this;
    }

    inline StringBuilder& operator =(StringBuilder&& move) noexcept
    {
        _string = new(::std::nothrow) char[move._size];
        _length = move._length;
        _size = move._size;

        memcpy(_string, move._string, _size);

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
            return strcmp(this->_string, other._string) == 0;
        }
        return false;
    }

    [[nodiscard]] inline i32 compareTo(const StringBuilder& other) const noexcept
    {
        if(this == &other || _string == other._string) { return 0; }
        return strcmp(this->_string, other._string);
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
        append(string._string, string._length);
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
    inline i32 operator <=> (const StringBuilder& other) const noexcept { return compareTo(other); }
#endif

    inline bool operator ==(const StringBuilder& other) const noexcept { return  equals(other); }
    inline bool operator !=(const StringBuilder& other) const noexcept { return !equals(other); }
    inline bool operator < (const StringBuilder& other) const noexcept { return compareTo(other) <  0; }
    inline bool operator > (const StringBuilder& other) const noexcept { return compareTo(other) >  0; }
    inline bool operator <=(const StringBuilder& other) const noexcept { return compareTo(other) <= 0; }
    inline bool operator >=(const StringBuilder& other) const noexcept { return compareTo(other) >= 0; }

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

    [[nodiscard]] inline char operator [](uSys index) const noexcept
    { return _string[index]; }

    [[nodiscard]] inline char at(uSys index) const noexcept
    {
        if(index >= _length) { return '\0'; }
        return _string[index];
    }
private:
    void append(const char* string, const std::size_t length) noexcept
    {
        const uSys newLen = _length + length;
        if(newLen >= _size)
        {
            const uSys newSize = newLen + (newLen >> 1);
            char* newStr = new(::std::nothrow) char[newSize];
            memcpy(newStr, _string, _length + 1);
            _string = newStr;
            _size = newSize;
        }
        memcpy(_string + _length, string, length);
        _length += length;
        _string[_length] = '\0';
    }
};

namespace std
{
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
        return strcmp(this->_string, other._string) == 0;
    }
    return false;
}

[[nodiscard]] inline bool String::equals(const DynString& other) const noexcept
{
    if(_string == other._string) { return true; }
    if(this->_length == other._length && this->_hash == other._hash)
    {
        return strcmp(this->_string, other._string) == 0;
    }
    return false;
}

[[nodiscard]] inline bool String::equals(const DynStringView& other) const noexcept
{
    if(this->_length == other._length && this->_hash == other._hash)
    {
        return strcmp(this->_string, other._string) == 0;
    }
    return false;
}

[[nodiscard]] inline i32 String::compareTo(const StringView& other) const noexcept
{
    return strncmp(this->_string, other._string, minT(this->_length, other._length));
}

[[nodiscard]] inline i32 String::compareTo(const DynString& other) const noexcept
{
    if(_string == other._string) { return 0; }
    return strcmp(this->_string, other._string);
}

[[nodiscard]] inline i32 String::compareTo(const DynStringView& other) const noexcept
{
    return strncmp(this->_string, other._string, other._length);
}

[[nodiscard]] inline bool StringView::equals(const DynString& other) const noexcept
{
    if(this->_length == other._length && this->_hash == other._hash)
    {
        return strncmp(this->_string, other._string, this->_length) == 0;
    }
    return false;
}

[[nodiscard]] inline bool StringView::equals(const DynStringView& other) const noexcept
{
    if(this->_length == other._length && this->_hash == other._hash)
    {
        return strncmp(this->_string, other._string, minT(this->_length, other._length)) == 0;
    }
    return false;
}

[[nodiscard]] inline i32 StringView::compareTo(const DynString& other) const noexcept
{
    return strncmp(this->_string, other._string, other._length);
}

[[nodiscard]] inline i32 StringView::compareTo(const DynStringView& other) const noexcept
{
    return strncmp(this->_string, other._string, minT(this->_length, other._length));
}

[[nodiscard]] inline bool DynString::equals(const DynStringView& other) const noexcept
{
    return strncmp(this->_string, other._string, other._length) == 0;
}

[[nodiscard]] inline i32 DynString::compareTo(const DynStringView& other) const noexcept
{
    return strncmp(this->_string, other._string, other._length);
}
