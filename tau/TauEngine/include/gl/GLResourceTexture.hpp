#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#include <new>
#pragma warning(pop)

#include "GLResource.hpp"
#include "system/Win32Event.hpp"

class TAU_DLL GLResourceTexture1D final : public GLResource
{
    DELETE_CM(GLResourceTexture1D);
private:
    ResourceTexture1DArgs _args;
    GLuint _texture;

    volatile iSys* _atomicMapCount;
    void* volatile _currentMapping;
    Win32ManualEvent _mappingEvent;
public:
    GLResourceTexture1D(const ResourceTexture1DArgs& args, const GLuint texture) noexcept
        : GLResource(ETexture::computeSizeArr(args.dataFormat, args.width, args.mipLevels, args.arrayCount))
        , _args(args)
        , _texture(texture)
        , _atomicMapCount(new(::std::align_val_t{ 64 }, ::std::nothrow) iSys(0))
        , _currentMapping(null)
    { }

    ~GLResourceTexture1D() noexcept
    {
        operator delete(const_cast<iSys*>(_atomicMapCount), ::std::align_val_t{ 64 }, ::std::nothrow);
    }

    [[nodiscard]] GLuint texture() const noexcept { return _texture; }

    [[nodiscard]] EResource::Type resourceType() const noexcept override { return EResource::Type::Texture1D; }

    [[nodiscard]] void* map(IRenderingContext& context, EResource::MapType mapType, uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapReadRange) noexcept override;
    void unmap(IRenderingContext& context, uSys mipLevel, uSys arrayIndex) noexcept override;
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }
};
