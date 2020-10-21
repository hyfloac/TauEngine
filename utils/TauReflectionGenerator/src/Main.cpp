#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Lex/MacroInfo.h>
#include <clang/Lex/MacroArgs.h>
#include <iostream>
#include <deque>
#include <vector>
#include <fstream>
#include <NumTypes.hpp>

#include "FrontendFactoryHelper.hpp"

struct PropertyIndex final
{
    clang::SourceLocation location;
    std::vector<clang::Token> tokens;
    std::string varName;
    
    bool genGet = false;
    bool genSet = false;

    PropertyIndex(clang::SourceLocation _location)
        : location(_location)
    { }
};

struct PropertyData final
{
    PropertyIndex index;
    std::string className;

    PropertyData(const PropertyIndex& _index, const std::string& _className) noexcept
        : index(_index)
        , className(_className)
    { }
};

static void printGeneratedHeader(std::vector<PropertyData>& finalizedProperties);

class FindNamedClassVisitor : public clang::RecursiveASTVisitor<FindNamedClassVisitor>
{
private:
    std::deque<PropertyIndex>& _properties;
    std::vector<PropertyData>& _finalizedProperties;
    clang::ASTContext* _context;
public:
    FindNamedClassVisitor(std::deque<PropertyIndex>& properties, std::vector<PropertyData>& finalizedProperties, clang::ASTContext* const context)
        : _properties(properties)
        , _finalizedProperties(finalizedProperties)
        , _context(context)
    { }

    bool VisitCXXRecordDecl(clang::CXXRecordDecl* Declaration)
    {
        return true;
    }

    bool VisitFieldDecl(clang::FieldDecl* Declaration)
    {
        if(_properties.empty())
        { return true; }

        PropertyIndex& prop = _properties.front();

        const clang::SourceLocation propLoc = prop.location;
        const clang::SourceLocation declLoc = Declaration->getBeginLoc();

        // if(_context->getFullLoc(prop.location) < _context->getFullLoc(Declaration->getBeginLoc()))
        if(_context->getSourceManager().isBeforeInTranslationUnit(propLoc, declLoc))
        {
            llvm::outs() << "Found property in AST.\n"
                         << Declaration->getType().getAsString() << " " << Declaration->getName() << '\n'
                         << "In class " << Declaration->getParent()->getDeclName() << '\n';

            prop.varName = Declaration->getName().str();


            _finalizedProperties.emplace_back(::std::move(prop), Declaration->getParent()->getDeclName().getAsString());

            _properties.pop_front();
        }

        return true;
    }
};

class FindNamedClassConsumer : public clang::ASTConsumer
{
private:
    FindNamedClassVisitor _visitor;
public:
    FindNamedClassConsumer(std::deque<PropertyIndex>& properties, std::vector<PropertyData>& finalizedProperties, clang::CompilerInstance& compiler, clang::ASTContext* const context)
        : _visitor(properties, finalizedProperties, context)
    { }

    void HandleTranslationUnit(clang::ASTContext& Ctx) override
    {
        _visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
    }
};

class FindNamedClassAction : public clang::ASTFrontendAction
{
private:
    std::deque<PropertyIndex>& _properties;
    std::vector<PropertyData>& _finalizedProperties;
public:
    FindNamedClassAction(std::deque<PropertyIndex>& properties, std::vector<PropertyData>& finalizedProperties)
        : _properties(properties)
        , _finalizedProperties(finalizedProperties)
    { }

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& CI, llvm::StringRef InFile) override
    {
        return std::unique_ptr<clang::ASTConsumer>(new FindNamedClassConsumer(_properties, _finalizedProperties, CI, &CI.getASTContext()));
    }
};

class TestPreprocessorCallback : public clang::PPCallbacks
{
private:
    std::deque<PropertyIndex>& _properties;
public:
    TestPreprocessorCallback(std::deque<PropertyIndex>& properties)
        : _properties(properties)
    { }

    void MacroExpands(const clang::Token& MacroNameTok, const clang::MacroDefinition& MD, clang::SourceRange Range, const clang::MacroArgs* Args) override
    {
        if(!MacroNameTok.isAnyIdentifier())
        {
            llvm::outs() << "Macro found, but it is not an identifier.\n";
        }

        // llvm::outs() << "Macro '" << MacroNameTok.getIdentifierInfo()->getName() << "' expanded\n";

        if(MacroNameTok.getIdentifierInfo()->getName() != "TAU_PROPERTY")
        {
            return;
        }

        PropertyIndex index(Range.getBegin());

        if(Args->isVarargsElidedUse())
        {
            _properties.push_back(::std::move(index));
            return;
        }
        
        index.genGet = false;
        index.genSet = false;
        
        const clang::Token* token = Args->getUnexpArgument(static_cast<unsigned>(0));

        for(; token->isNot(clang::tok::eof); ++token)
        {
            if(!token->isAnyIdentifier())
            { continue; }

            if(token->getIdentifierInfo()->getName().str() == "get")
            {
                index.genGet = true;
            }
            else if(token->getIdentifierInfo()->getName() == "set")
            {
                index.genSet = true;
            }
        }

        _properties.push_back(::std::move(index));
    }
};

class TestPreprocessorAction : public clang::PreprocessorFrontendAction
{
private:
    std::deque<PropertyIndex>& _properties;
public:
    TestPreprocessorAction(std::deque<PropertyIndex>& properties)
        : _properties(properties)
    { }
protected:
    void ExecuteAction() override
    {
        clang::Preprocessor &PP = getCompilerInstance().getPreprocessor();
        
        PP.addPPCallbacks(std::make_unique<TestPreprocessorCallback>(_properties));

        // Ignore unknown pragmas.
        PP.IgnorePragmas();

        clang::Token Tok;
        // Start parsing the specified input file.
        PP.EnterMainSourceFile();
        do {
          PP.Lex(Tok);
        } while (Tok.isNot(clang::tok::eof));
    }
};

int main(int argCount, const char* args[])
{
    llvm::cl::OptionCategory category("tau-refl-gen cat");

    {
        std::error_code ec;
        llvm::raw_fd_ostream stream(llvm::StringRef(R"(D:\TauEngine\utils\TauReflectionGenerator\test\Test.generated.hpp)"), ec);
        stream << "#pragma once \n";
        stream << "#include \"Base.hpp\" \n";
        stream << "#ifdef GENERATED_BODY \n";
        stream << "  #undef GENERATED_BODY \n";
        stream << "#endif \n";
        stream << "#define GENERATED_BODY()\n";
    }

    clang::tooling::CommonOptionsParser op(argCount, args, category);
    clang::tooling::ClangTool Tool(op.getCompilations(), op.getSourcePathList());

    llvm::outs() << "\n\n";

    std::deque<PropertyIndex> properties;
    std::vector<PropertyData> finalizedProperties;

    int result = Tool.run(clang::helperExt::newFrontendActionFactory<TestPreprocessorAction>(properties).get());
    if(result)
    { return result; }

    result = Tool.run(clang::helperExt::newFrontendActionFactory<FindNamedClassAction>(properties, finalizedProperties).get());

    if(finalizedProperties.empty())
    {
        llvm::outs() << "No properties found\n";
        return 0;
    }

    std::string firstClass = finalizedProperties[0].className;
    for(uSys i = 1; i < finalizedProperties.size(); ++i)
    {
        if(firstClass != finalizedProperties[i].className)
        {
            llvm::errs() << "Multiple classes found in file.\n";
            return -1;
        }
    }

    printGeneratedHeader(finalizedProperties);

    return result;
}

static void printGeneratedHeader(std::vector<PropertyData>& finalizedProperties)
{
    std::error_code ec;
    llvm::raw_fd_ostream stream(llvm::StringRef(R"(D:\TauEngine\utils\TauReflectionGenerator\test\Test.generated.hpp)"), ec);
    
    const std::string currentClass = finalizedProperties[0].className;

    stream << "#pragma once\n";
    stream << "\n";
    stream << "// Auto Generated - DO NOT EDIT\n";
    stream << "\n";
    stream << "#include \"Base.hpp\"\n";
    
    stream << "#ifdef GENERATED_BODY \n";
    stream << "  #undef GENERATED_BODY \n";
    stream << "#endif \n";
    stream << "\n";
    stream << "#define GENERATED_BODY() \\\n";
    stream << "    public: \\\n";
    stream << "        class TauClassImpl final : public ITauClass { \\\n";
    stream << "        public: \\\n";
    stream << "            TauClassImpl() noexcept = default; \\\n";
    stream << "            ~TauClassImpl() noexcept override = default; \\\n";
    stream << "            \\\n";
    stream << "            template<typename _T> \\\n";
    stream << "            _T* getProperty(" << currentClass << "* const object, const char* const propName) noexcept \\\n";
    stream << "            { return reinterpret_cast<_T*>(getProperty(object, propName)); } \\\n";
    stream << "            \\\n";
    stream << "            template<typename _T> \\\n";
    stream << "            const _T* getProperty(const " << currentClass << "* const object, const char* propName) noexcept \\\n";
    stream << "            { return reinterpret_cast<const _T*>(getProperty(object, propName)); } \\\n";
    stream << "            \\\n";
    stream << "            template<typename _T> \\\n";
    stream << "            void setProperty(" << currentClass << "* const object, const char* propName, const _T& value) noexcept \\\n";
    stream << "            { setProperty(object, propName, value); } \\\n";
    stream << "       protected: \\\n";
    stream << "            void* getProperty(void* const object, const char* const propName) noexcept override { return getProperty(reinterpret_cast<" << currentClass << "*>(object), propName); } \\\n";
    stream << "            const void* getProperty(const void* const object, const char* const propName) noexcept override { return getProperty(reinterpret_cast<const " << currentClass << "*>(object), propName); } \\\n";
    stream << "            \\\n";
    stream << "            void* getProperty(" << currentClass << "* object, const char* propName) noexcept { \\\n";

    bool reset = true;
    for(iSys i = 0; i < static_cast<iSys>(finalizedProperties.size()); ++i)
    {
        if(!reset || finalizedProperties[i].index.genSet)
        {
            stream << "                if(::std::strcmp(propName, \"" << finalizedProperties[i].index.varName << "\") == 0) { \\\n";
            stream << "                    return &object->" << finalizedProperties[i].index.varName << "; \\\n";
            stream << "                } \\\n";
        }

        if(i == static_cast<iSys>(finalizedProperties.size() - 1) && reset)
        {
            i = -1;
            reset = false;
            stream << "                return nullptr; \\\n";
            stream << "            } \\\n";
            stream << "            const void* getProperty(const " << currentClass << "* const object, const char* const propName) noexcept { \\\n";
        }
    }
    
    stream << "                return nullptr; \\\n";
    stream << "            } \\\n";
    stream << "        }; \\\n";
    stream << "        [[nodiscard]] static TauClassImpl& GetStaticClass() noexcept { \\\n";
    stream << "            static TauClassImpl tauClass; \\\n";
    stream << "            return tauClass; \\\n";
    stream << "        } \\\n";
    stream << "        [[nodiscard]] TauClassImpl& getClass() const noexcept { \\\n";
    stream << "            return GetStaticClass(); \\\n";
    stream << "        } \n";
}
