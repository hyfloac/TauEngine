#pragma warning(push, 0)
#include <cstring>
#pragma warning(pop)
#include <String.hpp>
#include <Safeties.hpp>

bool equalsIgnoreCase(const char* lhs, const char* rhs) noexcept
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

void toLower(char* str) noexcept
{
    while(*str != '\0')
    {
        *str = tolower(*str);
        ++str;
    }
}

void toLower(const char* str, char* store) noexcept
{
    while(*str != '\0')
    {
        *store = tolower(*str);
        ++str;
        ++store;
    }
}

void toUpper(char* str) noexcept
{
    while(*str != '\0')
    {
        *str = toupper(*str);
        ++str;
    }
}

void toUpper(const char* str, char* store) noexcept
{
    while(*str != '\0')
    {
        *store = toupper(*str);
        ++str;
        ++store;
    }
}

String::String(NotNull<const char> string) noexcept
    : _string(string), _length(static_cast<u32>(strlen(string))), _hash(findHashCode(string)) { }

String::String(const char* string) noexcept
    : _string(string), _length(static_cast<u32>(strlen(string))), _hash(findHashCode(string))
{
    Ensure(string != nullptr);
}

String::String(String&& move) noexcept
    : _string(move._string), _length(move._length), _hash(move._hash) { }

String& String::operator =(String&& move) noexcept
{
    this->_string = move._string;
    this->_length = move._length;
    this->_hash   = move._hash;
    return *this;
}

u32 String::findHashCode(NonNull const char* str) noexcept
{
    u32 hash = 0;
    u32 i    = 0;
    while (str[i])
    {
        hash = 31u * hash + static_cast<u32>(str[i++]);
    }
    return hash;
}

bool String::equals(const String& other) const noexcept
{
    if (this->_length == other._length && this->_hash == other._hash)
    {
        return strcmp(this->_string, other._string) == 0;
    }
    return false;
}

i32 String::compareTo(const String& other) const noexcept
{
    return strcmp(this->_string, other._string);
}
