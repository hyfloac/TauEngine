#include "ui/UIButton.hpp"
#include "events/WindowEvent.hpp"

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

UIRectButton::UIRectButton(IRenderingContext& context, clickHandler_f clickHandler, u32 x, u32 y, u32 width, u32 height, Vector3f color, UIElement* parent, bool visible) noexcept
    : UIButton(clickHandler, x, y, parent, visible), _width(width), _height(height), _color(color),
      _vbo(nullptr),
      _vao(context.createVertexArray(1, DrawType::SeparatedTriangles))
{
    const float xx = static_cast<float>(x);
    const float yy = static_cast<float>(y);
    const float ww = static_cast<float>(width);
    const float hh = static_cast<float>(height);

    float model[] = {
        xx, yy + hh,
        xx, yy,
        xx + ww, yy,

        xx, yy + hh,
        xx + ww, yy,
        xx + ww, yy + hh
    };

    Ref<IBufferBuilder> vboBuilder = context.createBuffer(1);
    vboBuilder->type(EBuffer::Type::ArrayBuffer);
    vboBuilder->usage(EBuffer::UsageType::StaticDraw);
    vboBuilder->bufferSize(sizeof(model));
    vboBuilder->descriptor().addDescriptor(ShaderDataType::Vector2Float);

    _vbo->bind(context);
    _vbo->fillBuffer(context, model);
    _vbo->unbind(context);
    _vao->addVertexBuffer(context, _vbo);
    _vao->drawCount() = 6;
}

void UIRectButton::onRender(float delta) noexcept
{
    // glm::vec3 pos(_x, _y, 0);
    // glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), pos);
    
}
