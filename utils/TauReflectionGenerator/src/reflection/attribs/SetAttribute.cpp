#include <llvm/Support/raw_ostream.h>
#include "reflection/attribs/SetAttribute.hpp"
#include "reflection/Class.hpp"

namespace tau { namespace reflection { namespace attribs {

AttributeData SetPropertyAttribute::parseAttribute(const DynString& attribName, const ::clang::MacroArgs*, const ::clang::Token*& currentToken) const noexcept
{
    currentToken = getNextToken(currentToken);

    return AttributeData(this, nullptr, attribName);
}

void SetPropertyAttribute::generateBaseTauClass(::llvm::raw_fd_ostream& base) const noexcept
{
    base << 
        "public:\n"
        "    template<typename _T>\n"
        "    [[nodiscard]] _T* getProperty(void* const object, const char* const propName) const noexcept\n"
        "    { return reinterpret_cast<_T*>(_getProperty(object, propName)); }\n"
        "\n"
        "    template<typename _T>\n"
        "    [[nodiscard]] _T* getProperty(void* const object, const unsigned propIndex) const noexcept\n"
        "    { return reinterpret_cast<_T*>(_getProperty(object, propIndex)); }\n"
        "\n"
        "    template<typename _T>\n"
        "    void setProperty(void* const object, const char* const propName, const _T& value) const noexcept\n"
        "    { _setProperty(object, propName, &value); }\n"
        "\n"
        "    template<typename _T>\n"
        "    void setProperty(void* const object, const unsigned propIndex, const _T& value) const noexcept\n"
        "    { _setProperty(object, propIndex, &value); }\n"
        "protected:\n"
        "    [[nodiscard]] virtual void* _getProperty(void* object, const char* propName) const noexcept = 0;\n"
        "    [[nodiscard]] virtual void* _getProperty(void* object, const unsigned propIndex) const noexcept = 0;\n"
        "    virtual void _setProperty(void* object, const char* propName, const void* value) const noexcept = 0;\n"
        "    virtual void _setProperty(void* object, const unsigned propIndex, const void* value) const noexcept = 0;\n";
}

void SetPropertyAttribute::generateImplTauClass(::llvm::raw_fd_ostream& base, const Ref<Class>& clazz) const noexcept
{
    base << 
        "        public:                                                                                                                            \\\n"
        "            template<typename _T>                                                                                                          \\\n"
        "            [[nodiscard]] _T* getProperty(" << clazz->name() << "* const object, const char* const propName) const noexcept                \\\n"
        "            { return reinterpret_cast<_T*>(getPropertyImpl(object, propName)); }                                                           \\\n"
        "                                                                                                                                           \\\n"
        "            template<typename _T>                                                                                                          \\\n"
        "            [[nodiscard]] _T* getProperty(" << clazz->name() << "* const object, const unsigned propIndex) const noexcept                  \\\n"
        "            { return reinterpret_cast<_T*>(getPropertyImpl(object, propIndex)); }                                                          \\\n"
        "                                                                                                                                           \\\n"
        "            template<typename _T>                                                                                                          \\\n"
        "            void setProperty(" << clazz->name() << "* const object, const char* const propName, const _T& value) const noexcept            \\\n"
        "            { return setPropertyImpl(object, propName, &value); }                                                                          \\\n"
        "                                                                                                                                           \\\n"
        "            template<typename _T>                                                                                                          \\\n"
        "            void setProperty(" << clazz->name() << "* const object, const unsigned propIndex, const _T& value) const noexcept              \\\n"
        "            { return setPropertyImpl(object, propIndex, &value); }                                                                         \\\n"
        "        protected:                                                                                                                         \\\n"
        "            [[nodiscard]] void* _getProperty(void* const object, const char* const propName) const noexcept override                       \\\n"
        "            { return getPropertyImpl(reinterpret_cast<" << clazz->name() << "*>(object), propName); }                                      \\\n"
        "                                                                                                                                           \\\n"
        "            [[nodiscard]] void* _getProperty(void* const object, const unsigned propIndex) const noexcept override                         \\\n"
        "            { return getPropertyImpl(reinterpret_cast<" << clazz->name() << "*>(object), propIndex); }                                     \\\n"
        "                                                                                                                                           \\\n"
        "            void _setProperty(void* const object, const char* const propName, const void* const value) const noexcept override             \\\n"
        "            { return setPropertyImpl(reinterpret_cast<" << clazz->name() << "*>(object), propName, value); }                               \\\n"
        "                                                                                                                                           \\\n"
        "            void _setProperty(void* const object, const unsigned propIndex, const void* const value) const noexcept override               \\\n"
        "            { return setPropertyImpl(reinterpret_cast<" << clazz->name() << "*>(object), propIndex, value); }                              \\\n"
        "                                                                                                                                           \\\n"
        "            [[nodiscard]] void* getPropertyImpl(" << clazz->name() << "* const object, const char* const propName) const noexcept          \\\n"
        "            {                                                                                                                              \\\n";

    for(uSys i = 0; i < clazz->properties().size(); ++i)
    {
        if(!clazz->properties()[i]->declaration()->hasAttribute("get") || !clazz->properties()[i]->declaration()->hasAttribute("set"))
        { continue; }

        base <<
            "                if(::std::strcmp(propName, \"" << clazz->properties()[i]->name() << "\") == 0) \\\n"
            "                { return &object->" << clazz->properties()[i]->name() << "; }                  \\\n";
    }

    base << 
        "                return nullptr;                                                                                                            \\\n"
        "            }                                                                                                                              \\\n"
        "                                                                                                                                           \\\n"
        "            [[nodiscard]] void* getPropertyImpl(" << clazz->name() << "* const object, const unsigned propIndex) const noexcept            \\\n"
        "            {                                                                                                                              \\\n"
        "                switch(propIndex)                                                                                                          \\\n"
        "                {                                                                                                                          \\\n";
    
    for(uSys i = 0; i < clazz->properties().size(); ++i)
    {
        if(!clazz->properties()[i]->declaration()->hasAttribute("get") || !clazz->properties()[i]->declaration()->hasAttribute("set"))
        { continue; }

        base << 
            "                case " << i << ": return &object->" << clazz->properties()[i]->name() << "; \\\n";
    }

    base <<
        "                    default: return nullptr;                                                                                               \\\n"
        "                }                                                                                                                          \\\n"
        "            }                                                                                                                              \\\n"
        "                                                                                                                                           \\\n"
        "            void setPropertyImpl(" << clazz->name() << "* const object, const char* propName, const void* const value) const noexcept      \\\n"
        "            {                                                                                                                              \\\n";
    
    for(uSys i = 0; i < clazz->properties().size(); ++i)
    {
        if(!clazz->properties()[i]->declaration()->hasAttribute("set"))
        { continue; }

        base <<
            "                if(::std::strcmp(propName, \"" << clazz->properties()[i]->name() << "\") == 0)                                                         \\\n"
            "                { object->" << clazz->properties()[i]->name() << " = *reinterpret_cast<" << clazz->properties()[i]->typeName() << " const*>(value); }  \\\n";
    }

    base <<
        "            }                                                                                                                              \\\n"
        "                                                                                                                                           \\\n"
        "            void setPropertyImpl(" << clazz->name() << "* const object, const unsigned propIndex, const void* const value) const noexcept  \\\n"
        "            {                                                                                                                              \\\n"
        "                switch(propIndex)                                                                                                          \\\n"
        "                {                                                                                                                          \\\n";

    for(uSys i = 0; i < clazz->properties().size(); ++i)
    {
        if(!clazz->properties()[i]->declaration()->hasAttribute("get") || !clazz->properties()[i]->declaration()->hasAttribute("set"))
        { continue; }

        base << 
            "                case " << i << ": object->" << clazz->properties()[i]->name() << " = *reinterpret_cast<" << clazz->properties()[i]->typeName() << " const*>(value); break; \\\n";
    }
    
    base <<
        "                    default: break;                                                                                                        \\\n"
        "                }                                                                                                                          \\\n"
        "            }                                                                                                                              \\\n";
}

} } }
