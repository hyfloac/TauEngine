#include "codegen/StringTemplateVisitor.hpp"
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

void IStringTemplateVisitor::visit(ast::StringTemplateAST* ast) noexcept
{
    if(ast)
    { ast->visit(*this); }
}

void IStringTemplateVisitor::visit(ast::StringTemplateAST& ast) noexcept
{ ast.visit(*this); }

void IStringTemplateVisitor::visit(ast::StringTemplateSequenceAST& ast) noexcept
{ visit(ast.next()); }

void IStringTemplateVisitor::visit(ast::StringTemplateRootAST& ast) noexcept
{ visit(ast.next()); }

void IStringTemplateVisitor::visit(ast::StringTemplateExprAST&) noexcept
{ }

void IStringTemplateVisitor::visit(ast::StringTemplateWildExprAST& ast) noexcept
{
    visit(ast.expr());
    visit(ast.next());
}

void IStringTemplateVisitor::visit(ast::StringTemplateTextBlockAST& ast) noexcept
{ visit(ast.next()); }

void IStringTemplateVisitor::visit(ast::StringTemplateStringifyAST& ast) noexcept
{
    visit(ast.expr());
    visit(ast.next());
}

void IStringTemplateVisitor::visit(ast::StringTemplateStringExprAST&) noexcept
{ }

void IStringTemplateVisitor::visit(ast::StringTemplateIntegerExprAST&) noexcept
{ }

void IStringTemplateVisitor::visit(ast::StringTemplateVarAssignExprAST& ast) noexcept
{ visit(ast.assignmentExpr()); }

void IStringTemplateVisitor::visit(ast::StringTemplateVarRetrieveExprAST&) noexcept
{ }

void IStringTemplateVisitor::visit(ast::StringTemplateMemberAssignExprAST& ast) noexcept
{
    visit(ast.objectExpr());
    visit(ast.assignmentExpr());
}

void IStringTemplateVisitor::visit(ast::StringTemplateMemberAccessExprAST& ast) noexcept
{
    visit(ast.objectExpr());
}

void IStringTemplateVisitor::visit(ast::StringTemplateMethodAccessExprAST& ast) noexcept
{
    visit(ast.objectExpr());

    for(auto& arg : ast.arguments())
    {
        visit(arg);
    }
}

void IStringTemplateVisitor::visit(ast::StringTemplateBooleanInvertExprAST& ast) noexcept
{ visit(ast.expr()); }

void IStringTemplateVisitor::visit(ast::StringTemplateVarDeclAST& ast) noexcept
{ visit(ast.next()); }

void IStringTemplateVisitor::visit(ast::StringTemplateBeginFragmentAST& ast) noexcept
{ visit(ast.next()); }

void IStringTemplateVisitor::visit(ast::StringTemplateEndFragmentAST& ast) noexcept
{ visit(ast.next()); }

void IStringTemplateVisitor::visit(ast::StringTemplateBeginLoopAST& ast) noexcept
{
    visit(ast.rangeExpr());
    visit(ast.next());
}

void IStringTemplateVisitor::visit(ast::StringTemplateEndLoopAST& ast) noexcept
{ visit(ast.next()); }

void IStringTemplateVisitor::visit(ast::StringTemplateLoopControlAST& ast) noexcept
{ visit(ast.next()); }

void IStringTemplateVisitor::visit(ast::StringTemplateBeginIfAST& ast) noexcept
{
    visit(ast.controlExpr());
    visit(ast.next());
}

void IStringTemplateVisitor::visit(ast::StringTemplateElseIfAST& ast) noexcept
{
    visit(ast.controlExpr());
    visit(ast.next());
}

void IStringTemplateVisitor::visit(ast::StringTemplateElseAST& ast) noexcept
{ visit(ast.next()); }

void IStringTemplateVisitor::visit(ast::StringTemplateEndIfAST& ast) noexcept
{ visit(ast.next()); }


void StringTemplatePrintingVisitor::visit(ast::StringTemplateStringExprAST& ast) noexcept
{
    llvm::outs() << "\"" << ast.string() << "\"";
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateIntegerExprAST& ast) noexcept
{
    llvm::outs() << ast.intVal();
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateVarAssignExprAST& ast) noexcept
{
    llvm::outs() << "%" << ast.varName() << " = ";

    visit(ast.assignmentExpr());
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateVarRetrieveExprAST& ast) noexcept
{
    llvm::outs() << "%" << ast.varName();
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateMemberAssignExprAST& ast) noexcept
{
    visit(ast.objectExpr());

    llvm::outs() << " = ";

    visit(ast.assignmentExpr());
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateMemberAccessExprAST& ast) noexcept
{
    visit(ast.objectExpr());

    llvm::outs() << "." << ast.memberName();
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateMethodAccessExprAST& ast) noexcept
{
    visit(ast.objectExpr());

    llvm::outs() << "." << ast.methodName() << "(";

    if(ast.arguments().size() == 1)
    {
        visit(ast.arguments()[0]);
    }
    else if(ast.arguments().size() > 1)
    {
        for(uSys i = 0; i < ast.arguments().size() - 2; ++i)
        {
            visit(ast.arguments()[i]);
            llvm::outs() << ", ";
        }

        visit(ast.arguments()[ast.arguments().size() - 1]);
    }
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateBooleanInvertExprAST& ast) noexcept
{
    llvm::outs() << "!";
    visit(ast.expr());
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateTextBlockAST& ast) noexcept
{
    llvm::outs() << ast.text();
    visit(ast.next());
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateStringifyAST& ast) noexcept
{
    llvm::outs() << "%<str>(";
    visit(ast.expr());
    llvm::outs() << ")";

    visit(ast.next());
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateVarDeclAST& ast) noexcept
{
    llvm::outs() << "%" << ast.varName();
    visit(ast.next());
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateBeginFragmentAST& ast) noexcept
{
    llvm::outs() << "%<fragment>(" << ast.fragmentName() << ")\n";
    ++_indent;
    visit(ast.next());
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateEndFragmentAST& ast) noexcept
{
    --_indent;
    llvm::outs() << "%<endfragment>\n\n";
    visit(ast.next());
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateBeginLoopAST& ast) noexcept
{
    llvm::outs() << "%<loop>(";

    visit(ast.varDecl());

    llvm::outs() << " : ";

    visit(ast.rangeExpr());

    llvm::outs() << ")\n";
    ++_indent;

    visit(ast.next());
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateEndLoopAST& ast) noexcept
{
    --_indent;
    llvm::outs() << "%<endloop>\n";
    visit(ast.next());
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateLoopControlAST& ast) noexcept
{
    switch(ast.controlType())
    {
        case ast::LoopControlType::Continue:
            llvm::outs() << "%<continue>(";
            break;
        case ast::LoopControlType::Break:
            llvm::outs() << "%<break>(";
            break;
        case ast::LoopControlType::LoopIndex:
            llvm::outs() << "%<loopindex>(";
            break;
    }

    llvm::outs() << ast.loopIndex() << ")";

    visit(ast.next());
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateBeginIfAST& ast) noexcept
{
    llvm::outs() << "%<if>(";

    visit(ast.controlExpr());

    llvm::outs() << ")\n";

    ++_indent;

    visit(ast.next());
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateElseIfAST& ast) noexcept
{
    --_indent;

    llvm::outs() << "%<elif>(";

    visit(ast.controlExpr());

    llvm::outs() << ")\n";

    ++_indent;

    visit(ast.next());
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateElseAST& ast) noexcept
{
    --_indent;
    llvm::outs() << "%<else>\n";
    ++_indent;
    visit(ast.next());
}

void StringTemplatePrintingVisitor::visit(ast::StringTemplateEndIfAST& ast) noexcept
{
    --_indent;
    llvm::outs() << "%<endif>\n";
    visit(ast.next());
}

} } }

