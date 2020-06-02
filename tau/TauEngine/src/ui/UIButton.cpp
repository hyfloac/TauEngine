#include "ui/UIButton.hpp"
#include "events/WindowEvent.hpp"
#include "system/GraphicsInterface.hpp"

// static CPPRef<IInputLayout> _inputLayoutCache = null;

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

UIRectButton::UIRectButton(IGraphicsInterface& gi, clickHandler_f clickHandler, u32 x, u32 y, u32 width, u32 height, Vector3f color, UIElement* parent, bool visible) noexcept
    : UIButton(clickHandler, x, y, parent, visible), _width(width), _height(height), _color(color),
      _vbo(nullptr), _vao(null)
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

    VertexBufferArgs vboBuilder(1);
    vboBuilder.type = EBuffer::Type::ArrayBuffer;
    vboBuilder.usage = EBuffer::UsageType::StaticDraw;
    vboBuilder.elementCount = 6;
    vboBuilder.initialBuffer = model;
    vboBuilder.descriptor.addDescriptor(ShaderSemantic::Position, ShaderDataType::Vector2Float);

    VertexArrayArgs vaArgs(1);
    vaArgs.buffers[0] = _vbo;
    vaArgs.drawCount = 6;
    vaArgs.drawType = DrawType::SeparatedTriangles;

    _vao = gi.createVertexArray().buildCPPRef(vaArgs, null);
}

void UIRectButton::onRender(float delta) noexcept
{ }
