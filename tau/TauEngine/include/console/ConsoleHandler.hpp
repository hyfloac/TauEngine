#pragma once

#include <NumTypes.hpp>
#include <unordered_map>
#include <String.hpp>
#include <DLL.hpp>
#include <cstdarg>
#include <Objects.hpp>

class ConsoleHandler;

#define NO_COMMAND_FOUND 849216 /* It's a number, nothing special. */

enum class BoolFromStr : i8
{
    BFS_FALSE = 0,
    BFS_TRUE = 1,
    BFS_UNKNOWN = -1
};

enum class ParseIntError : u8
{
    NONE,
    SIGNED_NUMBER_IN_UNSIGNED_PARSE,
    STRING_TOO_LONG,
    INVALID_CHARACTER,
    ZERO_LENGTH_STR,
    FLOAT_IN_INT_PARSE
};

struct PrintFunctions final
{
    void (* __cdecl print_f)(void*, const char*);
    void (* __cdecl println_f)(void*, const char*);
    void (* __cdecl printnl_f)(void*);
    void (* __cdecl printf_f)(void*, const char*, va_list);
};

class TAU_DLL ConsoleHandler final
{
    DEFAULT_COPY(ConsoleHandler);
public:
    class Command
    {
        DEFAULT_CONSTRUCT_PO(Command);
        DEFAULT_DESTRUCT_VI(Command);
        DELETE_COPY(Command);
    public:
        [[nodiscard]] virtual const char* name() const noexcept = 0;
        [[nodiscard]] virtual const char* usage() const noexcept = 0;
        [[nodiscard]] virtual const char* info() const noexcept = 0;
        [[nodiscard]] virtual i32 execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept = 0;
    };
private:
    std::unordered_map<String, Command*> _commands;
    PrintFunctions _printFunctions;
    void* _userParam;
public:
    ConsoleHandler() noexcept;

    ConsoleHandler(PrintFunctions printFunctions, void* userParam) noexcept;

    ~ConsoleHandler() noexcept;

    void addCommand(Command* command) noexcept;
    // void addCommand(const char* name, const char* usage, console_command_handler_f commandHandler, void* userParam) noexcept;

    i32 runCommand(const char* command) noexcept;

    inline i32 operator ()(const char* command) noexcept { return runCommand(command); }

    inline void print(const char* str)       const noexcept { _printFunctions.print_f(_userParam, str); }
    inline void println(const char* str)     const noexcept { _printFunctions.println_f(_userParam, str); }
    inline void println()                    const noexcept { _printFunctions.printnl_f(_userParam); }
    inline void printf(const char* fmt, ...) const noexcept
    {
        va_list args;
        va_start(args, fmt);

        _printFunctions.printf_f(_userParam, fmt, args);

        va_end(args);
    }

    inline const char* usage(const String name) noexcept 
    {
        if(_commands.count(name)) { return _commands[name]->usage(); }
        return nullptr;
    }

    inline const char* info(const String name) noexcept
    {
        if(_commands.count(name)) { return _commands[name]->info(); }
        return nullptr;
    }

    bool addAlias(const String commandName, const String aliasName) noexcept;
public:
    static BoolFromStr parseBool(const char* str) noexcept;
    static u32 parseU32(const char* RESTRICT str, ParseIntError* RESTRICT error) noexcept;
    static i32 parseI32(const char* RESTRICT str, ParseIntError* RESTRICT error) noexcept;
    static u64 parseU64(const char* RESTRICT str, ParseIntError* RESTRICT error) noexcept;
    static i64 parseI64(const char* RESTRICT str, ParseIntError* RESTRICT error) noexcept;
    static f32 parseF32(const char* RESTRICT str, ParseIntError* RESTRICT error) noexcept;
    static f64 parseF64(const char* RESTRICT str, ParseIntError* RESTRICT error) noexcept;
};

namespace dc //default_commands
{
    class TAU_DLL BoolAliasCommand final : public ConsoleHandler::Command
    {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "boolAlias"; }
        [[nodiscard]] const char* usage() const noexcept override { return "boolAlias <cmd{enum{print|add|remove}}> <(add|remove)aliasName{string}> <(add)remove{boolean}>"; }
        [[nodiscard]] const char* info() const noexcept override { return "Controls boolean aliases."; }
        [[nodiscard]] i32 execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler * consoleHandler) noexcept override;
    private:
        i32 unknownCommand(const char* commandName, ConsoleHandler* consoleHandler) const noexcept;
    };

    class TAU_DLL ExitCommand final : public ConsoleHandler::Command
    {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "exit"; }
        [[nodiscard]] const char* usage() const noexcept override { return "exit [exitCode{i32}]."; }
        [[nodiscard]] const char* info() const noexcept override { return "Exits with an optional exit code"; }
        [[nodiscard]] i32 execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler * consoleHandler) noexcept override;
    };

    class TAU_DLL ParseNumCommand final : public ConsoleHandler::Command
    {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "parseNum"; }
        [[nodiscard]] const char* usage() const noexcept override { return "parseInt <kind{i32|u32|i64|u64|f32|f64}> <num{i32|u32|i64|u64|f32|f64}>"; }
        [[nodiscard]] const char* info() const noexcept override { return "Parses a number and reprints it."; }
        [[nodiscard]] i32 execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler * consoleHandler) noexcept override;
    };

    class TAU_DLL AliasCommand final : public ConsoleHandler::Command
    {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "alias"; }
        [[nodiscard]] const char* usage() const noexcept override { return "alias <commandName{string}> <aliasName{string}>"; }
        [[nodiscard]] const char* info() const noexcept override { return "Adds a command alias."; }
        [[nodiscard]] i32 execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler * consoleHandler) noexcept override;
    };

    class TAU_DLL HelpCommand final : public ConsoleHandler::Command
    {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "help"; }
        [[nodiscard]] const char* usage() const noexcept override { return "help <commandName{string}>"; }
        [[nodiscard]] const char* info() const noexcept override { return "Prints the help info for a command."; }
        [[nodiscard]] i32 execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept override;
    };

    class TAU_DLL InfoCommand final : public ConsoleHandler::Command
    {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "info"; }
        [[nodiscard]] const char* usage() const noexcept override { return "info <commandName{string}>"; }
        [[nodiscard]] const char* info() const noexcept override { return "Prints the info for a command."; }
        [[nodiscard]] i32 execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler * consoleHandler) noexcept override;
    };

}
