#include <clang/Frontend/CompilerInstance.h>
#include <clang/Basic/Diagnostic.h>
#include "reflection/processing/ReflectionASTWalker.hpp"
#include "reflection/Class.hpp"
#include "reflection/processing/TagPreProcessor.hpp"

namespace tau { namespace reflection { namespace processing { 

::std::unique_ptr<clang::ASTConsumer> ReflectionASTWalkerAction::CreateASTConsumer(clang::CompilerInstance& CI, llvm::StringRef)
{
    return ::std::make_unique<PropertyASTConsumer>(_classTags, _propertyTags, _functionTags, _classes, CI, &CI.getASTContext());
}

static void reportInvalidClass(::clang::DiagnosticsEngine& diagEngine, ::clang::SourceLocation propLoc, ::clang::SourceLocation declLoc, ::clang::SourceLocation classLoc, unsigned int selectType, unsigned propDiagID, unsigned declDiagID) noexcept;
static void reportTagOnNonMember(::clang::DiagnosticsEngine& diagEngine, ::clang::SourceLocation tagLoc, ::clang::SourceLocation declLoc, unsigned selectType, ::clang::NamedDecl* decl, unsigned declDiagID, unsigned tagDiagID) noexcept;

bool ReflectionASTVisitor::VisitFieldDecl(::clang::FieldDecl* const field) noexcept
{
    if(_propertyTags.empty())
    { return true; }

    Ref<TagDeclaration> tagDecl = _propertyTags.front();

    {
        const int ret = validateTag(tagDecl, field, field->getParent(), TagType::Property);
        switch(ret)
        {
            case 0: return true;
            case 1: return false;
            case 2:
            default: break;
        }
    }

    _propertyTags.pop_front();

    // Ensure there aren't multiple property tags before this field.
    if(!_propertyTags.empty() && !validateDoubleTag(tagDecl, _propertyTags.front(), field, TagType::Property))
    { return false; }

    _currentClass->properties().emplace_back(tagDecl, _currentClass, field->getNameAsString().c_str(), field->getType().getAsString().c_str());

    return true;
}

bool ReflectionASTVisitor::VisitCXXMethodDecl(::clang::CXXMethodDecl* const method) noexcept
{
    if(_functionTags.empty())
    { return true; }
    
    Ref<TagDeclaration> tagDecl = _functionTags.front();
    
    {
        const int ret = validateTag(tagDecl, method, method->getParent(), TagType::Function);
        switch(ret)
        {
            case 0: return true;
            case 1: return false;
            case 2:
            default: break;
        }
    }

    _functionTags.pop_front();

    // Ensure there aren't multiple function tags before this field.
    if(!_functionTags.empty() && !validateDoubleTag(tagDecl, _functionTags.front(), method, TagType::Function))
    { return false; }

    _currentClass->functions().emplace_back(tagDecl, _currentClass, method->getNameAsString().c_str());

    return true;
}

bool ReflectionASTVisitor::VisitVarDecl(::clang::VarDecl* const var) noexcept
{
    if(_propertyTags.empty())
    { return true; }

    Ref<TagDeclaration> propDecl = _propertyTags.front();
    
    const ::clang::SourceLocation tagLoc = propDecl->range().getBegin();
    const ::clang::SourceLocation declLoc = var->getBeginLoc();

    if(_ctx->getSourceManager().isBeforeInTranslationUnit(tagLoc, declLoc))
    {
        reportTagOnNonMember(_ctx->getDiagnostics(), tagLoc, declLoc, 0, var, _propOnNonMemberDiagID_decl, _propOnNonMemberDiagID_tag);
        return false;
    }

    return true;
}

bool ReflectionASTVisitor::VisitFunctionDecl(::clang::FunctionDecl* const function) noexcept
{
    /*   This function is invoked for both member and non-member
       functions, check to see if this is a member function and
       process it later. */
    if(::llvm::isa<::clang::CXXMethodDecl>(function))
    { return true; }

    if(_functionTags.empty())
    { return true; }

    Ref<TagDeclaration> tagDecl = _functionTags.front();
    
    const ::clang::SourceLocation tagLoc = tagDecl->range().getBegin();
    const ::clang::SourceLocation declLoc = function->getBeginLoc();

    if(_ctx->getSourceManager().isBeforeInTranslationUnit(tagLoc, declLoc))
    {
        reportTagOnNonMember(_ctx->getDiagnostics(), tagLoc, declLoc, 1, function, _propOnNonMemberDiagID_decl, _propOnNonMemberDiagID_tag);
        return false;
    }

    return true;
}

void ReflectionASTVisitor::initDiagnostics() noexcept
{
    ::clang::DiagnosticsEngine& diagEngine = _ctx->getDiagnostics();

    _invalidClassDiagID_tag = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Error, "%select{Properties|Functions}0 can only be declared in classes tagged with 'TAU_CLASS'.");
    _invalidClassDiagID_decl = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Note, "%select{Property field|Method declaration|Class declaration}0:");

    _multipleTagDiagID_decl = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Error, "Found multiple %select{property|function}0 tags before %select{field|method}0 %1.");
    _multipleTagDiagID_tag = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Note, "%select{First|Second}0 %select{property|function}1 tag:");

    _propOnNonMemberDiagID_decl = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Error, "Found %select{property|function}0 tag on %select{variable|function}0 %1. %select{Property|Function}0 tags can only be placed on class %select{fields|methods}0.");
    _propOnNonMemberDiagID_tag = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Note, "%select{Property|Function}0 tag:");
}

int ReflectionASTVisitor::validateTag(const Ref<TagDeclaration>& tag, ::clang::DeclaratorDecl* const decl, ::clang::RecordDecl* const parent, const TagType tagType) noexcept
{
    const ::clang::SourceLocation tagLoc = tag->range().getBegin();
    const ::clang::SourceLocation declLoc = decl->getBeginLoc();

    if(!_ctx->getSourceManager().isBeforeInTranslationUnit(tagLoc, declLoc))
    { return 0; }

    if(!_currentClass || _currentClass->name() != parent->getNameAsString().c_str())
    {
        const ::clang::SourceLocation parentLoc = parent->getLocation();

        // Property is not in a valid class.
        if(_classTags.empty())
        {
            reportInvalidClass(_ctx->getDiagnostics(), tagLoc, declLoc, parentLoc, tagType == TagType::Property ? 0 : 1, _invalidClassDiagID_tag, _invalidClassDiagID_decl);
            return 1;
        }

        const Ref<TagDeclaration>& classDecl = _classTags.front();

        const ::clang::SourceLocation classLoc = classDecl->range().getBegin();
        
        // Property is not in a valid class.
        if(!_ctx->getSourceManager().isBeforeInTranslationUnit(classLoc, parentLoc))
        {
            reportInvalidClass(_ctx->getDiagnostics(), tagLoc, declLoc, parentLoc, tagType == TagType::Property ? 0 : 1, _invalidClassDiagID_tag, _invalidClassDiagID_decl);
            return 1;
        }

        _currentClass = Ref<Class>(classDecl, parent->getNameAsString().c_str());
        _classes.push_back(_currentClass);
        _classTags.pop_back();
    }

    return 2;
}

bool ReflectionASTVisitor::validateDoubleTag(const Ref<TagDeclaration>& firstTag, const Ref<TagDeclaration>& secondTag, ::clang::DeclaratorDecl* const decl, const TagType tagType) noexcept
{
    const ::clang::SourceLocation firstTagLoc = firstTag->range().getBegin();
    const ::clang::SourceLocation secondTagLoc = secondTag->range().getBegin();
    const ::clang::SourceLocation declLoc = decl->getBeginLoc();

    if(_ctx->getSourceManager().isBeforeInTranslationUnit(secondTagLoc, declLoc))
    {
        ::clang::DiagnosticsEngine& diagEngine = _ctx->getDiagnostics();

        {
            const auto db = diagEngine.Report(decl->getLocation(), _multipleTagDiagID_decl);
            db.AddTaggedVal(tagType == TagType::Property ? 0 : 1, ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
            db.AddTaggedVal(reinterpret_cast<::std::intptr_t>(decl), ::clang::DiagnosticsEngine::ArgumentKind::ak_nameddecl);
        }

        {
            const auto db = diagEngine.Report(firstTagLoc, _multipleTagDiagID_tag);
            db.AddTaggedVal(0, ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
            db.AddTaggedVal(tagType == TagType::Property ? 0 : 1, ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
        }
        {
            const auto db = diagEngine.Report(secondTagLoc, _multipleTagDiagID_tag);
            db.AddTaggedVal(1, ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
            db.AddTaggedVal(tagType == TagType::Property ? 0 : 1, ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
        }

        return false;
    }

    return true;
}

void PropertyASTConsumer::HandleTranslationUnit(clang::ASTContext& Ctx)
{
    _visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
}
  
static void reportInvalidClass(::clang::DiagnosticsEngine& diagEngine, const ::clang::SourceLocation tagLoc, const ::clang::SourceLocation declLoc, const ::clang::SourceLocation classLoc, unsigned int selectType, const unsigned tagDiagID, const unsigned declDiagID) noexcept
{
    {
        diagEngine.Report(tagLoc, tagDiagID)
           .AddTaggedVal(selectType, ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
    }

    {
        diagEngine.Report(declLoc, declDiagID)
           .AddTaggedVal(selectType, ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
    }
    {
        diagEngine.Report(classLoc, declDiagID)
          .AddTaggedVal(2, ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
    }
}

static void reportTagOnNonMember(::clang::DiagnosticsEngine& diagEngine, const ::clang::SourceLocation tagLoc, const ::clang::SourceLocation declLoc, const unsigned selectType, ::clang::NamedDecl* const decl, const unsigned declDiagID, const unsigned tagDiagID) noexcept
{
    {
        const auto db = diagEngine.Report(declLoc, declDiagID);
        db.AddTaggedVal(selectType, ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
        db.AddTaggedVal(reinterpret_cast<::std::intptr_t>(decl), ::clang::DiagnosticsEngine::ArgumentKind::ak_nameddecl);
    }

    {
        diagEngine.Report(tagLoc, tagDiagID)
           .AddTaggedVal(selectType, ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
    }
}

} } }
