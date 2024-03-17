#include "shader/bundle/ShaderInfoExtractorVisitor.hpp"
#include "shader/bundle/ast/FileAST.hpp"
#include "shader/bundle/ast/BlockAST.hpp"

void ShaderInfoExtractorVisitor::visit(const sbp::FileAST& expr) noexcept
{
    get(_currentStage).fileName = expr.filePath();
}

void ShaderInfoExtractorVisitor::visit(const sbp::UniformBindingAST& expr) noexcept
{
    get(_currentStage).uniformPoints.emplace(expr.crmTarget(), expr.bindPoint());
    visit(expr.next().Get());
}

void ShaderInfoExtractorVisitor::visit(const sbp::TextureParamsBlockAST& expr) noexcept
{
    get(_currentStage).texturePoints.emplace(expr.crmTarget(), expr.bindPoint(), expr.sampler());
    visit(expr.next().Get());
}

void ShaderInfoExtractorVisitor::visit(const sbp::ShaderStageBlockAST& expr) noexcept
{
    _currentStage = expr.stage();

    visit(expr.file().Get());
    visit(expr.uniforms().Get());
    visit(expr.textures().Get());
}

void ShaderInfoExtractorVisitor::visit(const sbp::APIBlockAST& expr) noexcept
{
    const sbp::APIBlockAST* curr = &expr;
    while(curr && !curr->hasAPI(_targetMode))
    { curr = curr->next().Get(); }

    if(!curr) { return; }

    visit(expr.vertex().Get());
    visit(expr.tessCtrl().Get());
    visit(expr.tessEval().Get());
    visit(expr.geometry().Get());
    visit(expr.pixel().Get());
}
