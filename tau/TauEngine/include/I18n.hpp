#pragma once

#pragma warning(push, 0)
#include <unordered_map>
#include <memory>
#pragma warning(pop)
#include <String.hpp>
#include <DLL.hpp>

class TAU_DLL I18n
{
private:
    std::unordered_map<String, std::shared_ptr<const char>> _translations;
    const char* _language;
public:
    enum ErrorCode : u8
    {
        SUCCESS = 0,
        UNKNOWN_TRANSLATION_KEY,
    };
public:
    I18n() noexcept;

    ~I18n() noexcept;

    void loadTranslations(const char* fileName) noexcept;

    std::shared_ptr<const char> translate(String key, ErrorCode* success = null) const noexcept;

    inline std::shared_ptr<const char> operator [](String key) const noexcept { return translate(key); }

    inline const std::unordered_map<String, std::shared_ptr<const char>>& operator()() const noexcept { return _translations; }

    inline const char* language() const noexcept { return _language; }
};
