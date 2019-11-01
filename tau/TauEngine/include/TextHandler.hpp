#pragma once

#include <TauEngine.hpp>
#pragma warning(push, 0)
#include <ft2build.h>
#include FT_FREETYPE_H

#include <GL/glew.h>
#pragma warning(pop)

#include <DLL.hpp>
#include <maths/Vector2f.hpp>
#include <maths/Vector3f.hpp>
#include <maths/Matrix4x4f.hpp>
#include <RenderingPipeline.hpp>
#include <shader/GLShader.hpp>
#include <DynArray.hpp>
#include <glm/mat4x4.hpp>
#include "ResourceLoader.hpp"
#include "model/IBuffer.hpp"

class ITexture;
class IBufferDescriptor;

struct GlyphCharacter final
{
    Ref<ITexture> texture;
    Vector2f size;
    Vector2f bearing;   // Offset from baseline to left/top of glyph
    GLuint   advance;   // Offset to advance to next glyph

    GlyphCharacter() noexcept
        : texture(nullptr), size(0.0f, 0.0f), bearing(0.0f, 0.0f), advance(0)
    { }

    GlyphCharacter(ITexture* _texture, Vector2f _size, Vector2f _bearing, GLuint _advance) noexcept
        : texture(_texture), size(_size), bearing(_bearing), advance(_advance)
    { }

    ~GlyphCharacter() noexcept = default;

    GlyphCharacter(const GlyphCharacter& copy) noexcept = default;
    GlyphCharacter(GlyphCharacter&& move) noexcept = default;

    GlyphCharacter& operator =(const GlyphCharacter& copy) noexcept = default;
    GlyphCharacter& operator =(GlyphCharacter&& move) noexcept = default;
};

class TAU_DLL TextHandler final
{
    DELETE_COPY(TextHandler);
private:
    bool _ready;
    FT_Library _ft;
    FT_Face    _face;
    RefDynArray<u8>* _data;
    GlyphCharacter* _chars;
    GLProgram _program;
    GLShader  _vertexShader;
    GLShader  _fragmentShader;
    Ref<IBufferDescriptor> _bufferDescriptor;
    Ref<IBuffer> _vertexBuffer;
    GLuint _projUni;
    GLuint _texUni;
    GLuint _colorUni;
public:
    TextHandler(IRenderingContext& context, const char* vertexPath, const char* fragmentPath) noexcept;

    ~TextHandler() noexcept;

    [[nodiscard]] FT_Error init() noexcept;

    [[nodiscard]] FT_Error loadTTFFile(const char* fileName) noexcept;
    [[nodiscard]] int loadTTFFile(const char* fileName, ResourceLoader& rl, ResourceLoader::finalizeLoad_f finalizeLoad) noexcept;

    void generateBitmapCharacters() const noexcept;

    void finishLoad() noexcept;

    void renderText(IRenderingContext& context, const char* str, float x, float y, float scale, Vector3f color, const glm::mat4& proj) const noexcept;
    float renderTextLineWrapped(IRenderingContext& context, const char* str, float x, float y, float scale, Vector3f color, const glm::mat4& proj, const Window& window, float lineHeight) const noexcept;

    float computeLength(const char* str, float scale) const noexcept;
    float computeHeight(const char* str, float scale, float x, const Window& window, float lineHeight) const noexcept;
private:
    static void* __cdecl load2(RefDynArray<u8> file, void* parseParam) noexcept;
};
