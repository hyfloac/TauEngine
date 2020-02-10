#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <Safeties.hpp>
#include <DynArray.hpp>
#include <String.hpp>
#include <unordered_map>

class IFile;

class ResourceSelectorTransformer final
{
    DEFAULT_DESTRUCT(ResourceSelectorTransformer);
    DEFAULT_COPY(ResourceSelectorTransformer);
private:
    ::std::unordered_map<DynString, uSys> _transforms;
public:
    ResourceSelectorTransformer() noexcept
    { }

    void addTransform(const DynString& key, uSys value)
    { _transforms.insert_or_assign(key, value); }

    [[nodiscard]] uSys transform(const DynString& key) noexcept
    {
        if(_transforms.count(key))
        { return _transforms[key]; }
        return -1;
    }
};

class ResourceSelector final
{
    DEFAULT_DESTRUCT(ResourceSelector);
    DEFAULT_COPY(ResourceSelector);
private:
    using RST = ResourceSelectorTransformer;
private:
    RefDynArray<Ref<IFile>> _files;
    const Ref<RST> _rst;
public:
    ResourceSelector(const RefDynArray<Ref<IFile>>& files, const Ref<RST>& rst) noexcept
        : _files(files), _rst(rst)
    { }

    [[nodiscard]] Ref<IFile> select(const char* sel) const noexcept
    { return _files[_rst->transform(sel)]; }
};

class ResourceSelectorLoader final
{
    DELETE_CONSTRUCT(ResourceSelectorLoader);
    DELETE_DESTRUCT(ResourceSelectorLoader);
    DELETE_COPY(ResourceSelectorLoader);
private:
    static DynString _cacheDir;
private:
    static RefDynArray<Ref<IFile>> loadFiles(const char* vfsMount, const char* path, const char* filename, const Ref<ResourceSelectorTransformer>& rst) noexcept;
public:
    [[nodiscard]] static ResourceSelector load(const char* vfsMount, const char* path, const char* filename, const Ref<ResourceSelectorTransformer>& rst) noexcept
    { return ResourceSelector(loadFiles(vfsMount, path, filename, rst), rst); }

    static void setCacheDirectory(const DynString& cacheDir) noexcept
    { _cacheDir = cacheDir; }
};

namespace _ResourceSelector {

enum class Token
{
    Eof = 0,
    String,
    Char,
    Unknown
};

class Lexer final
{
    DEFAULT_DESTRUCT(Lexer);
    DEFAULT_COPY(Lexer);
private:
    Ref<IFile> _file;
    Token _currentToken;
    DynString _currentString;
    char _currentChar;
public:
    inline Lexer(const Ref<IFile>& file) noexcept
        : _file(file), _currentToken(Token::Unknown), _currentString(""), _currentChar('\0')
    { }

    [[nodiscard]] Token getCurrentToken() const noexcept { return _currentToken; }
    [[nodiscard]] DynString getCurrentString() const noexcept { return _currentString; }
    [[nodiscard]] char getCurrentChar() const noexcept { return _currentChar; }

    [[nodiscard]] Token getNextToken() noexcept;
private:
    void skipWhitespace() noexcept;

    DynString readString() noexcept;
};
}
