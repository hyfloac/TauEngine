#pragma once

#include <NumTypes.hpp>

namespace Console {

class Controller;

class Command
{
    DEFAULT_CONSTRUCT_PO(Command);
    DEFAULT_DESTRUCT_VI(Command);
    DELETE_COPY(Command);
public:
    [[nodiscard]] virtual const char* name() const noexcept = 0;
    [[nodiscard]] virtual const char* usage() const noexcept = 0;
    [[nodiscard]] virtual const char* info() const noexcept = 0;
    [[nodiscard]] virtual i32 execute(const char* commandName, const char* args[], u32 argCount, Controller* consoleHandler) noexcept = 0;
};

class CommandClass;

class CommandFunc
{
    DEFAULT_CONSTRUCT_PO(CommandFunc);
    DEFAULT_DESTRUCT_VI(CommandFunc);
    DELETE_COPY(CommandFunc);
public:
    [[nodiscard]] virtual const char* name() const noexcept = 0;
    [[nodiscard]] virtual const char* usage() const noexcept = 0;
    [[nodiscard]] virtual const char* info() const noexcept = 0;
    [[nodiscard]] virtual i32 execute(const char* commandName, const char* args[], u32 argCount, Controller* consoleHandler, CommandClass* commandClass) noexcept = 0;
};

class CommandClass final
{
    DEFAULT_COPY(CommandClass);
    DEFAULT_DESTRUCT(CommandClass);
private:
    const char* _name;
public:
    CommandClass(const char* const name) noexcept
        : _name(name)
    { }

    [[nodiscard]] const char* name() const noexcept { return _name; }


};

}
