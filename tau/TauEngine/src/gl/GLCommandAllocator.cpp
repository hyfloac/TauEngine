#include "gl/GLCommandAllocator.hpp"
#include "gl/GLCommandList.hpp"

GLCommandAllocator::GLCommandAllocator(const uSys maxTotalCommands) noexcept
    : _fbAllocator(sizeof(GLCL::Command), maxTotalCommands, 16)
    , _freeList(maxTotalCommands, 16)
{ }
