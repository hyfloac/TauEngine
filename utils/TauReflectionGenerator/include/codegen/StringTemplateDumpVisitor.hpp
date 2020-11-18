#pragma once

#include "StringTemplateVisitor.hpp"

#include <unordered_map>

namespace tau { namespace codegen { namespace string {

class StringTemplateDumpVisitor final : public IStringTemplateVisitor
{
    DEFAULT_DESTRUCT(StringTemplateDumpVisitor);
    DELETE_CM(StringTemplateDumpVisitor);
protected:
    iSys _indent;
    uSys _id;
    ::std::unordered_map<ast::StringTemplateSequenceAST*, uSys> _idMap;
public:
    StringTemplateDumpVisitor() noexcept
        : _indent(0)
        , _id(0)
    { }

    void visit(ast::StringTemplateAST* ast) noexcept override
    { IStringTemplateVisitor::visit(ast); }

    template<typename _T, ::std::enable_if_t<::std::is_base_of_v<ast::StringTemplateAST, _T>, int>* = nullptr>
    void visit(StrongRef<_T>& ast) noexcept
    { visit(ast.get()); }
    
    template<typename _T, ::std::enable_if_t<::std::is_base_of_v<ast::StringTemplateAST, _T>, int>* = nullptr>
    void visit(WeakRef<_T>& ast) noexcept
    { visit(ast.get()); }

    void visit(ast::StringTemplateAST& ast) noexcept override
    { IStringTemplateVisitor::visit(ast); }

    void visit(ast::StringTemplateSequenceAST& ast) noexcept override
    { IStringTemplateVisitor::visit(ast); }

    void visit(ast::StringTemplateRootAST& ast) noexcept override;

    void visit(ast::StringTemplateExprAST& ast) noexcept override
    { IStringTemplateVisitor::visit(ast); }

    void visit(ast::StringTemplateWildExprAST& ast) noexcept override;

    void visit(ast::StringTemplateStringExprAST& ast) noexcept override;
    void visit(ast::StringTemplateIntegerExprAST& ast) noexcept override;
    void visit(ast::StringTemplateVarAssignExprAST& ast) noexcept override;
    void visit(ast::StringTemplateVarRetrieveExprAST& ast) noexcept override;
    void visit(ast::StringTemplateMemberAssignExprAST& ast) noexcept override;
    void visit(ast::StringTemplateMemberAccessExprAST& ast) noexcept override;
    void visit(ast::StringTemplateMethodAccessExprAST& ast) noexcept override;
    void visit(ast::StringTemplateBooleanInvertExprAST& ast) noexcept override;

    void visit(ast::StringTemplateTextBlockAST& ast) noexcept override;
    void visit(ast::StringTemplateStringifyAST& ast) noexcept override;
    void visit(ast::StringTemplateVarDeclAST& ast) noexcept override;
    void visit(ast::StringTemplateBeginFragmentAST& ast) noexcept override;
    void visit(ast::StringTemplateEndFragmentAST& ast) noexcept override;
    void visit(ast::StringTemplateBeginLoopAST& ast) noexcept override;
    void visit(ast::StringTemplateEndLoopAST& ast) noexcept override;
    void visit(ast::StringTemplateLoopControlAST& ast) noexcept override;
    void visit(ast::StringTemplateBeginIfAST& ast) noexcept override;
    void visit(ast::StringTemplateElseIfAST& ast) noexcept override;
    void visit(ast::StringTemplateElseAST& ast) noexcept override;
    void visit(ast::StringTemplateEndIfAST& ast) noexcept override;
private:
    void printIndent(iSys indent) noexcept;
};

} } }
