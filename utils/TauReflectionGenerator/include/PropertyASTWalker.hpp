#pragma once

#include <clang/Frontend/FrontendAction.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <deque>
#include <vector>
#include "Property.hpp"

namespace tau {

class PropertyASTWalkerAction : public ::clang::ASTFrontendAction
{
private:
    PropertyDeclQueue& _propertyTags;
    PropertyList& _properties;
public:
    PropertyASTWalkerAction(PropertyDeclQueue& propertyTags, PropertyList& properties) noexcept
        : _propertyTags(propertyTags)
        , _properties(properties)
    { }
protected:
    ::std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& CI, llvm::StringRef InFile) override;
};

class PropertyASTVisitor : public ::clang::RecursiveASTVisitor<PropertyASTVisitor>
{
private:
    PropertyDeclQueue& _propertyTags;
    PropertyList& _properties;
    ::clang::CompilerInstance& _compiler;
    ::clang::ASTContext* _ctx;

    unsigned _multiplePropDiagID_var;
    unsigned _multiplePropDiagID_prop0;
    unsigned _multiplePropDiagID_prop1;

    unsigned _propOnVarDiagID_var;
    unsigned _propOnVarDiagID_prop;
public:
    PropertyASTVisitor(PropertyDeclQueue& propertyTags, PropertyList& properties, ::clang::CompilerInstance& compiler, ::clang::ASTContext* const ctx) noexcept
        : _propertyTags(propertyTags)
        , _properties(properties)
        , _compiler(compiler)
        , _ctx(ctx)
        , _multiplePropDiagID_var(0)
        , _multiplePropDiagID_prop0(0)
        , _multiplePropDiagID_prop1(0)
        , _propOnVarDiagID_var(0)
        , _propOnVarDiagID_prop(0)
    { initDiagnostics(); }

    bool VisitFieldDecl(::clang::FieldDecl* field) noexcept;

    bool VisitVarDecl(::clang::VarDecl* var) noexcept;
private:
    void initDiagnostics() noexcept;
};

class PropertyASTConsumer : public ::clang::ASTConsumer
{
private:
    PropertyASTVisitor _visitor;
public:
    PropertyASTConsumer(PropertyDeclQueue& propertyTags, PropertyList& properties, ::clang::CompilerInstance& compiler, ::clang::ASTContext* const ctx) noexcept
        : _visitor(propertyTags, properties, compiler, ctx)
    { }

    void HandleTranslationUnit(clang::ASTContext& Ctx) override;
};

}
