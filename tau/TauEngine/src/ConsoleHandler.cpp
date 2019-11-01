#include <TauEngine.hpp>
#include <console/ConsoleHandler.hpp>
#include <VariableLengthArray.hpp>

#pragma warning(push, 0)
#include <cstdio>
#include <cstdarg>
#pragma warning(pop)

ConsoleHandler::ConsoleHandler() noexcept
    : _commands(),
      _printFunctions({
        [](void*, const char* str) { fputs(str, stdout); },
        [](void*, const char* str) { puts(str); },
        [](void*) { puts(""); },
        [](void*, const char* str, va_list args) { vprintf(str, args); }
                      }),
      _userParam(nullptr)
{ }

ConsoleHandler::ConsoleHandler(PrintFunctions printFunctions, void* userParam) noexcept
    : _commands(), _printFunctions(printFunctions), _userParam(userParam)
{ }

// ConsoleHandler::ConsoleHandler(ConsoleHandler&& move) noexcept
//     : _commands(std::move(move._commands)), _printFunctions(move._printFunctions), _userParam(move._userParam)
// { }

ConsoleHandler::~ConsoleHandler() noexcept
{
    for(auto& pair : _commands)
    {
        delete pair.second;
    }
}

// ConsoleHandler& ConsoleHandler::operator =(ConsoleHandler&& move) noexcept
// {
//     this->_commands = move._commands;
//     this->_printFunctions = move._printFunctions;
//     this->_userParam = move._userParam;
//     return *this;
// }

void ConsoleHandler::addCommand(Command* command) noexcept 
{
    _commands[command->name()] = command;
}

// void ConsoleHandler::addCommand(const char* name, const char* usage, console_command_handler_f commandHandler, void* userParam) noexcept
// {
//     _commands[name] = { name, usage, commandHandler, userParam };
// }

static void splitCommand(const char* RESTRICT command, std::vector<const char*>& RESTRICT sections)
{
    u32 count = 0;

    while(*command != '\0')
    {
        if(*command == ' ')
        {
            char* str = new char[count + 1];
            str[count] = '\0';
            memcpy(str, command - count, count);
            count = 0;
            sections.push_back(str);
            do
            {
                ++command;
            } while(*command == ' ');

            if(*command == '\0') { break; }
            
            // if(*command == '"')
            // {
            //     ++command;
            //     do
            //     {
            //         ++count;
            //         if(*command == '\\')
            //         {
            //             ++command;
            //             if(*command == '"') { ++command; }
            //         }
            //     } while(*command != '"' && *command != '\0');
            //
            //     if(*command == '\0') { break; }
            // }

            continue;
        }

        ++count;
        ++command;
    }

    if(count)
    {
        char* str = new char[count + 1];
        str[count] = '\0';
        memcpy(str, command - count, count);
        sections.push_back(str);
    }
}

i32 ConsoleHandler::runCommand(const char* command) noexcept 
{
    std::vector<const char*> sections;
    splitCommand(command, sections);
    if(!sections.empty())
    {
        if(_commands.count(sections[0]))
        {
            Command* cc = _commands[sections[0]];
            return cc->execute(sections[0], sections.data() + 1, static_cast<u32>(sections.size() - 1), this);
        }
    }

    return NO_COMMAND_FOUND;
}

bool ConsoleHandler::addAlias(const String commandName, const String aliasName) noexcept
{
    if(_commands.count(commandName))
    {
        Command* cc = _commands[commandName];
        _commands[aliasName] = cc;
        return true;
    }
    return false;
}

static bool streq(const char* RESTRICT lhs, const char* RESTRICT rhs) noexcept
{
    while(*lhs != '\0' && *rhs != '\0')
    {
        if(*lhs != *rhs)
        {
            return false;
        }
        ++lhs;
        ++rhs;
    }

    return *lhs == '\0' && *rhs == '\0';
}

#define MAX_BOOL_LEN 32

static std::vector<const char*> boolTrues;
static std::vector<const char*> boolFalses;

BoolFromStr ConsoleHandler::parseBool(const char* str) noexcept
{
    const u32 len = static_cast<u32>(strlen(str));
    if(len > MAX_BOOL_LEN) { return BoolFromStr::BFS_UNKNOWN; }

    VLA(char, lower, len + 1);
    lower[len] = '\0';
    toLower(str, lower);

    if(streq(lower, "true")      ||
       streq(lower, "yes")       ||
       streq(lower, "activate")  ||
       streq(lower, "active")    ||
       streq(lower, "1")) { return BoolFromStr::BFS_TRUE; }

    if(streq(lower, "false")      ||
       streq(lower, "no")         ||
       streq(lower, "deactivate") ||
       streq(lower, "inactive")   ||
       streq(lower, "0")) { return BoolFromStr::BFS_FALSE; }

    for(const char* alias : boolTrues)
    {
        if(streq(alias, lower)) { return BoolFromStr::BFS_TRUE; }
    }

    for(const char* alias : boolFalses)
    {
        if(streq(alias, lower)) { return BoolFromStr::BFS_FALSE; }
    }

    return BoolFromStr::BFS_UNKNOWN;
}

#define VALIDATE(__CHECK, __ERROR) if(!(__CHECK)) { *error = __ERROR; return 0; }

u32 ConsoleHandler::parseU32(const char* RESTRICT str, ParseIntError* RESTRICT error) noexcept
{
#define MAX_U32_LEN (10)

    const u32 len = static_cast<u32>(strlen(str));

    VALIDATE(len <= MAX_U32_LEN, ParseIntError::STRING_TOO_LONG);
    VALIDATE(len != 0, ParseIntError::ZERO_LENGTH_STR);
    VALIDATE(str[0] != '-', ParseIntError::SIGNED_NUMBER_IN_UNSIGNED_PARSE);

    u32 ret = 0;

    for(u32 i = 0; i < len; ++i)
    {
        ret *= 10;

        switch(str[i])
        {
            case '0': break;
            case '1': 
                ret += 1;
                break;
            case '2':
                ret += 2;
                break;
            case '3':
                ret += 3;
                break;
            case '4':
                ret += 4;
                break;
            case '5':
                ret += 5;
                break;
            case '6':
                ret += 6;
                break;
            case '7':
                ret += 7;
                break;
            case '8':
                ret += 8;
                break;
            case '9':
                ret += 9;
                break;
            default:
                *error = ParseIntError::INVALID_CHARACTER;
                return 0;
        }
    }

#undef MAX_U32_LEN

    *error = ParseIntError::NONE;

    return ret;
}

i32 ConsoleHandler::parseI32(const char* RESTRICT str, ParseIntError* RESTRICT error) noexcept
{
#define MAX_I32_LEN (11)

    const u32 len = static_cast<u32>(strlen(str));

    VALIDATE(len <= MAX_I32_LEN, ParseIntError::STRING_TOO_LONG);
    VALIDATE(len != 0, ParseIntError::ZERO_LENGTH_STR);

    bool negative = false;
    if(str[0] == '-') { negative = true; }

    i32 ret = 0;

    for(u32 i = negative ? 1 : 0; i < len; ++i)
    {
        ret *= 10;

        switch(str[i])
        {
            case '0': break;
            case '1':
                ret += 1;
                break;
            case '2':
                ret += 2;
                break;
            case '3':
                ret += 3;
                break;
            case '4':
                ret += 4;
                break;
            case '5':
                ret += 5;
                break;
            case '6':
                ret += 6;
                break;
            case '7':
                ret += 7;
                break;
            case '8':
                ret += 8;
                break;
            case '9':
                ret += 9;
                break;
            default:
                *error = ParseIntError::INVALID_CHARACTER;
                return 0;
        }
    }

#undef MAX_I32_LEN

    *error = ParseIntError::NONE;

    if(negative) { ret *= -1; }

    return ret;
}

u64 ConsoleHandler::parseU64(const char* RESTRICT str, ParseIntError* RESTRICT error) noexcept
{
#define MAX_U64_LEN (20)

    const u32 len = static_cast<u32>(strlen(str));

    VALIDATE(len <= MAX_U64_LEN, ParseIntError::STRING_TOO_LONG);
    VALIDATE(len != 0, ParseIntError::ZERO_LENGTH_STR);
    VALIDATE(str[0] != '-', ParseIntError::SIGNED_NUMBER_IN_UNSIGNED_PARSE);

    u64 ret = 0;

    for(u32 i = 0; i < len; ++i)
    {
        ret *= 10;

        switch(str[i])
        {
            case '0': break;
            case '1':
                ret += 1;
                break;
            case '2':
                ret += 2;
                break;
            case '3':
                ret += 3;
                break;
            case '4':
                ret += 4;
                break;
            case '5':
                ret += 5;
                break;
            case '6':
                ret += 6;
                break;
            case '7':
                ret += 7;
                break;
            case '8':
                ret += 8;
                break;
            case '9':
                ret += 9;
                break;
            default:
                *error = ParseIntError::INVALID_CHARACTER;
                return 0;
        }
    }

#undef MAX_U32_LEN

    *error = ParseIntError::NONE;

    return ret;
}

i64 ConsoleHandler::parseI64(const char* RESTRICT str, ParseIntError* RESTRICT error) noexcept
{
#define MAX_I64_LEN (20)

    const u32 len = static_cast<u32>(strlen(str));

    VALIDATE(len <= MAX_I64_LEN, ParseIntError::STRING_TOO_LONG);
    VALIDATE(len != 0, ParseIntError::ZERO_LENGTH_STR);

    bool negative = false;
    if(str[0] == '-') { negative = true; }

    i64 ret = 0;

    for(u32 i = 1; i < len; ++i)
    {
        ret *= 10;

        switch(str[i])
        {
            case '0': break;
            case '1':
                ret += 1;
                break;
            case '2':
                ret += 2;
                break;
            case '3':
                ret += 3;
                break;
            case '4':
                ret += 4;
                break;
            case '5':
                ret += 5;
                break;
            case '6':
                ret += 6;
                break;
            case '7':
                ret += 7;
                break;
            case '8':
                ret += 8;
                break;
            case '9':
                ret += 9;
                break;
            default:
                *error = ParseIntError::INVALID_CHARACTER;
                return 0;
        }
    }

#undef MAX_I32_LEN

    *error = ParseIntError::NONE;

    if(negative) { ret *= -1; }

    return ret;
}

f32 ConsoleHandler::parseF32(const char* RESTRICT str, ParseIntError* RESTRICT error) noexcept
{
    *error = ParseIntError::NONE;
    return std::strtof(str, nullptr);
}

f64 ConsoleHandler::parseF64(const char* RESTRICT str, ParseIntError* RESTRICT error) noexcept
{
    *error = ParseIntError::NONE;
    return std::strtod(str, nullptr);
}

#undef VALIDATE

namespace dc
{
    i32 BoolAliasCommand::execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept
    {
        if(argCount == 1)
        {
            if(streq(args[0], "print"))
            {
                consoleHandler->printf("true : %s\n", "true");
                consoleHandler->printf("true : %s\n", "yes");
                consoleHandler->printf("true : %s\n", "activate");
                consoleHandler->printf("true : %s\n", "active");
                consoleHandler->printf("true : %s\n", "1");

                consoleHandler->printf("false : %s\n", "false");
                consoleHandler->printf("false : %s\n", "no");
                consoleHandler->printf("false : %s\n", "deactivate");
                consoleHandler->printf("false : %s\n", "inactive");
                consoleHandler->printf("false : %s\n", "0");

                consoleHandler->println("\nUser Defined:");

                for(const char* alias : boolTrues)
                { consoleHandler->printf("true : %s\n", alias); }

                for(const char* alias : boolFalses)
                { consoleHandler->printf("false : %s\n", alias); }
            }
            else { return unknownCommand(args[0], consoleHandler); }
        }
        else if(argCount == 2)
        {
            if(streq(args[0], "remove"))
            {
                const u32 len = static_cast<u32>(strlen(args[1]));
                if(len > MAX_BOOL_LEN)
                {
                    consoleHandler->println("Alias length exceed 32 characters.");
                    consoleHandler->printf("Usage: %s\n", usage());
                    return 2;
                }

                VLA(char, lower, len + 1);
                lower[len] = '\0';
                toLower(args[1], lower);

                for(std::vector<const char*>::iterator iter = boolTrues.begin(); iter != boolTrues.end(); ++iter)
                {
                    if(streq(*iter, lower))
                    {
                        boolTrues.erase(iter);
                        consoleHandler->printf("Successfully removed alias `%s` for type `true`.", lower);
                        return 0;
                    }
                }

                for(std::vector<const char*>::iterator iter = boolFalses.begin(); iter != boolFalses.end(); ++iter)
                {
                    if(streq(*iter, lower))
                    {
                        boolFalses.erase(iter);
                        consoleHandler->printf("Successfully removed alias `%s` for type `false`.", lower);
                        return 0;
                    }
                }

                consoleHandler->printf("Unknown alias `%s`.", lower);
                consoleHandler->printf("Usage: %s\n", usage());
                return 3;
            }
            else { return unknownCommand(args[0], consoleHandler); }
        }
        else if(argCount == 3)
        {
            if(streq(args[0], "add"))
            {
                const BoolFromStr b = ConsoleHandler::parseBool(args[2]);

                const u32 len = static_cast<u32>(strlen(args[1]));
                if(len > MAX_BOOL_LEN)
                {
                    consoleHandler->println("Alias length exceed 32 characters.");
                    consoleHandler->printf("Usage: %s\n", usage());
                    return 2;
                }
                else
                {
                    if(b == BoolFromStr::BFS_TRUE)
                    {
                        char* lower = new char[len + 1];
                        lower[len] = '\0';
                        toLower(args[0], lower);

                        boolTrues.push_back(lower);
                        consoleHandler->printf("Successfully added alias `%s` for type `true`.\n", lower);
                        return 0;
                    }

                    if(b == BoolFromStr::BFS_FALSE)
                    {
                        char* lower = new char[len + 1];
                        lower[len] = '\0';
                        toLower(args[0], lower);

                        boolFalses.push_back(lower);
                        consoleHandler->printf("Successfully added alias `%s` for type `false`.\n", lower);
                        return 0;
                    }

                    consoleHandler->printf("Unknown alias type `%s`.\n", args[1]);
                    consoleHandler->printf("Usage: %s\n", usage());
                    return 1;
                }
            }
            else { return unknownCommand(args[0], consoleHandler); }
        }
        else
        {
            consoleHandler->println("Incorrect argument count.");
            consoleHandler->printf("Usage: %s\n", usage());
            return -1;
        }

        return 0;
    }

    i32 BoolAliasCommand::unknownCommand(const char* commandName, ConsoleHandler* consoleHandler) const noexcept
    {
        consoleHandler->printf("Unknown command `%s`.", commandName);
        consoleHandler->printf("Usage: %s\n", usage());
        return 4;
    }

    i32 ExitCommand::execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept
    {
        i32 exitCode = 0;

        if(argCount >= 1)
        {
            ParseIntError error;
            exitCode = ConsoleHandler::parseI32(args[0], &error);
            if(error != ParseIntError::NONE) { consoleHandler->printf("Unknown exit code `%s`.", args[0]); }
        }

        consoleHandler->printf("Exiting engine with code `%d`.", exitCode);

        tauExit(exitCode);

        return 0;
    }

    i32 ParseNumCommand::execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept
    {
        if(argCount < 2)
        {
            consoleHandler->println("Not enough arguments.");
            consoleHandler->printf("Usage: %s\n", usage());
            return -1;
        }

#define PARSE_INT_TYPE(__TYPE, __FUNC, __PRINT_TYPE) \
        if(streq(args[0], #__TYPE)) \
        { \
            ParseIntError error; \
            const __TYPE x = ConsoleHandler::__FUNC(args[1], &error); \
            if(error != ParseIntError::NONE) { consoleHandler->printf("Unknown Input: %s\n", args[1]); } \
            else { consoleHandler->printf(#__TYPE ": " __PRINT_TYPE "\n", x); } \
        }

        PARSE_INT_TYPE(i32, parseI32, "%d")
        else
        PARSE_INT_TYPE(u32, parseU32, "%u")
        else
        PARSE_INT_TYPE(i64, parseI64, "%ll")
        else
        PARSE_INT_TYPE(u64, parseU64, "%ull")
        else
        PARSE_INT_TYPE(u64, parseF32, "%f")
        else
        PARSE_INT_TYPE(u64, parseF64, "%f")
        else
        {
            consoleHandler->printf("Unknown Type: %s\n", args[1]);
        }

        return 0;
    }

    i32 AliasCommand::execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept
    {
        if(argCount == 2)
        {
            return consoleHandler->addAlias(args[0], args[1]) ? 0 : -1;
        }
        return 1;
    }

    i32 HelpCommand::execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept
    {
        if(argCount == 1)
        {
            const char* usage = consoleHandler->usage(args[0]);
            if(!usage)
            {
                consoleHandler->printf("Command `%s` not found.", args[0]);
                return -1;
            }

            consoleHandler->println(usage);
            return 0;
        }
        return 1;
    }

    i32 InfoCommand::execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept
    {
        if(argCount == 1)
        {
            const char* info = consoleHandler->info(args[0]);
            if(!info)
            {
                consoleHandler->printf("Command `%s` not found.", args[0]);
                return -1;
            }

            consoleHandler->println(info);
            return 0;
        }
        return 1;
    }
}
