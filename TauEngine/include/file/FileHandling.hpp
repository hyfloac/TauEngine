/** 
 * @file
 * 
 * Several classes for file handling.
 */
#pragma once

#include <DLL.hpp>

// enum WindowsFileAttributes
// {
//     FILE_ATTRIBUTE_READONLY              = 1 << 0,
//     FILE_ATTRIBUTE_HIDDEN                = 1 << 1,
//     FILE_ATTRIBUTE_SYSTEM                = 1 << 2,
//     FILE_ATTRIBUTE_DIRECTORY             = 1 << 4,
//     FILE_ATTRIBUTE_ARCHIVE               = 1 << 5,
//     FILE_ATTRIBUTE_DEVICE                = 1 << 6,
//     FILE_ATTRIBUTE_NORMAL                = 1 << 7,
//     FILE_ATTRIBUTE_TEMPORARY             = 1 << 8,
//     FILE_ATTRIBUTE_SPARSE_FILE           = 1 << 9,
//     FILE_ATTRIBUTE_REPARSE_POINT         = 1 << 10,
//     FILE_ATTRIBUTE_COMPRESSED            = 1 << 11,
//     FILE_ATTRIBUTE_OFFLINE               = 1 << 12,
//     FILE_ATTRIBUTE_NOT_CONTENT_INDEXED   = 1 << 13,
//     FILE_ATTRIBUTE_ENCRYPTED             = 1 << 14,
//     FILE_ATTRIBUTE_INTEGRITY_STREAM      = 1 << 15,
//     FILE_ATTRIBUTE_VIRTUAL               = 1 << 16,
//     FILE_ATTRIBUTE_NO_SCRUB_DATA         = 1 << 17,
//     FILE_ATTRIBUTE_RECALL_ON_OPEN        = 1 << 18,
//     FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS = 1 << 22
// };
//
// struct FileInformation
// {
//     u32 attributes;
//     u32 containingDrive;
//     u64 size;
//     u64 creationDate;
//     u64 modificationDate;
//     u64 accessDate;
// };

TAU_DLL char* readFile(const char* filePath) noexcept;
