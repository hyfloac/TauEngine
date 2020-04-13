#pragma once

#include "shader/TextureUploader.hpp"

class GLTextureSampler;

class TAU_DLL GLSingleTextureUploader final : public ISingleTextureUploader
{
    DEFAULT_DESTRUCT(GLSingleTextureUploader);
    DELETE_COPY(GLSingleTextureUploader);
public:
    GLSingleTextureUploader(ITextureView* texture, const CPPRef<GLTextureSampler>& textureSampler) noexcept;

    TextureIndices upload(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
    TextureIndices unbind(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
};

class TAU_DLL GLTextureUploader final : public ITextureUploader
{
    DEFAULT_DESTRUCT(GLTextureUploader);
    DELETE_COPY(GLTextureUploader);
public:
    GLTextureUploader(const RefDynArray<ITextureView*>& textures, const CPPRef<GLTextureSampler>& textureSampler) noexcept;

    TextureIndices upload(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
    TextureIndices unbind(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
};

class TAU_DLL GLSingleTextureUploaderBuilder final : public ISingleTextureUploaderBuilder
{
    DEFAULT_CONSTRUCT_PU(GLSingleTextureUploaderBuilder);
    DEFAULT_DESTRUCT(GLSingleTextureUploaderBuilder);
    DELETE_COPY(GLSingleTextureUploaderBuilder);
public:
    [[nodiscard]] GLSingleTextureUploader* build(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] GLSingleTextureUploader* build(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ISingleTextureUploader> buildCPPRef(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ISingleTextureUploader> buildTauRef(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ISingleTextureUploader> buildTauSRef(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] static bool processArgs(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error) noexcept;
};

class TAU_DLL GLTextureUploaderBuilder final : public ITextureUploaderBuilder
{
    DEFAULT_CONSTRUCT_PU(GLTextureUploaderBuilder);
    DEFAULT_DESTRUCT(GLTextureUploaderBuilder);
    DELETE_COPY(GLTextureUploaderBuilder);
public:
    [[nodiscard]] GLTextureUploader* build(const TextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] GLTextureUploader* build(const TextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureUploader> buildCPPRef(const TextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureUploader> buildTauRef(const TextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureUploader> buildTauSRef(const TextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] static bool processArgs(const TextureUploaderArgs& args, [[tau::out]] Error* error) noexcept;
};
