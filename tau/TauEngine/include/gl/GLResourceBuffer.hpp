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
    GLenum _bufferType;
    GLenum _glUsage;
    GLuint _buffer;

    volatile iSys* _atomicMapCount;
    void* volatile _currentMapping;
    Win32ManualEvent _mappingEvent;
    EResource::MapType _currentMapType;
public:
    GLResourceBuffer(const ResourceBufferArgs& args, const GLenum glUsage, const GLuint buffer) noexcept
        : _args(args)
        , _glUsage(glUsage)
        , _buffer(buffer)
        , _atomicMapCount(new(::std::align_val_t{ 64 }, ::std::nothrow) iSys(0))
        , _currentMapping(null)
    { }

    ~GLResourceBuffer() noexcept
    {
        operator delete(const_cast<iSys*>(_atomicMapCount), ::std::align_val_t{ 64 }, ::std::nothrow);
    }

    [[nodiscard]] GLenum glUsage() const noexcept { return _glUsage; }
    [[nodiscard]] GLuint buffer() const noexcept { return _buffer; }

    [[nodiscard]] EResource::Type resourceType() const noexcept override { return EResource::Type::Buffer; }

    [[nodiscard]] void* map(IRenderingContext& context, EResource::MapType mapType, uSys subResource, const ResourceMapRange* mapReadRange) noexcept override;
    void unmap(IRenderingContext& context, uSys subResource) noexcept override;
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }
};
