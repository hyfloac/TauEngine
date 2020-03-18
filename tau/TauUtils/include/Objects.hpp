#pragma once

#define DELETE_COPY(_TYPE) private: \
                               inline _TYPE(const _TYPE& copy) noexcept = delete;            \
                               inline _TYPE(_TYPE&& move) noexcept = delete;                 \
                               inline _TYPE& operator=(const _TYPE& copy) noexcept = delete; \
                               inline _TYPE& operator=(_TYPE&& move) noexcept = delete

#define DEFAULT_COPY(_TYPE) public: \
                                inline _TYPE(const _TYPE& copy) noexcept = default;            \
                                inline _TYPE(_TYPE&& move) noexcept = default;                 \
                                inline _TYPE& operator=(const _TYPE& copy) noexcept = default; \
                                inline _TYPE& operator=(_TYPE&& move) noexcept = default

#define DELETE_DESTRUCT(_TYPE) public: inline ~_TYPE() noexcept = delete
#define DEFAULT_DESTRUCT(_TYPE) public: inline ~_TYPE() noexcept = default
#define DEFAULT_DESTRUCT_VI(_TYPE) public: inline virtual ~_TYPE() noexcept = default
#define DELETE_CONSTRUCT(_TYPE) private: inline _TYPE() noexcept = delete
#define DEFAULT_CONSTRUCT(_TYPE) inline _TYPE() noexcept = default
#define DEFAULT_CONSTRUCT_PU(_TYPE) public: DEFAULT_CONSTRUCT(_TYPE)
#define DEFAULT_CONSTRUCT_PI(_TYPE) private: DEFAULT_CONSTRUCT(_TYPE)
#define DEFAULT_CONSTRUCT_PO(_TYPE) protected: DEFAULT_CONSTRUCT(_TYPE)

namespace tau {

struct TIPDefault final { constexpr TIPDefault() noexcept = default; };
struct TIPRecommended final { constexpr TIPRecommended() noexcept = default; };

static constexpr TIPDefault Default { };
static constexpr const TIPDefault& def = Default;
static constexpr TIPRecommended Recommended { };
static constexpr const TIPRecommended& rec = Recommended;

}
