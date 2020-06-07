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

void I18n::loadTranslations(const CPPRef<IFile>& file) noexcept
{
    LangHeader header;
    file->readType(&header);

    if(header.magic == 0x4C614E67)
    {
        char* languageName = new char[header.languageNameLength + 1];
        file->readString(languageName, header.languageNameLength);
        languageName[header.languageNameLength] = '\0';

        _language = DynString::passControl(languageName);

        for(u32 i = 0; i < header.translationCount; ++i)
        {
            TranslationHeader tHeader;
            file->readType(&tHeader);

            char* key = new char[tHeader.keyLength + 1];
            file->readString(key, tHeader.keyLength);
            key[tHeader.keyLength] = '\0';

            char* value = new char[tHeader.keyLength + 1];
            file->readString(value, tHeader.valueLength);
            value[tHeader.valueLength] = '\0';

            _translations.insert({ DynString::passControl(key), DynString::passControl(value) });
        }
    }
}

const DynString& I18n::translate(const DynString& key, Error* const error) const noexcept
{
    if(this->_translations.find(key) != this->_translations.end())
    {
        const DynString& translation = _translations.at(key);
        ERROR_CODE_V(Error::NoError, translation);
    }

    ERROR_CODE_V(Error::UnknownTranslationKey, key);
}
