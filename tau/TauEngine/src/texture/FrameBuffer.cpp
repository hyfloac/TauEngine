#include "texture/FrameBuffer.hpp"
#include "gl/GLFrameBuffer.hpp"
#include "system/RenderingContext.hpp"
#include <Safeties.hpp>

IFrameBufferAttachment* IFrameBufferAttachment::create(IRenderingContext& context, const Type type, const u32 width, const u32 height) noexcept
{
    switch(context.mode().currentMode())
    {
        case RenderingMode::Mode::DirectX9:
        #ifdef _WIN32
            return null;
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
        case RenderingMode::Mode::OpenGL3:
        case RenderingMode::Mode::OpenGL3_1:
        case RenderingMode::Mode::OpenGL3_2:
        case RenderingMode::Mode::OpenGL3_3:
        case RenderingMode::Mode::OpenGL4:
        case RenderingMode::Mode::OpenGL4_2:
        case RenderingMode::Mode::OpenGL4_3:
        case RenderingMode::Mode::OpenGL4_4:
        case RenderingMode::Mode::OpenGL4_5:
        case RenderingMode::Mode::OpenGL4_6:
        {
            if(type == Type::Color)
            {
                return new(std::nothrow) GLFrameBufferColorAttachment(width, height, context);
            }
            else if(type == Type::Depth)
            {
                return new(std::nothrow) GLFrameBufferDepthAttachment(width, height, context);
            }
            else if(type == Type::Stencil)
            {
                return new(std::nothrow) GLFrameBufferStencilAttachment(width, height, context);
            }
            else if(type == Type::DepthStencil)
            {
                return new(std::nothrow) GLFrameBufferDepthStencilAttachment(width, height);
            }
        }
        default: return null;
    }
}

IFrameBufferBuilder* IFrameBufferBuilder::create(IRenderingContext& context) noexcept
{
    switch(context.mode().currentMode())
    {
        case RenderingMode::Mode::DirectX9:
        #ifdef _WIN32
            return null;
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
        case RenderingMode::Mode::OpenGL3:
        case RenderingMode::Mode::OpenGL3_1:
        case RenderingMode::Mode::OpenGL3_2:
        case RenderingMode::Mode::OpenGL3_3:
        case RenderingMode::Mode::OpenGL4:
        case RenderingMode::Mode::OpenGL4_2:
        case RenderingMode::Mode::OpenGL4_3:
        case RenderingMode::Mode::OpenGL4_4:
        case RenderingMode::Mode::OpenGL4_5:
        case RenderingMode::Mode::OpenGL4_6:
            return new(std::nothrow) GLFrameBufferBuilder();
        default: return null;
    }
}

IFrameBufferAttachment::~IFrameBufferAttachment() noexcept
{
    delete _next;
}

IFrameBuffer::~IFrameBuffer() noexcept
{
    delete _attachment;
}

void IFrameBufferBuilder::attach(IFrameBufferAttachment* attachment, Error* error) noexcept
{
    if(!_attachment)
    {
        _attachment = attachment;
        return;
    }

    if(attachment->type() == IFrameBufferAttachment::Color)
    {
        ++_colorCount;

        IFrameBufferAttachment* curr = _attachment;

        while(curr->next() && curr->next()->type() == IFrameBufferAttachment::Color)
        {
            curr = curr->next();
        }

        attachment->next() = curr->next();
        curr->next() = attachment;
    }
    else if(attachment->type() == IFrameBufferAttachment::Depth)
    {
        ERROR_CODE_COND(_hasDepth, Error::DepthAlreadyExists);

        IFrameBufferAttachment* curr = _attachment;

        while(curr->next() && curr->next()->type() == IFrameBufferAttachment::Color)
        {
            curr = curr->next();
        }

        attachment->next() = curr->next();
        curr->next() = attachment;
    }
    else if(attachment->type() == IFrameBufferAttachment::Stencil)
    {
        ERROR_CODE_COND(_hasStencil, Error::StencilAlreadyExists);


        IFrameBufferAttachment* curr = _attachment;

        while(curr->next() && (curr->next()->type() == IFrameBufferAttachment::Color || curr->next()->type() == IFrameBufferAttachment::Depth))
        {
            curr = curr->next();
        }

        attachment->next() = curr->next();
        curr->next() = attachment;
    }
    else if(attachment->type() == IFrameBufferAttachment::DepthStencil)
    {
        if(_hasDepth)
        {
            ERROR_CODE_COND(_hasStencil, Error::DepthAlreadyExists);
            ERROR_CODE(Error::DepthAlreadyExists);
        }

        IFrameBufferAttachment* curr = _attachment;

        while(curr->next() && curr->next()->type() == IFrameBufferAttachment::Color)
        {
            curr = curr->next();
        }

        attachment->next() = curr->next();
        curr->next() = attachment;
    }
}
