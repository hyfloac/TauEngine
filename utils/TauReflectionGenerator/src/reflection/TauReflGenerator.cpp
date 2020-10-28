#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CommonOptionsParser.h>

#include "reflection/processing/FrontendFactoryHelper.hpp"
#include "reflection/processing/ReflectionASTWalker.hpp"
#include "reflection/processing/TagPreProcessor.hpp"
#include "reflection/processing/HeaderGenerator.hpp"
#include "reflection/attribs/GetAttribute.hpp"
#include "reflection/attribs/SetAttribute.hpp"
#include "reflection/attribs/ImplicitAttribute.hpp"

static ::llvm::cl::OptionCategory tauReflCategory("tau-reflection-generator options");

static ::llvm::cl::opt<::std::string> baseHeaderLoc("base-loc", ::llvm::cl::desc("The path to where the base header should be placed"), ::llvm::cl::cat(tauReflCategory));
static ::llvm::cl::opt<::std::string> outHeader("o", ::llvm::cl::desc("The path and name of the output file"), ::llvm::cl::cat(tauReflCategory));

int main(int argCount, const char* args[])
{
    static ::llvm::cl::extrahelp commonHelp(::clang::tooling::CommonOptionsParser::HelpMessage);
    UNUSED(commonHelp);

    tau::reflection::AttributeManager::registerAttribute<tau::reflection::attribs::ImplicitAttribute>("__implicit_base_0__");
    tau::reflection::AttributeManager::registerAttribute<tau::reflection::attribs::GetPropertyAttribute>("get");
    tau::reflection::AttributeManager::registerAttribute<tau::reflection::attribs::SetPropertyAttribute>("set");

    ::clang::tooling::CommonOptionsParser op(argCount, args, tauReflCategory);

    if(!baseHeaderLoc.getValue().empty())
    {
        StringBuilder pathBuilder(baseHeaderLoc.getValue().length() + 9);
        pathBuilder.append(baseHeaderLoc.getValue().c_str());

        if(pathBuilder[pathBuilder.length() - 1] != '\\' && pathBuilder[pathBuilder.length() - 1] != '/')
        { pathBuilder.append('/'); }

        pathBuilder.append("Base.hpp");

        tau::reflection::processing::BaseGenerator baseGen(pathBuilder.toString());
        baseGen.generate();
    }

    if(outHeader.getValue().empty())
    { return 0; }

    {
        tau::reflection::processing::HeaderGenerator headerGen(outHeader.getValue().c_str());
        headerGen.generateBegin();
        headerGen.generateDummy();
    }

    ::clang::tooling::ClangTool tool(op.getCompilations(), op.getSourcePathList());

    tau::reflection::TagDeclQueue classTags;
    tau::reflection::TagDeclQueue propertyTags;
    tau::reflection::TagDeclQueue functionTags;
    tau::reflection::ClassList classes;

    int result = tool.run(::clang::helperExt::newFrontendActionFactory<::tau::reflection::processing::TagPreProcessorAction>(classTags, propertyTags, functionTags).get());
    if(result)
    { return result; }

    result = tool.run(::clang::helperExt::newFrontendActionFactory<::tau::reflection::processing::ReflectionASTWalkerAction>(classTags, propertyTags, functionTags, classes).get());
    if(result)
    { return result; }

    {
        tau::reflection::processing::HeaderGenerator headerGen(outHeader.getValue().c_str());
        headerGen.generateBegin();
        for(const auto& clazz : classes)
        {
            headerGen.generateClassBody(clazz);
        }
    }

    return result;
}
