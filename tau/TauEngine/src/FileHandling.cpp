#pragma warning(push, 0)
#include <cstdio>
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
    data[length] = '\0';

    fseek(file, 0, SEEK_SET);
    fread(data, 1, length, file);
    fclose(file);

    return data;
}

