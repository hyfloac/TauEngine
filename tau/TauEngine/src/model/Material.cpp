#include "model/Material.hpp"
#include "system/GraphicsInterface.hpp"
#include "system/RenderingContext.hpp"

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
    uploaderArgs.textures[0] = _diffuseTexture;
    uploaderArgs.textures[1] = _specularTexture;
    uploaderArgs.textures[2] = _normalTexture;
    uploaderArgs.textureSampler = _textureSampler;

    return Material(_specularExponent, _gi.createTextureUploader().buildCPPRef(uploaderArgs, null));
}
