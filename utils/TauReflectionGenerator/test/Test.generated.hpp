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
        public:                                                                                            \
            const char* const * getProperties(unsigned* const count) const noexcept override               \
            {                                                                                              \
                static const char* _properties[] = {                                                       \
                    "_time",                                                                                              \
                    "_vec"                                                                                               \
                };                                                                                         \
                *count = 2;                                                              \
                return _properties;                                                                        \
            }                                                                                              \
                                                                                                           \
            const char* const * getFunctions(unsigned* const count) const noexcept override                \
            {                                                                                              \
                static const char* _functions[] = {                                                        \
                };                                                                                         \
                *count = 0;                                                              \
                return _functions;                                                                         \
            }                                                                                              \
                                                                                                           \
            [[nodiscard]] unsigned getPropertyIndex(const char* const propName) const noexcept override    \
            {                                                                                              \
                if(::std::strcmp(propName, "_time") == 0)         \
                { return 0; }                                                      \
                if(::std::strcmp(propName, "_vec") == 0)         \
                { return 1; }                                                      \
                return static_cast<unsigned>(-1);                                                          \
            }                                                                                              \
                                                                                                           \
            [[nodiscard]] unsigned getFunctionIndex(const char* const funcName) const noexcept override    \
            {                                                                                              \
                return static_cast<unsigned>(-1);                                                          \
            }                                                                                              \
        public:                                                                                                                                \
            template<typename _T>                                                                                                              \
            [[nodiscard]] const _T* getProperty(const Test* const object, const char* const propName) const noexcept        \
            { return reinterpret_cast<const _T*>(getPropertyImpl(object, propName)); }                                                         \
                                                                                                                                               \
            template<typename _T>                                                                                                              \
            [[nodiscard]] const _T* getProperty(const Test* const object, const unsigned propIndex) const noexcept          \
            { return reinterpret_cast<const _T*>(getPropertyImpl(object, propIndex)); }                                                        \
        protected:                                                                                                                             \
            [[nodiscard]] const void* _getProperty(const void* const object, const char* const propName) const noexcept override               \
            { return getPropertyImpl(reinterpret_cast<const Test*>(object), propName); }                                    \
                                                                                                                                               \
            [[nodiscard]] const void* _getProperty(const void* const object, const unsigned propIndex) const noexcept override                 \
            { return getPropertyImpl(reinterpret_cast<const Test*>(object), propIndex); }                                   \
                                                                                                                                               \
            [[nodiscard]] const void* getPropertyImpl(const Test* const object, const char* const propName) const noexcept  \
            {                                                                                                                                  \
                if(::std::strcmp(propName, "_time") == 0) \
                { return &object->_time; }                  \
                if(::std::strcmp(propName, "_vec") == 0) \
                { return &object->_vec; }                  \
                return nullptr;                                                                                                                \
            }                                                                                                                                  \
                                                                                                                                               \
            [[nodiscard]] const void* getPropertyImpl(const Test* const object, const unsigned propIndex) const noexcept    \
            {                                                                                                                                  \
                switch(propIndex)                                                                                                              \
                {                                                                                                                              \
                case 0: return &object->_time; \
                case 1: return &object->_vec; \
                    default: return nullptr;                                                                                                   \
                }                                                                                                                              \
            }                                                                                                                                  \
        public:                                                                                                                                \
            template<typename _T>                                                                                                              \
            [[nodiscard]] _T* getProperty(Test* const object, const char* const propName) const noexcept                    \
            { return reinterpret_cast<_T*>(getPropertyImpl(object, propName)); }                                                               \
                                                                                                                                               \
            template<typename _T>                                                                                                              \
            [[nodiscard]] _T* getProperty(Test* const object, const unsigned propIndex) const noexcept                      \
            { return reinterpret_cast<_T*>(getPropertyImpl(object, propIndex)); }                                                              \
                                                                                                                                               \
            template<typename _T>                                                                                                              \
            void setProperty(Test* const object, const char* const propName, const _T& value) const noexcept                \
            { return setPropertyImpl(object, propName, &value); }                                                                              \
                                                                                                                                               \
            template<typename _T>                                                                                                              \
            void setProperty(Test* const object, const unsigned propIndex, const _T& value) const noexcept                  \
            { return setPropertyImpl(object, propIndex, &value); }                                                                             \
        protected:                                                                                                                             \
            [[nodiscard]] void* _getProperty(void* const object, const char* const propName) const noexcept override                           \
            { return getPropertyImpl(reinterpret_cast<Test*>(object), propName); }                                          \
                                                                                                                                               \
            [[nodiscard]] void* _getProperty(void* const object, const unsigned propIndex) const noexcept override                             \
            { return getPropertyImpl(reinterpret_cast<Test*>(object), propIndex); }                                         \
                                                                                                                                               \
            void _setProperty(void* const object, const char* const propName, const void* const value) const noexcept override                 \
            { return setPropertyImpl(reinterpret_cast<Test*>(object), propName, value); }                                   \
                                                                                                                                               \
            void _setProperty(void* const object, const unsigned propIndex, const void* const value) const noexcept override                   \
            { return setPropertyImpl(reinterpret_cast<Test*>(object), propIndex, value); }                                  \
                                                                                                                                               \
            [[nodiscard]] void* getPropertyImpl(Test* const object, const char* const propName) const noexcept              \
            {                                                                                                                                  \
                if(::std::strcmp(propName, "_time") == 0) \
                { return &object->_time; }                  \
                return nullptr;                                                                                                                \
            }                                                                                                                                  \
                                                                                                                                               \
            [[nodiscard]] void* getPropertyImpl(Test* const object, const unsigned propIndex) const noexcept                \
            {                                                                                                                                  \
                switch(propIndex)                                                                                                              \
                {                                                                                                                              \
                case 0: return &object->_time; \
                    default: return nullptr;                                                                                                   \
                }                                                                                                                              \
            }                                                                                                                                  \
                                                                                                                                               \
            void setPropertyImpl(Test* const object, const char* const propName, const void* const value) const noexcept    \
            {                                                                                                                                  \
                if(::std::strcmp(propName, "_time") == 0)                                                         \
                { object->_time = *reinterpret_cast<int * const*>(value); }  \
            }                                                                                                                                  \
                                                                                                                                               \
            void setPropertyImpl(Test* const object, const unsigned propIndex, const void* const value) const noexcept      \
            {                                                                                                                                  \
                switch(propIndex)                                                                                                              \
                {                                                                                                                              \
                case 0: object->_time = *reinterpret_cast<int * const*>(value); break; \
                    default: break;                                                                                                            \
                }                                                                                                                              \
            }                                                                                                                                  \
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
        public:                                                                                            \
            const char* const * getProperties(unsigned* const count) const noexcept override               \
            {                                                                                              \
                static const char* _properties[] = {                                                       \
                    "_bar",                                                                                              \
                    "_baz"                                                                                               \
                };                                                                                         \
                *count = 2;                                                              \
                return _properties;                                                                        \
            }                                                                                              \
                                                                                                           \
            const char* const * getFunctions(unsigned* const count) const noexcept override                \
            {                                                                                              \
                static const char* _functions[] = {                                                        \
                    "testFunc"                                                                                               \
                };                                                                                         \
                *count = 1;                                                              \
                return _functions;                                                                         \
            }                                                                                              \
                                                                                                           \
            [[nodiscard]] unsigned getPropertyIndex(const char* const propName) const noexcept override    \
            {                                                                                              \
                if(::std::strcmp(propName, "_bar") == 0)         \
                { return 0; }                                                      \
                if(::std::strcmp(propName, "_baz") == 0)         \
                { return 1; }                                                      \
                return static_cast<unsigned>(-1);                                                          \
            }                                                                                              \
                                                                                                           \
            [[nodiscard]] unsigned getFunctionIndex(const char* const funcName) const noexcept override    \
            {                                                                                              \
                if(::std::strcmp(funcName, "testFunc") == 0)          \
                { return 0; }                                                      \
                return static_cast<unsigned>(-1);                                                          \
            }                                                                                              \
        public:                                                                                                                                \
            template<typename _T>                                                                                                              \
            [[nodiscard]] const _T* getProperty(const Foo* const object, const char* const propName) const noexcept        \
            { return reinterpret_cast<const _T*>(getPropertyImpl(object, propName)); }                                                         \
                                                                                                                                               \
            template<typename _T>                                                                                                              \
            [[nodiscard]] const _T* getProperty(const Foo* const object, const unsigned propIndex) const noexcept          \
            { return reinterpret_cast<const _T*>(getPropertyImpl(object, propIndex)); }                                                        \
        protected:                                                                                                                             \
            [[nodiscard]] const void* _getProperty(const void* const object, const char* const propName) const noexcept override               \
            { return getPropertyImpl(reinterpret_cast<const Foo*>(object), propName); }                                    \
                                                                                                                                               \
            [[nodiscard]] const void* _getProperty(const void* const object, const unsigned propIndex) const noexcept override                 \
            { return getPropertyImpl(reinterpret_cast<const Foo*>(object), propIndex); }                                   \
                                                                                                                                               \
            [[nodiscard]] const void* getPropertyImpl(const Foo* const object, const char* const propName) const noexcept  \
            {                                                                                                                                  \
                if(::std::strcmp(propName, "_bar") == 0) \
                { return &object->_bar; }                  \
                return nullptr;                                                                                                                \
            }                                                                                                                                  \
                                                                                                                                               \
            [[nodiscard]] const void* getPropertyImpl(const Foo* const object, const unsigned propIndex) const noexcept    \
            {                                                                                                                                  \
                switch(propIndex)                                                                                                              \
                {                                                                                                                              \
                case 0: return &object->_bar; \
                    default: return nullptr;                                                                                                   \
                }                                                                                                                              \
            }                                                                                                                                  \
        public:                                                                                                                                \
            template<typename _T>                                                                                                              \
            [[nodiscard]] _T* getProperty(Foo* const object, const char* const propName) const noexcept                    \
            { return reinterpret_cast<_T*>(getPropertyImpl(object, propName)); }                                                               \
                                                                                                                                               \
            template<typename _T>                                                                                                              \
            [[nodiscard]] _T* getProperty(Foo* const object, const unsigned propIndex) const noexcept                      \
            { return reinterpret_cast<_T*>(getPropertyImpl(object, propIndex)); }                                                              \
                                                                                                                                               \
            template<typename _T>                                                                                                              \
            void setProperty(Foo* const object, const char* const propName, const _T& value) const noexcept                \
            { return setPropertyImpl(object, propName, &value); }                                                                              \
                                                                                                                                               \
            template<typename _T>                                                                                                              \
            void setProperty(Foo* const object, const unsigned propIndex, const _T& value) const noexcept                  \
            { return setPropertyImpl(object, propIndex, &value); }                                                                             \
        protected:                                                                                                                             \
            [[nodiscard]] void* _getProperty(void* const object, const char* const propName) const noexcept override                           \
            { return getPropertyImpl(reinterpret_cast<Foo*>(object), propName); }                                          \
                                                                                                                                               \
            [[nodiscard]] void* _getProperty(void* const object, const unsigned propIndex) const noexcept override                             \
            { return getPropertyImpl(reinterpret_cast<Foo*>(object), propIndex); }                                         \
                                                                                                                                               \
            void _setProperty(void* const object, const char* const propName, const void* const value) const noexcept override                 \
            { return setPropertyImpl(reinterpret_cast<Foo*>(object), propName, value); }                                   \
                                                                                                                                               \
            void _setProperty(void* const object, const unsigned propIndex, const void* const value) const noexcept override                   \
            { return setPropertyImpl(reinterpret_cast<Foo*>(object), propIndex, value); }                                  \
                                                                                                                                               \
            [[nodiscard]] void* getPropertyImpl(Foo* const object, const char* const propName) const noexcept              \
            {                                                                                                                                  \
                if(::std::strcmp(propName, "_bar") == 0) \
                { return &object->_bar; }                  \
                return nullptr;                                                                                                                \
            }                                                                                                                                  \
                                                                                                                                               \
            [[nodiscard]] void* getPropertyImpl(Foo* const object, const unsigned propIndex) const noexcept                \
            {                                                                                                                                  \
                switch(propIndex)                                                                                                              \
                {                                                                                                                              \
                case 0: return &object->_bar; \
                    default: return nullptr;                                                                                                   \
                }                                                                                                                              \
            }                                                                                                                                  \
                                                                                                                                               \
            void setPropertyImpl(Foo* const object, const char* const propName, const void* const value) const noexcept    \
            {                                                                                                                                  \
                if(::std::strcmp(propName, "_bar") == 0)                                                         \
                { object->_bar = *reinterpret_cast<int const*>(value); }  \
                if(::std::strcmp(propName, "_baz") == 0)                                                         \
                { object->_baz = *reinterpret_cast<float const*>(value); }  \
            }                                                                                                                                  \
                                                                                                                                               \
            void setPropertyImpl(Foo* const object, const unsigned propIndex, const void* const value) const noexcept      \
            {                                                                                                                                  \
                switch(propIndex)                                                                                                              \
                {                                                                                                                              \
                case 0: object->_bar = *reinterpret_cast<int const*>(value); break; \
                case 1: object->_baz = *reinterpret_cast<float const*>(value); break; \
                    default: break;                                                                                                            \
                }                                                                                                                              \
            }                                                                                                                                  \
        };                                                                         \
    [[nodiscard]] static TauClassImpl& GetStaticClass() noexcept                   \
    {                                                                              \
        static TauClassImpl tauClass;                                              \
        return tauClass;                                                           \
    }                                                                              \
    [[nodiscard]] TauClassImpl& getClass() const noexcept                          \
    { return GetStaticClass(); }                                                   
// End _TAU_GENERATED_BODY_Foo()

