#pragma once

// Auto Generated - DO NOT EDIT

#define TAU_CLASS(...)
#define TAU_PROPERTY(...)
#define TAU_FUNCTION(...)

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
    virtual const char* const * getProperties(unsigned* count) const noexcept = 0;
    virtual const char* const * getFunctions(unsigned* count) const noexcept = 0;
    [[nodiscard]] virtual unsigned getPropertyIndex(const char* propName) const noexcept = 0;
    [[nodiscard]] virtual unsigned getFunctionIndex(const char* funcName) const noexcept = 0;
public:
    template<typename _T>
    [[nodiscard]] const _T* getProperty(const void* const object, const char* const propName) const noexcept
    { return reinterpret_cast<const _T*>(_getProperty(object, propName)); }

    template<typename _T>
    [[nodiscard]] const _T* getProperty(const void* const object, const int propIndex) const noexcept
    { return reinterpret_cast<const _T*>(_getProperty(object, propIndex)); }
protected:
    [[nodiscard]] virtual const void* _getProperty(const void* object, const char* propName) const noexcept = 0;
    [[nodiscard]] virtual const void* _getProperty(const void* object, unsigned propIndex) const noexcept = 0;
public:
    template<typename _T>
    [[nodiscard]] _T* getProperty(void* const object, const char* const propName) const noexcept
    { return reinterpret_cast<_T*>(_getProperty(object, propName)); }

    template<typename _T>
    [[nodiscard]] _T* getProperty(void* const object, const unsigned propIndex) const noexcept
    { return reinterpret_cast<_T*>(_getProperty(object, propIndex)); }

    template<typename _T>
    void setProperty(void* const object, const char* const propName, const _T& value) const noexcept
    { _setProperty(object, propName, &value); }

    template<typename _T>
    void setProperty(void* const object, const unsigned propIndex, const _T& value) const noexcept
    { _setProperty(object, propIndex, &value); }
protected:
    [[nodiscard]] virtual void* _getProperty(void* object, const char* propName) const noexcept = 0;
    [[nodiscard]] virtual void* _getProperty(void* object, const unsigned propIndex) const noexcept = 0;
    virtual void _setProperty(void* object, const char* propName, const void* value) const noexcept = 0;
    virtual void _setProperty(void* object, const unsigned propIndex, const void* value) const noexcept = 0;
};
