#include <console/ConsoleHandler.hpp>
#include <DynamicallySizedArray.hpp>
#pragma warning(push, 0)
#include <cstdio>
#include <cstdarg>
#pragma warning(pop)

ConsoleHandler::ConsoleHandler(ConsoleHandler&& move) noexcept
{
    this->_commands = move._commands;
}

ConsoleHandler& ConsoleHandler::operator =(ConsoleHandler&& move) noexcept
{
    this->_commands = move._commands;
    return *this;
}

void ConsoleHandler::addCommand(CommandContainer commandContainer) noexcept 
{
    _commands[commandContainer.name] = commandContainer;
}

void ConsoleHandler::addCommand(const char* name, const char* usage, console_command_handler_f commandHandler) noexcept 
{
    _commands[name] = { name, usage, commandHandler };
}

static void splitCommand(const char* command, std::vector<const char*>& sections)
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
            const CommandContainer cc = _commands[sections[0]];
            return cc.commandHandler(sections[0], sections.data() + 1, sections.size() - 1, this);
        }
    }

    return NO_COMMAND_FOUND;
}

void ConsoleHandler::print(const char* str) const noexcept
{
    puts(str);
}

void ConsoleHandler::println(const char* str) const noexcept 
{
    std::printf("%s\n", str);
}

void ConsoleHandler::println() const noexcept 
{
    puts("\n");
}

void ConsoleHandler::printf(const char* fmt, ...) const noexcept 
{
    va_list args;
    va_start(args, fmt);

    vprintf(fmt, args);

    va_end(args);
}

static bool streq(const char* lhs, const char* rhs) noexcept
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
    const u32 len = strlen(str);
    if(len > MAX_BOOL_LEN)
    {
        return BFS_UNKNOWN;
    }

    DSA(char, lower, len + 1);
    lower[len] = '\0';
    toLower(str, lower);

    if(streq(lower, "true")      ||
       streq(lower, "yes")       ||
       streq(lower, "activate")  ||
       streq(lower, "active")    ||
       streq(lower, "1")) { return BFS_TRUE; }

    if(streq(lower, "false")      ||
       streq(lower, "no")         ||
       streq(lower, "deactivate") ||
       streq(lower, "inactive")   ||
       streq(lower, "0")) { return BFS_FALSE; }

    for(const char* alias : boolTrues)
    {
        if(streq(alias, lower)) { return BFS_TRUE; }
    }

    for(const char* alias : boolFalses)
    {
        if(streq(alias, lower)) { return BFS_FALSE; }
    }

    return BFS_UNKNOWN;
}

namespace dc
{

    const char* printBooleanAliasesName = "printBooleanAliases";
    const char* printBooleanAliasesUsage = "printBooleanAliases";

    i32 printBooleanAliases(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept
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
        {
            consoleHandler->printf("true : %s\n", alias);
        }

        for(const char* alias : boolFalses)
        {
            consoleHandler->printf("false : %s\n", alias);
        }

        return 0;
    }

    const char* addBooleanAliasName = "addBooleanAlias";
    const char* addBooleanAliasUsage = "addBooleanAlias <alias{string}> <aliasType{boolean}>";

    i32 addBooleanAlias(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept
    {
        if(argCount >= 2)
        {
            BoolFromStr b = ConsoleHandler::parseBool(args[1]);

            const u32 len = strlen(args[0]);
            if(len > MAX_BOOL_LEN)
            {
                consoleHandler->println("Alias length exceed 32 characters.");
                consoleHandler->printf("Usage: %s\n", addBooleanAliasUsage);
                return 2;
            }
            else
            {
                if(b == BFS_TRUE)
                {
                    char* lower = new char[len + 1];
                    lower[len] = '\0';
                    toLower(args[0], lower);

                    boolTrues.push_back(lower);
                    consoleHandler->printf("Successfully added alias `%s` for type `true`.\n", lower);
                    return 0;
                }

                if(b == BFS_FALSE)
                {
                    char* lower = new char[len + 1];
                    lower[len] = '\0';
                    toLower(args[0], lower);

                    boolFalses.push_back(lower);
                    consoleHandler->printf("Successfully added alias `%s` for type `false`.\n", lower);
                    return 0;
                }

                consoleHandler->printf("Unknown alias type `%s`.\n", args[1]);
                consoleHandler->printf("Usage: %s\n", addBooleanAliasUsage);
                return 1;
            }
        }
        else
        {
            consoleHandler->println("Not enough arguments.");
            consoleHandler->printf("Usage: %s\n", addBooleanAliasUsage);
            return -1;
        }
    }

    const char* removeBooleanAliasName = "removeBooleanAlias";
    const char* removeBooleanAliasUsage = "removeBooleanAlias <alias{string}>";

    i32 removeBooleanAlias(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept
    {
        if(argCount >= 1)
        {
            const u32 len = strlen(args[0]);
            if(len > MAX_BOOL_LEN)
            {
                consoleHandler->println("Alias length exceed 32 characters.");
                consoleHandler->printf("Usage: %s\n", removeBooleanAliasUsage);
                return 2;
            }

            DSA(char, lower, len + 1);
            lower[len] = '\0';
            toLower(args[0], lower);

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
            consoleHandler->printf("Usage: %s\n", removeBooleanAliasUsage);
            return 1;
        }

        consoleHandler->println("Not enough arguments.");
        consoleHandler->printf("Usage: %s\n", removeBooleanAliasUsage);
        return -1;
    }
}
