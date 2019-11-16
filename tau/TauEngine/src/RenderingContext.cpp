#include "system/RenderingContext.hpp"
#include "gl/GLRenderingContext.hpp"
#include "Timings.hpp"

#ifdef _WIN32
#include "dx/DXRenderingContext.hpp"
#endif

IRenderingContext* IRenderingContext::create(const RenderingMode& mode) noexcept
{
    switch(mode.currentMode())
    {
        case RenderingMode::Mode::DirectX9:
#ifdef _WIN32
            return new(std::nothrow) DXRenderingContext(mode, mode.debugMode());
#else
            return null;
#endif
        case RenderingMode::Mode::DirectX11:
        case RenderingMode::Mode::DirectX12:
        case RenderingMode::Mode::DirectX12_1:
#ifdef _WIN32
            return null;
#else
            return null;
#endif
        case RenderingMode::Mode::Vulkan:
            return null;
        case RenderingMode::Mode::OpenGL2:
            return new(std::nothrow) GLRenderingContext(mode, mode.debugMode(), 2, 0, GLRenderingContext::GLProfile::Neither, false);
        case RenderingMode::Mode::OpenGL3:
            return new(std::nothrow) GLRenderingContext(mode, mode.debugMode(), 3, 0, GLRenderingContext::GLProfile::Compat, true);
        case RenderingMode::Mode::OpenGL3_1:
            return new(std::nothrow) GLRenderingContext(mode, mode.debugMode(), 3, 1, GLRenderingContext::GLProfile::Compat, true);
        case RenderingMode::Mode::OpenGL3_2:
            return new(std::nothrow) GLRenderingContext(mode, mode.debugMode(), 3, 2, GLRenderingContext::GLProfile::Compat, true);
        case RenderingMode::Mode::OpenGL3_3:
            return new(std::nothrow) GLRenderingContext(mode, mode.debugMode(), 3, 3, GLRenderingContext::GLProfile::Compat, true);
        case RenderingMode::Mode::OpenGL4:
            return new(std::nothrow) GLRenderingContext(mode, mode.debugMode(), 4, 0, GLRenderingContext::GLProfile::Compat, true);
        case RenderingMode::Mode::OpenGL4_2:
            return new(std::nothrow) GLRenderingContext(mode, mode.debugMode(), 4, 1, GLRenderingContext::GLProfile::Compat, true);
        case RenderingMode::Mode::OpenGL4_3:
            return new(std::nothrow) GLRenderingContext(mode, mode.debugMode(), 4, 3, GLRenderingContext::GLProfile::Compat, true);
        case RenderingMode::Mode::OpenGL4_4:
            return new(std::nothrow) GLRenderingContext(mode, mode.debugMode(), 4, 4, GLRenderingContext::GLProfile::Compat, true);
        case RenderingMode::Mode::OpenGL4_5:
            return new(std::nothrow) GLRenderingContext(mode, mode.debugMode(), 4, 5, GLRenderingContext::GLProfile::Compat, true);
        case RenderingMode::Mode::OpenGL4_6:
            return new(std::nothrow) GLRenderingContext(mode, mode.debugMode(), 4, 6, GLRenderingContext::GLProfile::Compat, true);
        default: return null;
    }
}

SharedRenderingContextsContainer IRenderingContext::createShared(const RenderingMode& mode, std::size_t count) noexcept
{
    PERF();
    if(count < 2) { return null; }
    IRenderingContext** contexts = new(std::nothrow) IRenderingContext*[count];
    if(!contexts) { return null; }
    contexts[0] = IRenderingContext::create(mode);
    if(!contexts[0])
    {
        delete[] contexts;
        return null;
    }
    for(std::size_t i = 1; i < count; ++i)
    {
        contexts[i] = IRenderingContext::create(mode);
        if(!contexts[i])
        {
            // Creation failed, cleanup
            for(std::size_t j = 0; j < i; ++j)
            {
                delete contexts[j];
            }
            delete[] contexts;
            return null;
        }
    }
    return new(std::nothrow) SharedRenderingContexts(contexts, count);
}

SharedRenderingContexts::~SharedRenderingContexts() noexcept
{
    for(std::size_t i = 0; i < _count; ++i)
    {
        delete _contexts[i];
    }

    delete _contexts;
}

// ReSharper disable once CppMemberFunctionMayBeConst
bool SharedRenderingContexts::createContexts(void* param) noexcept
{
    IRenderingContext* first = _contexts[0];
    return first->createContextsShared(param, _contexts + 1, _count - 1);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void SharedRenderingContexts::destroyVA(IVertexArray* vertexArray) noexcept
{
    for(std::size_t i = 0; i < _count; ++i)
    {
        _contexts[i]->destroyVA(vertexArray);
    }
}
