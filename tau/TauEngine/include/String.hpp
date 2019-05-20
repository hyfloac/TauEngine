#pragma once

#pragma warning(push, 0)
#include <cctype>
#include <unordered_map>
#pragma warning(pop)

#include <NumTypes.hpp>
#include <CVersion.hpp>
#include <Safeties.hpp>
#include <DLL.hpp>
#include <Utils.hpp>

TAU_DLL bool equalsIgnoreCase(const char* RESTRICT lhs, const char* RESTRICT rhs) noexcept;

TAU_DLL void toLower(char* str) noexcept;
TAU_DLL void toLower(const char* str, char* store) noexcept;

TAU_DLL void toUpper(char* str) noexcept;
TAU_DLL void toUpper(const char* str, char* store) noexcept;

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
}

class TAU_DLL String final
{
private:
    const char* _string;
    u32         _length;
    u32         _hash;
private:
    static u32 findHashCode(NonNull const char* str) noexcept;
public:
    String(NotNull<const char> string) noexcept;
    String(const char* string) noexcept;
    String(const String& copy) noexcept = default;
    String(String&& move) noexcept;

    ~String() noexcept = default;

    String& operator =(const String& copy) noexcept = default;
    String& operator =(String&& move) noexcept;

    inline NonNull const char* c_str() const noexcept { return _string; }
    inline u32 length() const noexcept { return _length; }

    inline operator const char*() const noexcept { return _string; }
    inline u32 operator()() const noexcept { return _hash; }

    inline u32 hashCode() const noexcept { return _hash; }

    bool equals(const String& other) const noexcept;

    i32 compareTo(const String& other) const noexcept;

#if defined(__CPP_20) && 0
    inline i32 operator <=>(const String& other) const noexcept { return compareTo(other); }
#endif

    inline bool operator ==(const String& other) const noexcept { return equals(other); }
    inline bool operator !=(const String& other) const noexcept { return !equals(other); }
    inline bool operator < (const String& other) const noexcept { return compareTo(other) < 0; }
    inline bool operator > (const String& other) const noexcept { return compareTo(other) > 0; }
    inline bool operator <=(const String& other) const noexcept { return compareTo(other) <= 0; }
    inline bool operator >=(const String& other) const noexcept { return compareTo(other) >= 0; }
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
}
