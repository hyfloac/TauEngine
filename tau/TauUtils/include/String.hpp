#pragma once

#pragma warning(push, 0)
#include <cctype>
#include <cstring>
// #include <unordered_map>
// #include <utility>
#pragma warning(pop)

#include <NumTypes.hpp>
#include <CVersion.hpp>
#include <Safeties.hpp>
#include <Utils.hpp>

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

static inline u32 findHashCode(NonNull const char* str) noexcept
{
    u32 hash = 0;
    u32 i = 0;
    while(str[i])
    {
        hash = 31u * hash + static_cast<u32>(str[i++]);
    }
    return hash;
}

namespace cexpr
{
    template<size_t _Len>
    constexpr inline size_t strlen(const char(&str)[_Len]) noexcept { return _Len - 1; }

    template<size_t _Len>
    constexpr inline size_t strlen(const wchar_t(&str)[_Len]) noexcept { return _Len - 1; }

    template<size_t _Len>
    constexpr inline size_t strlen(const char16_t(&str)[_Len]) noexcept { return _Len - 1; }

    template<size_t _Len>
    constexpr inline size_t strlen(const char32_t(&str)[_Len]) noexcept { return _Len - 1; }

    template<typename _Type, size_t _Len>
    constexpr inline size_t arrlen(const _Type(&arr)[_Len]) noexcept { return _Len - 1; }

    template<size_t _Len>
    constexpr bool streq(const char(&RESTRICT lhs)[_Len], const char(&RESTRICT rhs)[_Len])
    {
        while(*lhs != '\0' && *rhs != '\0')
        {
            if(tolower(*lhs) != tolower(*rhs))
            {
                return false;
            }
            ++lhs;
            ++rhs;
        }

        return *lhs == '\0' && *rhs == '\0';
    }

    template<size_t _Len>
    constexpr u32 findHashCode(NonNull const char(&str)[_Len]) noexcept
    {
        u32 hash = 0;
        u32 i = 0;
        while(str[i])
        {
            hash = 31u * hash + static_cast<u32>(str[i++]);
        }
        return hash;
    }
}

class String final
{
private:
    const char* _string;
    size_t      _length;
    u32         _hash;
public:
    inline String(NotNull<const char> string) noexcept
        : _string(string), _length(strlen(string)), _hash(findHashCode(string))
    { }

    inline String(const char* string) noexcept
        : _string(string), _length(strlen(string)), _hash(findHashCode(string))
    {
        Ensure(string != nullptr);
    }

    inline ~String() noexcept = default;

    inline String(const String& copy) noexcept = default;
    inline String(String&& move) noexcept = default;

    inline String& operator =(const String& copy) noexcept = default;
    inline String& operator =(String&& move) noexcept = default;

    [[nodiscard]] inline NonNull const char* c_str() const noexcept { return _string; }
    [[nodiscard]] inline size_t length() const noexcept { return _length; }

    inline operator const char*() const noexcept { return _string; }
    inline u32 operator()() const noexcept { return _hash; }

    [[nodiscard]] inline u32 hashCode() const noexcept { return _hash; }

    [[nodiscard]] inline bool equals(const String& other) const noexcept
    {
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return strcmp(this->_string, other._string) == 0;
        }
        return false;
        }

    [[nodiscard]] inline i32 compareTo(const String& other) const noexcept
    {
        return strcmp(this->_string, other._string);
    }

#if defined(__CPP_20) && 0
    inline i32 operator <=>(const String& other) const noexcept { return compareTo(other); }
#endif

    inline bool operator ==(const String& other) const noexcept { return  equals(other); }
    inline bool operator !=(const String& other) const noexcept { return !equals(other); }
    inline bool operator < (const String& other) const noexcept { return compareTo(other) <  0; }
    inline bool operator > (const String& other) const noexcept { return compareTo(other) >  0; }
    inline bool operator <=(const String& other) const noexcept { return compareTo(other) <= 0; }
    inline bool operator >=(const String& other) const noexcept { return compareTo(other) >= 0; }
};

class DynString final
{
private:
    const char* _string;
    size_t      _length;
    u32         _hash;
    i32*        _refCount;
private:
    DynString(const char* string, size_t length) noexcept
        : _string(string), _length(length), _hash(findHashCode(string)), _refCount(new i32(1))
    { }

    DynString& appendCopy(const char* newStr, size_t newLen) noexcept
    {
        if(--(*this->_refCount) <= 0)
        {
            delete[] this->_string;
            delete this->_refCount;
        }

        this->_string = newStr;
        this->_length = newLen;
        this->_hash = findHashCode(newStr);
        this->_refCount = new i32(1);

        return *this;
    }
public:
    inline DynString(NotNull<const char> string) noexcept
        : _string(nullptr), _length(static_cast<u32>(strlen(string))), _hash(findHashCode(string)), _refCount(new i32(1))
    {
        char* str = new char[_length + 1];
        memcpy(str, string, _length + 1);
        _string = str;
    }

    inline DynString(const char* string) noexcept
        : _string(nullptr), _length(static_cast<u32>(strlen(string))), _hash(findHashCode(string)), _refCount(new i32(1))
    {
        Ensure(string != nullptr);
        char* str = new char[_length + 1];
        memcpy(str, string, _length + 1);
        _string = str;
    }

    inline ~DynString() noexcept
    {
        if(--(*_refCount) <= 0)
        {
            delete[] _string;
            delete _refCount;
        }
    }

    inline DynString(const DynString& copy) noexcept
        : _string(copy._string), _length(copy._length), _hash(copy._hash), _refCount(copy._refCount)
    {
        ++(*_refCount);
    }

    inline DynString(DynString&& move) noexcept
        : _string(move._string), _length(move._length), _hash(move._hash), _refCount(move._refCount)
    { ++(*_refCount); }

    inline DynString& operator =(const DynString& copy) noexcept
    {
        if(--(*_refCount) <= 0)
        {
            delete[] _string;
        }

        _string = copy._string;
        _length = copy._length;
        _hash = copy._hash;
        _refCount = copy._refCount;
        ++(*_refCount);

        return *this;
    }

    inline DynString& operator =(DynString&& move) noexcept
    {
        if(--(*_refCount) <= 0)
        {
            delete[] _string;
        }

        _string = move._string;
        _length = move._length;
        _hash = move._hash;
        _refCount = move._refCount;
        ++(*_refCount);

        return *this;
    }

    inline NonNull const char* c_str() const noexcept { return _string; }
    inline size_t length() const noexcept { return _length; }

    inline operator const char*() const noexcept { return _string; }
    inline u32 operator()() const noexcept { return _hash; }

    inline u32 hashCode() const noexcept { return _hash; }

    [[nodiscard]] inline bool equals(const DynString& other) const noexcept
    {
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return strcmp(this->_string, other._string) == 0;
        }
        return false;
    }

    [[nodiscard]] inline i32 compareTo(const DynString& other) const noexcept
    {
        return strcmp(this->_string, other._string);
    }

#define CONCAT(__OTHER_LEN, __OTHER_STR) const size_t newLen = this->_length + (__OTHER_LEN); \
                                         char* newStr = new char[newLen + 1]; \
                                         newStr[newLen] = '\0'; \
                                         memcpy(newStr, this->_string, this->_length); \
                                         memcpy(newStr + this->_length, __OTHER_STR, __OTHER_LEN);

    DynString concat(DynString&& other) const noexcept
    {
        CONCAT(other._length, other._string);

        return DynString(newStr, newLen);
    }

    DynString concat(NotNull<const char> other) const noexcept
    {
        const size_t otherLen = strlen(other);
        CONCAT(otherLen, other);

        return DynString(newStr, newLen);
    }

    DynString concat(const char* other) const noexcept
    {
        const size_t otherLen = strlen(other);
        CONCAT(otherLen, other);

        return DynString(newStr, newLen);
    }

    DynString& append(DynString&& other) noexcept
    {
        CONCAT(other._length, other._string);

        return appendCopy(newStr, newLen);
    }

    DynString& append(NotNull<const char> other) noexcept
    {
        const size_t otherLen = strlen(other);
        CONCAT(otherLen, other);

        return appendCopy(newStr, newLen);
    }

    DynString& append(const char* other) noexcept
    {
        const size_t otherLen = strlen(other);
        CONCAT(otherLen, other);

        return appendCopy(newStr, newLen);
    }

#if defined(__CPP_20) && 0
    inline i32 operator <= > (const String& other) const noexcept { return compareTo(other); }
#endif

    inline bool operator ==(const DynString& other) const noexcept { return  equals(other); }
    inline bool operator !=(const DynString& other) const noexcept { return !equals(other); }
    inline bool operator < (const DynString& other) const noexcept { return compareTo(other) < 0; }
    inline bool operator > (const DynString& other) const noexcept { return compareTo(other) > 0; }
    inline bool operator <=(const DynString& other) const noexcept { return compareTo(other) <= 0; }
    inline bool operator >=(const DynString& other) const noexcept { return compareTo(other) >= 0; }

    inline DynString operator +(DynString&&         other) const noexcept { return concat(other); }
    inline DynString operator +(const char*         other) const noexcept { return concat(other); }
    inline DynString operator +(NotNull<const char> other) const noexcept { return concat(std::move(other)); }

    inline DynString operator +=(DynString&&         other) noexcept { return append(other); }
    inline DynString operator +=(const char*         other) noexcept { return append(other); }
    inline DynString operator +=(NotNull<const char> other) noexcept { return append(std::move(other)); }


    inline char operator [](size_t index) const noexcept
    { return _string[index]; }

    [[nodiscard]] inline char at(size_t index) const noexcept
    {
        if(index >= _length) { return '\0'; }
        return _string[index];
    }
};

namespace std
{
template<>
struct hash<String>
{
    inline size_t operator()(const String& str) const noexcept
    {
        return static_cast<size_t>(str.hashCode());
    }
};

template<>
struct hash<DynString>
{
    inline size_t operator()(const DynString& str) const noexcept
    {
        return static_cast<size_t>(str.hashCode());
    }
};
}
