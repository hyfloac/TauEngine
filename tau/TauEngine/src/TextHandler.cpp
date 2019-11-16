#pragma warning(push, 0)
#include <utility>
#pragma warning(pop)

#include <Utils.hpp>
#include <VFS.hpp>

#include "TextHandler.hpp"
#include "texture/Texture.hpp"
#include "RenderingMode.hpp"
#include "model/BufferDescriptor.hpp"
#include "model/IVertexArray.hpp"
#include "shader/IShaderProgram.hpp"
#include "shader/IShader.hpp"
#include "RenderingPipeline.hpp"
#include <Windows.h>
#include <winreg.h>
#include "Timings.hpp"

TextHandler::TextHandler(IRenderingContext& context, const char* vertexPath, const char* fragmentPath) noexcept
    : _ft(null), _glyphSets(), _shader(IShaderProgram::create(context)),
      _va(context.createVertexArray(2)),
      _positionBuffer(IBuffer::create(context, 1, IBuffer::Type::ArrayBuffer, IBuffer::UsageType::DynamicDraw)),
      _projUni(null), _texUni(null), _colorUni(null)
{
    PERF();
    Ref<IShader> vertexShader = IShader::create(context, IShader::Type::Vertex, vertexPath);
    Ref<IShader> pixelShader = IShader::create(context, IShader::Type::Pixel, fragmentPath);

    vertexShader->loadShader();
    pixelShader->loadShader();
    _shader->setVertexShader(context, vertexShader);
    _shader->setPixelShader(context, pixelShader);

    _shader->link(context);

    _projUni = _shader->getUniformMatrix4x4Float("projectionMatrix", false);
    _texUni = _shader->getUniformInt("textBMP");
    _colorUni = _shader->getUniformVector3f("textColor");

    Ref<IBuffer> textureCoordBuffer = IBuffer::create(context, 1, IBuffer::Type::ArrayBuffer);

    _positionBuffer->bind(context);
    _positionBuffer->fillBuffer(context, sizeof(float) * 2 * 6, null);
    _positionBuffer->descriptor().addDescriptor(ShaderDataType::Vector2Float);
    _positionBuffer->unbind(context);

    float textureCoords[6][2] = {
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
          
        { 0.0f, 0.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f }
    };

    textureCoordBuffer->bind(context);
    textureCoordBuffer->fillBuffer(context, sizeof(float) * 2 * 6, textureCoords);
    textureCoordBuffer->descriptor().addDescriptor(ShaderDataType::Vector2Float);
    textureCoordBuffer->unbind(context);

    _va->addVertexBuffer(context, _positionBuffer);
    _va->addVertexBuffer(context, textureCoordBuffer);
    _va->drawCount() = 6;
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

TextHandler::FileData* TextHandler::loadTTFFile(const char* const fileName, const FT_UInt pixelWidth, const FT_UInt pixelHeight) noexcept
{
    PERF();
    const Ref<IFile> file = VFS::Instance().openFile(fileName, FileProps::Read);
    
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
    const Ref<IFile> file = VFS::Instance().openFile(fileName, FileProps::Read);

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

GlyphSetHandle TextHandler::generateBitmapCharacters(const DynString& glyphSetName, const char minChar, const char maxChar, const bool smooth, FT_Face face) noexcept
{
    PERF();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GlyphSet& gs = _glyphSets.emplace_back(glyphSetName, minChar, maxChar);

    const GLint filterType = smooth ? GL_LINEAR : GL_NEAREST;

    for(GLchar c = minChar; c <= maxChar; ++c)
    {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)) { continue; }

        ITexture* texture = ITexture::create(RenderingMode::getGlobalMode());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                     face->glyph->bitmap.width, face->glyph->bitmap.rows,
                     0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        gs.glyphs[c - gs.minGlyph] = GlyphCharacter(texture,
                                     Vector2f(static_cast<float>(face->glyph->bitmap.width), static_cast<float>(face->glyph->bitmap.rows)),
                                     Vector2f(static_cast<float>(face->glyph->bitmap_left), static_cast<float>(face->glyph->bitmap_top)),
                                     face->glyph->advance.x);
    }

    return _glyphSets.size() - 1;
}

void TextHandler::renderText(IRenderingContext& context, GlyphSetHandle glyphSetHandle, const char* str, float x, float y, float scale, Vector3f color, const glm::mat4& proj) const noexcept
{
    const GlyphSet& glyphSet = _glyphSets[glyphSetHandle];

    _shader->bind(context);
    _projUni->set(proj);
    _texUni->set(0);
    _colorUni->set(color);

    _va->bind(context);
    _va->preDraw(context);

    glFrontFace(GL_CCW);

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

        gc->texture->bind(0);

        _positionBuffer->bind(context);
        _positionBuffer->modifyBuffer(context, 0, sizeof(vertices), vertices);
        _positionBuffer->unbind(context);

        _va->draw(context);

        gc->texture->unbind(0);

        x += (gc->advance >> 6) * scale;
    }

    _va->postDraw(context);
    _va->unbind(context);
    _shader->unbind(context);
}

float TextHandler::renderTextLineWrapped(IRenderingContext& context, GlyphSetHandle glyphSetHandle, const char* str, float x, float y, float scale, Vector3f color, const glm::mat4& proj, const Window& window, float lineHeight) const noexcept
{
    const GlyphSet& glyphSet = _glyphSets[glyphSetHandle];

    _shader->bind(context);
    _projUni->set(proj);
    _texUni->set(0);
    _colorUni->set(color);

    _va->bind(context);
    _va->preDraw(context);

    glFrontFace(GL_CCW);

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

        gc->texture->bind(0);

        _positionBuffer->bind(context);
        _positionBuffer->modifyBuffer(context, 0, sizeof(vertices), vertices);
        _positionBuffer->unbind(context);

        _va->draw(context);

        gc->texture->unbind(0);

        x += advance;
    }

    _va->postDraw(context);
    _va->unbind(context);
    _shader->unbind(context);

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

    ret.append("\\Fonts\\").append(reinterpret_cast<char*>(valueData));

    delete[] valueData;

    return ret;
}
#else
DynString findSystemFont(const char* fontName) noexcept
{
    return "";
}
#endif
