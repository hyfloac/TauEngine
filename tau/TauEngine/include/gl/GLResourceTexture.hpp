#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#include <new>
#pragma warning(pop)

#include "GLResource.hpp"
#include "system/Win32Event.hpp"

class TAU_DLL TAU_NOVTABLE GLResourceTexture : public GLResource
{
    DELETE_CM(GLResourceTexture);
protected:
    GLuint _texture;
private:
    volatile iSys* _atomicMapCount;
    void* volatile _currentMapping;
    Win32ManualEvent _mappingEvent;
protected:
    GLResourceTexture(const uSys size, const GLuint texture) noexcept
        : GLResource(size)
        , _texture(texture)
        , _atomicMapCount(new(::std::nothrow) iSys(0))
        , _currentMapping(null)
    { }
public:
    virtual ~GLResourceTexture() noexcept
    { delete _atomicMapCount; }

    [[nodiscard]] GLuint texture() const noexcept { return _texture; }

    [[nodiscard]] void* map(IRenderingContext& context, EResource::MapType mapType, uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapReadRange) noexcept override;
    void unmap(IRenderingContext& context, uSys mipLevel, uSys arrayIndex) noexcept override;
protected:
    [[nodiscard]] virtual uSys computeSize(uSys mipLevel) const noexcept = 0;

    virtual void uploadTexture(uSys mipLevel, uSys arrayIndex, const void* data) const noexcept = 0;
};

class TAU_DLL GLResourceTexture1D final : public GLResourceTexture
{
    DEFAULT_DESTRUCT(GLResourceTexture1D);
    DELETE_CM(GLResourceTexture1D);
private:
    ResourceTexture1DArgs _args;
public:
    GLResourceTexture1D(const ResourceTexture1DArgs& args, const GLuint texture) noexcept
        : GLResourceTexture(ETexture::computeSizeMipArr(args.dataFormat, args.width, args.mipLevels, args.arrayCount), texture)
        , _args(args)
    { }

    [[nodiscard]] EResource::Type resourceType() const noexcept override { return EResource::Type::Texture1D; }
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }

    [[nodiscard]] uSys computeSize(uSys mipLevel) const noexcept override;

    void uploadTexture(uSys mipLevel, uSys arrayIndex, const void* data) const noexcept override;
};

class TAU_DLL GLResourceTexture2D final : public GLResourceTexture
{
    DEFAULT_DESTRUCT(GLResourceTexture2D);
    DELETE_CM(GLResourceTexture2D);
private:
    ResourceTexture2DArgs _args;
public:
    GLResourceTexture2D(const ResourceTexture2DArgs& args, const GLuint texture) noexcept
        : GLResourceTexture(ETexture::computeSizeMipArr(args.dataFormat, args.width, args.height, args.mipLevels, args.arrayCount), texture)
        , _args(args)
    { }

    [[nodiscard]] EResource::Type resourceType() const noexcept override { return EResource::Type::Texture2D; }
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }

    [[nodiscard]] uSys computeSize(uSys mipLevel) const noexcept override;

    void uploadTexture(uSys mipLevel, uSys arrayIndex, const void* data) const noexcept override;
};

class TAU_DLL GLResourceTexture3D final : public GLResourceTexture
{
    DEFAULT_DESTRUCT(GLResourceTexture3D);
    DELETE_CM(GLResourceTexture3D);
private:
    ResourceTexture3DArgs _args;
public:
    GLResourceTexture3D(const ResourceTexture3DArgs& args, const GLuint texture) noexcept
        : GLResourceTexture(ETexture::computeSizeMip(args.dataFormat, args.width, args.height, args.depth, args.mipLevels), texture)
        , _args(args)
    { }

    [[nodiscard]] EResource::Type resourceType() const noexcept override { return EResource::Type::Texture3D; }
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }

    [[nodiscard]] uSys computeSize(uSys mipLevel) const noexcept override;

    void uploadTexture(uSys mipLevel, uSys arrayIndex, const void* data) const noexcept override;
};

