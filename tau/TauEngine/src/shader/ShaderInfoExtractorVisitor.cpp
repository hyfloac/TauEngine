#include "shader/bundle/ShaderInfoExtractorVisitor.hpp"
#include "shader/bundle/ast/FileExprAST.hpp"
#include "shader/bundle/ast/BlockExprAST.hpp"
#include "shader/bundle/ast/ShaderIOBindingExprAST.hpp"

void ShaderInfoExtractorVisitor::visit(const sbp::FileExprAST& expr) noexcept
{
    switch(_currentStage)
    {
        case EShader::Stage::Vertex:
            _vertexInfo.fileName = expr.filePath();
            break;
        case EShader::Stage::TessellationControl:
            _tessCtrlInfo.fileName = expr.filePath();
            break;
        case EShader::Stage::TessellationEvaluation:
            _tessEvalInfo.fileName = expr.filePath();
            break;
        case EShader::Stage::Geometry:
            _geometryInfo.fileName = expr.filePath();
            break;
        case EShader::Stage::Pixel:
            _pixelInfo.fileName = expr.filePath();
            break;
        default: break;
    }
}

void ShaderInfoExtractorVisitor::visit(const sbp::BlockExprAST& expr) noexcept
{
    visit(expr.container().get());
}

void ShaderInfoExtractorVisitor::visit(const sbp::ShaderStageBlockExprAST& expr) noexcept
{
    _currentStage = expr.stage();

    visit(expr.file().get());
    visit(expr.uniforms().get());
    visit(expr.textures().get());
}

void ShaderInfoExtractorVisitor::visit(const sbp::APIBlockExprAST& expr) noexcept
{
    const sbp::APIBlockExprAST* curr = &expr;
    while(curr && !curr->hasAPI(_targetMode))
    { curr = curr->next().get(); }

    if(!curr) { return; }

    visit(expr.vertex().get());
    visit(expr.tessCtrl().get());
    visit(expr.tessEval().get());
    visit(expr.geometry().get());
    visit(expr.pixel().get());
}

void ShaderInfoExtractorVisitor::visit(const sbp::ShaderIOMapPointExprAST& expr) noexcept
{
    switch(_currentStage)
    {
        case EShader::Stage::Vertex:
            switch(_currentBlock)
            {
                case sbp::BlockType::Uniforms:
                    _vertexInfo.uniformPoints.emplace(expr.crmTarget(), expr.shaderBind());
                    break;
                case sbp::BlockType::Textures:
                    _vertexInfo.texturePoints.emplace(expr.crmTarget(), expr.shaderBind());
                    break;
                default: break;
            }
            break;
        case EShader::Stage::TessellationControl:
            switch(_currentBlock)
            {
                case sbp::BlockType::Uniforms:
                    _tessCtrlInfo.uniformPoints.emplace(expr.crmTarget(), expr.shaderBind());
                    break;
                case sbp::BlockType::Textures:
                    _tessCtrlInfo.texturePoints.emplace(expr.crmTarget(), expr.shaderBind());
                    break;
                default: break;
            }
            break;
        case EShader::Stage::TessellationEvaluation:
            switch(_currentBlock)
            {
                case sbp::BlockType::Uniforms:
                    _tessEvalInfo.uniformPoints.emplace(expr.crmTarget(), expr.shaderBind());
                    break;
                case sbp::BlockType::Textures:
                    _tessEvalInfo.texturePoints.emplace(expr.crmTarget(), expr.shaderBind());
                    break;
                default: break;
            }
            break;
        case EShader::Stage::Geometry:
            switch(_currentBlock)
            {
                case sbp::BlockType::Uniforms:
                    _geometryInfo.uniformPoints.emplace(expr.crmTarget(), expr.shaderBind());
                    break;
                case sbp::BlockType::Textures:
                    _geometryInfo.texturePoints.emplace(expr.crmTarget(), expr.shaderBind());
                    break;
                default: break;
            }
            break;
        case EShader::Stage::Pixel:
            switch(_currentBlock)
            {
                case sbp::BlockType::Uniforms:
                    _pixelInfo.uniformPoints.emplace(expr.crmTarget(), expr.shaderBind());
                    break;
                case sbp::BlockType::Textures:
                    _pixelInfo.texturePoints.emplace(expr.crmTarget(), expr.shaderBind());
                    break;
                default: break;
            }
            break;
        default: break;
    }

    visit(expr.next().get());
}

void ShaderInfoExtractorVisitor::visit(const sbp::ShaderIOBindPointExprAST& expr) noexcept
{
    switch(_currentStage)
    {
        case EShader::Stage::Vertex:
            switch(_currentBlock)
            {
                case sbp::BlockType::Uniforms:
                    _vertexInfo.uniformPoints.emplace(expr.crmTarget(), expr.uniformName());
                    break;
                case sbp::BlockType::Textures:
                    _vertexInfo.texturePoints.emplace(expr.crmTarget(), expr.uniformName());
                    break;
                default: break;
            }
            break;
        case EShader::Stage::TessellationControl:
            switch(_currentBlock)
            {
                case sbp::BlockType::Uniforms:
                    _tessCtrlInfo.uniformPoints.emplace(expr.crmTarget(), expr.uniformName());
                    break;
                case sbp::BlockType::Textures:
                    _tessCtrlInfo.texturePoints.emplace(expr.crmTarget(), expr.uniformName());
                    break;
                default: break;
            }
            break;
        case EShader::Stage::TessellationEvaluation:
            switch(_currentBlock)
            {
                case sbp::BlockType::Uniforms:
                    _tessEvalInfo.uniformPoints.emplace(expr.crmTarget(), expr.uniformName());
                    break;
                case sbp::BlockType::Textures:
                    _tessEvalInfo.texturePoints.emplace(expr.crmTarget(), expr.uniformName());
                    break;
                default: break;
            }
            break;
        case EShader::Stage::Geometry:
            switch(_currentBlock)
            {
                case sbp::BlockType::Uniforms:
                    _geometryInfo.uniformPoints.emplace(expr.crmTarget(), expr.uniformName());
                    break;
                case sbp::BlockType::Textures:
                    _geometryInfo.texturePoints.emplace(expr.crmTarget(), expr.uniformName());
                    break;
                default: break;
            }
            break;
        case EShader::Stage::Pixel:
            switch(_currentBlock)
            {
                case sbp::BlockType::Uniforms:
                    _pixelInfo.uniformPoints.emplace(expr.crmTarget(), expr.uniformName());
                    break;
                case sbp::BlockType::Textures:
                    _pixelInfo.texturePoints.emplace(expr.crmTarget(), expr.uniformName());
                    break;
                default: break;
            }
            break;
        default: break;
    }

    visit(expr.next().get());
}
