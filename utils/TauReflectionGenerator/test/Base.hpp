#pragma once

#include <cstring>

#define TAU_CLASS(...)
#define TAU_PROPERTY(...)

class ITauClass
{
protected:
    ITauClass() noexcept = default;
    virtual ~ITauClass() noexcept = default;
public:
    template<typename _T>
    _T* getProperty(void* object, const char* name) noexcept
    { return reinterpret_cast<_T*>(getProperty(object, name)); }

    template<typename _T>
    const _T* getProperty(const void* object, const char* name) noexcept
    { return reinterpret_cast<const _T*>(getProperty(object, name)); }
protected:
    virtual void* getProperty(void* object, const char* name) noexcept = 0;
    virtual const void* getProperty(const void* object, const char* name) noexcept = 0;
};
