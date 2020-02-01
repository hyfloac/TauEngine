#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>
#include <DynArray.hpp>
#include "DLL.hpp"
#include "texture/Texture.hpp"
#include "texture/TextureSampler.hpp"

struct TextureIndices final
{
    DEFAULT_DESTRUCT(TextureIndices);
    DEFAULT_COPY(TextureIndices);
public:
    u32 textureStartIndex;
    u32 samplerStartIndex;
    u32 uniformIndex;

    inline TextureIndices() noexcept
        : textureStartIndex(0), samplerStartIndex(0), uniformIndex(0)
    { }

    inline TextureIndices(const u32 textureStartIndex, const u32 samplerStartIndex, const u32 uniformIndex) noexcept
        : textureStartIndex(textureStartIndex), samplerStartIndex(samplerStartIndex), uniformIndex(uniformIndex)
    { }
};

class TAU_DLL NOVTABLE ISingleTextureUploader
{
    DEFAULT_DESTRUCT_VI(ISingleTextureUploader);
    DELETE_COPY(ISingleTextureUploader);
protected:
    Ref<ITexture> _texture;
    Ref<ITextureSampler> _textureSampler;
protected:
    ISingleTextureUploader(const Ref<ITexture>& texture, const Ref<ITextureSampler>& textureSampler) noexcept
        : _texture(texture), _textureSampler(textureSampler)
    { }
public:
    void texture(const Ref<ITexture>& texture) noexcept { _texture = texture; }
    void textureSampler(const Ref<ITextureSampler>& textureSampler) noexcept { _textureSampler = textureSampler; }

    virtual TextureIndices& upload(IRenderingContext& context, TextureIndices& textureIndices) noexcept = 0;

    virtual TextureIndices& unbind(IRenderingContext& context, TextureIndices& textureIndices) noexcept = 0;
};

class TAU_DLL NOVTABLE ITextureUploader
{
    DEFAULT_DESTRUCT_VI(ITextureUploader);
    DELETE_COPY(ITextureUploader);
protected:
    RefDynArray<Ref<ITexture>> _textures;
    Ref<ITextureSampler> _textureSampler;
protected:
    ITextureUploader(const RefDynArray<Ref<ITexture>>& textures, const Ref<ITextureSampler>& textureSampler) noexcept
        : _textures(textures), _textureSampler(textureSampler)
    { }
public:
    virtual TextureIndices& upload(IRenderingContext& context, TextureIndices& textureIndices) noexcept = 0;

    virtual TextureIndices& unbind(IRenderingContext& context, TextureIndices& textureIndices) noexcept = 0;
};

class TAU_DLL NOVTABLE ISingleTextureUploaderBuilder
{
    DEFAULT_DESTRUCT_VI(ISingleTextureUploaderBuilder);
    DELETE_COPY(ISingleTextureUploaderBuilder);
public:
    enum class Error
    {
        NoError = 0,
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
         SystemMemoryAllocationFailure
    };
protected:
    Ref<ITexture> _texture;
    Ref<ITextureSampler> _textureSampler;
protected:
    inline ISingleTextureUploaderBuilder() noexcept
        : _texture(null), _textureSampler(null)
    { }
public:
    virtual void texture(const Ref<ITexture>& texture) noexcept
    {
        _texture = texture;
    }

    virtual void textureSampler(const Ref<ITextureSampler>& textureSampler) noexcept
    {
        _textureSampler = textureSampler;
    }

    [[nodiscard]] virtual ISingleTextureUploader* build([[tau::out]] Error* error = null) noexcept = 0;
};

class TAU_DLL NOVTABLE ITextureUploaderBuilder
{
    DEFAULT_DESTRUCT_VI(ITextureUploaderBuilder);
    DELETE_COPY(ITextureUploaderBuilder);
public:
    enum class Error
    {
        NoError = 0,
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
        SystemMemoryAllocationFailure
    };
protected:
    RefDynArray<Ref<ITexture>> _textures;
    Ref<ITextureSampler> _textureSampler;
protected:
    inline ITextureUploaderBuilder(const uSys textureCount) noexcept
        : _textures(textureCount), _textureSampler(null)
    { }
public:
    virtual void setTexture(uSys index, const Ref<ITexture>& texture) noexcept
    {
        if(index < _textures.count())
        {
            _textures[index] = texture;
        }
    }

    virtual void textureSampler(const Ref<ITextureSampler>& textureSampler) noexcept
    {
        _textureSampler = textureSampler;
    }

    [[nodiscard]] virtual ITextureUploader* build([[tau::out]] Error* error = null) noexcept = 0;
};
