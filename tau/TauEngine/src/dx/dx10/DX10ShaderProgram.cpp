#include "dx/dx10/DX10ShaderProgram.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10Shader.hpp"
#include "shader/bundle/ShaderBundleParser.hpp"
#include "shader/bundle/ShaderInfoExtractorVisitor.hpp"
#include "VFS.hpp"
#include "dx/dx10/DX10GraphicsInterface.hpp"
#include "TauConfig.hpp"

NullableRef<IShaderProgram> DX10ShaderProgramBuilder::build(const ShaderProgramAutoArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
}

NullableRef<IShaderProgram> DX10ShaderProgramBuilder::build(const ShaderProgramManualArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
}

bool DX10ShaderProgramBuilder::processArgs(const ShaderProgramAutoArgs& args, DXShaderProgramArgs* dxArgs, Error* error)
{
    ERROR_CODE_COND_F(!args.bundleFile, Error::InvalidFile);

    ShaderBundleParser parser(args.bundleFile);

    ShaderBundleParser::Error parseError;
    auto ast = parser.parse(&parseError);

    ShaderInfoExtractorVisitor visitor(RenderingMode::DirectX10);

    visitor.visit(ast.get());

    for(auto it = visitor.begin(); it != visitor.end(); ++it)
    {
        const auto& info = *it;

        ShaderFileArgs shaderArgs;
        shaderArgs.file = VFS::openFile(info.fileName, FileProps::Read);
        shaderArgs.stage = it.stage();

        IShaderBuilder::Error shaderError;
        const NullableRef<IShader> shader = _gi.createShader().buildTauRef(shaderArgs, &shaderError);

        ERROR_CODE_COND_F(!shader, Error::CompileError);

        switch(it.stage())
        {
            case EShader::Stage::Vertex: dxArgs->vertexShader = shader; break;
            case EShader::Stage::Geometry: dxArgs->geometryShader = shader; break;
            case EShader::Stage::Pixel: dxArgs->pixelShader = shader; break;
            default: return false;
        }
    }

    ERROR_CODE_COND_F(!dxArgs->vertexShader, Error::MissingVertexShader);
    ERROR_CODE_COND_F(!dxArgs->pixelShader, Error::MissingPixelShader);
}

bool DX10ShaderProgramBuilder::processArgs(const ShaderProgramManualArgs& args, DXShaderProgramArgs* dxArgs, Error* error)
{
    ERROR_CODE_COND_F(!args.vertexShader, Error::MissingVertexShader);
    ERROR_CODE_COND_F(!args.pixelShader, Error::MissingPixelShader);

#if TAU_RTTI_CHECK
    ERROR_CODE_COND_F(!RTTD_CHECK(args.vertexShader, DX10Shader, IShader), Error::InternalError);
    if(args.geometryShader)
    { ERROR_CODE_COND_F(!RTTD_CHECK(args.geometryShader, DX10Shader, IShader), Error::InternalError); }
    ERROR_CODE_COND_F(!RTTD_CHECK(args.pixelShader, DX10Shader, IShader), Error::InternalError);
#endif

#if TAU_GENERAL_SAFETY_CHECK
    ERROR_CODE_COND_F(args.vertexShader->shaderStage() != EShader::Stage::Vertex, Error::InvalidShaderStage);
    if(args.geometryShader)
    { ERROR_CODE_COND_F(args.geometryShader->shaderStage() != EShader::Stage::Geometry, Error::InternalError); }
    ERROR_CODE_COND_F(args.pixelShader->shaderStage() != EShader::Stage::Pixel, Error::InvalidShaderStage);
#endif

    dxArgs->vertexShader = RefCast<DX10VertexShader>(args.vertexShader);
    dxArgs->geometryShader = args.geometryShader ? RefCast<DX10GeometryShader>(args.geometryShader) : nullptr;
    dxArgs->pixelShader = RefCast<DX10PixelShader>(args.pixelShader);

    return true;
}
#endif
