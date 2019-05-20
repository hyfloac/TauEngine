#pragma once

#pragma warning(push, 0)
#include <ft2build.h>
#include FT_FREETYPE_H

#include <GL/glew.h>
#include <unordered_map>
#pragma warning(pop)

#include <DLL.hpp>
#include <maths/Vector2f.hpp>
#include <maths/Vector3f.hpp>
#include <maths/Matrix4x4f.hpp>
#include <RenderingPipeline.hpp>
#include <String.hpp>
#include <shader/GLShader.hpp>

struct GlyphCharacter final
{
    GLuint   textureID;
    Vector2f size;
    Vector2f bearing;   // Offset from baseline to left/top of glyph
    GLuint   advance;   // Offset to advance to next glyph

    GlyphCharacter() noexcept = default;

    GlyphCharacter(GLuint _textureID, Vector2f _size, Vector2f _bearing, GLuint _advance) noexcept
        : textureID(_textureID), size(_size), bearing(_bearing), advance(_advance)
    { }

    ~GlyphCharacter() noexcept = default;

    GlyphCharacter(const GlyphCharacter& copy) noexcept = default;
    GlyphCharacter(GlyphCharacter&& move) noexcept = default;

    GlyphCharacter& operator =(const GlyphCharacter& copy) noexcept = default;
    GlyphCharacter& operator =(GlyphCharacter&& move) noexcept = default;
};

struct RenderableString final
{
    GLuint vaoID;
    GLuint vboID;

    RenderableString() noexcept = default;

    RenderableString(GLuint _vaoID, GLuint _vboID) noexcept
        : vaoID(_vaoID), vboID(_vboID)
    { }

    ~RenderableString() noexcept = default;

    RenderableString (const RenderableString & copy) noexcept = default;
    RenderableString (RenderableString && move) noexcept = default;

    RenderableString& operator =(const RenderableString & copy) noexcept = default;
    RenderableString& operator =(RenderableString && move) noexcept = default;
};

class TAU_DLL TextHandler final
{
private:
    FT_Library _ft;
    FT_Face    _face;
    GlyphCharacter* _chars;
    std::unordered_map<String, RenderableString> _strings;
    GLProgram _program;
    GLShader  _vertexShader;
    GLShader  _fragmentShader;
    GLuint _vao;
    GLuint _vbo;
    GLuint _projUni;
    GLuint _texUni;
    GLuint _colorUni;
public:
    TextHandler(const char* vertexPath, const char* fragmentPath) noexcept;

    ~TextHandler() noexcept;

    FT_Error init() noexcept;

    FT_Error loadTTFFile(const char* fileName) noexcept;

    void generateBitmapCharacters() const noexcept;

    void finishLoad() const noexcept;

    void generateText(const String& str, GLfloat x, GLfloat y, GLfloat scale) noexcept;

    void renderTexts(RenderingPipeline& rp) const noexcept;

    void removeText(const String& str) noexcept;

    void renderText(const char* str, GLfloat x, GLfloat y, GLfloat scale, Vector3f color, const Matrix4f& proj, RenderingPipeline& rp) const noexcept;
    void renderText(const char* str, GLfloat x, GLfloat y, GLfloat scale, Vector3f color, const Matrix4f& proj) const noexcept;
};
