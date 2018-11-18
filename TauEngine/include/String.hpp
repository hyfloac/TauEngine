#pragma once
#include <NumTypes.hpp>
#include <CVersion.hpp>

class String
{
private:
    const char* _string;
    u32         _length;
    u32         _hash;
private:
    static u32 findHashCode(const char* str) noexcept;
public:
    String(const char* string) noexcept;
    String(const String& copy) noexcept = default;
    String(String&& move) noexcept;

    String& operator =(const String& copy) noexcept = default;
    String& operator =(String&& copy) noexcept;

    inline const char* c_str() const noexcept { return _string; }
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

struct StringHasher
{
    inline size_t operator()(const String& str) const
    {
        return static_cast<size_t>(str.hashCode());
    }
};