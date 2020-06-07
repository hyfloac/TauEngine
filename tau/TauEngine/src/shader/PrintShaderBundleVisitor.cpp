#include "shader/bundle/PrintShaderBundleVisitor.hpp"
#include "shader/bundle/ast/FileExprAST.hpp"
#include "shader/bundle/ast/BlockExprAST.hpp"
#include "shader/bundle/ast/ShaderIOBindingExprAST.hpp"

PrintShaderBundleVisitor::PrintShaderBundleVisitor(const PrintSBVArgs* args) noexcept
    : _currIndent(0)
{
    if(args)
    {
        _indentCount = args->indentCount;
        _indentChar = args->indentChar == PrintSBVArgs::Spaces ? ' ' : '\t';
        _newLine = args->newLineChar == PrintSBVArgs::LF ? "\n" : (args->newLineChar == PrintSBVArgs::CR ? "\r" : "\r\n");
        _tessCtrlName = args->tessCtrlName == PrintSBVArgs::TessellationControl ? "TessellationControl" : "Hull";
        _tessEvalName = args->tessEvalName == PrintSBVArgs::TessellationEvaluation ? "TessellationEvaluation" : "Domain";
        _pixelName = args->pixelName == PrintSBVArgs::Pixel ? "Pixel" : "Fragment";
        _bracesSameLine = args->braces == PrintSBVArgs::SameLine ? true : false;
        _file = args->file ? args->file : stdout;
    }
    else
    {
        _indentCount = 4;
        _indentChar = ' ';
        _newLine = "\n";
        _tessCtrlName = "TessellationControl";
        _tessEvalName = "TessellationEvaluation";
        _pixelName = "Pixel";
        _bracesSameLine = true;
        _file = stdout;
    }
}

void PrintShaderBundleVisitor::visit(const sbp::FileExprAST& expr) noexcept
{
    printIndent();
    ConPrinter::print(_file, "File: \"%\"", expr.filePath());
}

void PrintShaderBundleVisitor::visit(const sbp::BlockExprAST& expr) noexcept
{
    printIndent();
    switch(expr.type())
    {
        case sbp::BlockType::Uniforms: ConPrinter::print(_file, "Uniforms"); break;
        case sbp::BlockType::Textures: ConPrinter::print(_file, "Textures"); break;
    }
    ConPrinter::print(_file, ": ");
    printBrace();
    ++_currIndent;

    visit(expr.container().get());

    --_currIndent;
    ConPrinter::print(_file, _newLine);
    printIndent();
    ConPrinter::print(_file, '}');
}

void PrintShaderBundleVisitor::visit(const sbp::ShaderStageBlockExprAST& expr) noexcept
{
    printIndent();
    switch(expr.stage())
    {
        case EShader::Stage::Vertex:   ConPrinter::print(_file, "Vertex"); break;
        case EShader::Stage::Hull:     ConPrinter::print(_file, _tessCtrlName); break;
        case EShader::Stage::Domain:   ConPrinter::print(_file, _tessEvalName); break;
        case EShader::Stage::Geometry: ConPrinter::print(_file, "Geometry"); break;
        case EShader::Stage::Pixel:    ConPrinter::print(_file, _pixelName); break;
        default:                       ConPrinter::print(_file, "UnknownShader"); break;
    }
    ConPrinter::print(_file, ": ");
    printBrace();
    ++_currIndent;

    if(expr.file())
    {
        visit(*expr.file().get());
        printComma(expr.uniforms() || expr.textures());
    }

    if(expr.uniforms())
    {
        visit(*expr.uniforms().get());
        printComma(expr.textures());
    }

    if(expr.textures())
    {
        visit(*expr.textures().get());
    }

    --_currIndent;
    ConPrinter::print(_file, _newLine);
    printIndent();
    ConPrinter::print(_file, '}');
}

static const char* renderingModeName(const RenderingMode::Mode mode) noexcept
{
    switch(mode)
    {
        case RenderingMode::Mode::DirectX10:   return "DirectX10";
        case RenderingMode::Mode::DirectX11:   return "DirectX11";
        case RenderingMode::Mode::DirectX12:   return "DirectX12";
        case RenderingMode::Mode::DirectX12_1: return "DirectX12_1";
        case RenderingMode::Mode::Vulkan:      return "Vulkan";
        case RenderingMode::Mode::OpenGL4_1:   return "OpenGL4_1";
        case RenderingMode::Mode::OpenGL4_2:   return "OpenGL4_2";
        case RenderingMode::Mode::OpenGL4_3:   return "OpenGL4_3";
        case RenderingMode::Mode::OpenGL4_4:   return "OpenGL4_4";
        case RenderingMode::Mode::OpenGL4_5:   return "OpenGL4_5";
        case RenderingMode::Mode::OpenGL4_6:   return "OpenGL4_6";
        default:                               return "UnknownAPI";
    }
}

void PrintShaderBundleVisitor::visit(const sbp::APIBlockExprAST& expr) noexcept
{
    printIndent();

    const sbp::APIBlockExprAST::APISet& apiSet = expr.apis();

    bool shouldPrintComma = false;

    // https://lemire.me/blog/2018/02/21/iterating-over-set-bits-quickly/
    const uSys words = (apiSet.size() - 1) / (CHAR_BIT * sizeof(uSys)) + 1;
    for(uSys k = 0; k < words; ++k) 
    {
        u64 bitSet = apiSet._Getword(k);
        while(bitSet != 0) 
        {
            const u64 t = bitSet & -bitSet;
            unsigned long r;
            _BitScanForward64(&r, bitSet);
            bitSet ^= t;

            const u64 bit = k * 64 + r;

            if(shouldPrintComma)
            {
                ConPrinter::print(_file, ", ");
            }
            const RenderingMode::Mode api = static_cast<RenderingMode::Mode>(bit);
            ConPrinter::print(_file, renderingModeName(api));
            shouldPrintComma = true;
        }
    }

    ConPrinter::print(_file, ": ");
    printBrace();
    ++_currIndent;

    if(expr.vertex())
    {
        visit(*expr.vertex().get());
        printComma(expr.tessCtrl() || expr.tessEval() || expr.geometry() || expr.pixel());
    }

    if(expr.tessCtrl())
    {
        visit(*expr.tessCtrl().get());
        printComma(expr.tessEval() || expr.geometry() || expr.pixel());
    }

    if(expr.tessEval())
    {
        visit(*expr.tessEval().get());
        printComma(expr.geometry() || expr.pixel());
    }

    if(expr.geometry())
    {
        visit(*expr.geometry().get());
        printComma(expr.pixel());
    }

    if(expr.pixel())
    {
        visit(*expr.pixel().get());
    }

    --_currIndent;
    ConPrinter::print(_file, _newLine);
    printIndent();
    ConPrinter::print(_file, '}');

    if(expr.next())
    {
        printComma();
        visit(*expr.next().get());
    }
}

static const char* crmTargetName(const CommonRenderingModelToken crmTarget) noexcept
{
    switch(crmTarget)
    {
        case CommonRenderingModelToken::UniformBindingCameraDynamic: return "UniformBindingCameraDynamic";
        case CommonRenderingModelToken::UniformBindingCameraStatic:  return "UniformBindingCameraStatic";
        case CommonRenderingModelToken::TextureNormal:               return "TextureNormal";
        case CommonRenderingModelToken::TextureDiffuse:              return "TextureDiffuse";
        case CommonRenderingModelToken::TexturePBRCompound:          return "TexturePBRCompound";
        case CommonRenderingModelToken::TextureEmissivity:           return "TextureEmissivity";
        case CommonRenderingModelToken::TexturePosition:             return "TexturePosition";
        case CommonRenderingModelToken::TextureDepth:                return "TextureDepth";
        case CommonRenderingModelToken::TextureStencil:              return "TextureStencil";
        default:                                                     return "UnknownCRMTarget";
    }
}

void PrintShaderBundleVisitor::visit(const sbp::ShaderIOMapPointExprAST& expr) noexcept
{
    printIndent();
    ConPrinter::print(_file, "%: %", crmTargetName(expr.crmTarget()), expr.shaderBind());
    printComma(expr.next());
    visit(expr.next().get());
}

void PrintShaderBundleVisitor::visit(const sbp::ShaderIOBindPointExprAST& expr) noexcept
{
    printIndent();
    ConPrinter::print(_file, "%: \"%\"", crmTargetName(expr.crmTarget()), expr.uniformName());
    printComma(expr.next());
    visit(expr.next().get());
}

void PrintShaderBundleVisitor::printIndent() const noexcept
{
    const uSys numSpaces = _indentCount * _currIndent;

    for(uSys i = 0; i < numSpaces; ++i)
    {
        ConPrinter::print(_file, _indentChar);
    }
}

void PrintShaderBundleVisitor::printComma(const bool shouldPrint) const noexcept
{
    if(shouldPrint)
    {
        ConPrinter::print(_file, ',');
        ConPrinter::print(_file, _newLine);
    }
}

void PrintShaderBundleVisitor::printBrace() const noexcept
{
    if(_bracesSameLine)
    {
        ConPrinter::print(_file, '{');
        ConPrinter::print(_file, _newLine);
    }
    else
    {
        ConPrinter::print(_file, _newLine);
        printIndent();
        ConPrinter::print(_file, '{');
        ConPrinter::print(_file, _newLine);
    }
}
