#include <I18n.hpp>
#include <cstdio>

I18n::I18n() noexcept
    : _language(null)
{ }

I18n::~I18n() noexcept
{
    delete _language;
    for(const auto& el : _translations)
    {
        delete el.first.c_str();
    }
}

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

void I18n::loadTranslations(const char* fileName) noexcept
{
    FILE* file;
    fopen_s(&file, fileName, "rbe");

    LangHeader header = { };
    fread(&header, sizeof(LangHeader), 1, file);

    if(header.magic == 0x4C614E67)
    {
        char* languageName = new char[header.languageNameLength + 1];
        fread(languageName, header.languageNameLength, 1, file);
        languageName[header.languageNameLength] = '\0';
        this->_language = languageName;

        for(u32 i = 0; i < header.translationCount; ++i)
        {
            TranslationHeader tHeader = { };
            fread(&tHeader, sizeof(TranslationHeader), 1, file);

            char* key = new char[tHeader.keyLength + 1];
            fread(key, tHeader.keyLength, 1, file);
            key[tHeader.keyLength] = '\0';

            char* value = new char[tHeader.keyLength + 1];
            fread(value, tHeader.valueLength, 1, file);
            value[tHeader.valueLength] = '\0';

            std::shared_ptr<char> y = std::shared_ptr<char>(value);

            this->_translations.insert({ key, *(std::shared_ptr<const char>*) &y });
        }
    }

    fclose(file);
}

std::shared_ptr<const char> I18n::translate(String key, ErrorCode* success) const noexcept
{
    if(this->_translations.find(key) != this->_translations.end())
    {
        if(success) { *success = SUCCESS; }
        return this->_translations.at(key);
    }

    if(success) { *success = UNKNOWN_TRANSLATION_KEY; }
    return std::shared_ptr<const char>(key.c_str());
}
