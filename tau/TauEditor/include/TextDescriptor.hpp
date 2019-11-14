#pragma once

#include <NumTypes.hpp>
#include <Color.hpp>
#include <String.hpp>
#include <utility>

class TextDescriptor final
{
public:
    enum TextType : u8
    {
        Bold,
        Italic,
        BoldItalic
    };
private:
    RGBAColor _textColor;
    TextType _formatType;
    DynString _text;
    TextDescriptor* _next;
public:
    inline TextDescriptor(const RGBAColor& textColor, const TextType formatType, DynString text, TextDescriptor* parent = null) noexcept
        : _textColor(textColor), _formatType(formatType), _text(std::move(text)), _next(null)
    {
        if(parent)
        { parent->_next = this; }
    }

    inline ~TextDescriptor() noexcept
    { delete _next; }

    [[nodiscard]] inline RGBAColor textColor() const noexcept { return _textColor; }
    [[nodiscard]] inline TextType formatType() const noexcept { return _formatType; }
    [[nodiscard]] inline const DynString& text() const noexcept { return _text; }
    [[nodiscard]] inline const TextDescriptor* next() const noexcept { return _next; }
                   
    [[nodiscard]] inline RGBAColor& textColor() noexcept { return _textColor; }
    [[nodiscard]] inline TextType& formatType() noexcept { return _formatType; }
    [[nodiscard]] inline DynString& text() noexcept { return _text; }
    [[nodiscard]] inline TextDescriptor*& next() noexcept { return _next; }
};
