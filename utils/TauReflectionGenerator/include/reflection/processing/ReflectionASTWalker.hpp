#pragma once

#include <clang/Frontend/FrontendAction.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <deque>
#include <vector>
#include "reflection/Class.hpp"

namespace tau { namespace reflection { namespace processing {

enum class TagType;

class ReflectionASTWalkerAction : public ::clang::ASTFrontendAction
{
private:
    TagDeclQueue& _classTags;
    TagDeclQueue& _propertyTags;
    TagDeclQueue& _functionTags;
    ClassList& _classes;
public:
    ReflectionASTWalkerAction(TagDeclQueue& classTags, TagDeclQueue& propertyTags, TagDeclQueue& functionTags, ClassList& classes) noexcept
        : _classTags(classTags)
        , _propertyTags(propertyTags)
        , _functionTags(functionTags)
        , _classes(classes)
    { }
protected:
    ::std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& CI, llvm::StringRef InFile) override;
};

class ReflectionASTVisitor : public ::clang::RecursiveASTVisitor<ReflectionASTVisitor>
{
private:
    TagDeclQueue& _classTags;
    TagDeclQueue& _propertyTags;
    TagDeclQueue& _functionTags;
    ClassList& _classes;
    ::clang::CompilerInstance& _compiler;
    ::clang::ASTContext* _ctx;

    Ref<Class> _currentClass;

    unsigned _invalidClassDiagID_tag;
    unsigned _invalidClassDiagID_decl;

    unsigned _multipleTagDiagID_decl;
    unsigned _multipleTagDiagID_tag;

    unsigned _propOnNonMemberDiagID_decl;
    unsigned _propOnNonMemberDiagID_tag;
public:
    ReflectionASTVisitor(TagDeclQueue& classTags, TagDeclQueue& propertyTags, TagDeclQueue& functionTags, ClassList& classes, ::clang::CompilerInstance& compiler, ::clang::ASTContext* const ctx) noexcept
        : _classTags(classTags)
        , _propertyTags(propertyTags)
        , _functionTags(functionTags)
        , _classes(classes)
        , _compiler(compiler)
        , _ctx(ctx)
        , _currentClass(nullptr)
    { initDiagnostics(); }

    bool VisitFieldDecl(::clang::FieldDecl* field) noexcept;
    bool VisitCXXMethodDecl(::clang::CXXMethodDecl* method) noexcept;

    /**
     *   Used to detect when a reflection tag has been placed on a
     * non-member variable.
     */
    bool VisitVarDecl(::clang::VarDecl* var) noexcept;

    /**
     *   Used to detect when a reflection tag has been placed on a
     * non-member function.
     */
    bool VisitFunctionDecl(::clang::FunctionDecl* function) noexcept;
private:
    void initDiagnostics() noexcept;

    int validateTag(const Ref<TagDeclaration>& tag, ::clang::DeclaratorDecl* decl, ::clang::RecordDecl* parent, TagType tagType) noexcept;
    bool validateDoubleTag(const Ref<TagDeclaration>& firstTag, const Ref<TagDeclaration>& secondTag, ::clang::DeclaratorDecl* decl, TagType tagType) noexcept;
};

class PropertyASTConsumer : public ::clang::ASTConsumer
{
private:
    ReflectionASTVisitor _visitor;
public:
    PropertyASTConsumer(TagDeclQueue& classTags, TagDeclQueue& propertyTags, TagDeclQueue& functionTags, ClassList& classes, ::clang::CompilerInstance& compiler, ::clang::ASTContext* const ctx) noexcept
        : _visitor(classTags, propertyTags, functionTags, classes, compiler, ctx)
    { }

    void HandleTranslationUnit(clang::ASTContext& Ctx) override;
};

} } }
