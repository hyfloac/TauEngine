#pragma once

#include <llvm/Support/raw_ostream.h>
#include <String.hpp>
#include <Safeties.hpp>

namespace tau { namespace reflection {
    
class Class;

} }

namespace tau { namespace reflection { namespace processing {

class BaseGenerator
{
private:
    ::std::error_code _ec;
    ::llvm::raw_fd_ostream _header;
public:
    BaseGenerator(const DynString& header) noexcept
        : _header(::llvm::StringRef(header.c_str(), header.length()), _ec)
    { }

    void generate() noexcept;
private:
    void printHeaderBegin() noexcept;
    void printBasicMacros() noexcept;
    void printClass() noexcept;
};


class HeaderGenerator
{
private:
    ::std::error_code _ec;
    ::llvm::raw_fd_ostream _header;
public:
    HeaderGenerator(const DynString& header) noexcept
        : _header(::llvm::StringRef(header.c_str(), header.length()), _ec)
    { }

    void generateDummy() noexcept;
    void generateBegin() noexcept;
    void generateClassBody(const Ref<Class>& clazz) noexcept;
private:
    void printHeaderBegin() noexcept;
    void printBasicMacros() noexcept;
    void printDummyMacros() noexcept;
};

} } }
