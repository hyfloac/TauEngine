#include <TextHandler.hpp>
#include <Utils.hpp>

TextHandler::TextHandler(const char* vertexPath, const char* fragmentPath) noexcept
    : _ft(null), _face(null), _chars(new GlyphCharacter[95]), _vertexShader(ShaderType::VERTEX, vertexPath, &_program), _fragmentShader(ShaderType::FRAGMENT, fragmentPath, &_program)
{ 
    _vertexShader.loadShader();
    _fragmentShader.loadShader();
    _program.linkAndValidate();

    _projUni = _vertexShader.createUniform("projection");
    _texUni = _fragmentShader.createUniform("textBMP");
    _colorUni = _fragmentShader.createUniform("textColor");

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, null, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), null);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TextHandler::~TextHandler() noexcept
{
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);
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

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                     _face->glyph->bitmap.width, _face->glyph->bitmap.rows,
                     0, GL_RED, GL_UNSIGNED_BYTE, _face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        _chars[c - 32] = GlyphCharacter(texture,
                                     Vector2f(_face->glyph->bitmap.width, _face->glyph->bitmap.rows),
                                     Vector2f(_face->glyph->bitmap_left, _face->glyph->bitmap_top),
                                     _face->glyph->advance.x);

    }
}

void TextHandler::finishLoad() const noexcept
{
    FT_Done_Face(_face);
}

void TextHandler::generateText(const String& str, GLfloat x, GLfloat y, GLfloat scale) noexcept
{
    const u32 bufferSize = sizeof(GLfloat) * 6 * 4 * str.length();

    GLfloat* vertices = new GLfloat[bufferSize];

    for(u32 i = 0; i < str.length(); ++i)
    {
        const char c = str.c_str()[i];
        if(c < 32 || c > 126) 
        { 
            delete[] vertices;
            return; 
        }
        const GlyphCharacter* gc = &_chars[c - 32];

        const GLfloat xPos = x + gc->bearing.x() * scale;
        const GLfloat yPos = y - (gc->size.y() - gc->bearing.y()) * scale;

        const GLfloat w = gc->size.x() * scale;
        const GLfloat h = gc->size.y() * scale;

        vertices[i * 6 * 4 + 0x00] = xPos;
        vertices[i * 6 * 4 + 0x01] = yPos + h;
        vertices[i * 6 * 4 + 0x02] = 0.0f;
        vertices[i * 6 * 4 + 0x03] = 0.0f;

        vertices[i * 6 * 4 + 0x04] = xPos;
        vertices[i * 6 * 4 + 0x05] = yPos;
        vertices[i * 6 * 4 + 0x06] = 0.0f;
        vertices[i * 6 * 4 + 0x07] = 1.0f;

        vertices[i * 6 * 4 + 0x08] = xPos + w;
        vertices[i * 6 * 4 + 0x09] = yPos;
        vertices[i * 6 * 4 + 0x0A] = 1.0f;
        vertices[i * 6 * 4 + 0x0B] = 1.0f;


        vertices[i * 6 * 4 + 0x0C] = xPos;
        vertices[i * 6 * 4 + 0x0D] = yPos + h;
        vertices[i * 6 * 4 + 0x0E] = 0.0f;
        vertices[i * 6 * 4 + 0x0F] = 0.0f;

        vertices[i * 6 * 4 + 0x10] = xPos + w;
        vertices[i * 6 * 4 + 0x11] = yPos;
        vertices[i * 6 * 4 + 0x12] = 1.0f;
        vertices[i * 6 * 4 + 0x13] = 1.0f;

        vertices[i * 6 * 4 + 0x14] = xPos + w;
        vertices[i * 6 * 4 + 0x15] = yPos + h;
        vertices[i * 6 * 4 + 0x16] = 1.0f;
        vertices[i * 6 * 4 + 0x17] = 0.0f;

        x += (gc->advance >> 6) * scale;
    }

    RenderableString rs;

    glGenVertexArrays(1, &rs.vaoID);
    glGenBuffers(1, &rs.vboID);

    glBindVertexArray(rs.vaoID);
    glBindBuffer(GL_ARRAY_BUFFER, rs.vboID);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), null);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    _strings.insert({ str, rs });
}

void TextHandler::renderTexts(RenderingPipeline& rp) const noexcept
{
    
}

void TextHandler::removeText(const String& str) noexcept
{
    _strings.erase(str);
}

void TextHandler::renderText(const char* str, GLfloat x, GLfloat y, GLfloat scale, Vector3f color, const Matrix4f& proj, RenderingPipeline& rp) const noexcept
{
    rp.pushInstruction(RenderingOpcode::ACTIVATE_SHADER_PROGRAM, _program.programId());
    rp.pushInstruction(RenderingOpcode::LOAD_SHADER_UNIFORM, ParameterPack(static_cast<u64>(ShaderUniformType::MAT4F), _projUni, PACK_PTR(proj.data().m)));
    rp.pushInstruction(RenderingOpcode::LOAD_SHADER_UNIFORM, ParameterPack(static_cast<u64>(ShaderUniformType::INTEGER), _texUni, 0));
    rp.pushInstruction(RenderingOpcode::LOAD_SHADER_UNIFORM, ParameterPack(static_cast<u64>(ShaderUniformType::VEC3F), _colorUni, reinterpret_cast<const u32&>(color.x()), reinterpret_cast<const u32&>(color.y()), reinterpret_cast<const u32&>(color.z())));
    
    rp.pushInstruction(RenderingOpcode::ACTIVATE_TEXTURE_UNIT, ParameterPack(0));

    rp.pushInstruction(RenderingOpcode::BIND_VAO, ParameterPack(_vao));
    rp.pushInstruction(RenderingOpcode::ENABLE_VAO_ATTRIBUTE, ParameterPack(0));

    rp.pushInstruction(RenderingOpcode::GL_FACE_WINDING, ParameterPack(GL_CCW));
    rp.pushInstruction(RenderingOpcode::GL_DISABLE, ParameterPack(GL_CULL_FACE));

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

        rp.pushInstruction(RenderingOpcode::BIND_TEXTURE, ParameterPack(gc->textureID));
        
        rp.pushInstruction(RenderingOpcode::BIND_VBO, ParameterPack(GL_ARRAY_BUFFER, _vbo));
        rp.pushInstruction(RenderingOpcode::MODIFY_BUFFER_DATA, ParameterPack(GL_ARRAY_BUFFER, 0, sizeof(vertices), reinterpret_cast<u64>(vertices)));
        rp.pushInstruction(RenderingOpcode::BIND_VBO, ParameterPack(GL_ARRAY_BUFFER, 0));

        rp.pushInstruction(RenderingOpcode::GL_DRAW_ARRAYS, ParameterPack(GL_TRIANGLES, 0, 6));

        x += (gc->advance >> 6) * scale;
    }

    rp.pushInstruction(RenderingOpcode::GL_ENABLE, ParameterPack(GL_CULL_FACE));
    rp.pushInstruction(RenderingOpcode::GL_FACE_WINDING, ParameterPack(GL_CW));

    rp.pushInstruction(RenderingOpcode::DISABLE_VAO_ATTRIBUTE, ParameterPack(0));
    rp.pushInstruction(RenderingOpcode::BIND_VAO, ParameterPack(0));
    rp.pushInstruction(RenderingOpcode::BIND_TEXTURE, ParameterPack(0));
    rp.pushInstruction(RenderingOpcode::ACTIVATE_SHADER_PROGRAM, ParameterPack(0));
}

void TextHandler::renderText(const char* str, GLfloat x, GLfloat y, GLfloat scale, Vector3f color, const Matrix4f& proj) const noexcept
{
    _program.activate();
    _vertexShader.setUniform(_projUni, proj);
    _vertexShader.setUniform(_texUni, 0);
    _vertexShader.setUniform(_colorUni, color);

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(_vao);
    glEnableVertexAttribArray(0);
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

        glBindTexture(GL_TEXTURE_2D, gc->textureID);

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (gc->advance >> 6) * scale;
    }
    // glEnable(GL_CULL_FACE);

    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    GLProgram::deactivate();
}
