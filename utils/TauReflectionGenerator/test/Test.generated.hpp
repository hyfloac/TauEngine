#pragma once

// Auto Generated - DO NOT EDIT

#include "Base.hpp"
#ifdef GENERATED_BODY 
  #undef GENERATED_BODY 
#endif 

#define GENERATED_BODY() \
    public: \
        class TauClassImpl final : public ITauClass { \
        public: \
            TauClassImpl() noexcept = default; \
            ~TauClassImpl() noexcept override = default; \
            \
            template<typename _T> \
            _T* getProperty(Test* const object, const char* const propName) noexcept \
            { return reinterpret_cast<_T*>(getPropertyImpl(object, propName)); } \
            \
            template<typename _T> \
            const _T* getProperty(const Test* const object, const char* propName) noexcept \
            { return reinterpret_cast<const _T*>(getPropertyImpl(object, propName)); } \
            \
            template<typename _T> \
            void setProperty(Test* const object, const char* propName, const _T& value) noexcept \
            { setPropertyImpl(object, propName, &value); } \
       protected: \
            void* _getProperty(void* const object, const char* const propName) const noexcept override { return getPropertyImpl(reinterpret_cast<Test*>(object), propName); } \
            const void* _getProperty(const void* const object, const char* const propName) const noexcept override { return getPropertyImpl(reinterpret_cast<const Test*>(object), propName); } \
            void _setProperty(void* const object, const char* const propName, const void* const value) const noexcept override { return setPropertyImpl(reinterpret_cast<Test*>(object), propName, value); } \
            \
            void* getPropertyImpl(Test* const object, const char* const propName) const noexcept { \
                if(::std::strcmp(propName, "_time") == 0) { \
                    return &object->_time; \
                } \
                if(::std::strcmp(propName, "_vec") == 0) { \
                } \
                return nullptr; \
            } \
            const void* getPropertyImpl(const Test* const object, const char* const propName) const noexcept { \
                if(::std::strcmp(propName, "_time") == 0) { \
                    return &object->_time; \
                } \
                if(::std::strcmp(propName, "_vec") == 0) { \
                    return &object->_vec; \
                } \
                return nullptr; \
            } \
            void setPropertyImpl(Test* const object, const char* const propName, const void* const value) const noexcept { \
                if(::std::strcmp(propName, "_time") == 0) { \
                    object->_time = *reinterpret_cast<int * const *>(value); \
                } \
                if(::std::strcmp(propName, "_vec") == 0) { \
                } \
                if(::std::strcmp(propName, "_time") == 0) { \
                } \
                if(::std::strcmp(propName, "_vec") == 0) { \
                } \
            } \
        }; \
        [[nodiscard]] static TauClassImpl& GetStaticClass() noexcept { \
            static TauClassImpl tauClass; \
            return tauClass; \
        } \
        [[nodiscard]] TauClassImpl& getClass() const noexcept { \
            return GetStaticClass(); \
        } 
