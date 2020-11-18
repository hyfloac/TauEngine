#include "codegen/StringTemplateRepairVisitor.hpp"
#include "codegen/ast/StringTemplateAST.hpp"
#include "codegen/ast/StringTemplateExprAST.hpp"
#include "codegen/ast/StringTemplateStringifyAST.hpp"
#include "codegen/ast/StringTemplateVarAST.hpp"
#include "codegen/ast/StringTemplateMemberExprAST.hpp"
#include "codegen/ast/StringTemplateBooleanExprAST.hpp"
#include "codegen/ast/StringTemplateFragmentAST.hpp"
#include "codegen/ast/StringTemplateLoopAST.hpp"
#include "codegen/ast/StringTemplateIfAST.hpp"

#include "llvm/Support/raw_ostream.h"

namespace tau { namespace codegen { namespace string {

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateStringExprAST&) noexcept
{ }

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateIntegerExprAST&) noexcept
{ }

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateVarAssignExprAST& ast) noexcept
{ visit(ast.assignmentExpr()); }

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateVarRetrieveExprAST&) noexcept
{ }

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateMemberAssignExprAST& ast) noexcept
{
    visit(ast.objectExpr());
    visit(ast.assignmentExpr());
}

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateMemberAccessExprAST& ast) noexcept
{ visit(ast.objectExpr()); }

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateMethodAccessExprAST& ast) noexcept
{
    visit(ast.objectExpr());

    for(uSys i = 0; i < ast.arguments().size(); ++i)
    {
        visit(ast.arguments()[i]);
    }
}

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateBooleanInvertExprAST& ast) noexcept
{ visit(ast.expr()); }

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateTextBlockAST& ast) noexcept
{ visit(ast.next()); }

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateStringifyAST& ast) noexcept
{
    visit(ast.expr());
    visit(ast.next()); 
}

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateVarDeclAST& ast) noexcept
{ visit(ast.next()); }

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateBeginFragmentAST& ast) noexcept
{
    _loopDepth = 0;
    _ifDepth.clear();
    visit(ast.next());
}

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateEndFragmentAST& ast) noexcept
{
    if(_loopDepth > 0)
    {
        llvm::outs() << "Too few end loops.\n";
    }

    if(!_ifDepth.empty())
    {
        llvm::outs() << "Too few end ifs.\n";
    }

    visit(ast.next());
}

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateBeginLoopAST& ast) noexcept
{
    visit(ast.varDecl());
    visit(ast.rangeExpr());

    ++_loopDepth;

    visit(ast.next()); 
}

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateEndLoopAST& ast) noexcept
{
    --_loopDepth;

    if(_loopDepth < 0)
    {
        llvm::outs() << "Too many end loops found.\n";
    }

    visit(ast.next());
}

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateLoopControlAST& ast) noexcept
{
    if(_loopDepth <= 0)
    {
        llvm::outs() << "Found loop control outside of loop.\n";
    }

    if(static_cast<iSys>(ast.loopIndex()) > _loopDepth - 1)
    {
        llvm::outs() << "Loop control index exceeds the number of loops.\n";
    }

    visit(ast.next());
}

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateBeginIfAST& ast) noexcept
{
    _ifDepth.emplace_back(false);

    visit(ast.controlExpr());
    visit(ast.next());
}

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateElseIfAST& ast) noexcept
{
    if(_ifDepth.empty())
    {
        llvm::outs() << "No if statement found for else if.\n";
    }

    if(_ifDepth[_ifDepth.size() - 1].elseFound)
    {
        llvm::outs() << "Else if on if block after an else statement.\n";
    }

    visit(ast.controlExpr());
    visit(ast.next());
}

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateElseAST& ast) noexcept
{
    if(_ifDepth.empty())
    {
        llvm::outs() << "No if statement found for else.\n";
    }

    if(_ifDepth.back().elseFound)
    {
        llvm::outs() << "Else on if block that already has an else statement.\n";
    }

    visit(ast.next());
}

void StringTemplateReportErrorsVisitor::visit(ast::StringTemplateEndIfAST& ast) noexcept
{
    if(_ifDepth.empty())
    {
        llvm::outs() << "No if statement found for endif.\n";
    }

    _ifDepth.pop_back();

    visit(ast.next());
}

StringTemplateRepairVisitor::IfFrame::IfFrame() noexcept
    : begin(nullptr)
    , end(nullptr)
    , elseCount(0)
{ }

StringTemplateRepairVisitor::IfFrame::IfFrame(const StrongRef<ast::StringTemplateBeginIfAST>& _begin, const StrongRef<ast::StringTemplateEndIfAST>& _end) noexcept
    : begin(_begin)
    , end(_end)
    , elseCount(0)
{ }

StringTemplateRepairVisitor::StringTemplateRepairVisitor() noexcept
    : _repairIfTarget(-1)
{ }

StringTemplateRepairVisitor::~StringTemplateRepairVisitor() noexcept
{ }

void StringTemplateRepairVisitor::visit(ast::StringTemplateStringExprAST&) noexcept
{ }

void StringTemplateRepairVisitor::visit(ast::StringTemplateIntegerExprAST&) noexcept
{ }

void StringTemplateRepairVisitor::visit(ast::StringTemplateVarAssignExprAST& ast) noexcept
{
    if(_repairIfTarget == -1)
    { visit(ast.assignmentExpr()); }
}

void StringTemplateRepairVisitor::visit(ast::StringTemplateVarRetrieveExprAST&) noexcept
{ }

void StringTemplateRepairVisitor::visit(ast::StringTemplateMemberAssignExprAST& ast) noexcept
{
    if(_repairIfTarget == -1)
    {
        visit(ast.objectExpr());
        visit(ast.assignmentExpr());
    }
}

void StringTemplateRepairVisitor::visit(ast::StringTemplateMemberAccessExprAST& ast) noexcept
{
    if(_repairIfTarget == -1)
    { visit(ast.objectExpr()); }
}

void StringTemplateRepairVisitor::visit(ast::StringTemplateMethodAccessExprAST& ast) noexcept
{
    if(_repairIfTarget == -1)
    {
        visit(ast.objectExpr());

        for(uSys i = 0; i < ast.arguments().size(); ++i)
        {
            visit(ast.arguments()[i]);
        }
    }
}

void StringTemplateRepairVisitor::visit(ast::StringTemplateBooleanInvertExprAST& ast) noexcept
{
    if(_repairIfTarget == -1)
    { visit(ast.expr()); }
}

void StringTemplateRepairVisitor::visit(ast::StringTemplateTextBlockAST& ast) noexcept
{ visit(ast.next()); }

void StringTemplateRepairVisitor::visit(ast::StringTemplateStringifyAST& ast) noexcept
{
    if(_repairIfTarget == -1)
    { visit(ast.expr()); }
    visit(ast.next()); 
}

void StringTemplateRepairVisitor::visit(ast::StringTemplateVarDeclAST& ast) noexcept
{ visit(ast.next()); }

void StringTemplateRepairVisitor::visit(ast::StringTemplateBeginFragmentAST& ast) noexcept
{
    _loopStack.clear();
    _ifStack.clear();
    _repairIfTarget = -1;
    visit(ast.next());
}

void StringTemplateRepairVisitor::visit(ast::StringTemplateEndFragmentAST& ast) noexcept
{ visit(ast.next()); }

void StringTemplateRepairVisitor::visit(ast::StringTemplateBeginLoopAST& ast) noexcept
{
    if(_repairIfTarget == -1)
    {
        _loopStack.push_back(RefCast<ast::StringTemplateBeginLoopAST>(_currentAST));

        visit(ast.varDecl());
        visit(ast.rangeExpr());
    }

    visit(ast.next()); 
}

void StringTemplateRepairVisitor::visit(ast::StringTemplateEndLoopAST& ast) noexcept
{
    if(_repairIfTarget == -1)
    {
        StrongRef<ast::StringTemplateBeginLoopAST> begin = _loopStack.back();

        begin->end() = RefCast<ast::StringTemplateEndLoopAST>(_currentAST);
        ast.begin() = begin;

        _loopStack.pop_back();
    }

    visit(ast.next());
}

void StringTemplateRepairVisitor::visit(ast::StringTemplateLoopControlAST& ast) noexcept
{
    if(_repairIfTarget == -1)
    {
        const StrongRef<ast::StringTemplateBeginLoopAST> targetLoop = _loopStack[_loopStack.size() - ast.loopIndex() - 1];

        ast.loop() = targetLoop;
    }

    visit(ast.next());
}

void StringTemplateRepairVisitor::visit(ast::StringTemplateBeginIfAST& ast) noexcept
{
    if(_repairIfTarget == -1)
    {
        _ifStack.emplace_back(RefCast<ast::StringTemplateBeginIfAST>(_currentAST), nullptr);

        visit(ast.controlExpr());
    }
    else
    {
        ++_repairIfTarget;
    }

    visit(ast.next());
}

void StringTemplateRepairVisitor::visit(ast::StringTemplateElseIfAST& ast) noexcept
{
    auto& pair = _ifStack.back();

    if(_repairIfTarget == -1)
    {
        ast.begin() = pair.begin;
        ++pair.elseCount;
        visit(ast.controlExpr());
    }
    else if(_repairIfTarget == 0)
    {
        ast.end() = pair.end;
    }

    visit(ast.next());
}

void StringTemplateRepairVisitor::visit(ast::StringTemplateElseAST& ast) noexcept
{
    auto& pair = _ifStack.back();

    if(_repairIfTarget == -1)
    {
        ast.begin() = pair.begin;
        ++pair.elseCount;
    }
    else if(_repairIfTarget == 0)
    {
        ast.end() = pair.end;
    }


    visit(ast.next());
}

void StringTemplateRepairVisitor::visit(ast::StringTemplateEndIfAST& ast) noexcept
{
    if(_repairIfTarget == -1)
    {
        auto& pair = _ifStack.back();

        pair.end = RefCast<ast::StringTemplateEndIfAST>(_currentAST);

        ast.begin() = pair.begin;

        if(pair.elseCount > 0)
        {
            _repairIfTarget = 0;
            visit(pair.begin);
        }
    }
    else
    {
        --_repairIfTarget;
    }
    
    visit(ast.next());
}

} } }
