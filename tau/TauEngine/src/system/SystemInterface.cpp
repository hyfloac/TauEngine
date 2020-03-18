#include "system/SystemInterface.hpp"
#include <new>

SystemInterface* SystemInterface::_instance;

SystemInterface* SystemInterface::create() noexcept
{
    static bool created = false;
    if(!created)
    {
        created = true;
        _instance = new(::std::nothrow) SystemInterface;
        return _instance;
    }

    return nullptr;
}

void SystemInterface::finalize() noexcept
{
    delete _instance;
}

SystemInterface* SystemInterface::get() noexcept
{
    return _instance;
}
