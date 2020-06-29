#include "shader/bundle/PrintShaderBundleVisitor.hpp"
#include "shader/bundle/ast/FileAST.hpp"
#include "shader/bundle/ast/BlockAST.hpp"

static const char* renderingModeName(RenderingMode::Mode mode) noexcept;
static const char* crmTargetName(CommonRenderingModelToken crmTarget) noexcept;

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

void PrintShaderBundleVisitor::visit(const sbp::FileAST& expr) noexcept
{
    printIndent();
    ConPrinter::print(_file, "File: \"%\"", expr.filePath());
}

void PrintShaderBundleVisitor::visit(const sbp::UniformBindingAST& expr) noexcept
{
    printIndent();
    ConPrinter::print(_file, "%: ", crmTargetName(expr.crmTarget()));

    if(expr.bindPoint().type == sbp::BindingUnion::Number)
    {
        ConPrinter::print(_file, expr.bindPoint().number);
    }
    else
    {
        ConPrinter::print(_file, "\"%\"", expr.bindPoint().str);
    }

    printComma(expr.next());
    visit(expr.next().get());
}

void PrintShaderBundleVisitor::visit(const sbp::TextureParamsBlockAST& expr) noexcept
{
    printIndent();
    ConPrinter::print(_file, "%: ", crmTargetName(expr.crmTarget()));
    printBrace();
    ++_currIndent;

    ConPrinter::print(_file, "Location: ");
    printBindingUnion(expr.bindPoint());

    printComma();

    ConPrinter::print(_file, "Sampler: %", expr.sampler());

    --_currIndent;
    ConPrinter::print(_file, _newLine);
    printIndent();
    ConPrinter::print(_file, '}');

    printComma(expr.next());
    visit(expr.next().get());
}

void PrintShaderBundleVisitor::visit(const sbp::ShaderStageBlockAST& expr) noexcept
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
        printIndent();
        ConPrinter::print(_file, "Uniforms: ");
        printBrace();
        ++_currIndent;

        visit(*expr.uniforms().get());

        --_currIndent;
        ConPrinter::print(_file, _newLine);
        printIndent();
        ConPrinter::print(_file, '}');

        printComma(expr.textures());
    }

    if(expr.textures())
    {
        printIndent();
        ConPrinter::print(_file, "Textures: ");
        printBrace();
        ++_currIndent;

        visit(*expr.textures().get());

        --_currIndent;
        ConPrinter::print(_file, _newLine);
        printIndent();
        ConPrinter::print(_file, '}');
    }

    --_currIndent;
    ConPrinter::print(_file, _newLine);
    printIndent();
    ConPrinter::print(_file, '}');
}

void PrintShaderBundleVisitor::visit(const sbp::APIBlockAST& expr) noexcept
{
    printIndent();

    const sbp::APIBlockAST::APISet& apiSet = expr.apis();

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

void PrintShaderBundleVisitor::printBindingUnion(const sbp::BindingUnion& bindingUnion) const noexcept
{
    if(bindingUnion.type == sbp::BindingUnion::Number)
    {
        ConPrinter::print(_file, bindingUnion.number);
    }
    else
    {
        ConPrinter::print(_file, "\"%\"", bindingUnion.str);
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
