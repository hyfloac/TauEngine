#include <llvm/Support/raw_ostream.h>
#include "reflection/attribs/GetAttribute.hpp"
#include "reflection/Class.hpp"

namespace tau { namespace reflection { namespace attribs {

AttributeData GetPropertyAttribute::parseAttribute(const DynString& attribName, const ::clang::MacroArgs*, const ::clang::Token*& currentToken) const noexcept
{
    currentToken = getNextToken(currentToken);

    return AttributeData(this, nullptr, attribName);
}

void GetPropertyAttribute::generateBaseTauClass(::llvm::raw_fd_ostream& base) const noexcept
{
    base << 
        "public:\n"
        "    template<typename _T>\n"
        "    [[nodiscard]] const _T* getProperty(const void* const object, const char* const propName) const noexcept\n"
        "    { return reinterpret_cast<const _T*>(_getProperty(object, propName)); }\n"
        "\n"
        "    template<typename _T>\n"
        "    [[nodiscard]] const _T* getProperty(const void* const object, const int propIndex) const noexcept\n"
        "    { return reinterpret_cast<const _T*>(_getProperty(object, propIndex)); }\n"
        "protected:\n"
        "    [[nodiscard]] virtual const void* _getProperty(const void* object, const char* propName) const noexcept = 0;\n"
        "    [[nodiscard]] virtual const void* _getProperty(const void* object, unsigned propIndex) const noexcept = 0;\n";
}

void GetPropertyAttribute::generateImplTauClass(::llvm::raw_fd_ostream& base, const Ref<Class>& clazz) const noexcept
{
    base << 
        "        public:                                                                                                                                \\\n"
        "            template<typename _T>                                                                                                              \\\n"
        "            [[nodiscard]] const _T* getProperty(const " << clazz->name() << "* const object, const char* const propName) const noexcept        \\\n"
        "            { return reinterpret_cast<const _T*>(getPropertyImpl(object, propName)); }                                                         \\\n"
        "                                                                                                                                               \\\n"
        "            template<typename _T>                                                                                                              \\\n"
        "            [[nodiscard]] const _T* getProperty(const " << clazz->name() << "* const object, const unsigned propIndex) const noexcept          \\\n"
        "            { return reinterpret_cast<const _T*>(getPropertyImpl(object, propIndex)); }                                                        \\\n"
        "        protected:                                                                                                                             \\\n"
        "            [[nodiscard]] const void* _getProperty(const void* const object, const char* const propName) const noexcept override               \\\n"
        "            { return getPropertyImpl(reinterpret_cast<const " << clazz->name() << "*>(object), propName); }                                    \\\n"
        "                                                                                                                                               \\\n"
        "            [[nodiscard]] const void* _getProperty(const void* const object, const unsigned propIndex) const noexcept override                 \\\n"
        "            { return getPropertyImpl(reinterpret_cast<const " << clazz->name() << "*>(object), propIndex); }                                   \\\n"
        "                                                                                                                                               \\\n"
        "            [[nodiscard]] const void* getPropertyImpl(const " << clazz->name() << "* const object, const char* const propName) const noexcept  \\\n"
        "            {                                                                                                                                  \\\n";

    for(uSys i = 0; i < clazz->properties().size(); ++i)
    {
        if(!clazz->properties()[i]->declaration()->hasAttribute("get"))
        { continue; }

        base <<
            "                if(::std::strcmp(propName, \"" << clazz->properties()[i]->name() << "\") == 0) \\\n"
            "                { return &object->" << clazz->properties()[i]->name() << "; }                  \\\n";
    }

    base << 
        "                return nullptr;                                                                                                                \\\n"
        "            }                                                                                                                                  \\\n"
        "                                                                                                                                               \\\n"
        "            [[nodiscard]] const void* getPropertyImpl(const " << clazz->name() << "* const object, const unsigned propIndex) const noexcept    \\\n"
        "            {                                                                                                                                  \\\n"
        "                switch(propIndex)                                                                                                              \\\n"
        "                {                                                                                                                              \\\n";
    
    for(uSys i = 0; i < clazz->properties().size(); ++i)
    {
        if(!clazz->properties()[i]->declaration()->hasAttribute("get"))
        { continue; }

        base << 
            "                case " << i << ": return &object->" << clazz->properties()[i]->name() << "; \\\n";
    }

    base <<
        "                    default: return nullptr;                                                                                                   \\\n"
        "                }                                                                                                                              \\\n"
        "            }                                                                                                                                  \\\n";

}

} } }
