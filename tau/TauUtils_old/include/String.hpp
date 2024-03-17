#pragma once
// ReSharper disable CppClangTidyClangDiagnosticUnusedFunction

#pragma warning(push, 0)
#include <cctype>
#pragma warning(pop)

#include "Objects.hpp"
#include "NumTypes.hpp"
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

template<typename _C>
_C toLower(_C c) noexcept = delete;

template<typename _C>
_C toUpper(_C c) noexcept = delete;

/**
 * Converts a string to lower case.
 *
 * @return
 *      The number of characters that are, or would need to be stored.
 */
template<typename _C>
uSys toLower(const _C* str, [[tau::nullable]] _C* store) noexcept;

/**
 * Converts a string to upper case.
 *
 * @return
 *      The number of characters that are, or would need to be stored.
 */
template<typename _C>
uSys toUpper(const _C* str, [[tau::nullable]] _C* store) noexcept;

template<typename _C>
uSys findHashCode([[tau::nonnull]] const _C* str) noexcept;

template<typename _C>
uSys findHashCode(const _C* str, uSys len) noexcept;

template<typename _C>
uSys strLength(const _C* str) noexcept;

template<typename _C>
i32 strCompare(const _C* lhs, const _C* rhs) noexcept;

template<typename _C>
i32 strCompare(const _C* lhs, const _C* rhs, uSys length) noexcept;

inline bool equalsIgnoreCase(const char* RESTRICT lhs, const char* RESTRICT rhs) noexcept
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

namespace cexpr
{
    template<typename _C, uSys _Len>
    constexpr inline uSys strlen(const _C(&str)[_Len]) noexcept;

    template<typename _C, uSys _Len>
    constexpr inline bool streq(const _C(&lhs)[_Len], const _C(&rhs)[_Len]);

    template<typename _C, uSys _Len>
    constexpr inline bool streqIgnoreCase(const _C(&lhs)[_Len], const _C(&rhs)[_Len]);

    template<typename _C, uSys _Len>
    constexpr inline uSys findHashCode(const _C(&str)[_Len]) noexcept;
}

template<typename _C>
class StringIteratorT;

template<typename _C>
class ConstExprStringT;

template<typename _C>
class DynStringT;

template<typename _C>
class DynStringViewT;

template<typename _C>
class StringBuilderT;

using StringIterator = StringIteratorT<char>;
using WStringIterator = StringIteratorT<wchar_t>;

using ConstExprString = ConstExprStringT<char>;
using WConstExprString = ConstExprStringT<wchar_t>;

using DynString = DynStringT<char>;
using WDynString = DynStringT<wchar_t>;

using DynStringView = DynStringViewT<char>;
using WDynStringView = DynStringViewT<wchar_t>;

using StringBuilder = StringBuilderT<char>;
using WStringBuilder = StringBuilderT<wchar_t>;

template<typename _C>
class StringIteratorT final
{
    DEFAULT_DESTRUCT(StringIteratorT);
    DEFAULT_CM_PU(StringIteratorT);
private:
    const _C* _string;
    uSys _length;
    uSys _index;
public:
    StringIteratorT(const _C* string, uSys length, uSys index) noexcept;

    StringIteratorT<_C>& operator++() noexcept;
    StringIteratorT<_C>& operator--() noexcept;

    [[nodiscard]] StringIteratorT<_C> operator++(int) noexcept;
    [[nodiscard]] StringIteratorT<_C> operator--(int) noexcept;

    [[nodiscard]] inline _C operator*() const noexcept;

    [[nodiscard]] bool operator ==(const StringIteratorT<_C>& other) const noexcept;
    [[nodiscard]] bool operator !=(const StringIteratorT<_C>& other) const noexcept;
};

template<typename _C>
class ConstExprStringT final
{
    DEFAULT_DESTRUCT(ConstExprStringT);
    DEFAULT_CM_PU(ConstExprStringT);
private:
    const _C* _string;
    uSys _length;
    uSys _hash;
public:
    template<uSys _Len>
    constexpr ConstExprStringT(const _C (&str)[_Len]) noexcept;

    template<uSys _Len>
    constexpr ConstExprStringT<_C>& operator=(const _C (&str)[_Len]) noexcept;

    [[nodiscard]] constexpr const _C* c_str() const noexcept { return _string; }
    [[nodiscard]] constexpr operator const _C*() const noexcept { return _string; }
    [[nodiscard]] constexpr uSys length() const noexcept { return _length; }
    [[nodiscard]] constexpr uSys operator()() const noexcept { return _hash; }
    [[nodiscard]] constexpr uSys hashCode() const noexcept { return _hash; }
    
    [[nodiscard]] StringIteratorT<_C> begin() const noexcept { return StringIteratorT<_C>(_string, _length, 0); }
    [[nodiscard]] StringIteratorT<_C>   end() const noexcept { return StringIteratorT<_C>(_string, _length, _length - 1); }

    template<uSys _Len>
    [[nodiscard]] constexpr bool equals(const _C (&str)[_Len]) const noexcept;
    
    [[nodiscard]] bool equals(const ConstExprStringT<_C>& other) const noexcept;
    [[nodiscard]] bool equals(const DynStringT<_C>& other) const noexcept;
    [[nodiscard]] bool equals(const DynStringViewT<_C>& other) const noexcept;
    [[nodiscard]] bool equals(const _C* str) const noexcept;

    [[nodiscard]] i32 compareTo(const ConstExprStringT<_C>& other) const noexcept;
    [[nodiscard]] i32 compareTo(const DynStringT<_C>& other) const noexcept;
    [[nodiscard]] i32 compareTo(const DynStringViewT<_C>& other) const noexcept;
    [[nodiscard]] i32 compareTo(const _C* str) const noexcept;

#if __has_feature(__cpp_impl_three_way_comparison)
    [[nodiscard]] i32 operator <=>(const ConstExprStringT<_C>& other) const noexcept { return compareTo(other); }
    [[nodiscard]] i32 operator <=>(const DynStringT<_C>& other) const noexcept { return compareTo(other); }
    [[nodiscard]] i32 operator <=>(const DynStringViewT<_C>& other) const noexcept { return compareTo(other); }
    [[nodiscard]] i32 operator <=>(const _C* const other) const noexcept { return compareTo(other); }
#endif

    [[nodiscard]] bool operator ==(const ConstExprStringT<_C>& other) const noexcept { return  equals(other); }
    [[nodiscard]] bool operator !=(const ConstExprStringT<_C>& other) const noexcept { return !equals(other); }
    [[nodiscard]] bool operator < (const ConstExprStringT<_C>& other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] bool operator > (const ConstExprStringT<_C>& other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] bool operator <=(const ConstExprStringT<_C>& other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] bool operator >=(const ConstExprStringT<_C>& other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] bool operator ==(const DynStringT<_C>& other) const noexcept { return  equals(other); }
    [[nodiscard]] bool operator !=(const DynStringT<_C>& other) const noexcept { return !equals(other); }
    [[nodiscard]] bool operator < (const DynStringT<_C>& other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] bool operator > (const DynStringT<_C>& other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] bool operator <=(const DynStringT<_C>& other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] bool operator >=(const DynStringT<_C>& other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] bool operator ==(const DynStringViewT<_C>& other) const noexcept { return  equals(other); }
    [[nodiscard]] bool operator !=(const DynStringViewT<_C>& other) const noexcept { return !equals(other); }
    [[nodiscard]] bool operator < (const DynStringViewT<_C>& other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] bool operator > (const DynStringViewT<_C>& other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] bool operator <=(const DynStringViewT<_C>& other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] bool operator >=(const DynStringViewT<_C>& other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] bool operator ==(const _C* const other) const noexcept { return  equals(other); }
    [[nodiscard]] bool operator !=(const _C* const other) const noexcept { return !equals(other); }
    [[nodiscard]] bool operator < (const _C* const other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] bool operator > (const _C* const other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] bool operator <=(const _C* const other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] bool operator >=(const _C* const other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] _C operator [](uSys index) const noexcept;
    [[nodiscard]] _C at(uSys index) const noexcept;
private:
    template<typename _CC>
    friend class ConstExprStringT;

    template<typename _CC>
    friend class DynStringT;

    template<typename _CC>
    friend class DynStringViewT;

    template<typename _CC>
    friend class StringBuilderT;
};

template<typename _C>
class DynStringT final
{
public:
    static DynStringT<_C> passControl(const _C* str) noexcept;
private:
    struct LargeString final
    {
        uSys* refCount;
        const _C* string;
    };

    union
    {
        LargeString _largeString;
        _C _stackString[16];
    };
    uSys _length;
    uSys _hash;
private:
    DynStringT(const _C* string, uSys length) noexcept;
    DynStringT(uSys length) noexcept;
    DynStringT(uSys length, const _C* string) noexcept;
public:
    DynStringT() noexcept;
    DynStringT(const _C* string) noexcept;

    ~DynStringT() noexcept;

    DynStringT(const DynStringT<_C>& copy) noexcept;
    DynStringT(DynStringT<_C>&& move) noexcept;

    DynStringT<_C>& operator=(const DynStringT<_C>& copy) noexcept;
    DynStringT<_C>& operator=(DynStringT<_C>&& move) noexcept;

    DynStringT<_C>& operator=(const _C* string) noexcept;

    [[nodiscard]] [[tau::nonnull]] const _C* c_str() const noexcept { return _length >= 16 ? _largeString.string : _stackString; }
    [[nodiscard]] uSys length() const noexcept { return _length; }

    [[nodiscard]] [[tau::nonnull]] operator const _C*() const noexcept { return _length >= 16 ? _largeString.string : _stackString; }
    [[nodiscard]] uSys operator()() const noexcept { return _hash; }

    [[nodiscard]] uSys hashCode() const noexcept { return _hash; }
    
    [[nodiscard]] bool equals(const ConstExprStringT<_C>& other) const noexcept;
    [[nodiscard]] bool equals(const DynStringT<_C>& other) const noexcept;
    [[nodiscard]] bool equals(const DynStringViewT<_C>& other) const noexcept;
    [[nodiscard]] bool equals(const _C* str) const noexcept;

    [[nodiscard]] i32 compareTo(const ConstExprStringT<_C>& other) const noexcept;
    [[nodiscard]] i32 compareTo(const DynStringT<_C>& other) const noexcept;
    [[nodiscard]] i32 compareTo(const DynStringViewT<_C>& other) const noexcept;
    [[nodiscard]] i32 compareTo(const _C* str) const noexcept;

    [[nodiscard]] StringIteratorT<_C> begin() const noexcept { return StringIteratorT<_C>(_length >= 16 ? _largeString.string : _stackString, _length, 0); }
    [[nodiscard]] StringIteratorT<_C>   end() const noexcept { return StringIteratorT<_C>(_length >= 16 ? _largeString.string : _stackString, _length, _length - 1); }

    [[nodiscard]] DynStringT<_C> concat(const ConstExprStringT<_C>& other) const noexcept;
    [[nodiscard]] DynStringT<_C> concat(const DynStringT<_C>& other) const noexcept;
    [[nodiscard]] DynStringT<_C> concat(const DynStringViewT<_C>& other) const noexcept;
    [[nodiscard]] DynStringT<_C> concat(const _C* other) const noexcept;

    [[nodiscard]] DynStringT<_C> subString([[tau::inclusive]] uSys begin, [[tau::exclusive]] uSys end) const noexcept;
    [[nodiscard]] DynStringT<_C> subStringLen([[tau::inclusive]] uSys begin, uSys length) const noexcept;
    [[nodiscard]] DynStringT<_C> subString([[tau::inclusive]] uSys from) const noexcept;

#if __has_feature(__cpp_impl_three_way_comparison)
    [[nodiscard]] i32 operator <=>(const ConstExprStringT<_C>& other) const noexcept { return compareTo(other); }
    [[nodiscard]] i32 operator <=>(const DynStringT<_C>& other) const noexcept { return compareTo(other); }
    [[nodiscard]] i32 operator <=>(const DynStringViewT<_C>& other) const noexcept { return compareTo(other); }
    [[nodiscard]] i32 operator <=>(const _C* const other) const noexcept { return compareTo(other); }
#endif

    [[nodiscard]] bool operator ==(const ConstExprStringT<_C>& other) const noexcept { return  equals(other); }
    [[nodiscard]] bool operator !=(const ConstExprStringT<_C>& other) const noexcept { return !equals(other); }
    [[nodiscard]] bool operator < (const ConstExprStringT<_C>& other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] bool operator > (const ConstExprStringT<_C>& other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] bool operator <=(const ConstExprStringT<_C>& other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] bool operator >=(const ConstExprStringT<_C>& other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] bool operator ==(const DynStringT<_C>& other) const noexcept { return  equals(other); }
    [[nodiscard]] bool operator !=(const DynStringT<_C>& other) const noexcept { return !equals(other); }
    [[nodiscard]] bool operator < (const DynStringT<_C>& other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] bool operator > (const DynStringT<_C>& other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] bool operator <=(const DynStringT<_C>& other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] bool operator >=(const DynStringT<_C>& other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] bool operator ==(const DynStringViewT<_C>& other) const noexcept { return  equals(other); }
    [[nodiscard]] bool operator !=(const DynStringViewT<_C>& other) const noexcept { return !equals(other); }
    [[nodiscard]] bool operator < (const DynStringViewT<_C>& other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] bool operator > (const DynStringViewT<_C>& other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] bool operator <=(const DynStringViewT<_C>& other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] bool operator >=(const DynStringViewT<_C>& other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] bool operator ==(const _C* const other) const noexcept { return  equals(other); }
    [[nodiscard]] bool operator !=(const _C* const other) const noexcept { return !equals(other); }
    [[nodiscard]] bool operator < (const _C* const other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] bool operator > (const _C* const other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] bool operator <=(const _C* const other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] bool operator >=(const _C* const other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] DynStringT<_C> operator +(const ConstExprStringT<_C>& other) const noexcept { return concat(other); }
    [[nodiscard]] DynStringT<_C> operator +(const DynStringT<_C>&       other) const noexcept { return concat(other); }
    [[nodiscard]] DynStringT<_C> operator +(const DynStringViewT<_C>&   other) const noexcept { return concat(other); }
    [[nodiscard]] DynStringT<_C> operator +(const _C* const             other) const noexcept { return concat(other); }

    [[nodiscard]] _C operator [](uSys index) const noexcept;
    [[nodiscard]] _C at(uSys index) const noexcept;
private:
    [[nodiscard]] DynStringT<_C> _concat(uSys len, const _C* str) const noexcept;
private:
    template<typename _CC>
    friend class ConstExprStringT;

    template<typename _CC>
    friend class DynStringT;

    template<typename _CC>
    friend class DynStringViewT;

    template<typename _CC>
    friend class StringBuilderT;
};

template<typename _C>
class DynStringViewT final
{
private:
    const _C* _string;
    uSys* _refCount;
    uSys  _length;
    uSys  _hash;
private:
    DynStringViewT(uSys begin, uSys length, const DynStringT<_C>& str) noexcept;
    DynStringViewT(uSys begin, uSys length, const DynStringViewT<_C>& str) noexcept;
public:
    DynStringViewT(const DynStringT<_C>& str, uSys begin, uSys end) noexcept;
    DynStringViewT(const DynStringViewT<_C>& str, uSys begin, uSys end) noexcept;

    DynStringViewT(DynStringT<_C>&& str, uSys begin, uSys end) noexcept = delete;
    DynStringViewT(DynStringViewT<_C>&& str, uSys begin, uSys end) noexcept = delete;

    ~DynStringViewT() noexcept;

    inline DynStringViewT(const DynStringViewT<_C>& copy) noexcept;
    inline DynStringViewT(DynStringViewT<_C>&& move) noexcept;

    inline DynStringViewT<_C>& operator=(const DynStringViewT<_C>& copy) noexcept;
    inline DynStringViewT<_C>& operator=(DynStringViewT<_C>&& move) noexcept;

    [[nodiscard]] static inline DynStringViewT<_C> create(const DynStringT<_C>& str, uSys begin, uSys length) noexcept;
    [[nodiscard]] static inline DynStringViewT<_C> create(const DynStringViewT<_C>& str, uSys begin, uSys length) noexcept;

    [[nodiscard]] static inline DynStringViewT<_C> create(DynStringT<_C>&& str, uSys begin, uSys length) noexcept = delete;
    [[nodiscard]] static inline DynStringViewT<_C> create(DynStringViewT<_C>&& str, uSys begin, uSys length) noexcept = delete;

    DynStringViewT<_C>& reset(const DynStringT<_C>& str, uSys begin, uSys end) noexcept;
    DynStringViewT<_C>& resetLen(const DynStringT<_C>& str, uSys begin, uSys length) noexcept;

    DynStringViewT<_C>& reset(DynStringT<_C>&& str, uSys begin, uSys end) noexcept = delete;
    DynStringViewT<_C>& resetLen(DynStringT<_C>&& str, uSys begin, uSys length) noexcept = delete;

    [[nodiscard]] const _C* c_str() const noexcept { return _string; }
    [[nodiscard]] uSys length() const noexcept { return _length; }
    [[nodiscard]] uSys hashCode() const noexcept { return _hash; }

    [[nodiscard]] StringIteratorT<_C> begin() const noexcept { return StringIteratorT<_C>(_string, _length, 0); }
    [[nodiscard]] StringIteratorT<_C>   end() const noexcept { return StringIteratorT<_C>(_string, _length, _length - 1); }

    [[nodiscard]] bool equals(const DynStringT<_C>& other) const noexcept;
    [[nodiscard]] bool equals(const DynStringViewT<_C>& other) const noexcept;
    [[nodiscard]] bool equals(const _C* str) const noexcept;

    [[nodiscard]] i32 compareTo(const DynStringViewT<_C>& other) const noexcept;
    [[nodiscard]] i32 compareTo(const DynStringT<_C>& other) const noexcept;
    [[nodiscard]] i32 compareTo(const _C* str) const noexcept;

    [[nodiscard]] DynStringT<_C> concat(const DynStringT<_C>& other) const noexcept;
    [[nodiscard]] DynStringT<_C> concat(const DynStringViewT<_C>& other) const noexcept;
    [[nodiscard]] DynStringT<_C> concat(const _C* other) const noexcept;

    [[nodiscard]] inline DynStringT<_C> subString([[tau::inclusive]] uSys begin, [[tau::exclusive]] uSys end) const noexcept;
    [[nodiscard]] inline DynString subStringLen([[tau::inclusive]] uSys begin, uSys length) const noexcept;
    [[nodiscard]] inline DynString subString([[tau::inclusive]] uSys from) const noexcept;

#if __has_feature(__cpp_impl_three_way_comparison)
    [[nodiscard]] i32 operator <=>(const ConstExprStringT<_C>g& other) const noexcept { return compareTo(other); }
    [[nodiscard]] i32 operator <=>(const DynStrinT<_C>g& other) const noexcept { return compareTo(other); }
    [[nodiscard]] i32 operator <=>(const DynStringViewT<_C>& other) const noexcept { return compareTo(other); }
    [[nodiscard]] i32 operator <=>(const _C* other) const noexcept { return compareTo(other); }
#endif

    [[nodiscard]] bool operator ==(const ConstExprStringT<_C>& other) const noexcept { return  equals(other); }
    [[nodiscard]] bool operator !=(const ConstExprStringT<_C>& other) const noexcept { return !equals(other); }
    [[nodiscard]] bool operator < (const ConstExprStringT<_C>& other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] bool operator > (const ConstExprStringT<_C>& other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] bool operator <=(const ConstExprStringT<_C>& other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] bool operator >=(const ConstExprStringT<_C>& other) const noexcept { return compareTo(other) >= 0; }


    [[nodiscard]] inline bool operator ==(const DynStringT<_C>& str) const noexcept { return  equals(str); }
    [[nodiscard]] inline bool operator !=(const DynStringT<_C>& str) const noexcept { return !equals(str); }
    [[nodiscard]] inline bool operator < (const DynStringT<_C>& str) const noexcept { return compareTo(str) < 0; }
    [[nodiscard]] inline bool operator > (const DynStringT<_C>& str) const noexcept { return compareTo(str) > 0; }
    [[nodiscard]] inline bool operator <=(const DynStringT<_C>& str) const noexcept { return compareTo(str) <= 0; }
    [[nodiscard]] inline bool operator >=(const DynStringT<_C>& str) const noexcept { return compareTo(str) >= 0; }

    [[nodiscard]] inline bool operator ==(const DynStringViewT<_C>& str) const noexcept { return  equals(str); }
    [[nodiscard]] inline bool operator !=(const DynStringViewT<_C>& str) const noexcept { return !equals(str); }
    [[nodiscard]] inline bool operator < (const DynStringViewT<_C>& str) const noexcept { return compareTo(str) < 0; }
    [[nodiscard]] inline bool operator > (const DynStringViewT<_C>& str) const noexcept { return compareTo(str) > 0; }
    [[nodiscard]] inline bool operator <=(const DynStringViewT<_C>& str) const noexcept { return compareTo(str) <= 0; }
    [[nodiscard]] inline bool operator >=(const DynStringViewT<_C>& str) const noexcept { return compareTo(str) >= 0; }

    [[nodiscard]] inline bool operator ==(const _C* str) const noexcept { return  equals(str); }
    [[nodiscard]] inline bool operator !=(const _C* str) const noexcept { return !equals(str); }
    [[nodiscard]] inline bool operator < (const _C* str) const noexcept { return compareTo(str) < 0; }
    [[nodiscard]] inline bool operator > (const _C* str) const noexcept { return compareTo(str) > 0; }
    [[nodiscard]] inline bool operator <=(const _C* str) const noexcept { return compareTo(str) <= 0; }
    [[nodiscard]] inline bool operator >=(const _C* str) const noexcept { return compareTo(str) >= 0; }

    [[nodiscard]] inline _C operator [](uSys index) const noexcept;
    [[nodiscard]] inline _C at(uSys index) const noexcept;
private:
    [[nodiscard]] DynStringT<_C> _concat(uSys len, const _C* str) noexcept;
private:
    template<typename _CC>
    friend class ConstExprStringT;

    template<typename _CC>
    friend class DynStringT;

    template<typename _CC>
    friend class DynStringViewT;

    template<typename _CC>
    friend class StringBuilderT;
};

template<typename _C>
class StringBuilderT final
{
private:
    _C* _string;
    uSys _length;
    uSys _size;
public:
    StringBuilderT() noexcept;
    StringBuilderT(uSys initialSize) noexcept;
    StringBuilderT(const _C* string) noexcept;

    ~StringBuilderT() noexcept;

    StringBuilderT(const StringBuilderT<_C>& copy) noexcept;
    StringBuilderT(StringBuilderT<_C>&& move) noexcept;

    StringBuilderT<_C>& operator =(const StringBuilderT<_C>& copy) noexcept;
    StringBuilderT<_C>& operator =(StringBuilderT<_C>&& move) noexcept;

    [[nodiscard]] const _C* c_str() const noexcept { return _string; }
    [[nodiscard]] uSys length() const noexcept { return _length; }
    [[nodiscard]] uSys size() const noexcept { return _size; }

    [[nodiscard]] bool equals(const StringBuilderT<_C>& other) const noexcept;

    [[nodiscard]] i32 compareTo(const StringBuilderT<_C>& other) const noexcept;

    [[nodiscard]] StringIteratorT<_C> begin() const noexcept { return StringIteratorT<_C>(_string, _length, 0); }
    [[nodiscard]] StringIteratorT<_C>   end() const noexcept { return StringIteratorT<_C>(_string, _length, _length - 1); }

    inline StringBuilderT<_C>& append(const ConstExprStringT<_C>& string) noexcept;
    inline StringBuilderT<_C>& append(const DynStringT<_C>& string) noexcept;
    inline StringBuilderT<_C>& append(const DynStringViewT<_C>& string) noexcept;
    inline StringBuilderT<_C>& append(const StringBuilderT<_C>& string) noexcept;
    inline StringBuilderT<_C>& append(const _C* string) noexcept;
    inline StringBuilderT<_C>& append(_C c) noexcept;

    inline StringBuilderT<_C>& reset() noexcept;
    inline StringBuilderT<_C>& reset(uSys newSize) noexcept;
    inline StringBuilderT<_C>& resetIf(uSys maxSize, uSys newSize) noexcept;

    inline StringBuilderT<_C>& backspace() noexcept;
    inline StringBuilderT<_C>& backspace(uSys count) noexcept;

#if __has_feature(__cpp_impl_three_way_comparison)
    [[nodiscard]] i32 operator <=> (const StringBuilderT<_C>& other) const noexcept { return compareTo(other); }
#endif

    [[nodiscard]] inline bool operator ==(const StringBuilderT<_C>& other) const noexcept { return  equals(other); }
    [[nodiscard]] inline bool operator !=(const StringBuilderT<_C>& other) const noexcept { return !equals(other); }
    [[nodiscard]] inline bool operator < (const StringBuilderT<_C>& other) const noexcept { return compareTo(other) <  0; }
    [[nodiscard]] inline bool operator > (const StringBuilderT<_C>& other) const noexcept { return compareTo(other) >  0; }
    [[nodiscard]] inline bool operator <=(const StringBuilderT<_C>& other) const noexcept { return compareTo(other) <= 0; }
    [[nodiscard]] inline bool operator >=(const StringBuilderT<_C>& other) const noexcept { return compareTo(other) >= 0; }

    [[nodiscard]] inline StringBuilderT<_C>& operator +(const ConstExprStringT<_C>& other) noexcept { return append(other); }
    [[nodiscard]] inline StringBuilderT<_C>& operator +(const DynStringT<_C>& other) noexcept { return append(other); }
    [[nodiscard]] inline StringBuilderT<_C>& operator +(const DynStringViewT<_C>& other) noexcept { return append(other); }
    [[nodiscard]] inline StringBuilderT<_C>& operator +(const StringBuilderT<_C>& other) noexcept { return append(other); }
    [[nodiscard]] inline StringBuilderT<_C>& operator +(const _C* other) noexcept { return append(other); }

    inline StringBuilderT<_C>& operator +=(const ConstExprStringT<_C>& other) noexcept { return append(other); }
    inline StringBuilderT<_C>& operator +=(const DynStringT<_C>& other) noexcept { return append(other); }
    inline StringBuilderT<_C>& operator +=(const DynStringViewT<_C>& other) noexcept { return append(other); }
    inline StringBuilderT<_C>& operator +=(const StringBuilderT<_C>& other) noexcept { return append(other); }
    inline StringBuilderT<_C>& operator +=(const _C* other) noexcept { return append(other); }

    [[nodiscard]] inline DynStringT<_C> toString() const noexcept { return DynStringT<_C>(_string); }

    [[nodiscard]] inline _C operator[](uSys index) const noexcept;
    [[nodiscard]] inline _C at(uSys index) const noexcept;
private:
    void append(const _C* string, uSys length) noexcept;
};

template<typename _CTo, typename _CFrom>
DynStringT<_CTo> StringCast(const DynStringT<_CFrom>& string) noexcept;

#include "String.inl"
