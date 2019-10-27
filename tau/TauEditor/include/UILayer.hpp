#pragma once

#include <layer/ILayer.hpp>
#include <vector>
#include <Objects.hpp>
#include <NumTypes.hpp>

class Button
{
protected:
    u32 _x;
    u32 _y;
    u32 _width;
    u32 _height;
public:
    Button(const u32 x, const u32 y, const u32 width, const u32 height)
        : _x(x),
          _y(y),
          _width(width),
          _height(height)
    {
    }

    bool clicked(u32 mouseX, u32 mouseY)
    {
        if(mouseX >= _x && mouseX <= (_x + _width))
        {
            if(mouseY >= _y && mouseY <= (_y + _height))
            {
                return true;
            }
        }
        return false;
    }
};

class UILayer final : public ILayer
{
    DEFAULT_DESTRUCT(UILayer);
    LAYER_IMPL(UILayer);
private:
    std::vector<Button*> _buttons;
public:
    UILayer() noexcept = default;

    void addButton(Button* button) noexcept
    {
        _buttons.push_back(button);
    }

    void onUpdate(float fixedDelta) noexcept override;

    void onRender(float delta) noexcept override;

    void onEvent(Event& e) noexcept override;
};
