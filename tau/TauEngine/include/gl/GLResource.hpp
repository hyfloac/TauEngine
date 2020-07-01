#pragma once

#include "graphics/Resource.hpp"

class TAU_DLL TAU_NOVTABLE GLResource : public IResource
{
    DEFAULT_DESTRUCT_VI(GLResource);
    DEFAULT_CM_PO(GLResource);
    RESOURCE_IMPL(GLResource);
protected:
    GLResource(const uSys size) noexcept
        : IResource(size)
    { }
};

class GLResourceBuffer;
class GLResourceTexture1D;
class GLResourceTexture2D;
class GLResourceTexture3D;
