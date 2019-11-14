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
#include "shader/IUniform.hpp"
#include "ResourceLoader.hpp"
#include "system/Mutex.hpp"

struct LoadData;
class Window;
class IVertexArray;
class IBuffer;
class IShaderProgram;
class ITexture;
class IBufferDescriptor;

struct GlyphCharacter final
{
    DEFAULT_COPY(GlyphCharacter);
    DEFAULT_DESTRUCT(GlyphCharacter);
public:
    Ref<ITexture> texture;
    Vector2f size;
    Vector2f bearing; // Offset from baseline to left/top of glyph
    u32      advance; // Offset to advance to next glyph

    inline GlyphCharacter() noexcept
        : texture(nullptr), size(0.0f, 0.0f), bearing(0.0f, 0.0f), advance(0)
    { }

    inline GlyphCharacter(ITexture* _texture, Vector2f _size, Vector2f _bearing, u32 _advance) noexcept
        : texture(_texture), size(_size), bearing(_bearing), advance(_advance)
    { }
};

struct GlyphSet final
{
    DEFAULT_COPY(GlyphSet);
    DEFAULT_DESTRUCT(GlyphSet);
public:
    DynString setName;
    char minGlyph;
    char maxGlyph;
    size_t glyphCount;
    DynArray<GlyphCharacter> glyphs;

    inline GlyphSet(DynString _setName, char _minGlyph, char _maxGlyph) noexcept
        : setName(std::move(_setName)), minGlyph(_minGlyph), maxGlyph(_maxGlyph),
          glyphCount(_maxGlyph - _minGlyph + 1), glyphs(glyphCount)
    { }
};

using GlyphSetHandle = i64;

class TAU_DLL TextHandler final
{
    DELETE_COPY(TextHandler);
public:
    struct FileData final
    {
        FT_Face face;
        RefDynArray<u8> data;

        ~FileData() noexcept
        {
            FT_Done_Face(face);
        }
    };

    struct FinalizeData final
    {
        TextHandler& th;
        void* param;
    };
private:
    FT_Library _ft;

    std::vector<GlyphSet> _glyphSets;

    Ref<IShaderProgram> _shader;
    Ref<IVertexArray> _va;
    Ref<IBuffer> _positionBuffer;
    Ref<IUniform<const glm::mat4&>> _projUni;
    Ref<IUniform<int>> _texUni;
    Ref<IUniform<const Vector3f&>> _colorUni;
public:
    TextHandler(IRenderingContext& context, const char* vertexPath, const char* fragmentPath) noexcept;

    ~TextHandler() noexcept;

    [[nodiscard]] FT_Error init() noexcept;

    [[nodiscard]] FileData* loadTTFFile(const char* fileName, FT_UInt pixelWidth, FT_UInt pixelHeight) noexcept;
    [[nodiscard]] int loadTTFFile(const char* fileName, FT_UInt pixelWidth, FT_UInt pixelHeight, ResourceLoader& rl, ResourceLoader::finalizeLoadT_f<FinalizeData, FileData> finalizeLoad, void* userParam) noexcept;

    GlyphSetHandle generateBitmapCharacters(const DynString& glyphSetName, char minChar, char maxChar, bool smooth, FT_Face face) noexcept;

    void renderText(IRenderingContext& context, GlyphSetHandle glyphSetHandle, const char* str, float x, float y, float scale, Vector3f color, const glm::mat4& proj) const noexcept;
    float renderTextLineWrapped(IRenderingContext& context, GlyphSetHandle glyphSetHandle, const char* str, float x, float y, float scale, Vector3f color, const glm::mat4& proj, const Window& window, float lineHeight) const noexcept;

    float computeLength(GlyphSetHandle glyphSetHandle, const char* str, float scale) const noexcept;
    float computeHeight(GlyphSetHandle glyphSetHandle, const char* str, float scale, float x, const Window& window, float lineHeight) const noexcept;
private:
    static FileData* __cdecl load2(RefDynArray<u8> file, LoadData* parseParam) noexcept;
};

DynString findSystemFont(const char* fontName) noexcept;
