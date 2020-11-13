#pragma once

#include <Safeties.hpp>
#include <String.hpp>

namespace tau { namespace codegen { namespace string {

namespace ast {

class StringTemplateAST;
class StringTemplateRootAST;
class StringTemplateExprAST;

class StringTemplateStringExprAST;
class StringTemplateIntegerExprAST;
class StringTemplateVarAssignExprAST;
class StringTemplateVarRetrieveExprAST;
class StringTemplateMemberAssignExprAST;
class StringTemplateMemberAccessExprAST;
class StringTemplateMethodAccessExprAST;

class StringTemplateTextBlockAST;
class StringTemplateStringifyAST;
class StringTemplateVarDeclAST;
class StringTemplateBeginFragmentAST;
class StringTemplateEndFragmentAST;
class StringTemplateBeginLoopAST;
class StringTemplateEndLoopAST;
class StringTemplateLoopControlAST;
class StringTemplateBeginIfAST;
class StringTemplateElseIfAST;
class StringTemplateElseAST;
class StringTemplateEndIfAST;

}

class IStringTemplateVisitor
{
    DEFAULT_CONSTRUCT_PO(IStringTemplateVisitor);
    DEFAULT_DESTRUCT_VI(IStringTemplateVisitor);
    DELETE_CM(IStringTemplateVisitor);
public:
    virtual void visit(ast::StringTemplateAST* ast) noexcept
    {
        if(!ast)
        { return; }

        visit(*ast);
    }

    template<typename _T, ::std::enable_if_t<::std::is_base_of_v<ast::StringTemplateAST, _T>, int>* = nullptr>
    void visit(StrongRef<_T>& ast) noexcept
    { visit(ast.get()); }
    
    template<typename _T, ::std::enable_if_t<::std::is_base_of_v<ast::StringTemplateAST, _T>, int>* = nullptr>
    void visit(WeakRef<_T>& ast) noexcept
    { visit(ast.get()); }

    virtual void visit(ast::StringTemplateAST& ast) noexcept;
    virtual void visit(ast::StringTemplateRootAST& ast) noexcept;
    virtual void visit(ast::StringTemplateExprAST& ast) noexcept;

    virtual void visit(ast::StringTemplateStringExprAST& ast) noexcept;
    virtual void visit(ast::StringTemplateIntegerExprAST& ast) noexcept;
    virtual void visit(ast::StringTemplateVarAssignExprAST& ast) noexcept;
    virtual void visit(ast::StringTemplateVarRetrieveExprAST& ast) noexcept;
    virtual void visit(ast::StringTemplateMemberAssignExprAST& ast) noexcept;
    virtual void visit(ast::StringTemplateMemberAccessExprAST& ast) noexcept;
    virtual void visit(ast::StringTemplateMethodAccessExprAST& ast) noexcept;

    virtual void visit(ast::StringTemplateTextBlockAST& ast) noexcept;
    virtual void visit(ast::StringTemplateStringifyAST& ast) noexcept;
    virtual void visit(ast::StringTemplateVarDeclAST& ast) noexcept;
    virtual void visit(ast::StringTemplateBeginFragmentAST& ast) noexcept;
    virtual void visit(ast::StringTemplateEndFragmentAST& ast) noexcept;
    virtual void visit(ast::StringTemplateBeginLoopAST& ast) noexcept;
    virtual void visit(ast::StringTemplateEndLoopAST& ast) noexcept;
    virtual void visit(ast::StringTemplateLoopControlAST& ast) noexcept;
    virtual void visit(ast::StringTemplateBeginIfAST& ast) noexcept;
    virtual void visit(ast::StringTemplateElseIfAST& ast) noexcept;
    virtual void visit(ast::StringTemplateElseAST& ast) noexcept;
    virtual void visit(ast::StringTemplateEndIfAST& ast) noexcept;
};

} } }

