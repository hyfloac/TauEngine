#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <String.hpp>

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

struct BindingUnion final
{
public:
    enum Type
    {
        Number = 1,
        Str
    };
public:
    Type type;
    union
    {
        i32 number;
        const char* str;
    };
public:
    BindingUnion() noexcept
        : type(static_cast<Type>(0))
        , str(null)
    { }

    BindingUnion(const u32 _number) noexcept
        : type(Number)
        , number(_number)
    { }

    BindingUnion(const DynString& _str) noexcept
        : type(Str)
    {
        char* const tmpStr = new(::std::nothrow) char[_str.length() + 1];
        ::std::memcpy(tmpStr, _str.c_str(), _str.length() + 1);
        str = tmpStr;
    }

    BindingUnion(const char* _str) noexcept
        : type(Str)
        , str(new(::std::nothrow) char[strlen(_str) + 1])
    {
        const uSys len = strlen(_str);
        char* const tmpStr = new(::std::nothrow) char[len + 1];
        ::std::memcpy(tmpStr, _str, len + 1);
        str = tmpStr;
    }

    ~BindingUnion() noexcept
    {
        if(type == Str)
        { delete[] str; }
    }

    BindingUnion(const BindingUnion& copy) noexcept
        : type(copy.type)
    {
        if(type == Str)
        {
            const uSys len = strlen(copy.str);
            char* const tmpStr = new(::std::nothrow) char[len + 1];
            ::std::memcpy(tmpStr, copy.str, len + 1);
            str = tmpStr;
        }
        else if(type == Number)
        { number = copy.number; }
    }

    BindingUnion(BindingUnion&& move) noexcept
        : type(move.type)
    {
        if(type == Str)
        {
            str = move.str;
            move.str = null;
        }
        else if(type == Number)
        { number = move.number; }
    }

    BindingUnion& operator=(const BindingUnion& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(type == Str)
        { delete[] str; }

        type = copy.type;

        if(type == Str)
        {
            const uSys len = strlen(copy.str);
            char* const tmpStr = new(::std::nothrow) char[len + 1];
            ::std::memcpy(tmpStr, copy.str, len + 1);
            str = tmpStr;
        }
        else if(type == Number)
        { number = copy.number; }

        return *this;
    }

    BindingUnion& operator=(BindingUnion&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(type == Str)
        { delete[] str; }

        type = move.type;

        if(type == Str)
        {
            str = move.str;
            move.str = null;
        }
        else if(type == Number)
        { number = move.number; }

        return *this;
    }
};
}
