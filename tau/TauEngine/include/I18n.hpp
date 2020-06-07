#pragma once

#pragma warning(push, 0)
#include <unordered_map>
#pragma warning(pop)

#include <Objects.hpp>
#include <IFile.hpp>

#include "String.hpp"
#include "DLL.hpp"

class TAU_DLL I18n final
{
    DEFAULT_CONSTRUCT_PU(I18n);
    DEFAULT_DESTRUCT(I18n);
    DEFAULT_COPY(I18n);
public:
    using I18nMap = ::std::unordered_map<DynString, DynString>;
private:
    I18nMap _translations;
    DynString _language;
public:
    enum class Error
    {
        NoError = 0,
        UnknownTranslationKey
    };
public:
    [[nodiscard]] inline const char* language() const noexcept { return _language; }
    [[nodiscard]] inline const I18nMap& operator()() const noexcept { return _translations; }

    [[nodiscard]] inline const DynString& operator[](const DynString& key) const noexcept { return translate(key); }

    void loadTranslations(const CPPRef<IFile>& file) noexcept;

    [[nodiscard]] const DynString& translate(const DynString& key, [[tau::out]] Error* error = null) const noexcept;
};
