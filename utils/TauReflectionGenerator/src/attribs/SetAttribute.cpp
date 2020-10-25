#include <llvm/Support/raw_ostream.h>
#include "attribs/SetAttribute.hpp"
#include "class/Class.hpp"

namespace tau { namespace attribs {

AttributeData SetPropertyAttribute::parseAttribute(const DynString& attribName, const clang::MacroArgs*, const clang::Token*& currentToken) const noexcept
{
    currentToken = getNextToken(currentToken);

    return AttributeData(this, nullptr, attribName);
}

void SetPropertyAttribute::generateBaseClass(::llvm::raw_fd_ostream& base) const noexcept
{
    base << 
        "public:\n"
        "    template<typename _T>\n"
        "    _T* getProperty(void* const object, const char* const propName) const noexcept\n"
        "    { return reinterpret_cast<_T*>(_getProperty(object, propName)); }\n"
        "\n"
        "    template<typename _T>\n"
        "    void setProperty(void* const object, const char* const propName, const _T& value) const noexcept\n"
        "    { _setProperty(object, propName, &value); }\n"
        "protected:\n"
        "    virtual void* _getProperty(void* object, const char* propName) const noexcept = 0;\n"
        "    virtual void _setProperty(void* object, const char* propName, const void* value) const noexcept = 0;\n";
}

void SetPropertyAttribute::generateImplClass(::llvm::raw_fd_ostream& base, const Ref<ReflClass>& clazz) const noexcept
{
    base << 
        "        public:                                                                                                                        \\\n"
        "            template<typename _T>                                                                                                      \\\n"
        "            _T* getProperty(" << clazz->name() << "* const object, const char* const propName) const noexcept                          \\\n"
        "            { return reinterpret_cast<_T*>(getPropertyImpl(object, propName)); }                                                       \\\n"
        "                                                                                                                                       \\\n"
        "            template<typename _T>                                                                                                      \\\n"
        "            void setProperty(" << clazz->name() << "* const object, const char* const propName, const _T& value) const noexcept        \\\n"
        "            { return setPropertyImpl(object, propName, &value); }                                                                      \\\n"
        "        protected:                                                                                                                     \\\n"
        "            void* _getProperty(void* const object, const char* const propName) const noexcept override                                 \\\n"
        "            { return getPropertyImpl(reinterpret_cast<" << clazz->name() << "*>(object), propName); }                                  \\\n"
        "                                                                                                                                       \\\n"
        "            void _setProperty(void* const object, const char* const propName, const void* const value) const noexcept override         \\\n"
        "            { return setPropertyImpl(reinterpret_cast<" << clazz->name() << "*>(object), propName, value); }                           \\\n"
        "                                                                                                                                       \\\n"
        "            void* getPropertyImpl(" << clazz->name() << "* const object, const char* const propName) const noexcept                    \\\n"
        "            {                                                                                                                          \\\n";

    for(uSys i = 0; i < clazz->properties().size(); ++i)
    {
        if(!clazz->properties()[i]->declaration()->hasAttribute("get") || !clazz->properties()[i]->declaration()->hasAttribute("set"))
        { continue; }

        base <<
            "                if(::std::strcmp(propName, \"" << clazz->properties()[i]->name() << "\") == 0) \\\n"
            "                { return &object->" << clazz->properties()[i]->name() << "; }                  \\\n";
    }

    base << 
        "                return nullptr;                                                                                                        \\\n"
        "            }                                                                                                                          \\\n"
        "                                                                                                                                       \\\n"
        "            void setPropertyImpl(" << clazz->name() << "* const object, const char* propName, const void* const value) const noexcept  \\\n"
        "            {                                                                                                                          \\\n";
    
    for(uSys i = 0; i < clazz->properties().size(); ++i)
    {
        if(!clazz->properties()[i]->declaration()->hasAttribute("set"))
        { continue; }

        base <<
            "                if(::std::strcmp(propName, \"" << clazz->properties()[i]->name() << "\") == 0)                                                         \\\n"
            "                { object->" << clazz->properties()[i]->name() << " = *reinterpret_cast<" << clazz->properties()[i]->typeName() << " const*>(value); }  \\\n";
    }

    base <<
        "            }                                                                                                                      \\\n";
}

}}
