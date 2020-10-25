#include <llvm/Support/raw_ostream.h>
#include "attribs/GetAttribute.hpp"
#include "class/Class.hpp"

namespace tau { namespace attribs {

AttributeData GetPropertyAttribute::parseAttribute(const DynString& attribName, const clang::MacroArgs*, const clang::Token*& currentToken) const noexcept
{
    currentToken = getNextToken(currentToken);

    return AttributeData(this, nullptr, attribName);
}

void GetPropertyAttribute::generateBaseClass(::llvm::raw_fd_ostream& base) const noexcept
{
    base << 
        "public:\n"
        "    template<typename _T>\n"
        "    const _T* getProperty(const void* const object, const char* const propName) const noexcept\n"
        "    { return reinterpret_cast<const _T*>(_getProperty(object, propName)); }\n"
        "protected:\n"
        "    virtual const void* _getProperty(const void* object, const char* propName) const noexcept = 0;\n";
}

void GetPropertyAttribute::generateImplClass(::llvm::raw_fd_ostream& base, const Ref<ReflClass>& clazz) const noexcept
{
    base << 
        "        public:                                                                                                                    \\\n"
        "            template<typename _T>                                                                                                  \\\n"
        "            const _T* getProperty(const " << clazz->name() << "* const object, const char* const propName) const noexcept          \\\n"
        "            { return reinterpret_cast<const _T*>(getPropertyImpl(object, propName)); }                                             \\\n"
        "        protected:                                                                                                                 \\\n"
        "            const void* _getProperty(const void* const object, const char* const propName) const noexcept override                 \\\n"
        "            { return getPropertyImpl(reinterpret_cast<const " << clazz->name() << "*>(object), propName); }                        \\\n"
        "                                                                                                                                   \\\n"
        "            const void* getPropertyImpl(const " << clazz->name() << "* const object, const char* const propName) const noexcept    \\\n"
        "            {                                                                                                                      \\\n";

    for(uSys i = 0; i < clazz->properties().size(); ++i)
    {
        if(!clazz->properties()[i]->declaration()->hasAttribute("get"))
        { continue; }

        base <<
            "                if(::std::strcmp(propName, \"" << clazz->properties()[i]->name() << "\") == 0) \\\n"
            "                { return &object->" << clazz->properties()[i]->name() << "; }                  \\\n";
    }

    base << 
        "                return nullptr;                                                                                                    \\\n"
        "            }                                                                                                                      \\\n";
}

}}
