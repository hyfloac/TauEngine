#include "reflection/processing/HeaderGenerator.hpp"
#include "reflection/Attribute.hpp"
#include "reflection/Class.hpp"

namespace tau { namespace reflection { namespace processing { 

void BaseGenerator::generate() noexcept
{
    printHeaderBegin();
    printBasicMacros();
    printClass();
}

void BaseGenerator::printHeaderBegin() noexcept
{
    _header << 
        "#pragma once\n"
        "\n"
        "// Auto Generated - DO NOT EDIT\n"
        "\n";
}

void BaseGenerator::printBasicMacros() noexcept
{
    _header << 
        "#define TAU_CLASS(...)\n"
        "#define TAU_PROPERTY(...)\n"
        "#define TAU_FUNCTION(...)\n"
        "\n";
}

void BaseGenerator::printClass() noexcept
{
    _header << 
        "class ITauClass\n"
        "{\n"
        "protected:\n"
        "    ITauClass() noexcept = default;\n"
        "\n"
        "    ITauClass(const ITauClass& copy) noexcept = default;\n"
        "    ITauClass(ITauClass&& move) noexcept = default;\n"
        "\n"
        "    ITauClass& operator=(const ITauClass& copy) noexcept = default;\n"
        "    ITauClass& operator=(ITauClass&& move) noexcept = default;\n"
        "public:\n"
        "    virtual ~ITauClass() noexcept = default;\n";

    for(const auto& handler : AttributeManager::getAttributes())
    {
        handler.second->generateBaseTauClass(_header);
    }

    _header << "};\n";
}

void HeaderGenerator::generateDummy() noexcept
{
    printHeaderBegin();
    printDummyMacros();
}

void HeaderGenerator::generateBegin() noexcept
{
    printHeaderBegin();
    printBasicMacros();
}

void HeaderGenerator::generateClassBody(const Ref<Class>& clazz) noexcept
{
    _header << 
        "#ifdef _TAU_GENERATED_BODY_" << clazz->name() << "\n"
        "  #undef _TAU_GENERATED_BODY_" << clazz->name() << "\n"
        "#endif\n"
        "\n"
        "#define _TAU_GENERATED_BODY_" << clazz->name() << "() \\\n"
        "    public:                                                                        \\\n"
        "        class TauClassImpl final : public ITauClass                                \\\n"
        "        {                                                                          \\\n"
        "        public:                                                                    \\\n"
        "            TauClassImpl() noexcept = default;                                     \\\n"
        "            ~TauClassImpl() noexcept override = default;                           \\\n"
        "                                                                                   \\\n"
        "            TauClassImpl(const TauClassImpl& copy) noexcept = default;             \\\n"
        "            TauClassImpl(TauClassImpl&& move) noexcept = default;                  \\\n"
        "                                                                                   \\\n"
        "            TauClassImpl& operator=(const TauClassImpl& copy) noexcept = default;  \\\n"
        "            TauClassImpl& operator=(TauClassImpl&& move) noexcept = default;       \\\n";
    
    for(const auto& handler : AttributeManager::getAttributes())
    {
        handler.second->generateImplTauClass(_header, clazz);
    }

    _header << 
        "        };                                                                         \\\n";

    for(const auto& handler : AttributeManager::getAttributes())
    {
        handler.second->generateImplClass(_header, clazz);
    }

    _header <<
        "    [[nodiscard]] static TauClassImpl& GetStaticClass() noexcept                   \\\n"
        "    {                                                                              \\\n"
        "        static TauClassImpl tauClass;                                              \\\n"
        "        return tauClass;                                                           \\\n"
        "    }                                                                              \\\n"
        "    [[nodiscard]] TauClassImpl& getClass() const noexcept                          \\\n"
        "    { return GetStaticClass(); }                                                   \n"
        "// End _TAU_GENERATED_BODY_" << clazz->name() << "()\n"
        "\n";

}

void HeaderGenerator::printHeaderBegin() noexcept
{
    _header << 
        "#pragma once\n"
        "\n"
        "// Auto Generated - DO NOT EDIT\n"
        "\n"
        "#include \"Base.hpp\"\n"
        "#include <cstring>\n"
        "\n";
}

void HeaderGenerator::printBasicMacros() noexcept
{
    _header << 
        "#ifdef TAU_GENERATED_BODY \n"
        "  #undef TAU_GENERATED_BODY \n"
        "#endif \n"
        "\n"
        "#ifdef _TAU_CAT \n"
        "  #undef _TAU_CAT \n"
        "#endif \n"
        "\n"
        "#define _TAU_CAT(_A, _B) _A##_B\n"
        "#define TAU_GENERATED_BODY(_CLASS) _TAU_CAT(_TAU_GENERATED_BODY_, _CLASS)()\n"
        "\n";
}

void HeaderGenerator::printDummyMacros() noexcept
{
    _header <<
        "#ifdef TAU_GENERATED_BODY\n"
        "  #undef TAU_GENERATED_BODY\n"
        "#endif\n"
        "\n"
        "#define TAU_GENERATED_BODY(_CLASS) \n";
}

} } }
