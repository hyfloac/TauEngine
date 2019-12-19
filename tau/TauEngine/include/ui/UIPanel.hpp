#pragma once

#include "UIElement.hpp"

class UIPanel : UIElement
{
    DEFAULT_DESTRUCT_VI(UIPanel);
    UI_ELEMENT_IMPL(UIPanel);
protected:
public:
    UIPanel(u32 x, u32 y, UIElement* parent = nullptr, bool visible = true) noexcept
        : UIElement(x, y, parent, visible)
    { }
};
