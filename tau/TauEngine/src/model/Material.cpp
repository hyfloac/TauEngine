#include "model/Material.hpp"
#include "system/GraphicsInterface.hpp"
#include "system/RenderingContext.hpp"
#include "texture/FITextureLoader.hpp"

TextureIndices Material::upload(IRenderingContext& context, UniformBlockU<Material>& uniform, const EShader::Stage stage, const u32 uniformIndex, const TextureIndices& textureIndices) const noexcept
{
    uniform.set(context, *this);
    uniform.upload(context, stage, uniformIndex);
    return _textureUploader->upload(context, textureIndices, stage);
}

TextureIndices Material::unbind(IRenderingContext& context, UniformBlockU<Material>& uniform, const EShader::Stage stage, const u32 uniformIndex, const TextureIndices& textureIndices) const noexcept
{
    uniform.unbind(context, stage, uniformIndex);
    return _textureUploader->unbind(context, textureIndices, stage);
}

Material MaterialBuilder::build() const noexcept
{
    TextureUploaderArgs uploaderArgs(3);
    uploaderArgs.textures[0] = _diffuseTexture->textureView();
    uploaderArgs.textures[1] = _specularTexture->textureView();
    uploaderArgs.textures[2] = _normalTexture->textureView();
    uploaderArgs.textureSampler = _textureSampler;

    return Material(_specularExponent, _diffuseTexture, _specularTexture, _normalTexture, _gi.createTextureUploader().buildCPPRef(uploaderArgs, null));
}
