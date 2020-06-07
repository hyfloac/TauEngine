#include <cstdio>
#include <cstring>
#include <cstdint>

class ITextureView
{
public:
    virtual ~ITextureView() = default;

    virtual int format() const = 0;
    virtual void print() const = 0;
};

class ITexture 
{
public:
    virtual ~ITexture() = default;

    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual ITextureView* textureView() = 0;
};

class DXTextureView : public ITextureView
{
private:
    int _format;
    char* _textureData;
public:
    DXTextureView(const int format, char* const textureData)
        : _format(format)
        , _textureData(textureData)
    { }

    ~DXTextureView() 
    { delete _textureData; }

    int format() const override { return _format; }

    void print() const override
    {
        printf("%s\n", _textureData);
    }
};

class DXTexture : public ITexture
{
private:
    int _width;
    int _height;
    DXTextureView* _textureView;
public:
    DXTexture(char* const textureData, const int width, const int height, const int format)
        : _width(width)
        , _height(height)
        , _textureView(new DXTextureView(format, textureData))
    { }

    ~DXTexture()
    { delete _textureView; }

    int width() const override { return _width; }
    int height() const override { return _height; }
    ITextureView* textureView() override { return _textureView; }
};

static int _gl_maxCount = 0;
static int _gl_currCount = 0;
static char** _gl_textures = nullptr;

int glGenTexture()
{
    if(!_gl_textures)
    {
        _gl_maxCount = 16;
        _gl_textures = new char*[_gl_maxCount];
    }
    else if(_gl_currCount + 1 >= _gl_maxCount)
    {
        int newCount = _gl_maxCount * 2;
        char** newTextures = new char*[newCount];
        memcpy(newTextures, _gl_textures, _gl_maxCount);
        delete[] _gl_textures;
        _gl_maxCount = newCount;
        _gl_textures = newTextures;
    }

    for(int i = 0; i < _gl_maxCount; ++i)
    {
        if(!_gl_textures[i])
        {
            ++_gl_currCount;
            _gl_textures[i] = reinterpret_cast<char*>(static_cast<intptr_t>(1));
            return i;
        }
    }

    return -1;
}

void glDeleteTexture(int handle)
{
    if(handle < _gl_maxCount)
    {
        if(_gl_textures[handle])
        {
            delete[] _gl_textures[handle];
            _gl_textures[handle] = nullptr;
            --_gl_currCount;
        }
    }
}

void glSetTextureData(int handle, int length, void* data)
{
    if(handle < _gl_maxCount)
    {
        if(!_gl_textures[handle])
        { return; }

        if(_gl_textures[handle] != reinterpret_cast<char*>(static_cast<intptr_t>(1)))
        {
            delete[] _gl_textures[handle];
        }

        _gl_textures[handle] = new char[length];
        memcpy(_gl_textures[handle], data, length);
    }
}

void glPrintTexture(int handle)
{
    if(handle < _gl_maxCount)
    {
        if(!_gl_textures[handle])
        { return; }

        if(_gl_textures[handle] == reinterpret_cast<char*>(static_cast<intptr_t>(1)))
        { return; }

        printf("%s\n", _gl_textures[handle]);
    }
}

class GLTexture : public ITexture, public ITextureView
{
private:
    int _textureHandle;
    int _width;
    int _height;
    int _format;
public:
    GLTexture(const int textureHandle, const int width, const int height, const int format)
        : _textureHandle(textureHandle)
        , _width(width)
        , _height(height)
        , _format(format)
    { }

    ~GLTexture()
    { glDeleteTexture(_textureHandle); }

    int textureHandle() const { return _textureHandle; }

    int width() const override { return _width; }
    int height() const override { return _height; }
    int format() const override { return _format; }
    ITextureView* textureView() override { return this; }

    void print() const override
    {
        glPrintTexture(_textureHandle);
    }
};

void printWidth(ITexture* texture)
{
    printf("Width: %d\n", texture->width());
}

void printHeight(ITexture* texture)
{
    printf("Height: %d\n", texture->height());
}

void printFormat(ITextureView* view)
{
    printf("Format: %d\n", view->format());
}

void printFormat(ITexture* texture)
{
    printFormat(texture->textureView());
}

void fillGLTextureFast(ITexture* texture, int length, void* data)
{
    GLTexture* glTexture = reinterpret_cast<GLTexture*>(texture);
    glSetTextureData(glTexture->textureHandle(), length, data);
}

void fillGLTexture(ITexture* texture, int length, void* data)
{
    GLTexture* glTexture = dynamic_cast<GLTexture*>(texture);
    glSetTextureData(glTexture->textureHandle(), length, data);
}

int main(int argCount, char* args[])
{
    ITexture* texture;
    
    if(argCount > 3)
    {
        texture = new GLTexture(glGenTexture(), 4, 6, 2);
        fillGLTexture(texture, strlen(args[0]) + 1, args[0]);
    }
    else
    {
        char* data = new char[strlen(args[0]) + 1];
        memcpy(data, args[0], strlen(args[0]) + 1);
        texture = new DXTexture(data, 4, 6, 2);
    }

    texture->textureView()->print();
    printWidth(texture);
    printHeight(texture);
    printFormat(texture);

    delete texture;
    return 0;
}
