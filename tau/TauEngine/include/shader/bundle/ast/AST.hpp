#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <String.hpp>
#include <new>

#include "DLL.hpp"

class IShaderBundleVisitor;

namespace sbp {
class TAU_DLL TAU_NOVTABLE AST
{
    DEFAULT_CONSTRUCT_PU(AST);
    DEFAULT_DESTRUCT_VI(AST);
    DEFAULT_CM_PO(AST);
public:
    virtual void visit(IShaderBundleVisitor& visitor) const noexcept = 0;
};

union BindingUnionRaw final
{
    i32 Number;
    u8 StrBuffer[sizeof(C8DynString)];

    BindingUnionRaw() noexcept
        : Number(0)
    { }

    BindingUnionRaw(const i32 number) noexcept
        : Number(number)
    { }

    [[nodiscard]]       C8DynString* AsStringPtr()       noexcept { return reinterpret_cast<      C8DynString*>(StringRawPtr()); }
    [[nodiscard]] const C8DynString* AsStringPtr() const noexcept { return reinterpret_cast<const C8DynString*>(StringRawPtr()); }
    [[nodiscard]]       C8DynString& AsString()       noexcept { return *AsStringPtr(); }
    [[nodiscard]] const C8DynString& AsString() const noexcept { return *AsStringPtr(); }

    [[nodiscard]]       u8* StringRawPtr()       noexcept { return StrBuffer; }
    [[nodiscard]] const u8* StringRawPtr() const noexcept { return StrBuffer; }
};

struct BindingUnion final
{
public:
    enum BindingType
    {
        Number = 1,
        Str
    };
public:
    BindingType Type;
    BindingUnionRaw Union;
public:
    BindingUnion() noexcept
        : Type(static_cast<BindingType>(0))
        , Union(0)
    { }

    BindingUnion(const u32 number) noexcept
        : Type(Number)
        , Union(static_cast<i32>(number))
    { }

    BindingUnion(const C8DynString& str) noexcept
        : Type(Str)
        , Union(0)
    {
        ::new(Union.AsStringPtr()) C8DynString(str);
    }

    BindingUnion(const c8* str) noexcept
        : Type(Str)
        , Union(0)
    {
        ::new(Union.AsStringPtr()) C8DynString(str);
    }

    ~BindingUnion() noexcept
    {
        if(Type == Str)
        { Union.AsString().~DynStringT(); }
    }

    BindingUnion(const BindingUnion& copy) noexcept
        : Type(copy.Type)
    {
        if(Type == Str)
        {
            ::new(Union.AsStringPtr()) C8DynString(copy.Union.AsString());
        }
        else if(Type == Number)
        { Union.Number = copy.Union.Number; }
    }

    BindingUnion(BindingUnion&& move) noexcept
        : Type(move.Type)
    {
        if(Type == Str)
        {
            ::new(Union.AsStringPtr()) C8DynString(::std::move(move.Union.AsString()));
            move.Union.AsString().~DynStringT();
            move.Type = static_cast<BindingType>(0);
        }
        else if(Type == Number)
        { Union.Number = move.Union.Number; }
    }

    BindingUnion& operator=(const BindingUnion& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(Type == Str)
        { Union.AsString().~DynStringT(); }

        Type = copy.Type;

        if(Type == Str)
        {
            ::new(Union.AsStringPtr()) C8DynString(copy.Union.AsString());
        }
        else if(Type == Number)
        { Union.Number = copy.Union.Number; }

        return *this;
    }

    BindingUnion& operator=(BindingUnion&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(Type == Str)
        { Union.AsString().~DynStringT(); }

        Type = move.Type;

        if(Type == Str)
        {
            ::new(Union.AsStringPtr()) C8DynString(::std::move(move.Union.AsString()));
            move.Union.AsString().~DynStringT();
            move.Type = static_cast<BindingType>(0);
        }
        else if(Type == Number)
        { Union.Number = move.Union.Number; }

        return *this;
    }
};
}
