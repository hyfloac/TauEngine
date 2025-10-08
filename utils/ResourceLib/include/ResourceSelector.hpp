#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <Safeties.hpp>
#include <DynArray.hpp>
#include <String.hpp>
#include <TauCOM.hpp>
#include <unordered_map>
#include "IFile.hpp"

class SelectedResource final
{
    DEFAULT_CONSTRUCT_PU(SelectedResource);
    DEFAULT_DESTRUCT(SelectedResource);
    DEFAULT_COPY(SelectedResource);
public:
    SelectedResource(
        const uSys index,
        const C8DynString& name,
        const C8DynString& path,
        const ::tau::com::ComRef<::tau::IFileLoader>& fileLoader
    ) noexcept
        : m_Index(index)
        , m_Name(name)
        , m_Path(path)
        , m_Loader(fileLoader)
    { }

    [[nodiscard]] uSys index() const noexcept { return m_Index; }
    [[nodiscard]] const C8DynString& name() const noexcept { return m_Name; }
    [[nodiscard]] const C8DynString& path() const noexcept { return m_Path; }
    [[nodiscard]] const ::tau::com::ComRef<::tau::IFileLoader>& loader() const noexcept { return m_Loader; }

    [[nodiscard]] CPPRef<::tau::IFile> loadFile(const ::tau::FileProps props) const noexcept;
private:
    uSys m_Index;
    C8DynString m_Name;
    C8DynString m_Path;
    ::tau::com::ComRef<::tau::IFileLoader> m_Loader;
private:
    friend class ResourceSelectorLoader;
};

class NOVTABLE IResourceSelectorTransformer
{
    DEFAULT_CONSTRUCT_PO(IResourceSelectorTransformer);
    DEFAULT_DESTRUCT_VI(IResourceSelectorTransformer);
    DELETE_COPY(IResourceSelectorTransformer);
    DEFAULT_MOVE(IResourceSelectorTransformer);
public:
    using ResIndex = uSys;
public:
    [[nodiscard]] virtual ResIndex transform(const DynString& key) noexcept = 0;
};

class HashTableResourceSelectorTransformer final : public IResourceSelectorTransformer
{
    DEFAULT_CONSTRUCT_PU(HashTableResourceSelectorTransformer);
    DEFAULT_DESTRUCT_O(HashTableResourceSelectorTransformer);
    DELETE_COPY(HashTableResourceSelectorTransformer);
    DEFAULT_MOVE(HashTableResourceSelectorTransformer);
public:
    void addTransform(const DynString& key, ResIndex value)
    {
        _transforms.insert_or_assign(key, value);
    }

    [[nodiscard]] ResIndex transform(const DynString& key) noexcept override
    {
        if(_transforms.contains(key))
        {
            return _transforms[key];
        }

        return -1;
    }
private:
    ::std::unordered_map<DynString, uSys> _transforms;
};

class ResourceSelector final
{
    DEFAULT_DESTRUCT(ResourceSelector);
    DEFAULT_COPY(ResourceSelector);
private:
    using RST = IResourceSelectorTransformer;
private:
    RefDynArray<SelectedResource> _files;
    CPPRef<RST> _rst;
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
