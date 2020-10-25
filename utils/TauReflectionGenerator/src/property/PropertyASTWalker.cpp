#include <clang/Frontend/CompilerInstance.h>
#include <clang/Basic/Diagnostic.h>
#define TAU_NO_NULLABILITY
#define NOMINMAX
#include "class/Class.hpp"
#include "property/PropertyASTWalker.hpp"

namespace tau {

::std::unique_ptr<clang::ASTConsumer> PropertyASTWalkerAction::CreateASTConsumer(clang::CompilerInstance& CI, const llvm::StringRef InFile)
{
    return ::std::make_unique<PropertyASTConsumer>(_classTags, _propertyTags, _classes, CI, &CI.getASTContext());
}

static void reportInvalidClass(::clang::DiagnosticsEngine& diagEngine, ::clang::SourceLocation propLoc, ::clang::SourceLocation declLoc, ::clang::SourceLocation classLoc, unsigned propDiagID, unsigned declDiagID) noexcept;

bool PropertyASTVisitor::VisitFieldDecl(::clang::FieldDecl* const field) noexcept
{
    if(_propertyTags.empty())
    { return true; }

    Ref<TagDeclaration> propDecl = _propertyTags.front();

    const ::clang::SourceLocation propLoc = propDecl->range().getBegin();
    const ::clang::SourceLocation declLoc = field->getBeginLoc();

    if(!_ctx->getSourceManager().isBeforeInTranslationUnit(propLoc, declLoc))
    { return true; }

    if(!_currentClass || _currentClass->name() != field->getParent()->getNameAsString().c_str())
    {
        const ::clang::SourceLocation parentLoc = field->getParent()->getLocation();

        // Property is not in a valid class.
        if(_classTags.empty())
        {
            reportInvalidClass(_ctx->getDiagnostics(), propLoc, declLoc, parentLoc, _invalidClassDiagID_prop, _invalidClassDiagID_decl);
            return false;
        }

        const Ref<TagDeclaration>& classDecl = _classTags.front();

        const ::clang::SourceLocation classLoc = classDecl->range().getBegin();
        
        // Property is not in a valid class.
        if(!_ctx->getSourceManager().isBeforeInTranslationUnit(classLoc, parentLoc))
        {
            reportInvalidClass(_ctx->getDiagnostics(), propLoc, declLoc, parentLoc, _invalidClassDiagID_prop, _invalidClassDiagID_decl);
            return false;
        }

        _currentClass = Ref<ReflClass>(classDecl, field->getParent()->getNameAsString().c_str());
        _classes.push_back(_currentClass);
        _classTags.pop_back();
    }

    _propertyTags.pop_front();

    // Ensure there aren't multiple property tags before this field.
    if(!_propertyTags.empty())
    {
        Ref<TagDeclaration>& nextPropDecl = _propertyTags.front();
        const ::clang::SourceLocation nextPropLoc = nextPropDecl->range().getBegin();

        if(_ctx->getSourceManager().isBeforeInTranslationUnit(nextPropLoc, declLoc))
        {
            ::clang::DiagnosticsEngine& diagEngine = _ctx->getDiagnostics();

            diagEngine.Report(field->getLocation(), _multiplePropDiagID_var)
                .AddTaggedVal(reinterpret_cast<::std::intptr_t>(field), ::clang::DiagnosticsEngine::ArgumentKind::ak_nameddecl);

            diagEngine.Report(propLoc, _multiplePropDiagID_prop)
                .AddTaggedVal(0, ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
            diagEngine.Report(propLoc, _multiplePropDiagID_prop)
                .AddTaggedVal(1, ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);

            return false;
        }
    }

    _currentClass->properties().emplace_back(propDecl, _currentClass, field->getNameAsString().c_str(), field->getType().getAsString().c_str());

    return true;
}

bool PropertyASTVisitor::VisitVarDecl(::clang::VarDecl* const var) noexcept
{
    if(_propertyTags.empty())
    { return true; }

    Ref<TagDeclaration> propDecl = _propertyTags.front();
    
    const ::clang::SourceLocation propLoc = propDecl->range().getBegin();
    const ::clang::SourceLocation declLoc = var->getBeginLoc();

    if(_ctx->getSourceManager().isBeforeInTranslationUnit(propLoc, declLoc))
    {
        ::clang::DiagnosticsEngine& diagEngine = _ctx->getDiagnostics();

        diagEngine.Report(var->getLocation(), _propOnVarDiagID_var)
            .AddTaggedVal(reinterpret_cast<::std::intptr_t>(var), ::clang::DiagnosticsEngine::ArgumentKind::ak_nameddecl);

        diagEngine.Report(propLoc, _propOnVarDiagID_prop);

        return false;
    }

    return true;
}

void PropertyASTVisitor::initDiagnostics() noexcept
{
    ::clang::DiagnosticsEngine& diagEngine = _ctx->getDiagnostics();

    _invalidClassDiagID_prop = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Error, "Properties can only be declared in classes tagged with 'TAU_CLASS'.");
    _invalidClassDiagID_decl = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Note, "%select{Property field|Class declaration}0:");

    _multiplePropDiagID_var = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Error, "Found multiple property tags before field %0.");
    _multiplePropDiagID_prop = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Note, "%select{First|Second}0 property tag:");

    _propOnVarDiagID_var = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Error, "Found property tag on variable %0. Property tags can only be placed on class fields.");
    _propOnVarDiagID_prop = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Note, "Property tag:");
}

void PropertyASTConsumer::HandleTranslationUnit(clang::ASTContext& Ctx)
{
    _visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
}

    
static void reportInvalidClass(::clang::DiagnosticsEngine& diagEngine, const ::clang::SourceLocation propLoc, const ::clang::SourceLocation declLoc, const ::clang::SourceLocation classLoc, const unsigned propDiagID, const unsigned declDiagID) noexcept
{
    diagEngine.Report(propLoc, propDiagID);

    diagEngine.Report(declLoc, declDiagID)
        .AddTaggedVal(0, ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
    diagEngine.Report(classLoc, declDiagID)
        .AddTaggedVal(1, ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
}

}
