#include "dx/dx10/DX10CommandAllocator.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10CommandList.hpp"

DX10CommandAllocator::DX10CommandAllocator(const uSys maxTotalCommands) noexcept
    : _fbAllocator(sizeof(DX10CL::Command), maxTotalCommands, 16)
    , _freeList(maxTotalCommands * 2, 16)
{ }
#endif
