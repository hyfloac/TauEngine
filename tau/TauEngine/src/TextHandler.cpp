#include <TextHandler.hpp>
#include <Utils.hpp>
#include <texture/Texture.hpp>
#include <RenderingMode.hpp>
#include <model/BufferDescriptor.hpp>
#include <utility>
#include "VFS.hpp"
#include "model/IVertexArray.hpp"

TextHandler::TextHandler(IRenderingContext& context, const char* vertexPath, const char* fragmentPath) noexcept
    : _ready(false), _ft(null), _face(null), _data(), _chars(new GlyphCharacter[95]), _program(),
      _vertexShader(ShaderType::VERTEX, vertexPath, &_program), _fragmentShader(ShaderType::FRAGMENT, fragmentPath, &_program),
      _va(context.createVertexArray(1)),
      _vertexBuffer(IBuffer::create(context, 2, IBuffer::Type::ArrayBuffer, IBuffer::UsageType::DynamicDraw)),
      _projUni(0), _texUni(0), _colorUni(0)
{ 
    _vertexShader.loadShader();
    _fragmentShader.loadShader();
    _program.linkAndValidate();

    _projUni = _vertexShader.createUniform("projection");
    _texUni = _fragmentShader.createUniform("textBMP");
    _colorUni = _fragmentShader.createUniform("textColor");

    _vertexBuffer->bind(context);
    _vertexBuffer->fillBuffer(context, sizeof(GLfloat) * 4 * 6, null);
    _vertexBuffer->descriptor().addDescriptor(ShaderDataType::Vector2Float);
    _vertexBuffer->descriptor().addDescriptor(ShaderDataType::Vector2Float);
    _vertexBuffer->unbind(context);
    _va->addVertexBuffer(context, _vertexBuffer);
    _va->drawCount() = 6;

    // _vertexBuffer->bind(context);
    // _vertexBuffer->fillBuffer(context, 6, sizeof(GLfloat) * 6 * 4, null);

    // _bufferDescriptor->addAttribute(_vertexBuffer, 4, DataType::Float, false, 4 * sizeof(float), null);
    //
    // _vertexBuffer->unbind(context);
}

TextHandler::~TextHandler() noexcept
{
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
    Ref<IFile> file = VFS::Instance().openFile(fileName, FileProps::Read);
    
    if(!file)
    {
        return -1;
    }
    
    _data = new RefDynArray<u8>(file->readFile());
    
    const FT_Error error = FT_New_Memory_Face(_ft, _data->arr(), _data->size() - 1, 0, &_face);
    
    FT_Set_Pixel_Sizes(_face, 0, 48);

    return error;
}

int TextHandler::loadTTFFile(const char* fileName, ResourceLoader& rl, ResourceLoader::finalizeLoad_f finalizeLoad) noexcept
{
    const Ref<IFile> file = VFS::Instance().openFile(fileName, FileProps::Read);

    if(!file)
    {
        return -1;
    }

    rl.loadFile(file, TextHandler::load2, this, finalizeLoad, this);
    return 0;
}

void* TextHandler::load2(RefDynArray<u8> file, void* parseParam) noexcept
{
    TextHandler* th = reinterpret_cast<TextHandler*>(parseParam);
    th->_data = new RefDynArray<u8>(std::move(file));
    const FT_Error error = FT_New_Memory_Face(th->_ft, th->_data->arr(), th->_data->size() - 1, 0, &th->_face);
    FT_Set_Pixel_Sizes(th->_face, 0, 48);
    const intptr_t ret = error;
    return reinterpret_cast<void*>(ret);
}

void TextHandler::generateBitmapCharacters() const noexcept
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(GLchar c = 32; c <= 126; ++c)
    {
        if(FT_Load_Char(_face, c, FT_LOAD_RENDER)) { continue; }

        ITexture* texture = ITexture::create(RenderingMode::getGlobalMode());

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

void TextHandler::finishLoad() noexcept
{
    FT_Done_Face(_face);
    delete _data;
    _data = null;
    _ready = true;
}

void TextHandler::renderText(IRenderingContext& context, const char* str, float x, float y, float scale, Vector3f color, const glm::mat4& proj) const noexcept
{
    if(!_ready) { return; }
    _program.activate();
    _vertexShader.setUniform(_projUni, proj);
    _vertexShader.setUniform(_texUni, 0);
    _vertexShader.setUniform(_colorUni, color);

    _va->bind(context);
    _va->preDraw(context);

    // _bufferDescriptor->bind(context);
    // _bufferDescriptor->enableAttributes(context);

    glFrontFace(GL_CCW);

    for(char c = *str; c != '\0'; c = *(++str))
    {
        if(c < 32 || c > 126) { continue; }
        const GlyphCharacter* gc = &_chars[c - 32];

        const float xpos = x + gc->bearing.x() * scale;
        const float ypos = y - (gc->size.y() - gc->bearing.y()) * scale;

        const float w = gc->size.x() * scale;
        const float h = gc->size.y() * scale;
        // Update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        gc->texture->bind(0);

        _vertexBuffer->bind(context);
        _vertexBuffer->modifyBuffer(context, 0, sizeof(vertices), vertices);
        _vertexBuffer->unbind(context);

        _va->draw(context);

        gc->texture->unbind(0);

        x += (gc->advance >> 6) * scale;
    }

    // _bufferDescriptor->disableAttributes(context);
    // _bufferDescriptor->unbind(context);
    _va->postDraw(context);
    _va->unbind(context);
    // _chars[0].texture->unbind(0);
    GLProgram::deactivate();
}

float TextHandler::renderTextLineWrapped(IRenderingContext& context, const char* str, float x, float y, float scale, Vector3f color, const glm::mat4& proj, const Window& window, float lineHeight) const noexcept
{
    if(!_ready) { return 0.0f; }
    _program.activate();
    _vertexShader.setUniform(_projUni, proj);
    _vertexShader.setUniform(_texUni, 0);
    _vertexShader.setUniform(_colorUni, color);

    _va->bind(context);
    _va->preDraw(context);

    // _bufferDescriptor->bind(context);
    // _bufferDescriptor->enableAttributes(context);

    glFrontFace(GL_CCW);

    float height = lineHeight;

    const float initialX = x;
    const float maxX = static_cast<float>(window.width());

    for(char c = *str; c != '\0'; c = *(++str))
    {
        if(c < 32 || c > 126) { continue; }
        const GlyphCharacter* gc = &_chars[c - 32];

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
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        gc->texture->bind(0);

        _vertexBuffer->bind(context);
        _vertexBuffer->modifyBuffer(context, 0, sizeof(vertices), vertices);
        _vertexBuffer->unbind(context);

        _va->draw(context);

        gc->texture->unbind(0);

        x += advance;
    }

    // _bufferDescriptor->disableAttributes(context);
    // _bufferDescriptor->unbind(context);

    _va->postDraw(context);
    _va->unbind(context);
    GLProgram::deactivate();

    return height;
}

float TextHandler::computeLength(const char* str, float scale) const noexcept
{
    if(!_ready) { return 0.0f; }
    float length = 0.0f;

    for(char c = *str; c != '\0'; c = *(++str))
    {
        if(c < 32 || c > 126) { continue; }
        const GlyphCharacter* gc = &_chars[c - 32];
        length += (gc->advance >> 6) * scale;
    }

    return length;
}

float TextHandler::computeHeight(const char* str, float scale, float x, const Window& window, float lineHeight) const noexcept
{
    if(!_ready) { return 0.0f; }
    float height = lineHeight;

    const float initialX = x;
    const float maxX = static_cast<float>(window.width());

    for(char c = *str; c != '\0'; c = *(++str))
    {
        if(c < 32 || c > 126) { continue; }
        const GlyphCharacter* gc = &_chars[c - 32];

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

