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
// #include <texture/Texture.hpp>

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
private:
    FT_Library _ft;
    FT_Face    _face;
    GlyphCharacter* _chars;
    GLProgram _program;
    GLShader  _vertexShader;
    GLShader  _fragmentShader;
    IBufferDescriptor* _bufferDescriptor;
    GLuint _vbo;
    GLuint _projUni;
    GLuint _texUni;
    GLuint _colorUni;
public:
    TextHandler(const char* vertexPath, const char* fragmentPath) noexcept;

    ~TextHandler() noexcept;

    TextHandler(const TextHandler& copy) noexcept = default;
    TextHandler(TextHandler&& move) noexcept = default;

    TextHandler& operator =(const TextHandler& copy) noexcept = default;
    TextHandler& operator =(TextHandler&& move) noexcept = default;

    FT_Error init() noexcept;

    FT_Error loadTTFFile(const char* fileName) noexcept;

    void generateBitmapCharacters() const noexcept;

    void finishLoad() const noexcept;

    void renderText(const char* str, GLfloat x, GLfloat y, GLfloat scale, Vector3f color, const Matrix4f& proj, RenderingPipeline& rp) const noexcept;
    void renderText(const char* str, GLfloat x, GLfloat y, GLfloat scale, Vector3f color, const Matrix4f& proj) const noexcept;
};
