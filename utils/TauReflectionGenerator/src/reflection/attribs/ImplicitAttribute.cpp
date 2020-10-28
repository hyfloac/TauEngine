#include <llvm/Support/raw_ostream.h>
#include "reflection/attribs/ImplicitAttribute.hpp"
#include "reflection/Class.hpp"

namespace tau { namespace reflection { namespace attribs { 

AttributeData ImplicitAttribute::parseAttribute(const DynString& attribName, const ::clang::MacroArgs*, const ::clang::Token*& currentToken) const noexcept
{
    currentToken = getNextToken(currentToken);

    return AttributeData(this, nullptr, attribName);
}

void ImplicitAttribute::generateBaseTauClass(::llvm::raw_fd_ostream& base) const noexcept
{
    base <<
        "public:\n"
        "    virtual const char* const * getProperties(unsigned* count) const noexcept = 0;\n"
        "    virtual const char* const * getFunctions(unsigned* count) const noexcept = 0;\n"
        "    [[nodiscard]] virtual unsigned getPropertyIndex(const char* propName) const noexcept = 0;\n"
        "    [[nodiscard]] virtual unsigned getFunctionIndex(const char* funcName) const noexcept = 0;\n";
}

void ImplicitAttribute::generateImplTauClass(::llvm::raw_fd_ostream& base, const Ref<Class>& clazz) const noexcept
{
    base <<
        "        public:                                                                                            \\\n"
        "            const char* const * getProperties(unsigned* const count) const noexcept override               \\\n"
        "            {                                                                                              \\\n"
        "                static const char* _properties[] = {                                                       \\\n";

    for(uSys i = 0; i < clazz->properties().size(); ++i)
    {
        base <<
            "                    \"" << clazz->properties()[i]->name() << "\"";
        if(i < clazz->properties().size() - 1)
        {
            base << ", \\\n";
        }
        else
        {
            base << " \\\n";
        }
    }

    base <<
        "                };                                                                                         \\\n"
        "                *count = " << clazz->properties().size() << ";                                             \\\n"
        "                return _properties;                                                                        \\\n"
        "            }                                                                                              \\\n"
        "                                                                                                           \\\n"
        "            const char* const * getFunctions(unsigned* const count) const noexcept override                \\\n"
        "            {                                                                                              \\\n"
        "                static const char* _functions[] = {                                                        \\\n";
    
    for(uSys i = 0; i < clazz->functions().size(); ++i)
    {
        base <<
            "                    \"" << clazz->functions()[i]->name() << "\"";
        if(i < clazz->functions().size() - 1)
        {
            base << ", \\\n";
        }
        else
        {
            base << " \\\n";
        }
    }

    base <<
        "                };                                                                                         \\\n"
        "                *count = " << clazz->functions().size() << ";                                              \\\n"
        "                return _functions;                                                                         \\\n"
        "            }                                                                                              \\\n"
        "                                                                                                           \\\n"
        "            [[nodiscard]] unsigned getPropertyIndex(const char* const propName) const noexcept override    \\\n"
        "            {                                                                                              \\\n";

    for(uSys i = 0; i < clazz->properties().size(); ++i)
    {
        base <<
            "                if(::std::strcmp(propName, \"" << clazz->properties()[i]->name() << "\") == 0) \\\n"
            "                { return " << i << "; } \\\n";
    }

    base <<
        "                return static_cast<unsigned>(-1);                                                          \\\n"
        "            }                                                                                              \\\n"
        "                                                                                                           \\\n"
        "            [[nodiscard]] unsigned getFunctionIndex(const char* const funcName) const noexcept override    \\\n"
        "            {                                                                                              \\\n";

    for(uSys i = 0; i < clazz->functions().size(); ++i)
    {
        base <<
            "                if(::std::strcmp(funcName, \"" << clazz->functions()[i]->name() << "\") == 0) \\\n"
            "                { return " << i << "; } \\\n";
    }

    base <<
        "                return static_cast<unsigned>(-1);                                                          \\\n"
        "            }                                                                                              \\\n";

}

} } }
