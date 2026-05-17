#include "ResourceSelector.hpp"
#include "VFS.hpp"
#include <ArrayList.hpp>

#include <TUMaths.hpp>

using VFS = tau::VFS;

using RST = IResourceSelectorTransformer;

C8DynString ResourceSelectorLoader::s_CacheDir = u8"";

struct SelectorBundle final
{
    uSys index;
    C8DynString filePath;

    SelectorBundle() noexcept
        : index(0)
        , filePath()
    { }
};

struct ParseData final
{
    ::std::chrono::time_point<::std::chrono::utc_clock> lastModifyTime;
    RefDynArray<SelectorBundle> resources;
};

using UtcTimePoint = ::std::chrono::time_point<::std::chrono::utc_clock>;

static ParseData nullParse() noexcept
{
    return { UtcTimePoint{}, RefDynArray<SelectorBundle>(0) };
}

static ParseData parseBinaryCache(const VFS::Container& con) noexcept;
static ParseData parseBinaryFile(const VFS::Container& con) noexcept;
static ParseData parseTextFile(const VFS::Container& con, const CPPRef<RST>& rst) noexcept;
static void writeCache(const VFS::Container& con, const ParseData& parseData, const UtcTimePoint& lastModifyTime) noexcept;
// ReSharper disable once CppDeclaratorNeverUsed
static void writeBinary(const VFS::Container& con, const ParseData& parseData) noexcept;

static bool _fileExists(const VFS::Container& con) noexcept
{ return con.FileLoader->Exists(con.FilePath); }

static ::tau::com::ComRef<tau::IFileStream> _loadFile(const VFS::Container& con, const tau::FileProps props = tau::FileProps::Read) noexcept
{ return con.FileLoader->Load(con.FilePath, props); }

static ::std::chrono::time_point<::std::chrono::utc_clock> _creationTime(const VFS::Container& con) noexcept
{ return con.FileLoader->CreationTime(con.FilePath); }

static ::std::chrono::time_point<::std::chrono::utc_clock> _modifyTime(const VFS::Container& con) noexcept
{ return con.FileLoader->ModifyTime(con.FilePath); }

::tau::com::ComRef<tau::IFileStream> SelectedResource::loadFile(const tau::FileProps props) const noexcept
{ return m_Loader->Load(m_Path, props); }

RefDynArray<SelectedResource> ResourceSelectorLoader::loadFiles(const c8* vfsMount, const c8* path, const c8* filename, const CPPRef<RST>& rst) noexcept
{
    const VFS::Container cacheDir = VFS::Instance().ResolvePath(s_CacheDir.Concat(u8"/").Concat(path));
    if(!cacheDir.FileLoader->CreateFolders(cacheDir.FilePath))
    { return RefDynArray<SelectedResource>(0); }

    const VFS::Container binaryCache = VFS::Instance().ResolvePath(s_CacheDir, path, filename, u8".tauibcache");
    const VFS::Container binaryFile = VFS::Instance().ResolvePath(vfsMount, path, filename, u8".tauib");
    const VFS::Container textFile = VFS::Instance().ResolvePath(vfsMount, path, filename, u8".taui");

    const bool binaryCacheExists = _fileExists(binaryCache);
    const bool binaryFileExists = _fileExists(binaryFile);
    const bool textFileExists = _fileExists(textFile);

    if(!binaryFileExists && !textFileExists)
    { return RefDynArray<SelectedResource>(0); }

    ::std::chrono::time_point<::std::chrono::utc_clock> lastModifyBinary;
    ::std::chrono::time_point<::std::chrono::utc_clock> lastModifyText;

    if(binaryFileExists)
    {
        lastModifyBinary = maxT(_modifyTime(binaryFile), _creationTime(binaryFile));
    }

    if(textFileExists)
    {
        lastModifyText = maxT(_modifyTime(textFile), _creationTime(textFile));
    }

    bool parseText = lastModifyText > lastModifyBinary;
    const ::std::chrono::time_point<::std::chrono::utc_clock> lastModify = maxT(lastModifyBinary, lastModifyText);

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
        maxIndex = maxT(maxIndex, resourceData.resources[i].index);
    }

    RefDynArray<SelectedResource> ret(maxIndex + 1);

    for(uSys i = 0; i < resourceData.resources.size(); ++i)
    {
        const VFS::Container vfsRes = VFS::Instance().ResolvePath(vfsMount, path, resourceData.resources[i].filePath);
        const auto& res = resourceData.resources[i];
        const uSys ind = res.index;
        ret[ind].m_Index = ind;
        ret[ind].m_Name = res.filePath;
        ret[ind].m_Path = vfsRes.FilePath;
        ret[ind].m_Loader = vfsRes.FileLoader;
    }

    return ret;
}

static RefDynArray<SelectorBundle> parseBinaryData(const ::tau::com::ComRef<tau::IFileStream>& file) noexcept
{
    uSys numEntries;
    if(file->ReadType(&numEntries) != sizeof(uSys))
    { return RefDynArray<SelectorBundle>(0); }

    if(numEntries == 0)
    { return RefDynArray<SelectorBundle>(0); }

    RefDynArray<SelectorBundle> bundles(numEntries);

    for(uSys i = 0; i < numEntries; ++i)
    {
        if(file->ReadType(&bundles[i].index) != sizeof(uSys))
        { return RefDynArray<SelectorBundle>(0); }

        uSys fileNameLen;
        if(file->ReadType(&fileNameLen) != sizeof(uSys))
        { return RefDynArray<SelectorBundle>(0); }

        if(!fileNameLen)
        { return RefDynArray<SelectorBundle>(0); }

        c8* const fileName = new c8[fileNameLen + 1];
        fileName[fileNameLen] = u8'\0';

        const auto readResult = file->Read(fileName, fileNameLen);
        if(!readResult || readResult.value() != fileNameLen)
        {
            delete[] fileName;
            return RefDynArray<SelectorBundle>(0);
        }

        bundles[i].filePath = C8DynString::passControl(fileName);
    }

    return bundles;
}

static void writeBinaryData(const ::tau::com::ComRef<tau::IFileStream>& file, const ParseData& parseData) noexcept
{
    const uSys numEntries = parseData.resources.count();
    file->WriteType(numEntries);

    for(uSys i = 0; i < numEntries; ++i)
    {
        const uSys pathLength = parseData.resources[i].filePath.Length();
        file->WriteType(parseData.resources[i].index);
        file->WriteType(pathLength);
        file->Write(parseData.resources[i].filePath.String(), pathLength);
    }
}

static constexpr u32 CacheHeader = 0x7F94D03E;
static constexpr u32 BinaryHeader = CacheHeader + 1;
static constexpr u16 CacheVersion = 0;
static constexpr u16 BinaryVersion = 0;

ParseData parseBinaryCache(const VFS::Container& con) noexcept
{
    const ::tau::com::ComRef<tau::IFileStream> cacheFile = _loadFile(con);
    const i64 size = cacheFile->Length();

    if(size < 0 || static_cast<uSys>(size) < sizeof(u32) + sizeof(u16) + sizeof(UtcTimePoint) + sizeof(uSys))
    { return nullParse(); }

    u32 header;
    cacheFile->ReadType(&header);

    if(header != CacheHeader)
    { return nullParse(); }

    u16 version;
    cacheFile->ReadType(&version);

    if(version > CacheVersion)
    { return nullParse(); }

    UtcTimePoint lastModifyTime;
    cacheFile->ReadType(&lastModifyTime);

    return { lastModifyTime, parseBinaryData(cacheFile) };
}

ParseData parseBinaryFile(const VFS::Container& con) noexcept
{
    const ::tau::com::ComRef<tau::IFileStream> binaryFile = _loadFile(con);
    const i64 size = binaryFile->Length();

    if(size < 0 || static_cast<uSys>(size) < sizeof(u32) + sizeof(u16) + sizeof(uSys))
    { return nullParse(); }

    u32 header;
    binaryFile->ReadType(&header);

    if(header != BinaryHeader)
    { return nullParse(); }

    u16 version;
    binaryFile->ReadType(&version);

    if(version > BinaryVersion)
    { return nullParse(); }

    return { UtcTimePoint{}, parseBinaryData(binaryFile) };
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

    const ::tau::com::ComRef<tau::IFileStream> textFile = _loadFile(con);
    ::tau::com::ComRef<tau::IStream> stream;
    if(IsFailure(textFile->QueryInterface<tau::IStream>(stream.Load())))
    {
        return { };
    }

    Lexer lexer(stream);

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
        selectors[i].filePath = StringCast<c8>(entries[i].value);
    }

    return { _modifyTime(con), selectors };
}

void writeCache(const VFS::Container& con, const ParseData& parseData, const UtcTimePoint& lastModifyTime) noexcept
{
    const ::tau::com::ComRef<tau::IFileStream> cacheFile = _loadFile(con, ::tau::FileProps::WriteOverwrite);

    cacheFile->WriteType(CacheHeader);
    cacheFile->WriteType(CacheVersion);
    cacheFile->WriteType(lastModifyTime);

    writeBinaryData(cacheFile, parseData);
}

void writeBinary(const VFS::Container& con, const ParseData& parseData) noexcept  // NOLINT(clang-diagnostic-unused-function)
{
    const ::tau::com::ComRef<tau::IFileStream> cacheFile = _loadFile(con, ::tau::FileProps::WriteOverwrite);

    cacheFile->WriteType(BinaryHeader);
    cacheFile->WriteType(BinaryVersion);

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

i32 _ResourceSelector::Lexer::readByte() noexcept
{
    const auto result = _file->ReadByte();
    if(!result)
    { return -1; }
    return static_cast<i32>(result.value());
}

void _ResourceSelector::Lexer::skipWhitespace() noexcept
{
    i32 c;
    do
    {
        c = readByte();
    } while(c == ' ' || c == '\t' || c == '\n' || c == '\r');
    _currentChar = static_cast<char>(c);
}

// ReSharper disable once CppMemberFunctionMayBeConst
DynString _ResourceSelector::Lexer::readString() noexcept
{
    i32 c;
    StringBuilder sb;

    do
    {
        c = readByte();
        if(c < 0)
        { return ""; }

        if(c == '\\')
        {
            c = readByte();

            switch(c)
            {
                case 't':
                    sb.Append('\t');
                    break;
                case '0':
                    sb.Append('0');
                    break;
                case 'r':
                    sb.Append('\r');
                    break;
                case 'n':
                    sb.Append('\n');
                    break;
                case '\\':
                    sb.Append('\\');
                    break;
                default: return "";
            }
            continue;
        }

        sb.Append(static_cast<char>(c));
    } while(c != '"');

    return sb.backspace().toString();
}
