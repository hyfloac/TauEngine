#include "ui/UIButton.hpp"
#include "events/WindowEvent.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

bool UIButton::onClick(WindowMouseClickEvent& e) noexcept
{
    if(this->isMouseOver(e.x(), e.y(), e.window()))
    {
        this->_clickHandler(this, e.x(), e.y(), e.window());
    }
    return false;
}

void UIButton::onEvent(Event& e) noexcept
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowMouseClickEvent>(this, &UIButton::onClick);
}

bool UIRectButton::isMouseOver(u32 mouseX, u32 mouseY, Window& window) noexcept
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

void UIRectButton::onRender(float delta) noexcept
{
    // glm::vec3 pos(_x, _y, 0);
    // glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), pos);
    
}
