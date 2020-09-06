#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#include <new>
#pragma warning(pop)

#include "GLResource.hpp"
#include "system/Win32Event.hpp"

class TAU_DLL GLResourceBuffer final : public GLResource
{
    DELETE_CM(GLResourceBuffer);
private:
    ResourceBufferArgs _args;
    GLenum _glBufferType;
    GLenum _glUsage;
    GLuint _buffer;

    volatile iSys* _atomicMapCount;
    void* volatile _currentMapping;
    Win32ManualEvent _mappingEvent;
    volatile EResource::MapType _currentMapType;
public:
    GLResourceBuffer(const ResourceBufferArgs& args, const GLenum glBufferType, const GLenum glUsage, const GLuint buffer) noexcept
        : GLResource(args.size)
        , _args(args)
        , _glBufferType(glBufferType)
        , _glUsage(glUsage)
        , _buffer(buffer)
        , _atomicMapCount(new(::std::nothrow) iSys(0))
        , _currentMapping(null)
        , _currentMapType(static_cast<EResource::MapType>(0))
    { }

    ~GLResourceBuffer() noexcept override
    {
        delete _atomicMapCount;
    }

    [[nodiscard]] GLenum glUsage() const noexcept { return _glUsage; }
    [[nodiscard]] GLuint buffer() const noexcept { return _buffer; }

    [[nodiscard]] EResource::Type resourceType() const noexcept override { return EResource::Type::Buffer; }

    [[nodiscard]] void* map(IRenderingContext& context, EResource::MapType mapType, uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapReadRange) noexcept override;
    void unmap(IRenderingContext& context, uSys mipLevel, uSys arrayIndex) noexcept override;
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }
};
