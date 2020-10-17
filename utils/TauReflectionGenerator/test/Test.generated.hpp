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
            _T* getProperty(Test* object, const char* name) noexcept \
            { return reinterpret_cast<_T*>(getProperty(object, name)); } \
            \
            template<typename _T> \
            const _T* getProperty(const Test* object, const char* name) noexcept \
            { return reinterpret_cast<const _T*>(getProperty(object, name)); } \
       protected: \
            void* getProperty(void* object, const char* name) noexcept override { return getProperty(reinterpret_cast<Test*>(object), name); } \
            const void* getProperty(const void* object, const char* name) noexcept override { return getProperty(reinterpret_cast<const Test*>(object), name); } \
            \
            void* getProperty(Test* object, const char* name) noexcept { \
                if(::std::strcmp(name, "_time") == 0) { \
                    return &object->_time; \
                } \
                return nullptr; \
            } \
            const void* getProperty(const Test* object, const char* name) noexcept { \
                if(::std::strcmp(name, "_time") == 0) { \
                    return &object->_time; \
                } \
                if(::std::strcmp(name, "_vec") == 0) { \
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
