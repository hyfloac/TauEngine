#include "allocator/TauAllocator.hpp"

DefaultTauAllocator& DefaultTauAllocator::Instance() noexcept
{
    static DefaultTauAllocator _instance;
    return _instance;
}
