#include "PropertyASTWalker.hpp"
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Basic/Diagnostic.h>

namespace tau {

::std::unique_ptr<clang::ASTConsumer> PropertyASTWalkerAction::CreateASTConsumer(clang::CompilerInstance& CI, const llvm::StringRef InFile)
{
    return ::std::make_unique<PropertyASTConsumer>(_propertyTags, _properties, CI, &CI.getASTContext());
}

bool PropertyASTVisitor::VisitFieldDecl(::clang::FieldDecl* const field) noexcept
{
    if(_propertyTags.empty())
    { return true; }

    Ref<PropertyDeclaration> propDecl = _propertyTags.front();

    const ::clang::SourceLocation propLoc = propDecl->range().getBegin();
    const ::clang::SourceLocation declLoc = field->getBeginLoc();

    if(_ctx->getSourceManager().isBeforeInTranslationUnit(propLoc, declLoc))
    {
        _propertyTags.pop_front();

        // Ensure there aren't multiple property tags before this field.
        if(!_propertyTags.empty())
        {
            Ref<PropertyDeclaration>& nextPropDecl = _propertyTags.front();
            const ::clang::SourceLocation nextPropLoc = nextPropDecl->range().getBegin();

            if(_ctx->getSourceManager().isBeforeInTranslationUnit(nextPropLoc, declLoc))
            {
                ::clang::DiagnosticsEngine& diagEngine = _ctx->getDiagnostics();

                diagEngine.Report(field->getLocation(), _multiplePropDiagID_var)
                    .AddTaggedVal(reinterpret_cast<::std::intptr_t>(field), ::clang::DiagnosticsEngine::ArgumentKind::ak_nameddecl);

                diagEngine.Report(propLoc, _multiplePropDiagID_prop0);
                diagEngine.Report(nextPropLoc, _multiplePropDiagID_prop1);

                return false;
            }
        }

        _properties.emplace_back(propDecl, field->getNameAsString().c_str(), field->getType().getAsString().c_str(), field->getParent()->getDeclName().getAsString().c_str());
    }

    return true;
}

bool PropertyASTVisitor::VisitVarDecl(::clang::VarDecl* const var) noexcept
{
    if(_propertyTags.empty())
    { return true; }

    Ref<PropertyDeclaration> propDecl = _propertyTags.front();
    
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

    _multiplePropDiagID_var = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Error, "Found multiple property tags before field %0.");
    _multiplePropDiagID_prop0 = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Note, "First property tag:");
    _multiplePropDiagID_prop1 = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Note, "Second property tag:");

    _propOnVarDiagID_var = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Error, "Found property tag on variable %0. Property tags can only be placed on class fields.");
    _propOnVarDiagID_prop = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Note, "Property tag:");
}

void PropertyASTConsumer::HandleTranslationUnit(clang::ASTContext& Ctx)
{
    _visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
}

}
