#pragma once

#include <NumTypes.hpp>
#include <unordered_map>
#include <String.hpp>
#include <DLL.hpp>

class ConsoleHandler;

typedef i32 (* console_command_handler_f)(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler);

struct CommandContainer final
{
    const char* name;
    const char* usage;
    console_command_handler_f commandHandler;
};

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
    void (* print_f)(const char*);
    void (* println_f)(const char*);
    void (* printnl_f)(void);
    void (* printf_f)(const char*, va_list);
};

class TAU_DLL ConsoleHandler final
{
private:
    std::unordered_map<String, CommandContainer> _commands;
    PrintFunctions _printFunctions;
public:
    ConsoleHandler() noexcept;

    ConsoleHandler(PrintFunctions printFunctions) noexcept;

    ConsoleHandler(const ConsoleHandler& copy) noexcept = default;
    ConsoleHandler(ConsoleHandler&& move) noexcept;

    ConsoleHandler& operator =(const ConsoleHandler& copy) noexcept = default;
    ConsoleHandler& operator =(ConsoleHandler&& move) noexcept;

    ~ConsoleHandler() noexcept = default;

    void addCommand(CommandContainer commandContainer) noexcept;
    void addCommand(const char* name, const char* usage, console_command_handler_f commandHandler) noexcept;

    i32 runCommand(const char* command) noexcept;

    inline i32 operator ()(const char* command) noexcept { return runCommand(command); }

    inline void print(const char* str)       const noexcept { _printFunctions.print_f(str); }
    inline void println(const char* str)     const noexcept { _printFunctions.println_f(str); }
    inline void println()                    const noexcept { _printFunctions.printnl_f(); }
    inline void printf(const char* fmt, ...) const noexcept
    {
        va_list args;
        va_start(args, fmt);

        _printFunctions.printf_f(fmt, args);

        va_end(args);
    }

    inline const char* usage(const String name) noexcept 
    {
        if(_commands.count(name)) { return _commands[name].usage; }
        return nullptr;
    }
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
    static const char* printBooleanAliasesName = "printBooleanAliases";
    static const char* printBooleanAliasesUsage = "printBooleanAliases";
    i32 TAU_DLL printBooleanAliases(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept;

    static const char* addBooleanAliasName = "addBooleanAlias";
    static const char* addBooleanAliasUsage = "addBooleanAlias <alias{string}> <aliasType{boolean}>";
    i32 TAU_DLL addBooleanAlias(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept;

    static const char* removeBooleanAliasName = "removeBooleanAlias";
    static const char* removeBooleanAliasUsage = "removeBooleanAlias <alias{string}>";
    i32 TAU_DLL removeBooleanAlias(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept;

    static const char* exitName = "exit";
    static const char* exitUsage = "exit [exitCode{i32}]";
    i32 TAU_DLL exit(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept;

    static const char* parseIntName = "parseInt";
    static const char* parseIntUsage = "parseInt <kind(i32|u32|i64|u64)> <num{i32}>";
    i32 TAU_DLL parseInt(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept;
}
