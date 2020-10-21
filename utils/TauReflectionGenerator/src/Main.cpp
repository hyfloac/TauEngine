#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Frontend/CompilerInstance.h>

#include "PropertyASTWalker.hpp"
#include "PropertyPreProcessor.hpp"
#include "attribs/GetAttribute.hpp"
#include "attribs/SetAttribute.hpp"
#include "FrontendFactoryHelper.hpp"

static void printGeneratedHeader(tau::PropertyList& properties) noexcept;

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

    tau::PropertyAttributeManager::registerAttribute<tau::attribs::GetPropertyAttribute>("get");
    tau::PropertyAttributeManager::registerAttribute<tau::attribs::SetPropertyAttribute>("set");

    tau::PropertyDeclQueue propertyTags;
    tau::PropertyList properties;

    int result = Tool.run(::clang::helperExt::newFrontendActionFactory<::tau::PropertyPreProcessorAction>(propertyTags).get());
    if(result)
    { return result; }

    result = Tool.run(::clang::helperExt::newFrontendActionFactory<::tau::PropertyASTWalkerAction>(propertyTags, properties).get());
    if(result)
    { return result; }

    if(properties.empty())
    {
        llvm::outs() << "No properties found\n";
        return 0;
    }

    const DynString& firstClass = properties[0].className();
    for(uSys i = 1; i < properties.size(); ++i)
    {
        if(firstClass != properties[i].className())
        {
            llvm::errs() << "Multiple classes found in file.\n";
            return -1;
        }
    }

    printGeneratedHeader(properties);

    return result;
}

enum class GenStage
{
    Get = 0,
    GetConst,
    Set,
    END
};

static void printGeneratedHeader(tau::PropertyList& properties) noexcept
{
    std::error_code ec;
    llvm::raw_fd_ostream stream(llvm::StringRef(R"(D:\TauEngine\utils\TauReflectionGenerator\test\Test.generated.hpp)"), ec);
    
    const DynString& currentClass = properties[0].className();

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
    stream << "            { return reinterpret_cast<_T*>(getPropertyImpl(object, propName)); } \\\n";
    stream << "            \\\n";
    stream << "            template<typename _T> \\\n";
    stream << "            const _T* getProperty(const " << currentClass << "* const object, const char* propName) noexcept \\\n";
    stream << "            { return reinterpret_cast<const _T*>(getPropertyImpl(object, propName)); } \\\n";
    stream << "            \\\n";
    stream << "            template<typename _T> \\\n";
    stream << "            void setProperty(" << currentClass << "* const object, const char* propName, const _T& value) noexcept \\\n";
    stream << "            { setPropertyImpl(object, propName, &value); } \\\n";
    stream << "       protected: \\\n";
    stream << "            void* _getProperty(void* const object, const char* const propName) const noexcept override { return getPropertyImpl(reinterpret_cast<" << currentClass << "*>(object), propName); } \\\n";
    stream << "            const void* _getProperty(const void* const object, const char* const propName) const noexcept override { return getPropertyImpl(reinterpret_cast<const " << currentClass << "*>(object), propName); } \\\n";
    stream << "            void _setProperty(void* const object, const char* const propName, const void* const value) const noexcept override { return setPropertyImpl(reinterpret_cast<" << currentClass << "*>(object), propName, value); } \\\n";
    stream << "            \\\n";
    stream << "            void* getPropertyImpl(" << currentClass << "* const object, const char* const propName) const noexcept { \\\n";

    GenStage stage = GenStage::Get;
    for(iSys i = 0; i < static_cast<iSys>(properties.size()); ++i)
    {
        stream << "                if(::std::strcmp(propName, \"" << properties[i].name() << "\") == 0) { \\\n";
        if(stage == GenStage::GetConst || (stage == GenStage::Get && properties[i].declaration()->hasAttribute("set")))
        {
            stream << "                    return &object->" << properties[i].name() << "; \\\n";
        }
        else if(stage == GenStage::Set && properties[i].declaration()->hasAttribute("set"))
        {
            stream << "                    object->" << properties[i].name() << " = *reinterpret_cast<" << properties[i].typeName() << " const *>(value); \\\n";
        }
        stream << "                } \\\n";

        if(i == static_cast<iSys>(properties.size() - 1) && stage < GenStage::END)
        {
            i = -1;

            if(stage == GenStage::Get)
            {
                stream << "                return nullptr; \\\n";
                stream << "            } \\\n";
                stream << "            const void* getPropertyImpl(const " << currentClass << "* const object, const char* const propName) const noexcept { \\\n";
            }
            else if(stage == GenStage::GetConst)
            {
                stream << "                return nullptr; \\\n";
                stream << "            } \\\n";
                stream << "            void setPropertyImpl(" << currentClass << "* const object, const char* const propName, const void* const value) const noexcept { \\\n";
            }

            ++reinterpret_cast<int&>(stage);
        }
    }
    
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
