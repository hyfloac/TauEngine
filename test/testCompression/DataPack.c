#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "DataPack.h"
#include "sha1.h"

#define MEMBER_SIZE(__TYPE,__MEMBER) sizeof(((__TYPE *) 0)->__MEMBER)

static void packFiles(const char* const packFileName, const DPCompression compressionType, const u16 fileCount, char* const fileNames[]);
static void unpackFiles(const char* const packFileName);
static void printDPInfo(const char* const packFileName);

static void printHelp(const char* executable)
{
    printf("%s <flag|Flag> [packedFileName|string] [files|file...]\n", executable);
    puts(  "    -p        Pack Files");
    puts(  "    -u        Unpack Files");
    puts(  "    -c        Print Compilation Info");
}

int main(int argCount, char* args[])
{
    if(argCount < 2)
    {
        printHelp(args[0]);
        return 1;
    }

    const char* const flag = args[1];

    if(strcmp(flag, "-p") == 0)
    {
        const char* const packedFileName = args[2];
        packFiles(packedFileName, NONE, argCount - 3, args + 3);
    }
    else if(strcmp(flag, "-u") == 0)
    {
        const char* const packedFileName = args[2];
        unpackFiles(packedFileName);
    }
    else if(strcmp(flag, "-c") == 0)
    {
        printf("Program Compiled on %s at %s.\n", __DATE__, __TIME__);
    }
    else if(strcmp(flag, "-i") == 0)
    {
        const char* const packedFileName = args[2];
        printDPInfo(packedFileName);
    }
    else
    {
        printHelp(args[0]);
        return 2;
    }

    return 0;
}

static u32 getFileSize(FILE* file)
{
    fseek(file, 0, SEEK_END);
    const u32 length = ftell(file);
    fseek(file, 0, SEEK_SET);
    return length;
}

struct FileInfo
{
    u32 length;
    u8* data;
};

static struct FileInfo readFile(const char* const filePath)
{
    FILE* file;
    fopen_s(&file, filePath, "rt");
    const u32 length = getFileSize(file);
    u8* const data = malloc((length + 1) * sizeof(u8));
    data[length] = '\0';

    fread(data, 1, length, file);
    fclose(file);

    const struct FileInfo ret = { length, data };
    return ret;
}

// static void unpackFiles(const char* const fileName)
// {
//     struct DataPack dp;

//     FILE* packedFile;
//     fopen_s(&packedFile, fileName, "r");

//     fwrite(&dp.header,         MEMBER_SIZE(DataPack, header),            1, packedFile);
//     fwrite(&dp.data.byteCount, MEMBER_SIZE(DPCompressedData, byteCount), 1, packedFile);
//     fwrite( dp.data.sha1,      MEMBER_SIZE(DPCompressedData, sha1),      1, packedFile);
//     fwrite( dp.data.md5,       MEMBER_SIZE(DPCompressedData, md5),       1, packedFile);

//     dp.data.bytes = malloc(sizeof(u8) * dp.data.byteCount);

//     fread(dp.data.bytes, sizeof(u8), dp.data.byteCount, packedFile);

//     fclose(packedFile);

//     u32 offset = 0;

//     for(u32 i = 0; i <= dp.header.fileCount; ++i)
//     {
//         struct DPContents decomp;

//         decomp.header.fileSize = *((u32*) (dp.data.bytes + offset));
//         offset += MEMBER_SIZE(DPFileHeader, fileSize);
//         decomp.header.fileName = (const char*) (dp.data.bytes + offset);
//         const u32 nameLen = strlen(fileName) + 1;
//         offset += nameLen;
//         decomp.header.fileType = *((enum DPFileType*) (dp.data.bytes + offset));
//         offset += MEMBER_SIZE(DPFileHeader, fileType);
//         memcpy(decomp.header.sha1, dp.data.bytes + offset, MEMBER_SIZE(DPFileHeader, sha1));
//         offset += MEMBER_SIZE(DPFileHeader, sha1);
//         memcpy(decomp.header.md5, dp.data.bytes + offset, MEMBER_SIZE(DPFileHeader, md5));
//         offset += MEMBER_SIZE(DPFileHeader, md5);

//         decomp.bytes = malloc(sizeof(u8) * decomp.header.fileSize);
//         memcpy(decomp.bytes, dp.data.bytes + offset, sizeof(u8) * decomp.header.fileSize);

//         offset += decomp.header.fileSize;

//         printf("Preparing to write file: %s", decomp.header.fileName);

//         FILE* writeFile;
//         fopen_s(&writeFile, decomp.header.fileName, "w");
//         fwrite(decomp.bytes, sizeof(u8), decomp.header.fileSize, writeFile);
//         fclose(writeFile);

//         free(decomp.bytes);

//         printf("Finished writing file: %s\n", decomp.header.fileName);
//     }

//     free(dp.data.bytes);
// }

static void unpackFiles(const char* const fileName)
{
    FILE* packFile;
    errno_t fileError = fopen_s(&packFile, fileName, "r");
    if(fileError)
    {
        printf("Failed to open pack file: %s\n", fileName);
        return;
    }

    DPHeader header;
    fread(&header, sizeof(DPHeader), 1, packFile);

    if(header.magic != DP_MAGIC)
    {
        puts("Pack file does not contain the proper header.");
        return;
    }



    fclose(packFile);
}

static void printDPInfo(const char* const packFileName)
{
    FILE* packFile;
    errno_t fileError = fopen_s(&packFile, packFileName, "rb");
    if(fileError)
    {
        printf("Failed to pack file: %s\n", packFileName);
        return;
    }
    
    DPHeader header;
    fread(&header, sizeof(header), 1, packFile);

    if(header.magic != DP_MAGIC)
    {
        puts("Pack file header does is not correct.");
        fclose(packFile);
        return;
    }

    printf("File Count: %d\n", header.fileCount);
    printf("Compression Type: (0x%X) %s\n", header.compressionType, dpCompStr(header.compressionType));

    puts("Files:");

    char** fileNames = malloc(sizeof(char*) * header.fileCount);

    for(u32 i = 0; i < header.fileCount; ++i)
    {
        u16 fileLen;
        fread(&fileLen, sizeof(fileLen), 1, packFile);
        char* fileName = malloc(sizeof(char) * fileLen);
        fread(fileName, sizeof(char), fileLen, packFile);

        fileNames[i] = fileName;
    }

    for(u32 i = 0; i < header.fileCount; ++i)
    {
        DPFileHeader fileHeader;
        fread(&fileHeader, sizeof(fileHeader), 1, packFile);

        printf("  File: %s\n", fileNames[i]);
        printf("    Size: %d bytes\n", fileHeader.fileSize);
        printf("    Type: %s\n", "Text");
        printf("    Pack File Offset: 0x%llX (%" PRIu64 ")\n", fileHeader.ptrToFile, fileHeader.ptrToFile);

        free(fileNames[i]);
    }

    free(fileNames);
    fclose(packFile);
}

static void packFiles(const char* const packFileName, const DPCompression compressionType, const u16 fileCount, char* const fileNames[])
{
    FILE* packFile;
    errno_t fileError = fopen_s(&packFile, packFileName, "wb");
    if(fileError)
    {
        printf("Failed to open output file: %s\n", packFileName);
        return;
    }

    DPHeader header = { DP_MAGIC, fileCount, compressionType };
    fwrite(&header, sizeof(header), 1, packFile);

    DPFileHeader* dictionary = malloc(sizeof(DPFileHeader) * header.fileCount);
    
    u16* ignoredIndices = malloc(sizeof(u32) * fileCount);
    u16 currentIgnoreIndex = 0; 

    u32 dictionaryIndex = 0;
    for(u32 i = 0; i < fileCount; ++i)
    {
        FILE* inFile;
        fileError = fopen_s(&inFile, fileNames[i], "r");
        if(fileError)
        {
            printf("Failed to open input file: %s\n", packFileName);
            puts("    Skipping file.");
            --header.fileCount;
            ignoredIndices[currentIgnoreIndex++] = i;

            fseek(packFile, 0 + MEMBER_SIZE(DPHeader, magic), SEEK_SET);
            fwrite(&header.fileCount, MEMBER_SIZE(DPHeader, fileCount), 1, packFile);
            fseek(packFile, 0, SEEK_END);

            continue;
        }

        DPFileHeader header;
        header.fileSize = getFileSize(inFile);
        header.fileType = TEXT;
        header.ptrToFile = 0;

        fclose(inFile);

        dictionary[dictionaryIndex++] = header;

        const u16 fileLen = strlen(fileNames[i]) + 1;
        fwrite(&fileLen, sizeof(fileLen), 1, packFile);
        fwrite(fileNames[i], sizeof(char), fileLen, packFile);

        // writeFileHeader(file, header);
    }

    const u32 dictionaryBegin = ftell(packFile);

    for(u32 i = 0; i < header.fileCount; ++i)
    {
        fwrite(&dictionary[i], sizeof(DPFileHeader), 1, packFile);
    }

    const u32 numIgnored = currentIgnoreIndex;
    currentIgnoreIndex = 0;

    for(u32 i = 0; i < fileCount; ++i)
    {
        if(ignoredIndices[currentIgnoreIndex] == i)
        {
            ++currentIgnoreIndex;
            continue;
        }
        FILE* inFile;
        fileError = fopen_s(&inFile, fileNames[i], "rb");
        if(fileError) 
        { 
            fclose(packFile);
            remove(packFileName);
            puts("Failed to open file after it was previously accessible.");
            printf("    Offending file: %s\n", fileNames[i]);
            return;
        }

        const u32 fileLen = getFileSize(inFile);

        u8* fileData = malloc(sizeof(u8) * fileLen);
        fread(fileData, sizeof(u8), fileLen, inFile);
        fclose(inFile);

        printf("Packing file: %s\n", fileNames[i]);

        u32 compressedWriteSize = 0;

        if(compressionType == NONE)
        {
            const u32 ptrIndex = dictionaryBegin + i * sizeof(DPFileHeader) + sizeof(DPFileHeader) - MEMBER_SIZE(DPFileHeader, ptrToFile);
            
            const u64 filePtr = ftell(packFile);
            fseek(packFile, ptrIndex, SEEK_SET);
            fwrite(&filePtr, sizeof(filePtr), 1, packFile);
            fseek(packFile, 0, SEEK_END);

            compressedWriteSize = fileLen;

            fwrite(fileData, sizeof(u8), fileLen, packFile);
            free(fileData);
        }

        printf("  Compression Ratio: %f (%d/%d)\n", (float) compressedWriteSize / (float) fileLen, compressedWriteSize, fileLen);
    }

    fclose(packFile);
}
