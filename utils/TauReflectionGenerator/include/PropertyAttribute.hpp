#pragma once

#include <Objects.hpp>
#include <String.hpp>
#include <Safeties.hpp>
#include <allocator/FixedBlockAllocator.hpp>
#include <unordered_map>

namespace clang {
    class Token;
    class MacroArgs;
}

namespace tau {

class IPropertyAttribute;

class PropertyAttributeData final
{
    DEFAULT_DESTRUCT(PropertyAttributeData);
    DEFAULT_CM_PU(PropertyAttributeData);
private:
    IPropertyAttribute* _handler;
    void* _data;
    DynString _attribName;
public:
    PropertyAttributeData() noexcept
        : _handler(nullptr)
        , _data(nullptr)
        , _attribName("")
    { }

    PropertyAttributeData(IPropertyAttribute* const handler, void* const data, const DynString& attribName) noexcept
        : _handler(handler)
        , _data(data)
        , _attribName(attribName)
    { }

    PropertyAttributeData(IPropertyAttribute* const handler, void* const data, DynString&& attribName) noexcept
        : _handler(handler)
        , _data(data)
        , _attribName(::std::move(attribName))
    { }

    [[nodiscard]] IPropertyAttribute* handler() const noexcept { return _handler; }
    [[nodiscard]] const DynString& attribName() const noexcept { return _attribName; }

    template<typename _T>
    [[nodiscard]] _T* data() noexcept { return reinterpret_cast<_T*>(_data); }

    template<typename _T>
    [[nodiscard]] const _T* data() const noexcept { return reinterpret_cast<const _T*>(_data); }
};

class IPropertyAttribute
{
    DEFAULT_CONSTRUCT_PO(IPropertyAttribute);
    DEFAULT_DESTRUCT_VI(IPropertyAttribute);
    DEFAULT_CM_PO(IPropertyAttribute);
public:
    /**
     *   The parser should set currentToken to the token directly
     * after the final token it needed to parse. In a simple
     * attribute which only uses the name, this token should either
     * be the following ',' or the eof.
     *
     * @param attribName
     *      The name of the attribute that triggered this
     *    invocation.
     * @param Args
     *      The arguments that were passed into the macro.
     * @param currentToken
     *      The current argument token.
     */
    virtual PropertyAttributeData parseAttribute(const DynString& attribName, const clang::MacroArgs* Args, const clang::Token*& currentToken) noexcept = 0;

    virtual void destroyData(PropertyAttributeData& data) noexcept { }
protected:
    static const clang::Token* getNextToken(const clang::Token* currentToken) noexcept;
};

class PropertyAttributeManager final
{
    DELETE_CONSTRUCT(PropertyAttributeManager);
    DELETE_DESTRUCT(PropertyAttributeManager);
    DELETE_CM(PropertyAttributeManager);
public:
    using AttribHandlerSet = ::std::unordered_map<DynString, Ref<IPropertyAttribute>>;
    using FBAllocator = FixedBlockAllocator<AllocationTracking::None>;
private:
    static AttribHandlerSet _attributeHandlers;
    static FBAllocator _attribTreeAllocator;
public:
    template<typename _T, typename... _Args>
    static void registerAttribute(const DynString& attribName, _Args&&... args) noexcept
    { _attributeHandlers.emplace(attribName, Ref<_T>(::std::forward<_Args>(args...))); }

    template<typename _T, typename... _Args>
    static void registerAttribute(DynString&& attribName, _Args&&... args) noexcept
    { _attributeHandlers.emplace(::std::move(attribName), Ref<_T>(::std::forward<_Args>(args...))); }

    static Ref<IPropertyAttribute> getAttribute(const DynString& attribName) noexcept;

    [[nodiscard]] static FBAllocator& getAllocator() noexcept { return _attribTreeAllocator; }
};

}

#include "PropertyAttribute.inl"
