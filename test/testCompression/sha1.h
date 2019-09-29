#pragma once

/*
   SHA-1 in C
   By Steve Reid <steve@edmweb.com>
   100% Public Domain
 */

#include "NumTypes.h"

typedef struct
{
    u32 state[5];
    u32 count[2];
    u8 buffer[64];
} SHA1_CTX;

void SHA1Transform(u32 state[5], const u8 buffer[64]);

void SHA1Init(SHA1_CTX* const context);

void SHA1Update(SHA1_CTX* const restrict context, const u8* const restrict data, const size_t len);

void SHA1Final(u8 digest[20], SHA1_CTX* const context);

void SHA1(u8 hash_out[20], const char* const str, const size_t len);
