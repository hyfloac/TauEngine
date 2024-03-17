#pragma once

#define DELETE_COPY(_TYPE) \
    public: \
        inline _TYPE(const _TYPE& copy) noexcept = delete;            \
        inline _TYPE& operator=(const _TYPE& copy) noexcept = delete

#define DELETE_MOVE(_TYPE) \
    public: \
        inline _TYPE(_TYPE&& move) noexcept = delete; \
        inline _TYPE& operator=(_TYPE&& move) noexcept = delete

#define DELETE_CM(_TYPE) \
    DELETE_COPY(_TYPE); \
    DELETE_MOVE(_TYPE)

#define DEFAULT_COPY(_TYPE) \
    inline _TYPE(const _TYPE& copy) noexcept = default;            \
    inline _TYPE& operator=(const _TYPE& copy) noexcept = default

#define DEFAULT_MOVE(_TYPE) \
    inline _TYPE(_TYPE&& move) noexcept = default; \
    inline _TYPE& operator=(_TYPE&& move) noexcept = default

#define DEFAULT_CM(_TYPE) \
    DEFAULT_COPY(_TYPE); \
    DEFAULT_MOVE(_TYPE)

#define DEFAULT_COPY_PU(_TYPE) public:    DEFAULT_COPY(_TYPE)
#define DEFAULT_COPY_PI(_TYPE) private:   DEFAULT_COPY(_TYPE)
#define DEFAULT_COPY_PO(_TYPE) protected: DEFAULT_COPY(_TYPE)

#define DEFAULT_MOVE_PU(_TYPE) public:    DEFAULT_MOVE(_TYPE)
#define DEFAULT_MOVE_PI(_TYPE) private:   DEFAULT_MOVE(_TYPE)
#define DEFAULT_MOVE_PO(_TYPE) protected: DEFAULT_MOVE(_TYPE)

#define DEFAULT_CM_PU(_TYPE) public:    DEFAULT_CM(_TYPE)
#define DEFAULT_CM_PI(_TYPE) private:   DEFAULT_CM(_TYPE)
#define DEFAULT_CM_PO(_TYPE) protected: DEFAULT_CM(_TYPE)

#define DELETE_DESTRUCT(_TYPE) public: inline ~_TYPE() noexcept = delete
#define DEFAULT_DESTRUCT(_TYPE) public: inline ~_TYPE() noexcept = default
#define DEFAULT_DESTRUCT_VI(_TYPE) public: inline virtual ~_TYPE() noexcept = default

#define DELETE_CONSTRUCT(_TYPE) private: inline _TYPE() noexcept = delete
#define DEFAULT_CONSTRUCT(_TYPE) inline _TYPE() noexcept = default
#define DEFAULT_CONSTRUCT_PU(_TYPE) public: DEFAULT_CONSTRUCT(_TYPE)
#define DEFAULT_CONSTRUCT_PI(_TYPE) private: DEFAULT_CONSTRUCT(_TYPE)
#define DEFAULT_CONSTRUCT_PO(_TYPE) protected: DEFAULT_CONSTRUCT(_TYPE)

#define DECL_OPAQUE_TYPE(_TYPE)                      \
    struct _TYPE final {                             \
        DEFAULT_CONSTRUCT_PU(_TYPE);                 \
        DEFAULT_DESTRUCT(_TYPE);                     \
        DEFAULT_CM_PU(_TYPE);                        \
    public:                                          \
        void* raw;                                   \
    public:                                          \
        _TYPE(void* const _raw) noexcept             \
            : raw(_raw)                              \
        { }                                          \
        template<typename _T>                        \
        [[nodiscard]] _T* get() noexcept             \
        { return reinterpret_cast<_T*>(raw); }       \
        template<typename _T>                        \
        [[nodiscard]] const _T* get() const noexcept \
        { return reinterpret_cast<_T*>(raw); }       \
    }

namespace tau {

struct TIPDefault final { constexpr TIPDefault() noexcept = default; };
struct TIPRecommended final { constexpr TIPRecommended() noexcept = default; };

static constexpr TIPDefault Default { };
static constexpr const TIPDefault& def = Default;
static constexpr TIPRecommended Recommended { };
static constexpr const TIPRecommended& rec = Recommended;

}
