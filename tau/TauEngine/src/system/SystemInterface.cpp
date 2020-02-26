#include "system/SystemInterface.hpp"
#include <new>

SystemInterface* _g__systemInterfacePtr_;

SystemInterface* SystemInterface::create() noexcept
{
    static bool created = false;
    if(!created)
    {
        created = true;
        _g__systemInterfacePtr_ = new(::std::nothrow) SystemInterface;
        return _g__systemInterfacePtr_;
    }

    return nullptr;
}

SystemInterface* SystemInterface::get() noexcept
{
    return _g__systemInterfacePtr_;
}
