#include "shader/bundle/PrintShaderBundleVisitor.hpp"
#include "shader/bundle/ast/FileExprAST.hpp"
#include "shader/bundle/ast/BlockExprAST.hpp"
#include "shader/bundle/ast/ShaderIOBindingExprAST.hpp"

PrintShaderBundleVisitor::PrintShaderBundleVisitor(const PrintSBVArgs* args) noexcept
    : IShaderBundleVisitor(), _currIndent(0)
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
    fprintf(_file, "File: \"%s\"", expr.filePath().c_str());
}

void PrintShaderBundleVisitor::visit(const sbp::BlockExprAST& expr) noexcept
{
    printIndent();
    switch(expr.type())
    {
        case sbp::BlockType::Uniforms: fputs("Uniforms", _file); break;
        case sbp::BlockType::Textures: fputs("Textures", _file); break;
        case sbp::BlockType::Inputs:   fputs("Inputs", _file); break;
        case sbp::BlockType::Outputs:  fputs("Outputs", _file); break;
    }
    fputs(": ", _file);
    printBrace();
    ++_currIndent;

    visit(expr.container().get());

    --_currIndent;
    fputs(_newLine, _file);
    printIndent();
    fputc('}', _file);
}

void PrintShaderBundleVisitor::visit(const sbp::ShaderStageBlockExprAST& expr) noexcept
{
    printIndent();
    switch(expr.stage())
    {
        case EShader::Stage::TessellationControl:    fputs(_tessCtrlName, _file); break;
        case EShader::Stage::TessellationEvaluation: fputs(_tessEvalName, _file); break;
        case EShader::Stage::Geometry:               fputs("Geometry", _file); break;
        default:                                     fputs("UnknownShader", _file); break;
    }
    fputs(": ", stdout);
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
    fputs(_newLine, _file);
    printIndent();
    fputc('}', stdout);
}

void PrintShaderBundleVisitor::visit(const sbp::OuterShaderStageBlockExprAST& expr) noexcept
{
    printIndent();
    switch(expr.stage())
    {
        case EShader::Stage::Vertex: fputs("Vertex", _file); break;
        case EShader::Stage::Pixel:  fputs(_pixelName, _file); break;
        default:                     fputs("UnknownShader", _file); break;
    }
    fputs(": ", _file);
    printBrace();
    ++_currIndent;

    if(expr.file())
    {
        visit(*expr.file().get());
        printComma(expr.io() || expr.uniforms() || expr.textures());
    }

    if(expr.io())
    {
        visit(*expr.io().get());
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
    fputs(_newLine, _file);
    printIndent();
    fputc('}', _file);
}

static const char* renderingModeName(const RenderingMode::Mode mode) noexcept
{
    switch(mode)
    {
        case RenderingMode::Mode::DirectX9:    return "DirectX9";
        case RenderingMode::Mode::DirectX10:   return "DirectX10";
        case RenderingMode::Mode::DirectX11:   return "DirectX11";
        case RenderingMode::Mode::DirectX12:   return "DirectX12";
        case RenderingMode::Mode::DirectX12_1: return "DirectX12_1";
        case RenderingMode::Mode::Vulkan:      return "Vulkan";
        case RenderingMode::Mode::OpenGL3:     return "OpenGL3";
        case RenderingMode::Mode::OpenGL3_1:   return "OpenGL3_1";
        case RenderingMode::Mode::OpenGL3_2:   return "OpenGL3_2";
        case RenderingMode::Mode::OpenGL3_3:   return "OpenGL3_3";
        case RenderingMode::Mode::OpenGL4:     return "OpenGL4";
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
                fputs(", ", _file);
            }
            const RenderingMode::Mode api = static_cast<RenderingMode::Mode>(bit);
            fputs(renderingModeName(api), _file);
            shouldPrintComma = true;
        }
    }

    fputs(": ", _file);
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
    fputs(_newLine, _file);
    printIndent();
    fputc('}', _file);

    if(expr.next())
    {
        printComma();
        visit(*expr.next().get());
    }
}

void PrintShaderBundleVisitor::visit(const sbp::ShaderIOMapPointExprAST& expr) noexcept
{
    printIndent();
    fprintf(_file, "%d: %d", expr.virtualBindPoint(), expr.shaderBind());
    printComma(expr.next());
    visit(expr.next().get());
}

void PrintShaderBundleVisitor::visit(const sbp::ShaderIOBindPointExprAST& expr) noexcept
{
    printIndent();
    fprintf(_file, "%d: \"%s\"", expr.virtualBindPoint(), expr.uniformName().c_str());
    printComma(expr.next());
    visit(expr.next().get());
}

void PrintShaderBundleVisitor::printIndent() const noexcept
{
    const uSys numSpaces = _indentCount * _currIndent;

    for(uSys i = 0; i < numSpaces; ++i)
    {
        fputc(_indentChar, _file);
    }
}

void PrintShaderBundleVisitor::printComma(const bool shouldPrint) const noexcept
{
    if(shouldPrint)
    {
        fputc(',', _file);
        fputs(_newLine, _file);
    }
}

void PrintShaderBundleVisitor::printBrace() const noexcept
{
    if(_bracesSameLine)
    {
        fputc('{', _file);
        fputs(_newLine, _file);
    }
    else
    {
        fputs(_newLine, _file);
        printIndent();
        fputc('{', _file);
        fputs(_newLine, _file);
    }
}

