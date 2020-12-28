#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>
#include <DynArray.hpp>
#include <RunTimeType.hpp>

#include "BufferDescriptor.hpp"
#include "DLL.hpp"

class IShader;
class IGraphicsInterface;
class IRenderingContext;

#define INPUT_LAYOUT_IMPL_BASE(_TYPE) \
    RTT_IMPL(_TYPE, IInputLayout)

#define INPUT_LAYOUT_IMPL(_TYPE) INPUT_LAYOUT_IMPL_BASE(_TYPE)

class TAU_DLL TAU_NOVTABLE IInputLayout
{
    DEFAULT_CONSTRUCT_PU(IInputLayout);
    DEFAULT_DESTRUCT_VI(IInputLayout);
    DEFAULT_CM_PO(IInputLayout);
public:
    RTT_BASE_IMPL(IInputLayout);
    RTT_BASE_CHECK(IInputLayout);
    RTT_BASE_CAST(IInputLayout);
};

struct InputLayoutArgs final
{
    /**
     * Used to validate the layout.
     *
     * Can be null most of the time. A fake shader will be generated.
     */
    IShader* shader;
    uSys descriptorCount;
    BufferDescriptor* descriptors;
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
        InvalidDataType,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        MultipleSemanticOfInvalidType,
        InternalError
    };
public:
    /**
     * Gets the size required for an allocation.
     *
     * This is useful for fixed block allocators.
     */
    [[nodiscard]] uSys allocSize() const noexcept
    { return _allocSize(); }

    [[nodiscard]] virtual NullableRef<IInputLayout> buildTauRef(const InputLayoutArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
protected:
    [[nodiscard]] virtual uSys _allocSize() const noexcept = 0;
};
