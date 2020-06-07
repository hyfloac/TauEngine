#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>
#include <DynArray.hpp>

#include "model/BufferDescriptor.hpp"
#include "DLL.hpp"

class IShader;
class IGraphicsInterface;
class IRenderingContext;

class TAU_DLL TAU_NOVTABLE IInputLayout
{
    DEFAULT_CONSTRUCT_PU(IInputLayout);
    DEFAULT_DESTRUCT_VI(IInputLayout);
    DEFAULT_CM_PO(IInputLayout);
public:
    virtual void bind(IRenderingContext& context) noexcept = 0;
    virtual void unbind(IRenderingContext& context) noexcept = 0;
};

struct InputLayoutArgs final
{
    DEFAULT_DESTRUCT(InputLayoutArgs);
    DEFAULT_CM_PU(InputLayoutArgs);
public:
    /**
     * Used to validate the layout.
     *
     * Can be null most of the time. A fake shader will be generated.
     */
    IShader* shader;
    RefDynArray<BufferDescriptor> descriptors;
public:
    inline InputLayoutArgs(const uSys descriptorCount) noexcept
        : shader(null)
        , descriptors(descriptorCount)
    { }
};

class TAU_DLL NOVTABLE IInputLayoutBuilder
{
    DEFAULT_CONSTRUCT_PO(IInputLayoutBuilder);
    DEFAULT_DESTRUCT_VI(IInputLayoutBuilder);
    DEFAULT_CM_PO(IInputLayoutBuilder);
public:
    enum class Error
    {
        NoError = 0,
        BuffersNotSet,
        /**
         *   The shader is not set and an input layout cannot be
         * verified. This is only thrown if a fake shader cannot be
         * generated for some unknown reason.
         */
        ShaderNotSet,
        ShaderMustBeVertexShader,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        MultipleSemanticOfInvalidType,
        InternalError
    };
public:
    [[nodiscard]] virtual IInputLayout* build(const InputLayoutArgs& args, [[tau::out]] Error* error) noexcept = 0;
    [[nodiscard]] virtual IInputLayout* build(const InputLayoutArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept = 0;
    [[nodiscard]] virtual CPPRef<IInputLayout> buildCPPRef(const InputLayoutArgs& args, [[tau::out]] Error* error) noexcept = 0;
    [[nodiscard]] virtual NullableRef<IInputLayout> buildTauRef(const InputLayoutArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IInputLayout> buildTauSRef(const InputLayoutArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept = 0;
};
