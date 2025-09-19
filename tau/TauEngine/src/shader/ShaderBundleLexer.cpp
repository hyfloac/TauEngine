#include "shader/bundle/ShaderBundleLexer.hpp"

[[nodiscard]] static bool isWhiteSpace(c32 c) noexcept;
[[nodiscard]] static bool isDelimiter(c32 c) noexcept;
[[nodiscard]] static bool isNumber(c32 c) noexcept;
[[nodiscard]] static c32 getEscape(c32 c) noexcept;
[[nodiscard]] static bool isValidIdentifier(c32 c) noexcept;

void ShaderBundleLexer::reset(const CPPRef<IFile>& file) noexcept
{
    m_File = file;
    m_CurrentToken = SBPToken::Unknown;
    m_StrValue = u8"";
    m_IntValue = 0;
    m_LastRead = '\0';
    m_IsEof = false;
    m_FileIndex = 0;
    m_FileLine = 1;
    m_LineIndex = 0;

    (void) readChar();
}

SBPToken ShaderBundleLexer::getNextToken() noexcept
{
    if(skipWhiteSpace())
    {
        return m_CurrentToken = SBPToken::EndOfFile;
    }

    if(isNumber(m_LastRead))
    {
        if(readUnsignedInteger())
        { return m_CurrentToken = SBPToken::EndOfFile; }
        m_CurrentToken = SBPToken::UnsignedIntegerLiteral;
    }
    else if(m_LastRead == '-')
    {
        if(readInteger())
        { return m_CurrentToken = SBPToken::EndOfFile; }
        m_CurrentToken = SBPToken::IntegerLiteral;
    }
    else if(isValidIdentifier(m_LastRead))
    {
        if(readIdentifier())
        { return m_CurrentToken = SBPToken::EndOfFile; }

        m_CurrentToken = SBPToken::Identifier;

// #define TOKEN_STR_CASE(STR, TOKEN) C8STR_CASE(STR, { return m_CurrentToken = SBPToken::TOKEN; })
// #define TOKEN_BLOCK_STR_CASE(TOKEN) C8STR_CASE(u8#TOKEN, { return m_CurrentToken = SBPToken::TOKEN##Block; })
// #define RM_STR_CASE(API) C8STR_CASE(u8#API, {\
//         m_RmValue = RenderingMode::Mode::API; \
//         return m_CurrentToken = SBPToken::RenderingMode; \
//     })
// #define CRM_STR_CASE(TOKEN) C8STR_CASE(u8"CRM" u8#TOKEN, { \
//         m_CrmToken = CommonRenderingModelToken::TOKEN; \
//         return m_CurrentToken = SBPToken::CRMLiteral; \
//     })
//
//         STR_SWITCH(m_StrValue, {
//             TOKEN_BLOCK_STR_CASE(Vertex)
//             TOKEN_BLOCK_STR_CASE(TessellationControl)
//             TOKEN_STR_CASE(u8"Hull", TessellationControlBlock)
//             TOKEN_BLOCK_STR_CASE(TessellationEvaluation)
//             TOKEN_STR_CASE(u8"Domain", TessellationEvaluationBlock)
//             TOKEN_BLOCK_STR_CASE(Geometry)
//             TOKEN_BLOCK_STR_CASE(Pixel)
//             TOKEN_STR_CASE(u8"Fragment", PixelBlock)
//             TOKEN_BLOCK_STR_CASE(Uniforms)
//             TOKEN_BLOCK_STR_CASE(Textures)
//             TOKEN_STR_CASE(u8"File", File)
//             TOKEN_STR_CASE(u8"Location", Location)
//             TOKEN_STR_CASE(u8"Sampler", Sampler)
//             RM_STR_CASE(DirectX10)
//             RM_STR_CASE(DirectX11)
//             RM_STR_CASE(DirectX12)
//             RM_STR_CASE(DirectX12_1)
//             RM_STR_CASE(Vulkan)
//             RM_STR_CASE(OpenGL4_1)
//             RM_STR_CASE(OpenGL4_2)
//             RM_STR_CASE(OpenGL4_3)
//             RM_STR_CASE(OpenGL4_4)
//             RM_STR_CASE(OpenGL4_5)
//             RM_STR_CASE(OpenGL4_6)
//             // CRM_STR_CASE(InputPosition)
//             // CRM_STR_CASE(InputNormal)
//             // CRM_STR_CASE(InputTangent)
//             // CRM_STR_CASE(InputTextureCoord)
//             // CRM_STR_CASE(InputBoneID)
//             // CRM_STR_CASE(InputBoneWeight)
//             // CRM_STR_CASE(OutputNormal)
//             // CRM_STR_CASE(OutputDiffuse)
//             // CRM_STR_CASE(OutputAlbedo)
//             // CRM_STR_CASE(OutputPBRCompound)
//             // CRM_STR_CASE(OutputEmissivity)
//             // CRM_STR_CASE(OutputPosition)
//             // CRM_STR_CASE(InputFBPosition)
//             // CRM_STR_CASE(InputFBTextureCoord)
//             // CRM_STR_CASE(OutputFBColor)
//             CRM_STR_CASE(UniformBindingCameraDynamic)
//             CRM_STR_CASE(UniformBindingCameraStatic)
//             CRM_STR_CASE(TextureNormal)
//             CRM_STR_CASE(TextureDiffuse)
//             CRM_STR_CASE(TextureAlbedo)
//             CRM_STR_CASE(TexturePBRCompound)
//             CRM_STR_CASE(TextureEmissivity)
//             CRM_STR_CASE(TexturePosition)
//             CRM_STR_CASE(TextureDepth)
//             CRM_STR_CASE(TextureStencil)
//         },
//         { return m_CurrentToken = SBPToken::Identifier; })
//
// #undef TOKEN_STR_CASE
// #undef TOKEN_BLOCK_STR_CASE
// #undef RM_STR_CASE
// #undef CRM_STR_CASE
    }
    else if(m_LastRead == U'\"')
    {
        if(readString())
        { return m_CurrentToken = SBPToken::EndOfFile; }
        m_CurrentToken = SBPToken::StringLiteral;
    }
    else
    {
        m_CValue = m_LastRead;
        (void) readChar();
        m_CurrentToken = SBPToken::Character;
    }


    return m_CurrentToken;
}

bool ShaderBundleLexer::skipWhiteSpace() noexcept
{
    while(isWhiteSpace(m_LastRead))
    {
        if(readChar()) 
        { return true; }
    }
    return false;
}

bool ShaderBundleLexer::readInteger() noexcept
{
    bool negative = false;
    if(m_LastRead == '-')
    {
        negative = true;
        if(readChar()) 
        { return true; }
    }
    i32 x = static_cast<i32>(m_LastRead - U'0');
    while(true)
    {
        if(readChar()) { return true; }
        if(isDelimiter(m_LastRead))
        {
            if(negative)
            {
                x *= -1;
            }
            m_IntValue = x;
            return false;
        }
        
        if(isNumber(m_LastRead))
        {
            x *= 10;
            x += static_cast<i32>(m_LastRead - '0');
        }
        else
        { return true; }
    }
}

bool ShaderBundleLexer::readUnsignedInteger() noexcept
{
    u32 x = static_cast<u32>(m_LastRead - '0');
    while(true)
    {
        if(readChar()) { return true; }
        if(isDelimiter(m_LastRead))
        {
            m_UintValue = x;
            return false;
        }

        if(isNumber(m_LastRead))
        {
            x *= 10;
            x += static_cast<u32>(m_LastRead - '0');
        }
        else
        { return true; }
    }
}

bool ShaderBundleLexer::readString() noexcept
{
    C8StringBuilder sb;
    while(true)
    {
        if(readChar()) 
        { return true; }

        if(m_LastRead == U'"')
        {
            m_StrValue = sb.ToString();
            (void) readChar();
            return false;
        }

        if(m_LastRead == U'\\')
        {
            if(readChar()) 
            { return false; }

            sb.Append(getEscape(m_LastRead));
        }
        else
        { sb.Append(m_LastRead); }
    }
}

bool ShaderBundleLexer::readIdentifier() noexcept
{
    C8StringBuilder sb;
    sb.Append(m_LastRead);
    while(true)
    {
        if(readChar()) 
        { return true; }

        if(isDelimiter(m_LastRead))
        {
            m_StrValue = sb.ToString();
            return false;
        }

        if(isValidIdentifier(m_LastRead))
        { sb.Append(m_LastRead); }
        else
        { return true; }
    }
}

[[nodiscard]] c32 ShaderBundleLexer::DecodeCodePointForwardUnsafe() noexcept
{
    const int ci0 = m_File->ReadChar();

    if(ci0 == -1)
    {
        m_IsEof = true;
        return static_cast<c32>(-1);
    }

    if((ci0 & 0x80) == 0) // U+0000 - U+007F
    {
        return static_cast<c32>(ci0);
    }
    else
    {
        if((ci0 & 0xE0) == 0xC0) // U+0080 - U+07FF
        {
            const int ci1 = m_File->ReadChar();

            if(ci1 == -1)
            {
                m_IsEof = true;
                return static_cast<c32>(-1);
            }

            const c32 byte1Bits = (ci0 & 0x1F) << 6;
            const c32 byte2Bits = (ci1 & 0x3F);
            return byte1Bits | byte2Bits;
        }
        else if((ci0 & 0xF0) == 0xE0) // U+0800 - U+FFFF
        {
            const int ci1 = m_File->ReadChar();

            if(ci1 == -1)
            {
                m_IsEof = true;
                return static_cast<c32>(-1);
            }

            const int ci2 = m_File->ReadChar();

            if(ci2 == -1)
            {
                m_IsEof = true;
                return static_cast<c32>(-1);
            }

            const c32 byte1Bits = (ci0 & 0x0F) << 12;
            const c32 byte2Bits = (ci1 & 0x3F) << 6;
            const c32 byte3Bits = (ci2 & 0x3F);
            return byte1Bits | byte2Bits | byte3Bits;
        }
        else if((ci0 & 0xF0) == 0xF0) // U+10000 - U+10FFFF
        {
            const int ci1 = m_File->ReadChar();

            if(ci1 == -1)
            {
                m_IsEof = true;
                return static_cast<c32>(-1);
            }

            const int ci2 = m_File->ReadChar();

            if(ci2 == -1)
            {
                m_IsEof = true;
                return static_cast<c32>(-1);
            }

            const int ci3 = m_File->ReadChar();

            if(ci3 == -1)
            {
                m_IsEof = true;
                return static_cast<c32>(-1);
            }

            const c32 byte1Bits = (ci0 & 0x07) << 18;
            const c32 byte2Bits = (ci1 & 0x3F) << 12;
            const c32 byte3Bits = (ci2 & 0x3F) << 6;
            const c32 byte4Bits = (ci3 & 0x3F);
            return byte1Bits | byte2Bits | byte3Bits | byte4Bits;
        }
        else
        {
            return static_cast<c32>(-1);
        }
    }
}

bool ShaderBundleLexer::readChar() noexcept
{
    if(m_IsEof) 
    { return true; }

    const c32 ci = DecodeCodePointForwardUnsafe();

    if(ci == static_cast<c32>(-1))
    { return true; }

    m_LastRead = ci;

    ++m_FileIndex;
    ++m_LineIndex;
    if(ci == U'\n')
    {
        ++m_FileLine;
        m_LineIndex = 0;
    }

    return false;
}

[[nodiscard]] static bool isWhiteSpace(const c32 c) noexcept
{
    switch(c)
    {
        case U' ':
        case U'\n':
        case U'\r':
        case U'\t':
            return true;
        default: return false;
    }
}

[[nodiscard]] static bool isDelimiter(const c32 c) noexcept
{
    switch(c)
    {
        case U' ':
        case U'\n':
        case U'\r':
        case U'\t':
        case U':':
        case U',':
            return true;
        default: return false;
    }
}

[[nodiscard]] static bool isNumber(const c32 c) noexcept
{
    return c >= U'0' && c <= U'9';
}

[[nodiscard]] static c32 getEscape(const c32 c) noexcept
{
    switch(c)
    {
        case U'\\': return U'\\';
        case U'r':  return U'\r';
        case U'n':  return U'\n';
        case U'0':  return U'\0';
        case U't':  return U'\t';
        case U'\"': return U'\"';
        case U'\'': return U'\'';
        default:   return static_cast<c32>(0x7F);
    }
}

[[nodiscard]] static bool isValidIdentifier(const c32 c) noexcept
{
    if(c >= U'A' && c <= U'Z')
    { return true; }
    if(c >= U'a' && c <= U'z')
    { return true; }
    if(c >= U'0' && c <= U'9')
    { return true; }
    if(c == U'_')
    { return true; }
    return false;
}
