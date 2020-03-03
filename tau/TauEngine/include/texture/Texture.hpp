#pragma once

#include <DLL.hpp>
#include <NumTypes.hpp>
#include <Objects.hpp>
#include <ReferenceCountingPointer.hpp>
#include <RunTimeType.hpp>
#include <Safeties.hpp>

#include "TextureEnums.hpp"
#include "shader/EShader.hpp"

class RenderingMode;
class IRenderingContext;

#define TEXTURE_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                                 RTT_IMPL(_TYPE, ITexture)

#define TEXTURE_IMPL(_TYPE) TEXTURE_IMPL_BASE(_TYPE)

class TAU_DLL ITexture
{
    DEFAULT_DESTRUCT_VI(ITexture);
    DELETE_COPY(ITexture);
protected:
    u32 _width;
    u32 _height;
    ETexture::Format _dataFormat;
protected:
    inline ITexture(u32 width, u32 height, ETexture::Format dataFormat) noexcept
        : _width(width), _height(height), _dataFormat(dataFormat)
    { }
public:
    [[nodiscard]] u32 width() const noexcept { return _width; }
    [[nodiscard]] u32 height() const noexcept { return _height; }

    [[nodiscard]] ETexture::Format dataFormat() const noexcept { return _dataFormat; }

    [[nodiscard]] virtual inline ETexture::Type textureType() const noexcept = 0;

    virtual void set(u32 level, const void* data) noexcept = 0;

    virtual void bind(u8 textureUnit, EShader::Stage stage) noexcept = 0;

    virtual void unbind(u8 textureUnit, EShader::Stage stage) noexcept = 0;

    virtual void generateMipmaps() noexcept = 0;

    RTT_BASE_IMPL(ITexture);
    RTT_BASE_CHECK(ITexture);
    RTT_BASE_CAST(ITexture);
};

class TAU_DLL ITextureCube : public ITexture
{
    DEFAULT_DESTRUCT_VI(ITextureCube);
    DELETE_COPY(ITextureCube);
protected:
    inline ITextureCube(u32 width, u32 height, ETexture::Format dataFormat) noexcept
        : ITexture(width, height, dataFormat)
    { }
public:
    [[nodiscard]] inline ETexture::Type textureType() const noexcept override { return ETexture::Type::Cube; }

    virtual void setCube(u32 level, ETexture::CubeSide side, const void* data) noexcept = 0;
};

struct TextureArgs final
{
    DEFAULT_DESTRUCT(TextureArgs);
    DEFAULT_COPY(TextureArgs);
public:
    u32 width;
    u32 height;
    i32 mipmapLevels;
    ETexture::Format dataFormat;
    const void* initialBuffer;
public:
    inline TextureArgs() noexcept
        : width(0), height(0), mipmapLevels(0),
          dataFormat(static_cast<ETexture::Format>(0)),
          initialBuffer(null)
    { }
};

class TAU_DLL NOVTABLE ITextureBuilder
{
    DEFAULT_CONSTRUCT_PO(ITextureBuilder);
    DEFAULT_DESTRUCT_VI(ITextureBuilder);
    DELETE_COPY(ITextureBuilder);
public:
    enum class Error
    {
        NoError = 0,
        WidthIsZero,
        HeightIsZero,
        MipMapLevelsIsUnset,
        DataFormatIsUnset,
        /**
         * Failed to allocate system memory.
         *
         *   This error is produced when a call to the system memory
         * allocator (malloc, new, new[], HeapAlloc, LocalAlloc,
         * GlobalAlloc) fails (returns null). This generally occurs
         * when the system has run out of memory. It may also mean
         * that the process itself has exceed its maximum allotted
         * memory. It may also occur when allocating a buffer that
         * is too large.
         *
         *   If this occurs its probably safe to assume that the
         * program or system will crash shortly. As such I prescribe to
         * the philosophy of simply exiting, or outright ignoring the
         * error, given that it likely won't matter in a couple of seconds
         * at most.
         */
        SystemMemoryAllocationFailure,
        /**
         * The driver failed to allocate system memory.
         *
         *   This occurs when the driver failed to allocate memory on
         * the system. This is likely caused by the same reasons as
         * Error::SystemMemoryAllocationFailure.
         */
        DriverMemoryAllocationFailure,
        /**
         * The driver failed to allocate memory on the GPU.
         *
         *   This occurs when the GPU runs out of memory. It may also mean
         * that there isn't a GPU installed, or at the very least, one that
         * isn't suitable. This may be more resolvable by scaling back
         * texture sizes and model sizes. As well as deallocating unnecessary
         * objects. Depending on the backing API it may also be possible to
         * move certain allocation from the GPU back into system memory.
         */
        GPUMemoryAllocationFailure,
        /**
         * TauEngine screwed up somehow.
         *
         * I fucked up. Tell me about.
         *
         *   This shouldn't ever occur. If it does I must have done something
         * very wrong. Please, tell me about it, including any relevant
         * information - things like the call stack (I mostly care about within
         * TauEngine), the model (or at the very least, the model size), etc.
         */
    };
public:
    [[nodiscard]] virtual ITexture* build(const TextureArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITexture* build(const TextureArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITexture> buildCPPRef(const TextureArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITexture> buildTauRef(const TextureArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITexture> buildTauSRef(const TextureArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};

struct TextureCubeArgs final
{
    DEFAULT_DESTRUCT(TextureCubeArgs);
    DEFAULT_COPY(TextureCubeArgs);
public:
    u32 width;
    u32 height;
    i32 mipmapLevels;
    ETexture::Format dataFormat;
    const void* initialBuffer[6];
public:
    inline TextureCubeArgs() noexcept
        : width(0), height(0), mipmapLevels(0),
          dataFormat(static_cast<ETexture::Format>(0)),
          initialBuffer{ null, null, null, null, null, null }
    { }
};

class TAU_DLL NOVTABLE ITextureCubeBuilder
{
    DEFAULT_CONSTRUCT_PO(ITextureCubeBuilder);
    DEFAULT_DESTRUCT_VI(ITextureCubeBuilder);
    DELETE_COPY(ITextureCubeBuilder);
public:
    using Error = ITextureBuilder::Error;
public:
    [[nodiscard]] virtual ITextureCube* build(const TextureCubeArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITextureCube* build(const TextureCubeArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITextureCube> buildCPPRef(const TextureCubeArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITextureCube> buildTauRef(const TextureCubeArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITextureCube> buildTauSRef(const TextureCubeArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
