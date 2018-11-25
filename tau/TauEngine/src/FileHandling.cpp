#pragma warning(push, 0)
#include <cstdio>
#include <cstring>
#pragma warning(pop)
#include <NumTypes.hpp>
#include <file/FileHandling.hpp>

char* readFile(const char* filePath) noexcept
{
    FILE* file;
    fopen_s(&file, filePath, "rt");
    fseek(file, 0, SEEK_END);
    const u32 length = ftell(file);
    char* data = new char[length + 1];
    memset(data, 0, length + 1);
    fseek(file, 0, SEEK_SET);
    fread(data, 1, length, file);
    fclose(file);

    return data;
}

