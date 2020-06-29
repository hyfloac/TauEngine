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
    visit(expr.next().get());
}

void ShaderInfoExtractorVisitor::visit(const sbp::TextureParamsBlockAST& expr) noexcept
{
    get(_currentStage).texturePoints.emplace(expr.crmTarget(), expr.bindPoint(), expr.sampler());
    visit(expr.next().get());
}

void ShaderInfoExtractorVisitor::visit(const sbp::ShaderStageBlockAST& expr) noexcept
{
    _currentStage = expr.stage();

    visit(expr.file().get());
    visit(expr.uniforms().get());
    visit(expr.textures().get());
}

void ShaderInfoExtractorVisitor::visit(const sbp::APIBlockAST& expr) noexcept
{
    const sbp::APIBlockAST* curr = &expr;
    while(curr && !curr->hasAPI(_targetMode))
    { curr = curr->next().get(); }

    if(!curr) { return; }

    visit(expr.vertex().get());
    visit(expr.tessCtrl().get());
    visit(expr.tessEval().get());
    visit(expr.geometry().get());
    visit(expr.pixel().get());
}
