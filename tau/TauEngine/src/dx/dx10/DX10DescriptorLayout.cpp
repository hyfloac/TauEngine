#include "dx/dx10/DX10DescriptorLayout.hpp"

#ifdef _WIN32

NullableRef<IDescriptorLayout> DX10DescriptorLayoutBuilder::build(const DescriptorLayoutArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    RefDynArray<DescriptorLayoutEntry> entries(args.entryCount);
    (void) ::std::memcpy(entries.arr(), args.entries, args.entryCount * sizeof(DescriptorLayoutEntry));

    NullableRef<SimpleDescriptorLayout> layout(allocator, ::std::move(entries));
    ERROR_CODE_COND_N(!layout, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, ::std::move(layout));
}
#endif
