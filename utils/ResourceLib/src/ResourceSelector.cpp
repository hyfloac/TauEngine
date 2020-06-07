#include "ResourceSelector.hpp"
#include "VFS.hpp"
#include <ArrayList.hpp>

using RST = IResourceSelectorTransformer;

DynString ResourceSelectorLoader::_cacheDir = "";

struct SelectorBundle final
{
    uSys index;
    DynString filePath;

    SelectorBundle() noexcept
        : index(0), filePath("")
    { }
};

struct ParseData final
{
    u64 lastModifyTime;
    RefDynArray<SelectorBundle> resources;
};

static ParseData nullParse() noexcept
{
    return { 0, RefDynArray<SelectorBundle>(0) };
}

static ParseData parseBinaryCache(const VFS::Container& con) noexcept;
static ParseData parseBinaryFile(const VFS::Container& con) noexcept;
static ParseData parseTextFile(const VFS::Container& con, const CPPRef<RST>& rst) noexcept;
static void writeCache(const VFS::Container& con, const ParseData& parseData, uSys lastModifyTim) noexcept;
// ReSharper disable once CppDeclaratorNeverUsed
static void writeBinary(const VFS::Container& con, const ParseData& parseData) noexcept;

template<typename _T>
[[nodiscard]] _T max(const _T x, const _T y) noexcept
{ return x > y ? x : y; }

template<typename _T>
[[nodiscard]] _T min(const _T x, const _T y) noexcept
{ return x < y ? x : y; }

static bool _fileExists(const VFS::Container& con) noexcept
{ return con.fileLoader->fileExists(con.path); }

static CPPRef<IFile> _loadFile(const VFS::Container& con, const FileProps props = FileProps::Read) noexcept
{ return con.fileLoader->load(con.path, props); }

static u64 _creationTime(const VFS::Container& con) noexcept
{ return con.fileLoader->creationTime(con.path); }

static u64 _modifyTime(const VFS::Container& con) noexcept
{ return con.fileLoader->modifyTime(con.path); }

CPPRef<IFile> SelectedResource::loadFile(const FileProps props) const noexcept
{ return _loader->load(_path, props); }

RefDynArray<SelectedResource> ResourceSelectorLoader::loadFiles(const char* vfsMount, const char* path, const char* filename, const CPPRef<RST>& rst) noexcept
{
    const VFS::Container cacheDir = VFS::Instance().resolvePath(_cacheDir, path);
    if(!cacheDir.fileLoader->createFolders(cacheDir.path))
    { return RefDynArray<SelectedResource>(0); }

    const VFS::Container binaryCache = VFS::Instance().resolvePath(_cacheDir, path, filename, ".tauibcache");
    const VFS::Container binaryFile = VFS::Instance().resolvePath(vfsMount, path, filename, ".tauib");
    const VFS::Container textFile = VFS::Instance().resolvePath(vfsMount, path, filename, ".taui");

    const bool binaryCacheExists = _fileExists(binaryCache);
    const bool binaryFileExists = _fileExists(binaryFile);
    const bool textFileExists = _fileExists(textFile);

    if(!binaryFileExists && !textFileExists)
    { return RefDynArray<SelectedResource>(0); }

    u64 lastModifyBinary = 0;
    u64 lastModifyText = 0;

    if(binaryFileExists)
    {
        lastModifyBinary = max(_modifyTime(binaryFile), _creationTime(binaryFile));
    }

    if(textFileExists)
    {
        lastModifyText = max(_modifyTime(textFile), _creationTime(textFile));
    }

    bool parseText = lastModifyText > lastModifyBinary;
    const u64 lastModify = max(lastModifyBinary, lastModifyText);

    ParseData resourceData = nullParse();

    if(binaryCacheExists)
    {
        const ParseData cacheData = parseBinaryCache(binaryCache);

        if(lastModify <= cacheData.lastModifyTime)
        {
            resourceData = cacheData;
            parseText = false;
        }
    }
	
    if(parseText)
    {
        resourceData = parseText ? parseTextFile(textFile, rst) : parseBinaryFile(binaryFile);
        writeCache(binaryCache, resourceData, parseText ? lastModifyText : lastModifyBinary);
    }

    uSys maxIndex = 0;

    for(uSys i = 0; i < resourceData.resources.size(); ++i)
    {
        maxIndex = max(maxIndex, resourceData.resources[i].index);
    }

    RefDynArray<SelectedResource> ret(maxIndex + 1);

    for(uSys i = 0; i < resourceData.resources.size(); ++i)
    {
        const VFS::Container vfsRes = VFS::Instance().resolvePath(vfsMount, path, resourceData.resources[i].filePath);
        const auto& res = resourceData.resources[i];
        const uSys ind = res.index;
        ret[ind]._index = ind;
        ret[ind]._name = res.filePath;
        ret[ind]._path = vfsRes.path;
        ret[ind]._loader = vfsRes.fileLoader;
    }

    return ret;
}

static RefDynArray<SelectorBundle> parseBinaryData(const CPPRef<IFile>& file) noexcept
{
    uSys numEntries;
    file->readType(&numEntries);

    if(numEntries == 0)
    { return RefDynArray<SelectorBundle>(0); }

    RefDynArray<SelectorBundle> bundles(numEntries);

    for(uSys i = 0; i < numEntries; ++i)
    {
        if(file->readType(&bundles[i].index) != sizeof(uSys))
        { return RefDynArray<SelectorBundle>(0); }

        uSys fileNameLen;
        if(file->readType(&fileNameLen) != sizeof(uSys))
        { return RefDynArray<SelectorBundle>(0); }

        if(!fileNameLen)
        { return RefDynArray<SelectorBundle>(0); }

        char* fileName = new char[fileNameLen + 1];
        fileName[fileNameLen] = '\0';

        if(file->readString(fileName, fileNameLen) != static_cast<i64>(fileNameLen))
        { return RefDynArray<SelectorBundle>(0); }

        bundles[i].filePath = DynString::passControl(fileName);
    }

    return bundles;
}

static void writeBinaryData(const CPPRef<IFile>& file, const ParseData& parseData) noexcept
{
    const uSys numEntries = parseData.resources.count();
    file->writeType(numEntries);

    for(uSys i = 0; i < numEntries; ++i)
    {
        file->writeType(parseData.resources[i].index);
        file->writeType(parseData.resources[i].filePath.length());
        file->write(parseData.resources[i].filePath.c_str(), parseData.resources[i].filePath.length());
    }
}

static constexpr u32 CacheHeader = 0x7F94D03E;
static constexpr u32 BinaryHeader = CacheHeader + 1;
static constexpr u16 CacheVersion = 0;
static constexpr u16 BinaryVersion = 0;

ParseData parseBinaryCache(const VFS::Container& con) noexcept
{
    const CPPRef<IFile> cacheFile = _loadFile(con);
    const i64 size = cacheFile->size();

    if(size < 0 || static_cast<uSys>(size) < sizeof(u32) + sizeof(u64) + sizeof(uSys))
    { return nullParse(); }

    u32 header;
    cacheFile->readType(&header);

    if(header != CacheHeader)
    { return nullParse(); }

    u16 version;
    cacheFile->readType(&version);

    if(version > CacheVersion)
    { return nullParse(); }

    u64 lastModifyTime;
    cacheFile->readType(&lastModifyTime);

    return { lastModifyTime, parseBinaryData(cacheFile) };
}

ParseData parseBinaryFile(const VFS::Container& con) noexcept
{
    const CPPRef<IFile> binaryFile = _loadFile(con);
    const i64 size = binaryFile->size();

    if(size < 0 || static_cast<uSys>(size) < sizeof(u32) + sizeof(u16) + sizeof(uSys))
    { return nullParse(); }

    u32 header;
    binaryFile->readType(&header);

    if(header != BinaryHeader)
    { return nullParse(); }

    u16 version;
    binaryFile->readType(&version);

    if(version > BinaryVersion)
    { return nullParse(); }

    return { 0, parseBinaryData(binaryFile) };
}

struct TextParseKV final
{
    DEFAULT_DESTRUCT(TextParseKV);
    DEFAULT_COPY(TextParseKV);

    DynString key;
    DynString value;

    TextParseKV() noexcept
        : key(""), value("")
    { }


    TextParseKV(const DynString& key, const DynString& value) noexcept
        : key(key), value(value)
    { }
};

ParseData parseTextFile(const VFS::Container& con, const CPPRef<RST>& rst) noexcept
{
    using Token = _ResourceSelector::Token;
    using Lexer = _ResourceSelector::Lexer;

    const CPPRef<IFile> textFile = _loadFile(con);
    Lexer lexer(textFile);

    ArrayList<TextParseKV> entries(4096);

    while(true)
    {
        if(lexer.getNextToken() != Token::String)
        { return nullParse(); }

        const DynString key = lexer.getCurrentString();

        if(lexer.getNextToken() != Token::Char || lexer.getCurrentChar() != ':')
        { return nullParse(); }

        if(lexer.getNextToken() != Token::String)
        { return nullParse(); }

        const DynString value = lexer.getCurrentString();

        entries.emplace(key, value);

        if(lexer.getNextToken() != Token::Char || lexer.getCurrentChar() != ',')
        { break; }
    }

    RefDynArray<SelectorBundle> selectors(entries.count());

    for(uSys i = 0; i < entries.count(); ++i)
    {
        selectors[i].index = rst->transform(entries[i].key);
        selectors[i].filePath = entries[i].value;
    }

    return { _modifyTime(con), selectors };
}

void writeCache(const VFS::Container& con, const ParseData& parseData, const uSys lastModifyTime) noexcept
{
    const CPPRef<IFile> cacheFile = _loadFile(con, FileProps::WriteOverwrite);

    cacheFile->writeType(CacheHeader);
    cacheFile->writeType(CacheVersion);
    cacheFile->writeType(lastModifyTime);

    writeBinaryData(cacheFile, parseData);
}

void writeBinary(const VFS::Container& con, const ParseData& parseData) noexcept  // NOLINT(clang-diagnostic-unused-function)
{
    const CPPRef<IFile> cacheFile = _loadFile(con, FileProps::WriteOverwrite);

    cacheFile->writeType(BinaryHeader);
    cacheFile->writeType(BinaryVersion);

    writeBinaryData(cacheFile, parseData);
}

_ResourceSelector::Token _ResourceSelector::Lexer::getNextToken() noexcept
{
    skipWhitespace();

    if(_currentChar == '"')
    {
        _currentString = readString(); 
        _currentToken = Token::String;
    }
    else
    {
        _currentToken = Token::Char;
    }

    return _currentToken;
}

void _ResourceSelector::Lexer::skipWhitespace() noexcept
{
    int c;
    do
    {
        c = _file->readChar();
    } while(c == ' ' || c == '\t' || c == '\n' || c == '\r');
    _currentChar = static_cast<char>(c);
}

// ReSharper disable once CppMemberFunctionMayBeConst
DynString _ResourceSelector::Lexer::readString() noexcept
{
    int c;
    StringBuilder sb;

    do
    {
        c = _file->readChar();
        if(c == '\\')
        {
            c = _file->readChar();

            switch(c)
            {
                case 't':
                    sb.append('\t');
                    break;
                case '0':
                    sb.append('0');
                    break;
                case 'r':
                    sb.append('\r');
                    break;
                case 'n':
                    sb.append('\n');
                    break;
                case '\\':
                    sb.append('\\');
                    break;
                default: return "";
            }
            continue;
        }

        sb.append(static_cast<char>(c));
    } while(c != '"');
    
    return sb.backspace().toString();
}
