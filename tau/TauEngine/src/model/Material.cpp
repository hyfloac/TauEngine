#include "model/Material.hpp"
#include "../../include/graphics/GraphicsInterface.hpp"
#include "system/RenderingContext.hpp"
#include "texture/FITextureLoader.hpp"

TextureIndices Material::upload(IRenderingContext& context, UniformBlockU<Material>& uniform, const EShader::Stage stage, const u32 uniformIndex, const TextureIndices& textureIndices) const noexcept
{
    uniform.set(context, *this);
    uniform.upload(context, stage, uniformIndex);
#if 0 // TODO: port to ICommandQueue upload path
    return _textureUploader->upload(context, textureIndices, stage);
#else
    return textureIndices;
#endif
}

TextureIndices Material::unbind(IRenderingContext& context, UniformBlockU<Material>& uniform, const EShader::Stage stage, const u32 uniformIndex, const TextureIndices& textureIndices) const noexcept
{
    uniform.unbind(context, stage, uniformIndex);
#if 0 // TODO: port to ICommandQueue upload path
    return _textureUploader->unbind(context, textureIndices, stage);
#else
    return textureIndices;
#endif
}

Material MaterialBuilder::build() const noexcept
{
#if 0 // TODO: port to ICommandQueue upload path
    TextureUploaderArgs uploaderArgs(3);
    uploaderArgs.textures[0] = _diffuseTexture->textureView();
    uploaderArgs.textures[1] = _specularTexture->textureView();
    uploaderArgs.textures[2] = _normalTexture->textureView();
    uploaderArgs.textureSampler = _textureSampler;
#endif

    return Material(_specularExponent, _diffuseTexture, _specularTexture, _normalTexture);
}
