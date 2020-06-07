#include "gl/gl4_3/GL4_3VertexArray.hpp"
#include "gl/GLBuffer.hpp"
#include "gl/GLRenderingContext.hpp"

#if TAU_RTTI_CHECK
  #include "TauEngine.hpp"
  #define CTX() \
      if(!RTT_CHECK(context, GLRenderingContext)) \
      { TAU_THROW(IncorrectContextException); } \
      auto& ctx = reinterpret_cast<GLRenderingContext&>(context)
#else
  #define CTX() \
      auto& ctx = reinterpret_cast<GLRenderingContext&>(context)
#endif

void GL4_3VertexArray::bind(IRenderingContext& context) noexcept
{
    CTX();
    ctx.setBuffers(_buffers.count(), _glBuffers);

    if(_indexBuffer)
    {
        _indexBuffer->bind();
    }
}

void GL4_3VertexArray::unbind(IRenderingContext& context) noexcept
{
    if(_indexBuffer)
    {
        _indexBuffer->unbind();
    }
}

void GL4_3VertexArray::draw(IRenderingContext& context, uSys drawCount, const uSys drawOffset) noexcept
{
    if(drawCount == 0)
    { drawCount = _drawCount; }

    if(_indexBuffer)
    {
        glDrawElements(_glDrawType, drawCount, _indexBuffer->glIndexSize(), reinterpret_cast<const void*>(drawOffset));
    }
    else
    {
        glDrawArrays(_glDrawType, drawOffset, drawCount);
    }
}

void GL4_3VertexArray::drawInstanced(IRenderingContext& context, const uSys instanceCount, uSys drawCount, const uSys drawOffset) noexcept
{
    if(drawCount == 0)
    { drawCount = _drawCount; }

    if(this->_indexBuffer)
    {
        glDrawElementsInstanced(_glDrawType, drawCount, _indexBuffer->glIndexSize(), reinterpret_cast<const void*>(drawOffset), instanceCount);
    }
    else
    {
        glDrawArraysInstanced(_glDrawType, drawOffset, drawCount, instanceCount);
    }
}

static GLenum glDrawType(DrawType drawType) noexcept;

bool GL4_3VertexArrayBuilder::processArgs(const VertexArrayArgs& args, GL4_3VertexArrayArgs* glArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(args.drawCount == 0, Error::DrawCountNotSet);
    ERROR_CODE_COND_F(args.drawType == static_cast<DrawType>(0), Error::DrawTypeNotSet);

    for(uSys i = 0; i < args.buffers.count(); ++i)
    {
        ERROR_CODE_COND_F(!args.buffers[i], Error::BuffersNotSet);
        ERROR_CODE_COND_F(!RTT_CHECK(args.buffers[i].get(), GLVertexBuffer), Error::InternalError);
    }

    if(args.indexBuffer)
    {
        ERROR_CODE_COND_F(!RTT_CHECK(args.indexBuffer.get(), GLIndexBuffer), Error::InternalError);
        glArgs->indexBuffer = RefCast<GLIndexBuffer>(args.indexBuffer);
    }
    else
    {
        glArgs->indexBuffer = null;
    }

    glArgs->drawType = glDrawType(args.drawType);

    glArgs->buffers = new(std::nothrow) GLuint[args.buffers.count()];
    ERROR_CODE_COND_F(!glArgs->buffers, Error::SystemMemoryAllocationFailure);

    for(uSys i = 0; i < args.buffers.count(); ++i)
    {
        glArgs->buffers[i] = RefCast<GLVertexBuffer>(args.buffers[i])->buffer();
    }

    return true;
}

static GLenum glDrawType(const DrawType drawType) noexcept
{
    switch(drawType)
    {
        case DrawType::SeparatedTriangles: return GL_TRIANGLES;
        case DrawType::ConnectedTriangles: return GL_TRIANGLE_STRIP;
        case DrawType::PointConnectedTriangles: return GL_TRIANGLE_FAN;
        default: return 0;
    }
}
