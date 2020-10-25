#pragma once

#include <clang/Frontend/FrontendAction.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <deque>
#include <vector>
#include "class/Class.hpp"

namespace tau {

class PropertyASTWalkerAction : public ::clang::ASTFrontendAction
{
private:
    TagDeclQueue& _classTags;
    TagDeclQueue& _propertyTags;
    ClassList& _classes;
public:
    PropertyASTWalkerAction(TagDeclQueue& classTags, TagDeclQueue& propertyTags, ClassList& classes) noexcept
        : _classTags(classTags)
        , _propertyTags(propertyTags)
        , _classes(classes)
    { }
protected:
    ::std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& CI, llvm::StringRef InFile) override;
};

class PropertyASTVisitor : public ::clang::RecursiveASTVisitor<PropertyASTVisitor>
{
private:
    TagDeclQueue& _classTags;
    TagDeclQueue& _propertyTags;
    ClassList& _classes;
    ::clang::CompilerInstance& _compiler;
    ::clang::ASTContext* _ctx;

    Ref<ReflClass> _currentClass;

    unsigned _invalidClassDiagID_prop;
    unsigned _invalidClassDiagID_decl;

    unsigned _multiplePropDiagID_var;
    unsigned _multiplePropDiagID_prop;

    unsigned _propOnVarDiagID_var;
    unsigned _propOnVarDiagID_prop;
public:
    PropertyASTVisitor(TagDeclQueue& classTags, TagDeclQueue& propertyTags, ClassList& classes, ::clang::CompilerInstance& compiler, ::clang::ASTContext* const ctx) noexcept
        : _classTags(classTags)
        , _propertyTags(propertyTags)
        , _classes(classes)
        , _compiler(compiler)
        , _ctx(ctx)
        , _currentClass(nullptr)
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
    PropertyASTConsumer(TagDeclQueue& classTags, TagDeclQueue& propertyTags, ClassList& classes, ::clang::CompilerInstance& compiler, ::clang::ASTContext* const ctx) noexcept
        : _visitor(classTags, propertyTags, classes, compiler, ctx)
    { }

    void HandleTranslationUnit(clang::ASTContext& Ctx) override;
};

}
