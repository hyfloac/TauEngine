#pragma once

#define NOMINMAX
#include <Objects.hpp>
#include <String.hpp>
#include <Safeties.hpp>
#include <allocator/FixedBlockAllocator.hpp>
#include <unordered_map>

namespace clang {
    class Token;
    class MacroArgs;
}

namespace llvm {
    class raw_fd_ostream;
}

namespace tau {

class IAttribute;
class ReflClass;

class AttributeData final
{
    DEFAULT_DESTRUCT(AttributeData);
    DEFAULT_CM_PU(AttributeData);
private:
    const IAttribute* _handler;
    void* _data;
    DynString _attribName;
public:
    AttributeData() noexcept
        : _handler(nullptr)
        , _data(nullptr)
        , _attribName("")
    { }

    AttributeData(const IAttribute* const handler, void* const data, const DynString& attribName) noexcept
        : _handler(handler)
        , _data(data)
        , _attribName(attribName)
    { }

    AttributeData(const IAttribute* const handler, void* const data, DynString&& attribName) noexcept
        : _handler(handler)
        , _data(data)
        , _attribName(::std::move(attribName))
    { }

    [[nodiscard]] const IAttribute* handler() const noexcept { return _handler; }
    [[nodiscard]] const DynString& attribName() const noexcept { return _attribName; }

    template<typename _T>
    [[nodiscard]] _T* data() noexcept { return reinterpret_cast<_T*>(_data); }

    template<typename _T>
    [[nodiscard]] const _T* data() const noexcept { return reinterpret_cast<const _T*>(_data); }
};

class IAttribute
{
    DEFAULT_CONSTRUCT_PO(IAttribute);
    DEFAULT_DESTRUCT_VI(IAttribute);
    DEFAULT_CM_PO(IAttribute);
public:
    [[nodiscard]] virtual bool isForClass() const noexcept { return false; }
    [[nodiscard]] virtual bool isForProperty() const noexcept { return false; }
    [[nodiscard]] virtual bool isForFunction() const noexcept { return false; }

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
    virtual AttributeData parseAttribute(const DynString& attribName, const clang::MacroArgs* Args, const clang::Token*& currentToken) const noexcept = 0;

    virtual void destroyData(AttributeData& data) const noexcept { }

    virtual void generateBaseClass(::llvm::raw_fd_ostream& base) const noexcept { }
    virtual void generateImplClass(::llvm::raw_fd_ostream& base, const Ref<ReflClass>& clazz) const noexcept { }
protected:
    static const clang::Token* getNextToken(const clang::Token* currentToken) noexcept;
};

class AttributeManager final
{
    DELETE_CONSTRUCT(AttributeManager);
    DELETE_DESTRUCT(AttributeManager);
    DELETE_CM(AttributeManager);
public:
    using AttribHandlerSet = ::std::unordered_map<DynString, Ref<IAttribute>>;
    using FBAllocator = FixedBlockAllocator<AllocationTracking::None>;

    struct AttribIterator final
    {
        AttribHandlerSet::const_iterator _begin;
        AttribHandlerSet::const_iterator _end;

        [[nodiscard]] AttribHandlerSet::const_iterator begin() const noexcept { return _begin; }
        [[nodiscard]] AttribHandlerSet::const_iterator   end() const noexcept { return _end;   }
    };
private:
    static AttribHandlerSet _attributeHandlers;
    static FBAllocator _attribTreeAllocator;
public:
    template<typename _T, typename... _Args>
    static void registerAttribute(const DynString& attribName, _Args&&... args) noexcept
    { _attributeHandlers.emplace(attribName, Ref<_T>(DefaultTauAllocator::Instance(), ::std::forward<_Args>(args)...)); }

    template<typename _T, typename... _Args>
    static void registerAttribute(DynString&& attribName, _Args&&... args) noexcept
    { _attributeHandlers.emplace(::std::move(attribName), Ref<_T>(DefaultTauAllocator::Instance(), ::std::forward<_Args>(args)...)); }

    static Ref<IAttribute> getAttribute(const DynString& attribName) noexcept;

    [[nodiscard]] static AttribIterator getAttributes() noexcept
    { return { _attributeHandlers.begin(), _attributeHandlers.end() }; }

    [[nodiscard]] static FBAllocator& getAllocator() noexcept { return _attribTreeAllocator; }
};

}

#include "Attribute.inl"
