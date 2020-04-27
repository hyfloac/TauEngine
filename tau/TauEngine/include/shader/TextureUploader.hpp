#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>
#include <DynArray.hpp>

#include "DLL.hpp"
#include "texture/Texture.hpp"
#include "texture/TextureSampler.hpp"
#include "shader/EShader.hpp"
#include "ShaderBindMap.hpp"

struct TextureIndices final
{
    DEFAULT_DESTRUCT(TextureIndices);
    DEFAULT_COPY(TextureIndices);
public:
    u32 textureStartIndex;
    u32 samplerStartIndex;
    u32 uniformIndex;
    ShaderBindMap* bindMap;

    inline TextureIndices() noexcept
        : textureStartIndex(0), samplerStartIndex(0), uniformIndex(0), bindMap(null)
    { }

    inline TextureIndices(const u32 textureStartIndex, const u32 samplerStartIndex, const u32 uniformIndex, ShaderBindMap* const bindMap = null) noexcept
        : textureStartIndex(textureStartIndex), samplerStartIndex(samplerStartIndex), uniformIndex(uniformIndex), bindMap(bindMap)
    { }
};

class TAU_DLL NOVTABLE ISingleTextureUploader
{
    DEFAULT_DESTRUCT_VI(ISingleTextureUploader);
    DELETE_COPY(ISingleTextureUploader);
protected:
    ITextureView* _texture;
    CPPRef<ITextureSampler> _textureSampler;
protected:
    ISingleTextureUploader(ITextureView* const texture, const CPPRef<ITextureSampler>& textureSampler) noexcept
        : _texture(texture), _textureSampler(textureSampler)
    { }
public:
    void texture(ITextureView* const texture) noexcept { _texture = texture; }

    virtual TextureIndices upload(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept = 0;
    virtual TextureIndices unbind(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept = 0;
};

class TAU_DLL ITextureUploader
{
    DEFAULT_DESTRUCT_VI(ITextureUploader);
    DELETE_COPY(ITextureUploader);
protected:
    RefDynArray<ITextureView*> _textures;
    CPPRef<ITextureSampler> _textureSampler;
protected:
    ITextureUploader(const RefDynArray<ITextureView*>& textures, const CPPRef<ITextureSampler>& textureSampler) noexcept
        : _textures(textures), _textureSampler(textureSampler)
    { }
public:
    virtual TextureIndices upload(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept = 0;
    virtual TextureIndices unbind(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept = 0;
};

struct SingleTextureUploaderArgs final
{
    DEFAULT_CONSTRUCT_PU(SingleTextureUploaderArgs);
    DEFAULT_DESTRUCT(SingleTextureUploaderArgs);
    DEFAULT_COPY(SingleTextureUploaderArgs);
public:
    ITextureView* texture;
    CPPRef<ITextureSampler> textureSampler;
public:
    SingleTextureUploaderArgs(ITextureView* const _texture, const CPPRef<ITextureSampler>& _textureSampler)
        : texture(_texture), textureSampler(_textureSampler)
    { }
};

struct TextureUploaderArgs final
{
    DEFAULT_DESTRUCT(TextureUploaderArgs);
    DEFAULT_COPY(TextureUploaderArgs);
public:
    RefDynArray<ITextureView*> textures;
    CPPRef<ITextureSampler> textureSampler;
public:
    TextureUploaderArgs(const uSys count) noexcept
        : textures(count), textureSampler(null)
    { }

    TextureUploaderArgs(const uSys count, const CPPRef<ITextureSampler>& _textureSampler) noexcept
        : textures(count), textureSampler(_textureSampler)
    { }

    TextureUploaderArgs(const RefDynArray<ITextureView*>& _textures, const CPPRef<ITextureSampler>& _textureSampler)
        : textures(_textures), textureSampler(_textureSampler)
    { }
};

class TAU_DLL NOVTABLE ISingleTextureUploaderBuilder
{
    DEFAULT_CONSTRUCT_PO(ISingleTextureUploaderBuilder);
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
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        TextureSamplerNotSet,
        TextureNotSet,
        ZeroTextures,
        /**
         * Using multiple non-compatible backend graphics API's.
         */
        CrossAPIFailure
    };
public:
    [[nodiscard]] virtual ISingleTextureUploader* build(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ISingleTextureUploader* build(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ISingleTextureUploader> buildCPPRef(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ISingleTextureUploader> buildTauRef(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ISingleTextureUploader> buildTauSRef(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};

class TAU_DLL NOVTABLE ITextureUploaderBuilder
{
    DEFAULT_CONSTRUCT_PO(ITextureUploaderBuilder);
    DEFAULT_DESTRUCT_VI(ITextureUploaderBuilder);
    DELETE_COPY(ITextureUploaderBuilder);
public:
    using Error = ISingleTextureUploaderBuilder::Error;
public:
    [[nodiscard]] virtual ITextureUploader* build(const TextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITextureUploader* build(const TextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITextureUploader> buildCPPRef(const TextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITextureUploader> buildTauRef(const TextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITextureUploader> buildTauSRef(const TextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
