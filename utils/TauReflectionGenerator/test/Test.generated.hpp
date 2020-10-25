#pragma once

// Auto Generated - DO NOT EDIT

#include "Base.hpp"
#include <cstring>

#ifdef TAU_GENERATED_BODY 
  #undef TAU_GENERATED_BODY 
#endif 

#ifdef _TAU_CAT 
  #undef _TAU_CAT 
#endif 

#define _TAU_CAT(_A, _B) _A##_B
#define TAU_GENERATED_BODY(_CLASS) _TAU_CAT(_TAU_GENERATED_BODY_, _CLASS)()

#ifdef _TAU_GENERATED_BODY_Test
  #undef _TAU_GENERATED_BODY_Test
#endif

#define _TAU_GENERATED_BODY_Test() \
    public:                                                                        \
        class TauClassImpl final : public ITauClass                                \
        {                                                                          \
        public:                                                                    \
            TauClassImpl() noexcept = default;                                     \
            ~TauClassImpl() noexcept override = default;                           \
                                                                                   \
            TauClassImpl(const TauClassImpl& copy) noexcept = default;             \
            TauClassImpl(TauClassImpl&& move) noexcept = default;                  \
                                                                                   \
            TauClassImpl& operator=(const TauClassImpl& copy) noexcept = default;  \
            TauClassImpl& operator=(TauClassImpl&& move) noexcept = default;       \
        public:                                                                                                                    \
            template<typename _T>                                                                                                  \
            const _T* getProperty(const Test* const object, const char* const propName) const noexcept          \
            { return reinterpret_cast<const _T*>(getPropertyImpl(object, propName)); }                                             \
        protected:                                                                                                                 \
            const void* _getProperty(const void* const object, const char* const propName) const noexcept override                 \
            { return getPropertyImpl(reinterpret_cast<const Test*>(object), propName); }                        \
                                                                                                                                   \
            const void* getPropertyImpl(const Test* const object, const char* const propName) const noexcept    \
            {                                                                                                                      \
                if(::std::strcmp(propName, "_time") == 0) \
                { return &object->_time; }                  \
                if(::std::strcmp(propName, "_vec") == 0) \
                { return &object->_vec; }                  \
                return nullptr;                                                                                                    \
            }                                                                                                                      \
        public:                                                                                                                        \
            template<typename _T>                                                                                                      \
            _T* getProperty(Test* const object, const char* const propName) const noexcept                          \
            { return reinterpret_cast<_T*>(getPropertyImpl(object, propName)); }                                                       \
                                                                                                                                       \
            template<typename _T>                                                                                                      \
            void setProperty(Test* const object, const char* const propName, const _T& value) const noexcept        \
            { return setPropertyImpl(object, propName, &value); }                                                                      \
        protected:                                                                                                                     \
            void* _getProperty(void* const object, const char* const propName) const noexcept override                                 \
            { return getPropertyImpl(reinterpret_cast<Test*>(object), propName); }                                  \
                                                                                                                                       \
            void _setProperty(void* const object, const char* const propName, const void* const value) const noexcept override         \
            { return setPropertyImpl(reinterpret_cast<Test*>(object), propName, value); }                           \
                                                                                                                                       \
            void* getPropertyImpl(Test* const object, const char* const propName) const noexcept                    \
            {                                                                                                                          \
                if(::std::strcmp(propName, "_time") == 0) \
                { return &object->_time; }                  \
                return nullptr;                                                                                                        \
            }                                                                                                                          \
                                                                                                                                       \
            void setPropertyImpl(Test* const object, const char* propName, const void* const value) const noexcept  \
            {                                                                                                                          \
                if(::std::strcmp(propName, "_time") == 0)                                                         \
                { object->_time = *reinterpret_cast<int * const*>(value); }  \
            }                                                                                                                      \
        };                                                                         \
    [[nodiscard]] static TauClassImpl& GetStaticClass() noexcept                   \
    {                                                                              \
        static TauClassImpl tauClass;                                              \
        return tauClass;                                                           \
    }                                                                              \
    [[nodiscard]] TauClassImpl& getClass() const noexcept                          \
    { return GetStaticClass(); }                                                   
// End _TAU_GENERATED_BODY_Test()

#ifdef _TAU_GENERATED_BODY_Foo
  #undef _TAU_GENERATED_BODY_Foo
#endif

#define _TAU_GENERATED_BODY_Foo() \
    public:                                                                        \
        class TauClassImpl final : public ITauClass                                \
        {                                                                          \
        public:                                                                    \
            TauClassImpl() noexcept = default;                                     \
            ~TauClassImpl() noexcept override = default;                           \
                                                                                   \
            TauClassImpl(const TauClassImpl& copy) noexcept = default;             \
            TauClassImpl(TauClassImpl&& move) noexcept = default;                  \
                                                                                   \
            TauClassImpl& operator=(const TauClassImpl& copy) noexcept = default;  \
            TauClassImpl& operator=(TauClassImpl&& move) noexcept = default;       \
        public:                                                                                                                    \
            template<typename _T>                                                                                                  \
            const _T* getProperty(const Foo* const object, const char* const propName) const noexcept          \
            { return reinterpret_cast<const _T*>(getPropertyImpl(object, propName)); }                                             \
        protected:                                                                                                                 \
            const void* _getProperty(const void* const object, const char* const propName) const noexcept override                 \
            { return getPropertyImpl(reinterpret_cast<const Foo*>(object), propName); }                        \
                                                                                                                                   \
            const void* getPropertyImpl(const Foo* const object, const char* const propName) const noexcept    \
            {                                                                                                                      \
                if(::std::strcmp(propName, "_bar") == 0) \
                { return &object->_bar; }                  \
                return nullptr;                                                                                                    \
            }                                                                                                                      \
        public:                                                                                                                        \
            template<typename _T>                                                                                                      \
            _T* getProperty(Foo* const object, const char* const propName) const noexcept                          \
            { return reinterpret_cast<_T*>(getPropertyImpl(object, propName)); }                                                       \
                                                                                                                                       \
            template<typename _T>                                                                                                      \
            void setProperty(Foo* const object, const char* const propName, const _T& value) const noexcept        \
            { return setPropertyImpl(object, propName, &value); }                                                                      \
        protected:                                                                                                                     \
            void* _getProperty(void* const object, const char* const propName) const noexcept override                                 \
            { return getPropertyImpl(reinterpret_cast<Foo*>(object), propName); }                                  \
                                                                                                                                       \
            void _setProperty(void* const object, const char* const propName, const void* const value) const noexcept override         \
            { return setPropertyImpl(reinterpret_cast<Foo*>(object), propName, value); }                           \
                                                                                                                                       \
            void* getPropertyImpl(Foo* const object, const char* const propName) const noexcept                    \
            {                                                                                                                          \
                if(::std::strcmp(propName, "_bar") == 0) \
                { return &object->_bar; }                  \
                return nullptr;                                                                                                        \
            }                                                                                                                          \
                                                                                                                                       \
            void setPropertyImpl(Foo* const object, const char* propName, const void* const value) const noexcept  \
            {                                                                                                                          \
                if(::std::strcmp(propName, "_bar") == 0)                                                         \
                { object->_bar = *reinterpret_cast<int const*>(value); }  \
                if(::std::strcmp(propName, "_baz") == 0)                                                         \
                { object->_baz = *reinterpret_cast<float const*>(value); }  \
            }                                                                                                                      \
        };                                                                         \
    [[nodiscard]] static TauClassImpl& GetStaticClass() noexcept                   \
    {                                                                              \
        static TauClassImpl tauClass;                                              \
        return tauClass;                                                           \
    }                                                                              \
    [[nodiscard]] TauClassImpl& getClass() const noexcept                          \
    { return GetStaticClass(); }                                                   
// End _TAU_GENERATED_BODY_Foo()

