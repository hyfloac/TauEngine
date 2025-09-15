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
    // VertexBlock,
    // TessellationControlBlock,
    // TessellationEvaluationBlock,
    // GeometryBlock,
    // PixelBlock,
    // UniformsBlock,
    // TexturesBlock,
    // Location,
    // Sampler,
    // File,
    // RenderingMode,
    // CRMLiteral,
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
    CPPRef<IFile> m_File;
    SBPToken m_CurrentToken;
    C8DynString m_StrValue;
    union
    {
        i32 m_IntValue;
        u32 m_UintValue;
        c32 m_CValue;
        RenderingMode::Mode m_RmValue;
        CommonRenderingModelToken m_CrmToken;
    };

    c32 m_LastRead;
    bool m_IsEof;
    uSys m_FileIndex;
    uSys m_FileLine;
    uSys m_LineIndex;
public:
    ShaderBundleLexer(const CPPRef<IFile>& file) noexcept
        : m_File(file)
        , m_CurrentToken(SBPToken::Unknown)
        , m_StrValue(u8"")
        , m_IntValue(0)
        , m_LastRead(U'\0')
        , m_IsEof(false)
        , m_FileIndex(0)
        , m_FileLine(1)
        , m_LineIndex(0)
    { (void) readChar(); }

    [[nodiscard]] SBPToken currentToken() const noexcept { return m_CurrentToken; }
    [[nodiscard]] const C8DynString& strValue() const noexcept { return m_StrValue; }
    [[nodiscard]] const C8DynString& identifierValue() const noexcept { return m_StrValue; }
    [[nodiscard]] i32 intValue() const noexcept { return m_IntValue; }
    [[nodiscard]] u32 uintValue() const noexcept { return m_UintValue; }
    [[nodiscard]] c32 cValue() const noexcept { return m_CValue; }
    // [[nodiscard]] RenderingMode::Mode rmValue() const noexcept { return m_RmValue; }
    // [[nodiscard]] CommonRenderingModelToken crmToken() const noexcept { return m_CrmToken; }
    [[nodiscard]] bool isEOF() const noexcept { return m_IsEof; }

    /**
     * The current index within the file.
     */
    [[nodiscard]] uSys fileIndex() const noexcept { return m_FileIndex; }
    /**
     * The current line within the file.
     */
    [[nodiscard]] uSys fileLine() const noexcept { return m_FileLine; }
    /**
     * The current index within the current line.
     */
    [[nodiscard]] uSys lineIndex() const noexcept { return m_FileLine; }

    void reset(const CPPRef<IFile>& file) noexcept;

    [[nodiscard]] SBPToken getNextToken() noexcept;
private:
    [[nodiscard]] bool skipWhiteSpace() noexcept;

    [[nodiscard]] bool readInteger() noexcept;
    [[nodiscard]] bool readUnsignedInteger() noexcept;

    [[nodiscard]] bool readString() noexcept;

    [[nodiscard]] bool readIdentifier() noexcept;

    [[nodiscard]] c32 DecodeCodePointForwardUnsafe() noexcept;

    /**
     * Returns true if EOF is encountered.
     *
     * Every read function acts the same way.
     */
    [[nodiscard]] bool readChar() noexcept;
};
