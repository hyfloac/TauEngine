#pragma once

#include <cstring>

#define TAU_CLASS(...)
#define TAU_PROPERTY(...)

class ITauClass
{
protected:
    ITauClass() noexcept = default;
private:
    ITauClass(const ITauClass& copy) = delete;
    ITauClass(ITauClass&& move) = delete;

    ITauClass& operator=(const ITauClass& copy) = delete;
    ITauClass& operator=(ITauClass&& move) = delete;
public:
    virtual ~ITauClass() noexcept = default;
    
    template<typename _T>
    _T* getProperty(void* const object, const char* propName) const noexcept
    { return reinterpret_cast<_T*>(getProperty(object, propName)); }

    template<typename _T>
    const _T* getProperty(const void* const object, const char* propName) const noexcept
    { return reinterpret_cast<const _T*>(getProperty(object, propName)); }

    template<typename _T>
    void setProperty(void* const object, const char* const propName, const _T& value) const noexcept
    { setProperty(object, propName, &value); }
protected:
    virtual void* _getProperty(void* object, const char* propName) const noexcept = 0;
    virtual const void* _getProperty(const void* object, const char* propName) const noexcept = 0;
    virtual void _setProperty(void* object, const char* propName, const void* value) const noexcept = 0;
};
