#include "gl/GLRasterizerState.hpp"

void GLRasterizerState::apply() const noexcept
{
    if(_args.enableScissorTest)
    {
        glEnable(GL_SCISSOR_TEST);
    }
    else
    {
        glDisable(GL_SCISSOR_TEST);
    }

    glFrontFace(_frontFace);
    glCullFace(_cullMode);
    glPolygonMode(GL_FRONT_AND_BACK, _fillMode);
}

GLenum GLRasterizerStateBuilder::glCullMode(RasterizerArgs::CullMode cullMode) noexcept
{
    switch(cullMode)
    {
        case RasterizerArgs::CullMode::None: return GL_FRONT_AND_BACK;
        case RasterizerArgs::CullMode::Front: return GL_FRONT_FACE;
        case RasterizerArgs::CullMode::Back: return GL_BACK;
        default: return static_cast<GLenum>(0);
    }
}

GLenum GLRasterizerStateBuilder::glFillMode(RasterizerArgs::FillMode fillMode) noexcept
{
    switch(fillMode)
    {
        case RasterizerArgs::FillMode::Vertices: return GL_POINT;
        case RasterizerArgs::FillMode::Wireframe: return GL_LINE;
        case RasterizerArgs::FillMode::Filled: return GL_FILL;
        default: return static_cast<GLenum>(0);
    }
}

GLRasterizerState* GLRasterizerStateBuilder::build(const RasterizerArgs& args, Error* error) const noexcept
{
    GLRasterizerArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLRasterizerState* const state = new(::std::nothrow) GLRasterizerState(args, glArgs.frontFace, glArgs.cullMode, glArgs.fillMode);
    ERROR_CODE_COND_F(!state, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, state);
}

GLRasterizerState* GLRasterizerStateBuilder::build(const RasterizerArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLRasterizerArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLRasterizerState* const state = allocator.allocateT<GLRasterizerState>(args, glArgs.frontFace, glArgs.cullMode, glArgs.fillMode);
    ERROR_CODE_COND_F(!state, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, state);
}

CPPRef<IRasterizerState> GLRasterizerStateBuilder::buildCPPRef(const RasterizerArgs& args, Error* error) const noexcept
{
    GLRasterizerArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const CPPRef<GLRasterizerState> state = CPPRef<GLRasterizerState>(new(::std::nothrow) GLRasterizerState(args, glArgs.frontFace, glArgs.cullMode, glArgs.fillMode));
    ERROR_CODE_COND_F(!state, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, state);
}

NullableRef<IRasterizerState> GLRasterizerStateBuilder::buildTauRef(const RasterizerArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLRasterizerArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableRef<GLRasterizerState> state(allocator, args, glArgs.frontFace, glArgs.cullMode, glArgs.fillMode);
    ERROR_CODE_COND_F(!state, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, RefCast<IRasterizerState>(state));
}

NullableStrongRef<IRasterizerState> GLRasterizerStateBuilder::buildTauSRef(const RasterizerArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLRasterizerArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongRef<GLRasterizerState> state(allocator, args, glArgs.frontFace, glArgs.cullMode, glArgs.fillMode);
    ERROR_CODE_COND_F(!state, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, RefCast<IRasterizerState>(state));
}

bool GLRasterizerStateBuilder::processArgs(const RasterizerArgs& args, GLRasterizerArgs* glArgs, Error* error) const noexcept
{
    glArgs->frontFace = args.frontFaceCounterClockwise ? GL_CCW : GL_CW;
    glArgs->cullMode = glCullMode(args.cullMode);
    glArgs->fillMode = glFillMode(args.fillMode);

    ERROR_CODE_COND_F(glArgs->cullMode == 0, Error::InvalidCullMode);
    ERROR_CODE_COND_F(glArgs->fillMode == 0, Error::InvalidFillMode);

    return true;
}
