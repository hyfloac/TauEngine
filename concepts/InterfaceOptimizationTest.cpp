#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstring>

#define SUPPORT_GL 1
#define SUPPORT_DX 1

#ifdef _WIN32
  #define DLL_EXPORT __declspec(dllexport)
#else
  #define DLL_EXPORT __attribute__((visibility("default")))
#endif

typedef ::std::size_t uSys;
typedef ::std::uint8_t u8;

typedef int GLenum;
typedef int GLint;
typedef int GLsizei;
typedef unsigned GLuint;

typedef unsigned long ULONG;

class DLL_EXPORT IUnknown
{
public:
    virtual ULONG AddRef(void) = 0;
    virtual ULONG Release(void) = 0;
};

class DLL_EXPORT ID3D10Texture2D : public IUnknown
{
public:
    virtual void set(uSys count, const void* data) = 0;
};

#define GL_TEXTURE_2D 0x8173

static uSys _g_textureCount = 0;
static u8** _g_textures = nullptr;

DLL_EXPORT void glGenTextures(const GLsizei n, GLuint* const textures)
{
    if(textures)
    {
        uSys handleIndex = 0;
        uSys texIndex = 0;
        for(; texIndex < _g_textureCount && handleIndex < n; ++texIndex)
        {
            if(!_g_textures[texIndex])
            {
                textures[handleIndex] = texIndex;
                _g_textures[handleIndex] = reinterpret_cast<u8*>(static_cast<uintptr_t>(1));
                ++handleIndex;
            }
        }

        if(handleIndex == n)
        {
            return;
        }

        const uSys newCount = (_g_textureCount + (n - handleIndex)) * 2;
        u8** texturesNew = new u8*[newCount];
        ::std::memcpy(texturesNew, _g_textures, _g_textureCount);

        delete[] _g_textures;

        _g_textureCount = newCount;
        _g_textures = texturesNew;

        for(; texIndex < _g_textureCount && handleIndex < n; ++texIndex)
        {
            if(!_g_textures[texIndex])
            {
                textures[handleIndex] = texIndex;
                _g_textures[handleIndex] = reinterpret_cast<u8*>(static_cast<uintptr_t>(1));
                ++handleIndex;
            }
        }
    }
    else
    {
        _g_textureCount = n * 2;
        _g_textures = new u8*[_g_textureCount];
        for(uSys i = 0; i < n; ++i)
        {
            textures[i] = i;
            _g_textures[i] = reinterpret_cast<u8*>(static_cast<uintptr_t>(1));
        }
    }
}

DLL_EXPORT void glDeleteTextures(const GLsizei n, GLuint* const textures)
{
    for(uSys i = 0; i < n; ++i)
    {
        const GLuint index = textures[i];
        delete[] _g_textures[index];
        _g_textures[index] = nullptr;
    }
}

DLL_EXPORT void glTexImage2D(GLuint texture, uSys count, const void* data)
{
    if(_g_textures[texture] != reinterpret_cast<u8*>(static_cast<uintptr_t>(1)))
    {
        delete[] _g_textures[texture];
    }

    _g_textures[texture] = new u8[count];
    ::std::memcpy(_g_textures[texture], data, count);
}

class DLL_EXPORT ITexture2D
{
protected:
    uSys _width;
    uSys _height;
protected:
    ITexture2D(uSys width, uSys height);
public:
    inline ~ITexture2D() noexcept = default;

    inline ITexture2D(const ITexture2D& copy) noexcept = delete;
    inline ITexture2D(ITexture2D&& move) noexcept = delete;
    inline ITexture2D& operator=(const ITexture2D& copy) noexcept = delete;
    inline ITexture2D& operator=(ITexture2D&& move) noexcept = delete;

    [[nodiscard]] uSys width() const noexcept { return _width; }
    [[nodiscard]] uSys height() const noexcept { return _height; }

    virtual void set(const void* data) = 0;
};

ITexture2D::ITexture2D(const uSys width, const uSys height)
    : _width(width)
    , _height(height)
{ }

#if SUPPORT_GL
class DLL_EXPORT GLTexture2D final : public ITexture2D
{
private:
    GLuint _textureHandle;
public:
    GLTexture2D(uSys width, uSys height);

    inline ~GLTexture2D();

    inline GLTexture2D(const GLTexture2D& copy) noexcept = delete;
    inline GLTexture2D(GLTexture2D&& move) noexcept = delete;
    inline GLTexture2D& operator=(const GLTexture2D& copy) noexcept = delete;
    inline GLTexture2D& operator=(GLTexture2D&& move) noexcept = delete;

    void set(const void* data);
};

GLTexture2D::GLTexture2D(const uSys width, const uSys height)
    : ITexture2D(width, height)
    , _textureHandle(0)
{  glGenTextures(1, &_textureHandle); }

GLTexture2D::~GLTexture2D()
{ glDeleteTextures(1, &_textureHandle); }

void GLTexture2D::set(const void* const data)
{ glTexImage2D(GL_TEXTURE_2D, _width * _height * 4, data); }
#endif

#if SUPPORT_DX
class DLL_EXPORT DXTexture2D final : public ITexture2D
{
private:
    ID3D10Texture2D* _texture;
public:
    DXTexture2D(uSys width, uSys height, ID3D10Texture2D* texture);

    ~DXTexture2D() noexcept;

    inline DXTexture2D(const DXTexture2D& copy) noexcept = delete;
    inline DXTexture2D(DXTexture2D&& move) noexcept = delete;
    inline DXTexture2D& operator=(const DXTexture2D& copy) noexcept = delete;
    inline DXTexture2D& operator=(DXTexture2D&& move) noexcept = delete;

    void set(const void* data);
};

DXTexture2D::DXTexture2D(const uSys width, const uSys height, ID3D10Texture2D* const texture)
    : ITexture2D(width, height)
    , _texture(texture)
{ _texture->AddRef(); }

DXTexture2D::~DXTexture2D() noexcept
{ _texture->Release(); }

void DXTexture2D::set(const void* const data)
{ _texture->set(_width * _height * 4, data); }
#endif

DLL_EXPORT bool loadTexture(ITexture2D* const texture, FILE* const file) noexcept
{
    const uSys len = texture->width() * texture->height() * 4;
    u8* const buffer = new u8[len];

    const uSys readCount = fread(buffer, sizeof(u8), len, file);
    if(readCount != len)
    { return false; }

    texture->set(buffer);

    delete[] buffer;

    return true;
}
