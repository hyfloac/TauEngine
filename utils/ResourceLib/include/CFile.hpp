/**
 * @file
 *
 * Describes a C standard IO file handle.
 */
#pragma once

#include "IFile.hpp"

#include <cstdio>
#include <String.hpp>
#include <TauCOM.hpp>

class Win32File;
class Win32FileLoader;

namespace tau {

class ICFile : public com::IUnknown
{
    DEFAULT_CONSTRUCT_PO(ICFile);
    DEFAULT_DESTRUCT_VIO(ICFile);
    DEFAULT_CM_PO(ICFile);
public:
    [[nodiscard]] virtual FILE* GetFileHandle() noexcept = 0;
};

}

TAU_DECL_UUID(tau::ICFile, 0xB2CB4560062E4BC2ull, 0x8549CC5BD7BD02E8ull);

