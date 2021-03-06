#pragma once

#pragma warning(push, 0)
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/mat4x4.hpp>
#include <utility>
#include <vector>
#pragma warning(pop)

#include <DynArray.hpp>
#include <String.hpp>

#include "DLL.hpp"
#include "maths/Vector2f.hpp"
#include "maths/Vector3f.hpp"
#include "shader/Uniform.hpp"
#include "ResourceLoader.hpp"
#include "graphics/Resource.hpp"

struct LoadData;
class Window;
class IVertexArray;
class IVertexBuffer;
class IShaderProgram;
class ITexture;
class IBufferDescriptor;
class IRasterizerState;
class IGraphicsInterface;

struct GlyphCharacter final
{
    DEFAULT_DESTRUCT(GlyphCharacter);
    DELETE_CM(GlyphCharacter);
public:
    Vector2f coord; // Location in font map texture
    Vector2f size;
    Vector2f bearing; // Offset from baseline to left/top of glyph
    u32      advance; // Offset to advance to next glyph

    GlyphCharacter() noexcept
        : size(0.0f, 0.0f)
        , bearing(0.0f, 0.0f)
        , advance(0)
    { }

    GlyphCharacter(const Vector2f _size, const Vector2f _bearing, const u32 _advance) noexcept
        : size(_size)
        , bearing(_bearing)
        , advance(_advance)
    { }
};

struct GlyphSet final
{
    DEFAULT_DESTRUCT(GlyphSet);
    DEFAULT_CM(GlyphSet);
public:
    DynString setName;
    wchar_t minGlyph;
    wchar_t maxGlyph;
    uSys glyphCount;
    DynArray<GlyphCharacter> glyphs;
    NullableRef<IResource> texture;

    GlyphSet(const DynString& _setName, const wchar_t _minGlyph, const wchar_t _maxGlyph) noexcept
        : setName(_setName)
        , minGlyph(_minGlyph)
        , maxGlyph(_maxGlyph)
        , glyphCount(_maxGlyph - _minGlyph + 1)
        , glyphs(glyphCount)
        , texture(nullptr)
    { }
};

// using GlyphSetHandle = i64;
using GlyphSetHandle = NullableRef<GlyphSet>;

class TAU_DLL TextHandler final
{
    DELETE_COPY(TextHandler);
public:
    struct FileData final
    {
        DEFAULT_CONSTRUCT_PU(FileData);
        DEFAULT_CM_PU(FileData);
    public:
        FT_Face face;
        RefDynArray<u8> data;
    public:
        FileData(const FT_Face _face, const RefDynArray<u8>& _data)
            : face(_face)
            , data(_data)
        { }

        ~FileData() noexcept
        { FT_Done_Face(face); }
    };

    struct FinalizeData final
    {
        TextHandler& th;
        void* param;
    };

    struct ProjectionUniforms final
    {
        glm::mat4 projectionMatrix;
    };

    struct ColorUniforms final
    {
        Vector3f color;
    };
private:
    static NullableRef<IRasterizerState> rs;
private:
    FT_Library _ft;

    // std::vector<GlyphSet> _glyphSets;

    CPPRef<IShaderProgram> _shader;
    CPPRef<IVertexArray> _va;
    CPPRef<IVertexBuffer> _positionBuffer;
    UniformBlockS<ProjectionUniforms> _viewUniforms;
    UniformBlockS<ColorUniforms> _colorUniforms;
    NullableRef<ISingleTextureUploader> _textureUploader;
public:
    TextHandler(IGraphicsInterface& gi, IRenderingContext& context, const char* vfsMount, const char* path, const char* vertexName, const char* pixelName) noexcept;

    ~TextHandler() noexcept;

    [[nodiscard]] FT_Error init() noexcept;

    [[nodiscard]] FileData* loadTTFFile(const char* fileName, FT_UInt pixelWidth, FT_UInt pixelHeight) const noexcept;
    [[nodiscard]] int loadTTFFile(const char* fileName, FT_UInt pixelWidth, FT_UInt pixelHeight, ResourceLoader::finalizeLoadT_f<FinalizeData, FileData> finalizeLoad, void* userParam) noexcept;

    GlyphSetHandle generateBitmapCharacters(IGraphicsInterface& gi, IRenderingContext& context, const DynString& glyphSetName, wchar_t minChar, wchar_t maxChar, bool smooth, FT_Face face) noexcept;

    void renderText(IRenderingContext& context, GlyphSetHandle glyphSetHandle, const char* str, float x, float y, float scale, Vector3f color, const glm::mat4& proj) noexcept;
    float renderTextLineWrapped(IRenderingContext& context, GlyphSetHandle glyphSetHandle, const char* str, float x, float y, float scale, Vector3f color, const glm::mat4& proj, const Window& window, float lineHeight) noexcept;

    float computeLength(GlyphSetHandle glyphSetHandle, const char* str, float scale) const noexcept;
    float computeHeight(GlyphSetHandle glyphSetHandle, const char* str, float scale, float x, const Window& window, float lineHeight) const noexcept;
private:
    static FileData* __cdecl load2(RefDynArray<u8> file, LoadData* ld) noexcept;
};

WDynString findSystemFont(const WDynString& fontName) noexcept;
