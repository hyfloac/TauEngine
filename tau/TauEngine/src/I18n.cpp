#include <I18n.hpp>

#pragma pack(push, 1)
struct LangHeader
{
    u32 magic;
    u32 translationCount;
    u32 languageNameLength;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct TranslationHeader
{
    u32 keyLength;
    u32 valueLength;
};
#pragma pack(pop)

bool I18n::loadTranslations(const CPPRef<IFile>& file) noexcept
{
    LangHeader header;
    file->readType(&header);

    if(header.magic == 0x4C614E67)
    {
        _translations.clear();

        wchar_t* languageName = new wchar_t[header.languageNameLength + 1];
        languageName[header.languageNameLength] = '\0';
        if(file->readString(languageName, header.languageNameLength) != header.languageNameLength * sizeof(wchar_t))
        { return false; }

        _language = WDynString::passControl(languageName);

        for(u32 i = 0; i < header.translationCount; ++i)
        {
            TranslationHeader tHeader;
            if(file->readType(&tHeader) != sizeof(tHeader))
            { return false; }

            char* key = new char[tHeader.keyLength + 1];
            key[tHeader.keyLength] = '\0';
            if(file->readString(key, tHeader.keyLength) != tHeader.keyLength * sizeof(char))
            { return false; }

            wchar_t* value = new wchar_t[tHeader.valueLength + 1];
            value[tHeader.valueLength] = '\0';
            if(file->readString(value, tHeader.valueLength) != tHeader.valueLength * sizeof(wchar_t))
            { return false; }

            _translations.emplace(DynString::passControl(key), WDynString::passControl(value));
        }
    }

    return true;
}

const WDynString& I18n::translate(const DynString& key, Error* const error) const noexcept
{
    if(this->_translations.find(key) != this->_translations.end())
    {
        const WDynString& translation = _translations.at(key);
        ERROR_CODE_V(Error::NoError, translation);
    }

    ERROR_CODE_V(Error::UnknownTranslationKey, WDynString());
}
