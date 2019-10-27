#pragma once

#include "UIElement.hpp"

class UILayer : UIElement
{
    DEFAULT_DESTRUCT_VI(UILayer);
    UI_ELEMENT_IMPL(UILayer);
protected:
public:
    UILayer(u32 x, u32 y, UIElement* parent = nullptr, bool visible = true) noexcept
        : UIElement(x, y, parent, visible)
    { }
};
