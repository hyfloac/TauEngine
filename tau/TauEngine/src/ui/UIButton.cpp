#include "ui/UIButton.hpp"
#include "events/WindowEvent.hpp"

// static Ref<IInputLayout> _inputLayoutCache = null;

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
      // _vao(context.createVertexArray(1, DrawType::SeparatedTriangles))
      _vao(null)
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

    BufferArgs vboBuilder(1);
    // Ref<IBufferBuilder> vboBuilder = context.createBuffer(1);
    vboBuilder.type = EBuffer::Type::ArrayBuffer;
    vboBuilder.usage = EBuffer::UsageType::StaticDraw;
    vboBuilder.elementCount = 6;
    vboBuilder.descriptor.addDescriptor(ShaderSemantic::Position, ShaderDataType::Vector2Float);

    _vbo->bind(context);
    _vbo->fillBuffer(context, model);
    _vbo->unbind(context);

    // if(!_inputLayoutCache)
    // {
        // Ref<IInputLayoutBuilder> ilBuilder = context.createInputLayout(1);
        // ilBuilder->setLayoutDescriptor(0, ShaderDataType::Vector2Float, ShaderSemantic::Position);
        // _inputLayoutCache = Ref<IInputLayout>(ilBuilder->build());
    // }


    Ref<IVertexArrayBuilder> vaBuilder = context.createVertexArray(1);
    vaBuilder->setVertexBuffer(0, _vbo);
    // vaBuilder->inputLayout(_inputLayoutCache);
    vaBuilder->drawCount(6);
    vaBuilder->drawType(DrawType::SeparatedTriangles);

    _vao = Ref<IVertexArray>(vaBuilder->build());

    // _vao->addVertexBuffer(context, _vbo);
    // _vao->drawCount() = 6;
}

void UIRectButton::onRender(float delta) noexcept
{
    // glm::vec3 pos(_x, _y, 0);
    // glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), pos);
    
}
