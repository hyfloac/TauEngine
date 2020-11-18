#pragma once

#include "StringTemplateVisitor.hpp"

#include <deque>

namespace tau { namespace codegen { namespace string {

class StringTemplateReportErrorsVisitor : public IStringTemplateVisitor
{
    DEFAULT_DESTRUCT(StringTemplateReportErrorsVisitor);
    DELETE_CM(StringTemplateReportErrorsVisitor);
public:
    struct IfFrame final
    {
        bool elseFound;

        IfFrame(bool _elseFound = false)
            : elseFound(_elseFound)
        { }
    };
protected:
    iSys _loopDepth;
    ::std::deque<IfFrame> _ifDepth;
public:
    StringTemplateReportErrorsVisitor() noexcept
        : _loopDepth(0)
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

    void visit(ast::StringTemplateRootAST& ast) noexcept override
    { IStringTemplateVisitor::visit(ast); }

    void visit(ast::StringTemplateExprAST& ast) noexcept override
    { IStringTemplateVisitor::visit(ast); }

    void visit(ast::StringTemplateWildExprAST& ast) noexcept override
    { IStringTemplateVisitor::visit(ast); }

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
};

class StringTemplateRepairVisitor : public IStringTemplateVisitor
{
    DELETE_CM(StringTemplateRepairVisitor);
public:
    struct IfFrame final
    {
        StrongRef<ast::StringTemplateBeginIfAST> begin;
        StrongRef<ast::StringTemplateEndIfAST> end;
        uSys elseCount;

        IfFrame() noexcept;

        IfFrame(const StrongRef<ast::StringTemplateBeginIfAST>& _begin, const StrongRef<ast::StringTemplateEndIfAST>& _end) noexcept;
    };
protected:
    StrongRef<ast::StringTemplateAST> _currentAST;

    ::std::deque<StrongRef<ast::StringTemplateBeginLoopAST>> _loopStack;
    ::std::deque<IfFrame> _ifStack;
    i32 _repairIfTarget;
public:
    StringTemplateRepairVisitor() noexcept;
    ~StringTemplateRepairVisitor() noexcept;

    void visit(ast::StringTemplateAST* ast) noexcept override
    { IStringTemplateVisitor::visit(ast); }

    template<typename _T, ::std::enable_if_t<::std::is_base_of_v<ast::StringTemplateAST, _T>, int>* = nullptr>
    void visit(StrongRef<_T>& ast) noexcept
    {
        _currentAST = ast;
        visit(ast.get());
    }
    
    template<typename _T, ::std::enable_if_t<::std::is_base_of_v<ast::StringTemplateAST, _T>, int>* = nullptr>
    void visit(WeakRef<_T>& ast) noexcept
    { visit(ast.get()); }

    void visit(ast::StringTemplateAST& ast) noexcept override
    { IStringTemplateVisitor::visit(ast); }

    void visit(ast::StringTemplateSequenceAST& ast) noexcept override
    { IStringTemplateVisitor::visit(ast); }

    void visit(ast::StringTemplateRootAST& ast) noexcept override
    { IStringTemplateVisitor::visit(ast); }

    void visit(ast::StringTemplateExprAST& ast) noexcept override
    { IStringTemplateVisitor::visit(ast); }

    void visit(ast::StringTemplateWildExprAST& ast) noexcept override
    { IStringTemplateVisitor::visit(ast); }

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
};

} } }
