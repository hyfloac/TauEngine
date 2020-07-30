#pragma once

#pragma warning(push, 0)
#include <unordered_map>
#pragma warning(pop)

#include <Objects.hpp>
#include <IFile.hpp>
#include <String.hpp>

#include "DLL.hpp"

class TAU_DLL I18n final
{
    DEFAULT_CONSTRUCT_PU(I18n);
    DEFAULT_DESTRUCT(I18n);
    DEFAULT_CM_PU(I18n);
public:
    using I18nMap = ::std::unordered_map<DynString, WDynString>;
private:
    I18nMap _translations;
    WDynString _language;
public:
    enum class Error
    {
        NoError = 0,
        UnknownTranslationKey
    };
public:
    [[nodiscard]] inline const WDynString& language() const noexcept { return _language; }
    [[nodiscard]] inline const I18nMap& operator()() const noexcept { return _translations; }

    [[nodiscard]] inline const WDynString& operator[](const DynString& key) const noexcept { return translate(key); }

    bool loadTranslations(const CPPRef<IFile>& file) noexcept;

    [[nodiscard]] const WDynString& translate(const DynString& key, [[tau::out]] Error* error = null) const noexcept;
};
