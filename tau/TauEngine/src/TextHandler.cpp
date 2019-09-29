#include <TextHandler.hpp>
#include <Utils.hpp>
#include <texture/Texture.hpp>
#include <RenderingMode.hpp>
#include <model/BufferDescriptor.hpp>

// GlyphCharacter::~GlyphCharacter() noexcept
// {
//     delete texture;
// }

TextHandler::TextHandler(const char* vertexPath, const char* fragmentPath) noexcept
    : _ft(null), _face(null), _chars(new GlyphCharacter[95]), 
      _vertexShader(ShaderType::VERTEX, vertexPath, &_program), _fragmentShader(ShaderType::FRAGMENT, fragmentPath, &_program),
      _bufferDescriptor(IBufferDescriptor::create(RenderingMode::getGlobalMode())), _vbo(0),
      _projUni(0), _texUni(0), _colorUni(0)
{ 
    _vertexShader.loadShader();
    _fragmentShader.loadShader();
    _program.linkAndValidate();

    _projUni = _vertexShader.createUniform("projection");
    _texUni = _fragmentShader.createUniform("textBMP");
    _colorUni = _fragmentShader.createUniform("textColor");

    // glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    // glBindVertexArray(_vao);
    _bufferDescriptor->bind();
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, null, GL_DYNAMIC_DRAW);

    _bufferDescriptor->addAttribute(4, DataType::Float, false, 4 * sizeof(float), null);

    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), null);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);
    _bufferDescriptor->unbind();
}

TextHandler::~TextHandler() noexcept
{
    glDeleteBuffers(1, &_vbo);
    delete _bufferDescriptor;
    // glDeleteVertexArrays(1, &_vao);
    delete[] _chars;
    FT_Done_FreeType(_ft);
}

FT_Error TextHandler::init() noexcept
{
    const FT_Error error = FT_Init_FreeType(&_ft);
    return error;
}

FT_Error TextHandler::loadTTFFile(const char* fileName) noexcept
{
    const FT_Error error = FT_New_Face(_ft, fileName, 0, &_face);

    FT_Set_Pixel_Sizes(_face, 0, 48);

    return error;
}

void TextHandler::generateBitmapCharacters() const noexcept
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(GLchar c = 32; c <= 126; ++c)
    {
        if(FT_Load_Char(_face, c, FT_LOAD_RENDER)) { continue; }

        ITexture* texture = ITexture::create(RenderingMode::getGlobalMode());
        // GLuint texture;
        // glGenTextures(1, &texture);
        // glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                     _face->glyph->bitmap.width, _face->glyph->bitmap.rows,
                     0, GL_RED, GL_UNSIGNED_BYTE, _face->glyph->bitmap.buffer);

        _chars[c - 32] = GlyphCharacter(texture,
                                     Vector2f(static_cast<float>(_face->glyph->bitmap.width), static_cast<float>(_face->glyph->bitmap.rows)),
                                     Vector2f(static_cast<float>(_face->glyph->bitmap_left), static_cast<float>(_face->glyph->bitmap_top)),
                                     _face->glyph->advance.x);

    }
}

void TextHandler::finishLoad() const noexcept
{
    FT_Done_Face(_face);
}

void TextHandler::renderText(const char* str, GLfloat x, GLfloat y, GLfloat scale, Vector3f color, const Matrix4f& proj, RenderingPipeline& rp) const noexcept
{
    rp.pushActivateShaderProgram(_program.programId());
    rp.pushLoadUniform<ShaderUniformType::MAT4F> (ParameterPack(_projUni, PACK_PTR(proj.data().m)));
    rp.pushLoadUniform<ShaderUniformType::INTEGER>(ParameterPack( _texUni, 0));
    rp.pushLoadUniform<ShaderUniformType::VEC3F>(ParameterPack(_colorUni, reinterpret_cast<const u32&>(color.x()), reinterpret_cast<const u32&>(color.y()), reinterpret_cast<const u32&>(color.z())));
    
    // rp.pushInstruction<RenderingOpcode::ACTIVATE_TEXTURE_UNIT, 0>();

    rp.pushEnableBufferDescriptor(_bufferDescriptor);
    // rp.pushInstruction<RenderingOpcode::BIND_VAO>(ParameterPack(_vao));
    // rp.pushInstruction<RenderingOpcode::ENABLE_VAO_ATTRIBUTE, 0>();

    rp.pushGLFaceWinding(GL_CCW);
    rp.pushGLDisable(GL_CULL_FACE);

    for(char c = *str; c != '\0'; c = *(++str))
    {
        if(c < 32 || c > 126) { continue; }
        const GlyphCharacter* gc = &_chars[c - 32]; 
        
        const GLfloat xpos = x + gc->bearing.x() * scale;
        const GLfloat ypos = y - (gc->size.y() - gc->bearing.y()) * scale;

        const GLfloat w = gc->size.x() * scale;
        const GLfloat h = gc->size.y() * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };

        rp.pushBindTexture(gc->texture.get(), 0);
        
        rp.pushBindVBO(GL_ARRAY_BUFFER, _vbo);
        rp.pushModifyBufferData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        rp.pushBindVBO(GL_ARRAY_BUFFER, 0);

        rp.pushGLDrawArrays(GL_TRIANGLES, 0, 6);

        x += (gc->advance >> 6) * scale;
    }

    rp.pushGLEnable(GL_CULL_FACE);
    rp.pushGLFaceWinding(GL_CW);

    // rp.pushInstruction<RenderingOpcode::DISABLE_VAO_ATTRIBUTE, 0>();
    // rp.pushInstruction<RenderingOpcode::BIND_VAO, 0>();
    rp.pushDisableBufferDescriptor(_bufferDescriptor);
    rp.pushUnbindTexture(_chars[0].texture.get(), 0);
    rp.pushActivateShaderProgram(0);
}

void TextHandler::renderText(const char* str, GLfloat x, GLfloat y, GLfloat scale, Vector3f color, const Matrix4f& proj) const noexcept
{
    _program.activate();
    _vertexShader.setUniform(_projUni, proj);
    _vertexShader.setUniform(_texUni, 0);
    _vertexShader.setUniform(_colorUni, color);

    glActiveTexture(GL_TEXTURE0);

    // glBindVertexArray(_vao);
    _bufferDescriptor->bind();
    _bufferDescriptor->enableAttributes();
    // glEnableVertexAttribArray(0);
    // glDisable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    for(char c = *str; c != '\0'; c = *(++str))
    {
        if(c < 32 || c > 126) { continue; }
        const GlyphCharacter* gc = &_chars[c - 32];

        const GLfloat xpos = x + gc->bearing.x() * scale;
        const GLfloat ypos = y - (gc->size.y() - gc->bearing.y()) * scale;

        const GLfloat w = gc->size.x() * scale;
        const GLfloat h = gc->size.y() * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };

        gc->texture->bind(0);

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (gc->advance >> 6) * scale;
    }
    // glEnable(GL_CULL_FACE);

    // glDisableVertexAttribArray(0);
    // glBindVertexArray(0);
    _bufferDescriptor->disableAttributes();
    _bufferDescriptor->unbind();
    _chars[0].texture->unbind(0);
    // glBindTexture(GL_TEXTURE_2D, 0);
    GLProgram::deactivate();
}
