#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <Safeties.hpp>
#include <DynArray.hpp>
#include <String.hpp>
#include <unordered_map>

enum class FileProps;
class IFile;
class IFileLoader;

class SelectedResource final
{
    DEFAULT_DESTRUCT(SelectedResource);
    DEFAULT_COPY(SelectedResource);
private:
    uSys _index;
    DynString _name;
    DynString _path;
    CPPRef<IFileLoader> _loader;
public:
    inline SelectedResource() noexcept
        : _index(0), _name(""), _path(""), _loader(null)
    { }

    inline SelectedResource(const uSys index, const DynString& name, const DynString& path, const CPPRef<IFileLoader>& fileLoader) noexcept
        : _index(index), _name(name), _path(path), _loader(fileLoader)
    { }

    [[nodiscard]] uSys index() const noexcept { return _index; }
    [[nodiscard]] const DynString& name() const noexcept { return _name; }
    [[nodiscard]] const DynString& path() const noexcept { return _path; }
    [[nodiscard]] const CPPRef<IFileLoader>& loader() const noexcept { return _loader; }

    [[nodiscard]] CPPRef<IFile> loadFile(FileProps props) const noexcept;
private:
    friend class ResourceSelectorLoader;
};

class NOVTABLE IResourceSelectorTransformer
{
    DEFAULT_CONSTRUCT_PO(IResourceSelectorTransformer);
    DEFAULT_DESTRUCT_VI(IResourceSelectorTransformer);
    DELETE_COPY(IResourceSelectorTransformer);
public:
    using ResIndex = uSys;
public:
    [[nodiscard]] virtual ResIndex transform(const DynString& key) noexcept = 0;
};

class HashTableResourceSelectorTransformer final : public IResourceSelectorTransformer
{
    DEFAULT_DESTRUCT(HashTableResourceSelectorTransformer);
    DELETE_COPY(HashTableResourceSelectorTransformer);
private:
    ::std::unordered_map<DynString, uSys> _transforms;
public:
    HashTableResourceSelectorTransformer() noexcept
    { }

    void addTransform(const DynString& key, ResIndex value)
    { _transforms.insert_or_assign(key, value); }

    [[nodiscard]] ResIndex transform(const DynString& key) noexcept override
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
    using RST = IResourceSelectorTransformer;
private:
    RefDynArray<SelectedResource> _files;
    const CPPRef<RST> _rst;
public:
    ResourceSelector(const RefDynArray<SelectedResource>& files, const CPPRef<RST>& rst) noexcept
        : _files(files), _rst(rst)
    { }

    [[nodiscard]] const SelectedResource& select(const char* sel) const noexcept
    { return _files[_rst->transform(sel)]; }

    [[nodiscard]] const SelectedResource& select(const RST::ResIndex index) const noexcept
    { return _files[index]; }

    [[nodiscard]] uSys count() const noexcept { return _files.count(); }
};

class ResourceSelectorLoader final
{
    DELETE_CONSTRUCT(ResourceSelectorLoader);
    DELETE_DESTRUCT(ResourceSelectorLoader);
    DELETE_COPY(ResourceSelectorLoader);
private:
    using RST = IResourceSelectorTransformer;
private:
    static DynString _cacheDir;
private:
    static RefDynArray<SelectedResource> loadFiles(const char* vfsMount, const char* path, const char* filename, const CPPRef<RST>& rst) noexcept;
public:
    [[nodiscard]] static ResourceSelector load(const char* vfsMount, const char* path, const char* filename, const CPPRef<RST>& rst) noexcept
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
    CPPRef<IFile> _file;
    Token _currentToken;
    DynString _currentString;
    char _currentChar;
public:
    inline Lexer(const CPPRef<IFile>& file) noexcept
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
