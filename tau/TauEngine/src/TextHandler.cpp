#pragma warning(push, 0)
#include <utility>
#pragma warning(pop)

#include <Utils.hpp>
#include <VFS.hpp>

#include "TextHandler.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "texture/Texture.hpp"
#include "model/BufferDescriptor.hpp"
#include "model/VertexArray.hpp"
#include "shader/IShaderProgram.hpp"
#include "shader/IShader.hpp"
#include <Windows.h>
#include <winreg.h>

#include "GL/glew.h"
#include "Timings.hpp"
#include "system/RenderingContext.hpp"
#include "system/Window.hpp"
#include "graphics/RasterizerState.hpp"
#include "system/GraphicsInterface.hpp"
#include "texture/NullTexture.hpp"
#include "texture/FITextureLoader.hpp"
#include <EnumBitFields.hpp>

template<>
class UniformAccessor<TextHandler::ProjectionUniforms> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_COPY(UniformAccessor);
public:
    static constexpr uSys MATRIX_SIZE = sizeof(float) * 4 * 4;

    [[nodiscard]] static inline uSys size() noexcept { return MATRIX_SIZE; }

    static inline void set(IRenderingContext& context, const CPPRef<IUniformBuffer>& buffer, const TextHandler::ProjectionUniforms& t) noexcept
    {
        buffer->fillBuffer(context, glm::value_ptr(t.projectionMatrix));
    }
};

template<>
class UniformAccessor<TextHandler::ColorUniforms> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_COPY(UniformAccessor);
public:
    [[nodiscard]] static inline uSys size() noexcept { return sizeof(float) * 4; }

    static inline void set(IRenderingContext& context, const CPPRef<IUniformBuffer>& buffer, const TextHandler::ColorUniforms& t) noexcept
    {
        const __m128 vec = t.color.data().vec;
        buffer->fillBuffer(context, reinterpret_cast<const void*>(&vec));
    }
};

NullableRef<IRasterizerState> TextHandler::rs = nullptr;

#define INSTANCE_COUNT 128

TextHandler::TextHandler(IGraphicsInterface& gi, IRenderingContext& context, const char* const vfsMount, const char* const path, const char* const vertexName, const char* const pixelName) noexcept
    : _ft(null), _glyphSets(), _shader(IShaderProgram::create(context)),
      _va(null),
      _viewUniforms(gi.createUniformBuffer()),
      _colorUniforms(gi.createUniformBuffer())
{
    PERF();
    ShaderArgs shaderArgs;
    shaderArgs.vfsMount = vfsMount;
    shaderArgs.path = path;
    
    shaderArgs.fileName = vertexName;
    shaderArgs.stage = EShader::Stage::Vertex;
    CPPRef<IShader> vertexShader = gi.createShader().buildCPPRef(shaderArgs, null);

    shaderArgs.fileName = pixelName;
    shaderArgs.stage = EShader::Stage::Pixel;
    CPPRef<IShader> pixelShader = gi.createShader().buildCPPRef(shaderArgs, null);

    _shader->setVertexShader(context, vertexShader);
    _shader->setPixelShader(context, pixelShader);

    _shader->link(context);

    TextureSamplerArgs textureSamplerArgs;
    textureSamplerArgs.magFilter() = ETexture::Filter::Linear;
    textureSamplerArgs.minFilter() = ETexture::Filter::Linear;
    textureSamplerArgs.mipFilter() = ETexture::Filter::Linear;
    textureSamplerArgs.wrapU = ETexture::WrapMode::ClampToEdge;
    textureSamplerArgs.wrapV = ETexture::WrapMode::ClampToEdge;
    textureSamplerArgs.wrapW = ETexture::WrapMode::ClampToEdge;
    textureSamplerArgs.depthCompareFunc = ETexture::CompareFunc::Never;

    SingleTextureUploaderArgs tuArgs;
    tuArgs.texture = TextureLoader::getMissingTexture()->textureView();
    tuArgs.textureSampler = gi.createTextureSampler().buildCPPRef(textureSamplerArgs, null);
    _textureUploader = gi.createSingleTextureUploader().buildTauRef(tuArgs, null);

    BufferArgs bufferBuilder(1);
    bufferBuilder.type = EBuffer::Type::ArrayBuffer;
    bufferBuilder.usage = EBuffer::UsageType::DynamicDraw;
    bufferBuilder.elementCount = 6;
    bufferBuilder.descriptor.addDescriptor(ShaderSemantic::Position, ShaderDataType::Vector2Float);
    bufferBuilder.initialBuffer = null;
    
    _positionBuffer = gi.createBuffer().buildCPPRef(bufferBuilder, nullptr);

    float textureCoords[6][2] = {
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
          
        { 0.0f, 0.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f }
    };
    
    bufferBuilder.initialBuffer = textureCoords;
    bufferBuilder.usage = EBuffer::UsageType::StaticDraw;
    bufferBuilder.descriptor.reset(1);
    bufferBuilder.descriptor.addDescriptor(ShaderSemantic::TextureCoord, ShaderDataType::Vector2Float);
    
    const CPPRef<IBuffer> textureCoordBuffer = gi.createBuffer().buildCPPRef(bufferBuilder, null);

    VertexArrayArgs vaArgs(2);
    vaArgs.shader = vertexShader;
    vaArgs.buffers[0] = _positionBuffer;
    vaArgs.buffers[1] = textureCoordBuffer;
    vaArgs.drawCount = 6;
    vaArgs.drawType = DrawType::SeparatedTriangles;

    _va = gi.createVertexArray().buildCPPRef(vaArgs, null);

    if(!rs)
    {
        RasterizerArgs rArgs = context.getDefaultRasterizerArgs();
        if(rArgs.frontFaceCounterClockwise)
        {
            rs = context.getDefaultRasterizerState();
        }
        else
        {
            rArgs.frontFaceCounterClockwise = true;
            rs = gi.createRasterizerState().buildTauRef(rArgs, null);
        }
    }
}

TextHandler::~TextHandler() noexcept
{
    FT_Done_FreeType(_ft);
}

FT_Error TextHandler::init() noexcept
{
    const FT_Error error = FT_Init_FreeType(&_ft);
    return error;
}

TextHandler::FileData* TextHandler::loadTTFFile(const char* const fileName, const FT_UInt pixelWidth, const FT_UInt pixelHeight) const noexcept
{
    PERF();
    const CPPRef<IFile> file = VFS::Instance().openFile(fileName, FileProps::Read);
    
    if(!file) { return null; }
    
    RefDynArray<u8> data = file->readFile();

    FT_Face face;
    const FT_Error error = FT_New_Memory_Face(_ft, data.arr(), data.size() - 1, 0, &face);

    if(error) { return null; }

    FT_Set_Pixel_Sizes(face, pixelWidth, pixelHeight);

    return new FileData { face, data };
}

struct LoadData final
{
    TextHandler& th;
    FT_UInt pixelWidth;
    FT_UInt pixelHeight;
};

int TextHandler::loadTTFFile(const char* const fileName, const FT_UInt pixelWidth, const FT_UInt pixelHeight, const ResourceLoader::finalizeLoadT_f<FinalizeData, FileData> finalizeLoad, void* const userParam) noexcept
{
    PERF();
    const CPPRef<IFile> file = VFS::Instance().openFile(fileName, FileProps::Read);

    if(!file)
    { return -1; }

    ResourceLoader::loadFileT(file, TextHandler::load2, new LoadData { *this, pixelWidth, pixelHeight }, finalizeLoad, new FinalizeData { *this, userParam });
    return 0;
}

TextHandler::FileData* TextHandler::load2(RefDynArray<u8> file, LoadData* ld) noexcept
{
    PERF();
    TextHandler& th = ld->th;

    FT_Face face;
    const FT_Error error = FT_New_Memory_Face(th._ft, file.arr(), file.size() - 1, 0, &face);

    if(error)
    {
        delete ld;
        return nullptr;
    }

    FT_Set_Pixel_Sizes(face, ld->pixelWidth, ld->pixelHeight);

    delete ld;

    return new FileData { face, file };
}

GlyphSetHandle TextHandler::generateBitmapCharacters(IGraphicsInterface& gi, IRenderingContext& context, const DynString& glyphSetName, const char minChar, const char maxChar, const bool smooth, FT_Face face) noexcept
{
    PERF();
    GlyphSet& gs = _glyphSets.emplace_back(glyphSetName, minChar, maxChar);

    Texture2DArgs args;
    args.mipmapLevels = 1;
    args.dataFormat = ETexture::Format::Red8UnsignedInt;
    args.flags = ETexture::BindFlags::ShaderAccess | ETexture::BindFlags::GenerateMipmaps;

    for(GLchar c = minChar; c <= maxChar; ++c)
    {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)) { continue; }

        ITexture2D* texture;

        if(face->glyph->bitmap.buffer)
        {
            args.width = face->glyph->bitmap.width;
            args.height = face->glyph->bitmap.rows;
            args.initialBuffer = face->glyph->bitmap.buffer;

            texture = gi.createTexture().build(args, null);

            texture->textureView()->generateMipmaps(context);
        }
        else
        {
            texture = new(::std::nothrow) NullTexture2D;
        }

        gs.glyphs[c - gs.minGlyph] = GlyphCharacter(texture,
                                     Vector2f(static_cast<float>(face->glyph->bitmap.width), static_cast<float>(face->glyph->bitmap.rows)),
                                     Vector2f(static_cast<float>(face->glyph->bitmap_left), static_cast<float>(face->glyph->bitmap_top)),
                                     face->glyph->advance.x);
    }

    return _glyphSets.size() - 1;
}

void TextHandler::renderText(IRenderingContext& context, GlyphSetHandle glyphSetHandle, const char* str, float x, float y, float scale, Vector3f color, const glm::mat4& proj) noexcept
{
    const NullableRef<IRasterizerState> tmpRS = context.setRasterizerState(rs);
    const GlyphSet& glyphSet = _glyphSets[glyphSetHandle];

    _shader->bind(context);

    _viewUniforms.data().projectionMatrix = proj;
    _viewUniforms.upload(context, EShader::Stage::Vertex, 0);

    _colorUniforms.data().color = color;
    _colorUniforms.upload(context, EShader::Stage::Pixel, 1);

    _va->bind(context);
    _va->preDraw(context);

    for(char c = *str; c != '\0'; c = *(++str))
    {
        if(c < glyphSet.minGlyph || c > glyphSet.maxGlyph) { continue; }
        const GlyphCharacter* gc = &glyphSet.glyphs[c - glyphSet.minGlyph];

        const float xpos = x + gc->bearing.x() * scale;
        const float ypos = y - (gc->size.y() - gc->bearing.y()) * scale;

        const float w = gc->size.x() * scale;
        const float h = gc->size.y() * scale;
        // Update VBO for each character
        float vertices[6][2] = {
            { xpos,     ypos + h },
            { xpos,     ypos,    },
            { xpos + w, ypos,    },

            { xpos,     ypos + h },
            { xpos + w, ypos,    },
            { xpos + w, ypos + h }
        };

        _textureUploader->texture(gc->texture->textureView());

        (void) _textureUploader->upload(context, TextureIndices(0, 0, 0), EShader::Stage::Pixel);

        _positionBuffer->beginModification(context);
        _positionBuffer->modifyBuffer(0, sizeof(vertices), vertices);
        _positionBuffer->endModification(context);

        _va->draw(context);

        (void) _textureUploader->unbind(context, TextureIndices(0, 0, 0), EShader::Stage::Pixel);

        x += (gc->advance >> 6) * scale;
    }

    _va->postDraw(context);
    _va->unbind(context);
    _viewUniforms.unbind(context, EShader::Stage::Vertex, 2);
    _colorUniforms.unbind(context, EShader::Stage::Pixel, 1);
    _shader->unbind(context);

    (void) context.setRasterizerState(tmpRS);
}

float TextHandler::renderTextLineWrapped(IRenderingContext& context, GlyphSetHandle glyphSetHandle, const char* str, float x, float y, float scale, Vector3f color, const glm::mat4& proj, const Window& window, float lineHeight) noexcept
{
    const NullableRef<IRasterizerState> tmpRS = context.setRasterizerState(rs);
    const GlyphSet& glyphSet = _glyphSets[glyphSetHandle];

    _shader->bind(context);

    _viewUniforms.data().projectionMatrix = proj;
    _viewUniforms.upload(context, EShader::Stage::Vertex, 0);

    _colorUniforms.data().color = color;
    _colorUniforms.upload(context, EShader::Stage::Pixel, 1);

    _va->bind(context);
    _va->preDraw(context);

    float height = lineHeight;

    const float initialX = x;
    const float maxX = static_cast<float>(window.width());

    for(char c = *str; c != '\0'; c = *(++str))
    {
        if(c < glyphSet.minGlyph || c > glyphSet.maxGlyph) { continue; }
        const GlyphCharacter* gc = &glyphSet.glyphs[c - glyphSet.minGlyph];

        const float advance = (gc->advance >> 6) * scale;
        if(x + advance > maxX)
        {
            x = initialX;
            y += lineHeight;
            height += lineHeight;
        }

        const float xpos = x + gc->bearing.x() * scale;
        const float ypos = y - (gc->size.y() - gc->bearing.y()) * scale;

        const float w = gc->size.x() * scale;
        const float h = gc->size.y() * scale;
        // Update VBO for each character
        float vertices[6][2] = {
            { xpos,     ypos + h },
            { xpos,     ypos,    },
            { xpos + w, ypos,    },

            { xpos,     ypos + h },
            { xpos + w, ypos,    },
            { xpos + w, ypos + h }
        };

        _textureUploader->texture(gc->texture->textureView());
        (void) _textureUploader->upload(context, TextureIndices(0, 0, 0), EShader::Stage::Pixel);

        _positionBuffer->beginModification(context);
        _positionBuffer->modifyBuffer(0, sizeof(vertices), vertices);
        _positionBuffer->endModification(context);

        _va->draw(context);

        (void) _textureUploader->unbind(context, TextureIndices(0, 0, 0), EShader::Stage::Pixel);

        x += advance;
    }

    _va->postDraw(context);
    _va->unbind(context);
    _viewUniforms.unbind(context, EShader::Stage::Vertex, 2);
    _colorUniforms.unbind(context, EShader::Stage::Pixel, 1);
    _shader->unbind(context);

    (void) context.setRasterizerState(tmpRS);

    return height;
}

float TextHandler::computeLength(GlyphSetHandle glyphSetHandle, const char* str, float scale) const noexcept
{
    const GlyphSet& glyphSet = _glyphSets[glyphSetHandle];

    float length = 0.0f;

    for(char c = *str; c != '\0'; c = *(++str))
    {
        if(c < glyphSet.minGlyph || c > glyphSet.maxGlyph) { continue; }
        const GlyphCharacter* gc = &glyphSet.glyphs[c - glyphSet.minGlyph];
        length += (gc->advance >> 6) * scale;
    }

    return length;
}

float TextHandler::computeHeight(GlyphSetHandle glyphSetHandle, const char* str, float scale, float x, const Window& window, float lineHeight) const noexcept
{
    const GlyphSet& glyphSet = _glyphSets[glyphSetHandle];

    float height = lineHeight;

    const float initialX = x;
    const float maxX = static_cast<float>(window.width());

    for(char c = *str; c != '\0'; c = *(++str))
    {
        if(c < glyphSet.minGlyph || c > glyphSet.maxGlyph) { continue; }
        const GlyphCharacter* gc = &glyphSet.glyphs[c - glyphSet.minGlyph];

        const float advance = (gc->advance >> 6) * scale;
        if(x + advance > maxX)
        {
            x = initialX;
            height += lineHeight;
        }

        x += advance;
    }
    return height;
}

#ifdef _WIN32
DynString findSystemFont(const char* fontName) noexcept
{
    PERF();
    static const LPCSTR fontRegistryPath = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";

    HKEY hKey;
    LONG result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, fontRegistryPath, 0, KEY_READ, &hKey);
    if(result != ERROR_SUCCESS)
    { return ""; }

    DWORD maxValueNameSize, maxValueDataSize;
    result = RegQueryInfoKey(hKey, 
                             NULL, NULL, NULL, 
                             NULL, NULL, NULL, NULL, 
                             &maxValueNameSize, &maxValueDataSize, 
                             NULL, NULL);
    if(result != ERROR_SUCCESS)
    { return ""; }

    DWORD valueIndex = 0;
    LPSTR  valueName = new CHAR[maxValueNameSize];
    LPBYTE valueData = new BYTE[maxValueDataSize];
    DWORD valueNameSize, valueDataSize, valueType;

    do
    {
        valueNameSize = maxValueNameSize;
        valueDataSize = maxValueDataSize;

        result = RegEnumValueA(hKey, valueIndex++, valueName, &valueNameSize, 0, &valueType, valueData, &valueDataSize);
        if(result != ERROR_SUCCESS && valueType != REG_SZ)
        { continue; }

        valueName[valueNameSize] = '\0';

        if(strcmp(fontName, valueName) == 0)
        {
            valueData[valueDataSize] = '\0';
            break;
        }
    }
    while(result != ERROR_NO_MORE_ITEMS);

    delete[] valueName;

    RegCloseKey(hKey);

    CHAR winDir[MAX_PATH];
    GetWindowsDirectory(winDir, MAX_PATH);

    DynString ret(winDir);

    ret = ret.concat("\\Fonts\\").concat(reinterpret_cast<char*>(valueData));

    delete[] valueData;

    return ret;
}
#else
DynString findSystemFont(const char* fontName) noexcept
{
    return "";
}
#endif
