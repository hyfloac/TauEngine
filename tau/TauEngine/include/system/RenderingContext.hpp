/**
 * @file 
 */
#pragma once

#include <NumTypes.hpp>
#include <RunTimeType.hpp>
#include <Objects.hpp>
#include "Color.hpp"
#include "DLL.hpp"
#include "events/Exception.hpp"
#include "RenderingMode.hpp"

#define RC_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                            public: \
                                [[nodiscard]] static _TYPE::ContextType getStaticType() noexcept \
                                { static IRenderingContext::ContextType type = IRenderingContext::ContextType::define(); \
                                  return type; } \
                                [[nodiscard]] virtual IRenderingContext::ContextType getContextType() const noexcept override \
                                { return _TYPE::getStaticType(); }

#define RC_IMPL(_TYPE) RC_IMPL_BASE(_TYPE)

class IVertexArray;

class SharedRenderingContexts;
class SharedRenderingContextsContainer;

class TAU_DLL IRenderingContext
{
    DEFAULT_DESTRUCT_VI(IRenderingContext);
    DELETE_COPY(IRenderingContext);
public:
    static IRenderingContext* create(const RenderingMode& mode) noexcept;

    static SharedRenderingContextsContainer createShared(const RenderingMode& mode, std::size_t count) noexcept;

    using ContextType = RunTimeType<IRenderingContext>;
protected:
    const RenderingMode& _mode;
    bool _debug;
protected:
    IRenderingContext(const RenderingMode & mode, const bool debug)
        : _mode(mode), _debug(debug)
    { }
public:
    [[nodiscard]] const RenderingMode& mode() const noexcept { return _mode; }
    [[nodiscard]] bool debug() const noexcept { return _debug; }

    [[nodiscard]] virtual IRenderingContext::ContextType getContextType() const noexcept = 0;

    [[nodiscard]] virtual bool createContext(void* param) noexcept = 0;

    virtual void createFromShared(void* param) noexcept = 0;

    virtual void deactivateContext() noexcept = 0;

    virtual void activateContext() noexcept = 0;

    [[nodiscard]] virtual Ref<IVertexArray> createVertexArray(std::size_t attribCount) noexcept = 0;

    [[nodiscard]] virtual void* getVertexArrayHandle(IVertexArray* vertexArray) noexcept = 0;

    virtual void destroyVA(IVertexArray* vertexArray) noexcept = 0;

    virtual void clearVAs() noexcept = 0;

    virtual void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ = 0, float maxZ = 0) noexcept = 0;

    virtual void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue = 1.0f, int stencilValue = 0) noexcept = 0;

    virtual void setVSync(bool vsync) noexcept = 0;

    template<typename _T>
    [[nodiscard]] bool isContextType() const noexcept
    { return _T::getStaticType() == getContextType(); }

    template<typename _T>
    [[nodiscard]] _T* getVertexArrayHandle(IVertexArray* vertexArray) noexcept
    { return reinterpret_cast<_T*>(getVertexArrayHandle(vertexArray)); }
protected:
    virtual bool createContextsShared(void* param, IRenderingContext** sharers, std::size_t count) noexcept = 0;
private:
    friend class SharedRenderingContexts;
};

class IncorrectContextException final : public Exception
{
public:
    IncorrectContextException() noexcept = default;
    ~IncorrectContextException() noexcept override final = default;

    EXCEPTION_IMPL(IncorrectContextException);
};

class TAU_DLL SharedRenderingContexts
{
    DELETE_COPY(SharedRenderingContexts);
private:
    IRenderingContext** _contexts;
    std::size_t _count;
private:
    friend class IRenderingContext;

    SharedRenderingContexts(IRenderingContext** contexts, std::size_t count) noexcept
        : _contexts(contexts), _count(count)
    { }
public:
    ~SharedRenderingContexts() noexcept;

    [[nodiscard]] std::size_t count() const noexcept { return _count; }
    // ReSharper disable once CppMemberFunctionMayBeConst
    [[nodiscard]] IRenderingContext** contexts() noexcept { return _contexts; }

    [[nodiscard]] bool createContexts(void* param) noexcept;

    void destroyVA(IVertexArray* vertexArray) noexcept;
};

class TAU_DLL SharedRenderingContextsContainer
{
private:
    SharedRenderingContexts* _contexts;
    i32* _refCount;
public:
    SharedRenderingContextsContainer(SharedRenderingContexts* contexts) noexcept
        : _contexts(contexts), _refCount(new i32(1))
    { }

    ~SharedRenderingContextsContainer() noexcept
    {
        if(--(*_refCount) <= 0)
        {
            delete _contexts;
            delete _refCount;
        }
    }

    SharedRenderingContextsContainer(const SharedRenderingContextsContainer& copy) noexcept
        : _contexts(copy._contexts), _refCount(copy._refCount)
    { ++(*_refCount); }

    SharedRenderingContextsContainer(SharedRenderingContextsContainer&& move) noexcept
        : _contexts(move._contexts), _refCount(move._refCount)
    { ++(*_refCount); }

    SharedRenderingContextsContainer& operator=(const SharedRenderingContextsContainer& copy) noexcept
    {
        _contexts = copy._contexts;
        _refCount = copy._refCount;

        ++(*_refCount);

        return *this;
    }

    SharedRenderingContextsContainer& operator=(SharedRenderingContextsContainer&& move) noexcept
    {
        _contexts = move._contexts;
        _refCount = move._refCount;

        ++(*_refCount);

        return *this;
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    [[nodiscard]] IRenderingContext* operator [](std::size_t index) noexcept
    {
        if(!_contexts) { return null; }
        return _contexts->contexts()[index];
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    SharedRenderingContexts* operator ->() noexcept { return _contexts; }

    operator bool() const noexcept { return _contexts; }
};
