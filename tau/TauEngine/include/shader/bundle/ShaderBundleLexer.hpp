#pragma once

#include <String.hpp>
#include <IFile.hpp>

#include "DLL.hpp"
#include "RenderingMode.hpp"

enum class SBPToken
{
    EndOfFile = -1,
    Unknown = 0,
    Identifier,
    VertexBlock,
    TessellationControlBlock,
    TessellationEvaluationBlock,
    GeometryBlock,
    PixelBlock,
    UniformsBlock,
    TexturesBlock,
    File,
    RenderingMode,
    CRMLiteral,
    StringLiteral,
    IntegerLiteral,
    UnsignedIntegerLiteral,
    Character
};

enum class CommonRenderingModelToken
{
    // InputPosition = -1,
    // InputNormal = -1,
    // InputTangent = -1,
    // InputTextureCoord = -1,
    // InputBoneID = -1,
    // InputBoneWeight = -1,
    // OutputNormal = -1,
    // OutputDiffuse = -1,
    // OutputAlbedo = OutputDiffuse,
    // OutputPBRCompound = -1,
    // OutputEmissivity = -1,
    // OutputPosition = -1,
    // InputFBPosition = -1,
    // InputFBTextureCoord = -1,
    // OutputFBColor = -1,
    UniformBindingCameraDynamic = 1,
    UniformBindingCameraStatic,
    TextureNormal,
    TextureDiffuse,
    TextureAlbedo = TextureDiffuse,
    TexturePBRCompound,
    TextureEmissivity,
    TexturePosition,
    TextureDepth,
    TextureStencil
};

class TAU_DLL ShaderBundleLexer final
{
    DEFAULT_DESTRUCT(ShaderBundleLexer);
    DEFAULT_CM(ShaderBundleLexer);
private:
    CPPRef<IFile> _file;
    SBPToken _currentToken;
    DynString _strValue;
    union
    {
        i32 _intValue;
        u32 _uintValue;
        char _cValue;
        RenderingMode::Mode _rmValue;
        CommonRenderingModelToken _crmToken;
    };

    char _lastRead;
    bool _isEOF;
    uSys _fileIndex;
    uSys _fileLine;
    uSys _lineIndex;
public:
    inline ShaderBundleLexer(const CPPRef<IFile>& file) noexcept
        : _file(file)
        , _currentToken(SBPToken::Unknown)
        , _strValue("")
        , _intValue(0)
        , _lastRead('\0')
        , _isEOF(false)
        , _fileIndex(0)
        , _fileLine(1)
        , _lineIndex(0)
    { (void) readChar(); }

    [[nodiscard]] SBPToken currentToken() const noexcept { return _currentToken; }
    [[nodiscard]] const DynString& strValue() const noexcept { return _strValue; }
    [[nodiscard]] const DynString& identifierValue() const noexcept { return _strValue; }
    [[nodiscard]] i32 intValue() const noexcept { return _intValue; }
    [[nodiscard]] u32 uintValue() const noexcept { return _uintValue; }
    [[nodiscard]] char cValue() const noexcept { return _cValue; }
    [[nodiscard]] RenderingMode::Mode rmValue() const noexcept { return _rmValue; }
    [[nodiscard]] CommonRenderingModelToken crmToken() const noexcept { return _crmToken; }
    [[nodiscard]] bool isEOF() const noexcept { return _isEOF; }

    /**
     * The current index within the file.
     */
    [[nodiscard]] uSys fileIndex() const noexcept { return _fileIndex; }
    /**
     * The current line within the file.
     */
    [[nodiscard]] uSys fileLine() const noexcept { return _fileLine; }
    /**
     * The current index within the current line.
     */
    [[nodiscard]] uSys lineIndex() const noexcept { return _fileLine; }

    void reset(const CPPRef<IFile>& file) noexcept;

    [[nodiscard]] SBPToken getNextToken() noexcept;
private:
    [[nodiscard]] bool skipWhiteSpace() noexcept;

    [[nodiscard]] bool readInteger() noexcept;
    [[nodiscard]] bool readUnsignedInteger() noexcept;

    [[nodiscard]] bool readString() noexcept;

    [[nodiscard]] bool readIdentifier() noexcept;

    /**
     * Returns true if EOF is encountered.
     *
     * Every read function acts the same way.
     */
    [[nodiscard]] bool readChar() noexcept;
};
