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
            { return reinterpret_cast<_T*>(getProperty(object, propName)); } \
            \
            template<typename _T> \
            const _T* getProperty(const Test* const object, const char* propName) noexcept \
            { return reinterpret_cast<const _T*>(getProperty(object, propName)); } \
            \
            template<typename _T> \
            void setProperty(Test* const object, const char* propName, const _T& value) noexcept \
            { setProperty(object, propName, value); } \
       protected: \
            void* getProperty(void* const object, const char* const propName) noexcept override { return getProperty(reinterpret_cast<Test*>(object), propName); } \
            const void* getProperty(const void* const object, const char* const propName) noexcept override { return getProperty(reinterpret_cast<const Test*>(object), propName); } \
            \
            void* getProperty(Test* object, const char* propName) noexcept { \
                if(::std::strcmp(propName, "_time") == 0) { \
                    return &object->_time; \
                } \
                return nullptr; \
            } \
            const void* getProperty(const Test* const object, const char* const propName) noexcept { \
                if(::std::strcmp(propName, "_time") == 0) { \
                    return &object->_time; \
                } \
                if(::std::strcmp(propName, "_vec") == 0) { \
                    return &object->_vec; \
                } \
                return nullptr; \
            } \
        }; \
        [[nodiscard]] static TauClassImpl& GetStaticClass() noexcept { \
            static TauClassImpl tauClass; \
            return tauClass; \
        } \
        [[nodiscard]] TauClassImpl& getClass() const noexcept { \
            return GetStaticClass(); \
        } 
