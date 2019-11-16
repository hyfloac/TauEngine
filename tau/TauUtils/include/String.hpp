#pragma once

#pragma warning(push, 0)
#include <cctype>
#include <cstring>
#pragma warning(pop)

#include "ReferenceCounter.hpp"
#include "NumTypes.hpp"
#include "CVersion.hpp"
#include "Safeties.hpp"
#include "Objects.hpp"
#include "Utils.hpp"

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
    for(u32 i = 0; str[i]; ++i)
    {
        hash = 31u * hash + static_cast<u32>(str[i]);
    }
    return hash;
}

static inline u32 findHashCode(NonNull const char* str, const u32 len) noexcept
{
    u32 hash = 0;
    for(u32 i = 0; i < len; ++i)
    {
        hash = 31u * hash + static_cast<u32>(str[i]);
    }
    return hash;
}

namespace cexpr
{
    template<std::size_t _Len>
    constexpr inline std::size_t strlen(const char(&str)[_Len]) noexcept { return _Len - 1; }

    template<std::size_t _Len>
    constexpr inline std::size_t strlen(const wchar_t(&str)[_Len]) noexcept { return _Len - 1; }

    template<std::size_t _Len>
    constexpr inline std::size_t strlen(const char16_t(&str)[_Len]) noexcept { return _Len - 1; }

    template<std::size_t _Len>
    constexpr inline std::size_t strlen(const char32_t(&str)[_Len]) noexcept { return _Len - 1; }

    template<typename _Type, std::size_t _Len>
    constexpr inline std::size_t arrlen(const _Type(&arr)[_Len]) noexcept { return _Len - 1; }

    template<std::size_t _Len>
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

    template<std::size_t _Len>
    constexpr u32 findHashCode(NonNull const char(&str)[_Len]) noexcept
    {
        u32 hash = 0;
        for(u32 i = 0; str[i]; ++i)
        {
            hash = 31u * hash + static_cast<u32>(str[i]);
        }
        return hash;
    }
}

class StringIterator final
{
    DEFAULT_DESTRUCT(StringIterator);
    DEFAULT_COPY(StringIterator);
private:
    const char* _string;
    std::size_t _length;
    std::size_t _index;
public:
    StringIterator(const char* const string, const std::size_t length, const std::size_t index)
        : _string(string), _length(length), _index(index)
    { }

    inline StringIterator& operator++() noexcept
    {
        if(_index - 1 < _length)
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
    { return !this->operator==(other); }
};

class String final
{
private:
    const char* _string;
    std::size_t _length;
    u32         _hash;
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

    inline String& operator =(const String& copy) noexcept = default;
    inline String& operator =(String&& move) noexcept = default;

    [[nodiscard]] inline NonNull const char* c_str() const noexcept { return _string; }
    [[nodiscard]] inline std::size_t length() const noexcept { return _length; }

    [[nodiscard]] inline operator const char*() const noexcept { return _string; }
    [[nodiscard]] inline u32 operator()() const noexcept { return _hash; }

    [[nodiscard]] inline u32 hashCode() const noexcept { return _hash; }

    [[nodiscard]] inline bool equals(const String& other) const noexcept
    {
        if(this == &other || _string == other._string) { return true; }
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return strcmp(this->_string, other._string) == 0;
        }
        return false;
    }

    [[nodiscard]] inline i32 compareTo(const String& other) const noexcept
    {
        if(this == &other || _string == other._string) { return 0; }
        return strcmp(this->_string, other._string);
    }

    [[nodiscard]] inline StringIterator begin() const noexcept { return StringIterator(_string, _length, 0); }
    [[nodiscard]] inline StringIterator   end() const noexcept { return StringIterator(_string, _length, _length - 1); }

#if __has_feature(__cpp_impl_three_way_comparison)
    inline i32 operator <=>(const String& other) const noexcept { return compareTo(other); }
#endif

    inline bool operator ==(const String& other) const noexcept { return  equals(other); }
    inline bool operator !=(const String& other) const noexcept { return !equals(other); }
    inline bool operator < (const String& other) const noexcept { return compareTo(other) <  0; }
    inline bool operator > (const String& other) const noexcept { return compareTo(other) >  0; }
    inline bool operator <=(const String& other) const noexcept { return compareTo(other) <= 0; }
    inline bool operator >=(const String& other) const noexcept { return compareTo(other) >= 0; }
private:
    friend class DynString;
    friend class StringView;
    friend class StringBuilder;
};

class StringView final
{
private:
    const char* _string;
    std::size_t _length;
    u32         _hash;
private:
    inline StringView(const std::size_t begin, const std::size_t length, const String& str) noexcept
        : _string(str._string + begin), _length(length), _hash(findHashCode(_string, length))
    { }
public:
    inline StringView(const String& str, const std::size_t begin, const std::size_t end) noexcept
        : _string(str._string + begin), _length(end - begin), _hash(findHashCode(_string, _length))
    { }

    [[nodiscard]] static inline StringView create(const String& str, const std::size_t begin, const std::size_t length) noexcept
    { return StringView(begin, length, str); }

    [[nodiscard]] inline const char* c_str() const noexcept { return _string; }
    [[nodiscard]] inline std::size_t length() const noexcept { return _length; }
    [[nodiscard]] inline u32 hashCode() const noexcept { return _hash; }

    [[nodiscard]] inline StringIterator begin() const noexcept { return StringIterator(_string, _length, 0); }
    [[nodiscard]] inline StringIterator   end() const noexcept { return StringIterator(_string, _length, _length - 1); }
private:
    friend class DynString;
    friend class StringBuilder;
};

class DynStringView;

class DynString final
{
    DEFAULT_COPY(DynString);
private:
    const char* _string;
    std::size_t _length;
    u32         _hash;
    RefCounter  _refCount;
private:
    DynString(const char* string, std::size_t length) noexcept
        : _string(string), _length(length), _hash(findHashCode(string)), _refCount()
    { }
public:
    inline DynString(const NotNull<const char>& string) noexcept
        : _string(nullptr), _length(strlen(string)), _hash(findHashCode(string)), _refCount()
    {
        char* str = new(std::nothrow) char[_length + 1];
        memcpy(str, string, _length + 1);
        _string = str;
    }

    inline DynString(const char* string) noexcept
        : _string(nullptr), _length(strlen(string)), _hash(findHashCode(string)), _refCount()
    {
        Ensure(string != nullptr);
        char* str = new(std::nothrow) char[_length + 1];
        memcpy(str, string, _length + 1);
        _string = str;
    }

    inline ~DynString() noexcept
    {
        if(_refCount)
        { delete[] _string; }
    }

    [[nodiscard]] inline NonNull const char* c_str() const noexcept { return _string; }
    [[nodiscard]] inline std::size_t length() const noexcept { return _length; }

    inline operator const char*() const noexcept { return _string; }
    [[nodiscard]] inline u32 operator()() const noexcept { return _hash; }

    [[nodiscard]] inline u32 hashCode() const noexcept { return _hash; }

    [[nodiscard]] inline bool equals(const DynString& other) const noexcept
    {
        if(this == &other || _string == other._string) { return true; }
        if(this->_length == other._length && this->_hash == other._hash)
        {
            return strcmp(this->_string, other._string) == 0;
        }
        return false;
    }

    [[nodiscard]] inline i32 compareTo(const DynString& other) const noexcept
    {
        if(this == &other || _string == other._string) { return 0; }
        return strcmp(this->_string, other._string);
    }

    [[nodiscard]] inline StringIterator begin() const noexcept { return StringIterator(_string, _length, 0); }
    [[nodiscard]] inline StringIterator   end() const noexcept { return StringIterator(_string, _length, _length - 1); }

#define CONCAT(__OTHER_LEN, __OTHER_STR) const std::size_t newLen = this->_length + (__OTHER_LEN); \
                                         char* newStr = new(std::nothrow) char[newLen + 1]; \
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
        const std::size_t otherLen = strlen(other);
        CONCAT(otherLen, other);
        return DynString(newStr, newLen);
    }

    [[nodiscard]] inline DynString concat(const char* other) const noexcept
    {
        const std::size_t otherLen = strlen(other);
        CONCAT(otherLen, other);
        return DynString(newStr, newLen);
    }

#if __has_feature(__cpp_impl_three_way_comparison)
    inline i32 operator <=> (const DynString& other) const noexcept { return compareTo(other); }
#endif

    inline bool operator ==(const DynString& other) const noexcept { return  equals(other); }
    inline bool operator !=(const DynString& other) const noexcept { return !equals(other); }
    inline bool operator < (const DynString& other) const noexcept { return compareTo(other) < 0; }
    inline bool operator > (const DynString& other) const noexcept { return compareTo(other) > 0; }
    inline bool operator <=(const DynString& other) const noexcept { return compareTo(other) <= 0; }
    inline bool operator >=(const DynString& other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] inline DynString operator +(const String&        other) const noexcept { return concat(other); }
    [[nodiscard]] inline DynString operator +(const StringView&    other) const noexcept { return concat(other); }
    [[nodiscard]] inline DynString operator +(const DynString&     other) const noexcept { return concat(other); }
    [[nodiscard]] inline DynString operator +(const DynStringView& other) const noexcept { return concat(other); }
    [[nodiscard]] inline DynString operator +(const char*          other) const noexcept { return concat(other); }
    [[nodiscard]] inline DynString operator +(const NotNull<const char>& other) const noexcept { return concat(other); }

    [[nodiscard]] inline char operator [](std::size_t index) const noexcept
    { return _string[index]; }

    [[nodiscard]] inline char at(std::size_t index) const noexcept
    {
        if(index >= _length) { return '\0'; }
        return _string[index];
    }
private:
    friend class String;
    friend class StringBuilder;
    friend class DynStringView;
};

class DynStringView final
{
private:
    const char* _string;
    std::size_t _length;
    u32         _hash;
    RefCounter  _refCount;
private:
    inline DynStringView(const std::size_t begin, const std::size_t length, const DynString& str) noexcept
        : _string(str._string + begin), _length(length), _hash(findHashCode(_string, length)), _refCount(str._refCount)
    { }
public:
    inline DynStringView(const DynString& str, const std::size_t begin, const std::size_t end) noexcept
        : _string(str._string + begin), _length(end - begin), _hash(findHashCode(_string, _length)), _refCount(str._refCount)
    { }

    [[nodiscard]] static inline DynStringView create(const DynString& str, const std::size_t begin, const std::size_t length) noexcept
    { return DynStringView(begin, length, str); }

    [[nodiscard]] inline const char* c_str() const noexcept { return _string; }
    [[nodiscard]] inline std::size_t length() const noexcept { return _length; }
    [[nodiscard]] inline u32 hashCode() const noexcept { return _hash; }

    [[nodiscard]] inline StringIterator begin() const noexcept { return StringIterator(_string, _length, 0); }
    [[nodiscard]] inline StringIterator   end() const noexcept { return StringIterator(_string, _length, _length - 1); }
private:
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
    char*       _string;
    std::size_t _length;
    std::size_t _size;
public:
    inline StringBuilder() noexcept
        : _string(new char[64]), _length(0), _size(64)
    { }

    inline StringBuilder(std::size_t initialSize) noexcept
        : _string(new(std::nothrow) char[initialSize]), _length(0), _size(initialSize)
    { Ensure(initialSize != 0); }

    inline StringBuilder(const NotNull<const char>& string) noexcept
        : _string(nullptr), _length(strlen(string)), _size(_length + 64)
    {
        char* str = new(std::nothrow) char[_size];
        memcpy(str, string, _length + 1);
        _string = str;
    }

    inline StringBuilder(const char* string) noexcept
        : _string(nullptr), _length(strlen(string)), _size(_length + 64)
    {
        Ensure(string != nullptr);
        char* str = new(std::nothrow) char[_size];
        memcpy(str, string, _length + 1);
        _string = str;
    }

    inline ~StringBuilder() noexcept
    { delete[] _string; }

    inline StringBuilder(const StringBuilder& copy) noexcept
        : _string(new(std::nothrow) char[copy._size]), _length(copy._length), _size(copy._size)
    {
        memcpy(_string, copy._string, _size);
    }

    inline StringBuilder(StringBuilder&& move) noexcept
        : _string(new(std::nothrow) char[move._size]), _length(move._length), _size(move._size)
    {
        memcpy(_string, move._string, _size);
    }

    inline StringBuilder& operator =(const StringBuilder& copy) noexcept
    {
        _string = new(std::nothrow) char[copy._size];
        _length = copy._length;
        _size = copy._size;

        memcpy(_string, copy._string, _size);

        return *this;
    }

    inline StringBuilder& operator =(StringBuilder&& move) noexcept
    {
        _string = new(std::nothrow) char[move._size];
        _length = move._length;
        _size = move._size;

        memcpy(_string, move._string, _size);

        return *this;
    }

    [[nodiscard]] const char* c_str() const noexcept { return _string; }
    [[nodiscard]] size_t length() const noexcept { return _length; }
    [[nodiscard]] size_t size() const noexcept { return _size; }

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

    inline StringBuilder& reset(const std::size_t newSize) noexcept
    {
        if(newSize == 0) { return *this; }
        delete[] _string;
        _string = new(std::nothrow) char[newSize];
        _size = newSize;
        _length = 0;
        _string[0] = '\0';
        return *this;
    }

    inline StringBuilder& resetIf(const std::size_t maxSize, const std::size_t newSize) noexcept
    {
        if(newSize == 0) { return *this; }
        if(maxSize <= newSize) { return *this; }
        if(_size >= maxSize)
        {
            delete[] _string;
            _string = new(std::nothrow) char[newSize];
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

    inline StringBuilder& backspace(const std::size_t count) noexcept
    {
        if(_length > 0)
        {
            const std::size_t newLen = _length - count;
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

    [[nodiscard]] inline char operator [](std::size_t index) const noexcept
    { return _string[index]; }

    [[nodiscard]] inline char at(std::size_t index) const noexcept
    {
        if(index >= _length) { return '\0'; }
        return _string[index];
    }
private:
    void append(const char* string, const std::size_t length) noexcept
    {
        const std::size_t newLen = _length + length;
        if(newLen >= _size)
        {
            const std::size_t newSize = newLen + (newLen >> 1);
            char* newStr = new(std::nothrow) char[newSize];
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
    inline std::size_t operator()(const String& str) const noexcept
    { return static_cast<std::size_t>(str.hashCode()); }
};

template<>
struct hash<StringView>
{
    inline std::size_t operator()(const StringView& str) const noexcept
    { return static_cast<std::size_t>(str.hashCode()); }
};

template<>
struct hash<DynString>
{
    inline std::size_t operator()(const DynString& str) const noexcept
    { return static_cast<std::size_t>(str.hashCode()); }
};

template<>
struct hash<DynStringView>
{
    inline std::size_t operator()(const DynStringView& str) const noexcept
    { return static_cast<std::size_t>(str.hashCode()); }
};
}
