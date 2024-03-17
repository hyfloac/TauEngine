/**
 * @file
 * 
 * A series of things to determine the endian of system.
 * 
 * This does not work with a middle endian (`3 4 1 2`).
 */
#pragma once

#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || \
    defined(__BIG_ENDIAN__) || \
    defined(__ARMEB__) || \
    defined(__THUMBEB__) || \
    defined(__AARCH64EB__) || \
    defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__) || \
    defined(_M_PPC) || defined(_M_MPPC)
  #define IS_LITTLE_ENDIAN 0
  #define IS_BIG_ENDIAN    1
  #define BIG_ENDIAN
#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || \
    defined(__LITTLE_ENDIAN__) || \
    defined(__ARMEL__) || \
    defined(__THUMBEL__) || \
    defined(__AARCH64EL__) || \
    defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__) || \
    defined(_M_IA64) || defined(_M_AMD64) || defined(_M_IX86) || defined(_M_X64) || \
    defined(_WIN32)
  #define IS_LITTLE_ENDIAN 1
  #define IS_BIG_ENDIAN    0
  #define LITTLE_ENDIAN
#else
  #error "Unknown Endianness."
#endif

/**
 *   A simple little union that determines what endian the 
 * system is.
 * 
 *   I'm not entirely sure if this is runtime or compile time.
 * I would guess compile time if optimizations are on.
 * 
 */
#define NON_PP_IS_LITTLE_ENDIAN (((union { unsigned short x; unsigned char c; }){ 1 }).c)

/**
 *   A runtime function used to determine what endian the 
 * system is.
 */
inline bool isLittleEndian() noexcept
{
    static const unsigned short number = 0x0001;
    static const unsigned char* numPtr = reinterpret_cast<const unsigned char*>(&number);
    return numPtr[0] == 1;
}

enum Endian
{
    EndianUnknown = 0,
    EndianLittle,
    EndianBig
};
