#pragma once

// Auto Generated - DO NOT EDIT

#define TAU_CLASS(...)
#define TAU_PROPERTY(...)

class ITauClass
{
protected:
    ITauClass() noexcept = default;

    ITauClass(const ITauClass& copy) noexcept = default;
    ITauClass(ITauClass&& move) noexcept = default;

    ITauClass& operator=(const ITauClass& copy) noexcept = default;
    ITauClass& operator=(ITauClass&& move) noexcept = default;
public:
    virtual ~ITauClass() noexcept = default;
public:
    template<typename _T>
    const _T* getProperty(const void* const object, const char* const propName) const noexcept
    { return reinterpret_cast<const _T*>(_getProperty(object, propName)); }
protected:
    virtual const void* _getProperty(const void* object, const char* propName) const noexcept = 0;
public:
    template<typename _T>
    _T* getProperty(void* const object, const char* const propName) const noexcept
    { return reinterpret_cast<_T*>(_getProperty(object, propName)); }

    template<typename _T>
    void setProperty(void* const object, const char* const propName, const _T& value) const noexcept
    { _setProperty(object, propName, &value); }
protected:
    virtual void* _getProperty(void* object, const char* propName) const noexcept = 0;
    virtual void _setProperty(void* object, const char* propName, const void* value) const noexcept = 0;
};
