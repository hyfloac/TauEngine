#pragma once

#include "UIElement.hpp"
#include "maths/Vector3f.hpp"
#include "model/BufferDescriptor.hpp"
#include "model/IBuffer.hpp"
#include "model/IVertexArray.hpp"
#include "system/RenderingContext.hpp"

class Window;
class WindowMouseClickEvent;

class UIButton : public UIElement
{
    DEFAULT_DESTRUCT_VI(UIButton);
    UI_ELEMENT_IMPL(UIButton);
public:
    typedef void(__cdecl* clickHandler_f)(UIButton* button, u32 x, u32 y, Window& window);
protected:
    clickHandler_f _clickHandler;
protected:
    UIButton(clickHandler_f clickHandler, u32 x, u32 y, UIElement* parent = nullptr, bool visible = true) noexcept
        : UIElement(x, y, parent, visible), _clickHandler(clickHandler)
    { }
protected:
    [[nodiscard]] virtual bool isMouseOver(u32 mouseX, u32 mouseY, Window& window) noexcept = 0;
public:
    bool onClick(WindowMouseClickEvent& e) noexcept;

    virtual void onEvent(Event& e) noexcept override;
};

class UIRectButton : public UIButton
{
    DEFAULT_DESTRUCT_VI(UIRectButton);
    UI_ELEMENT_IMPL(UIRectButton);
protected:
    u32 _width;
    u32 _height;
    Vector3f _color;
    Ref<IBuffer> _vbo;
    Ref<IVertexArray> _vao;
protected:
    [[nodiscard]] virtual bool isMouseOver(u32 mouseX, u32 mouseY, Window& window) noexcept override;
public:
    UIRectButton(IRenderingContext& context, clickHandler_f clickHandler, u32 x, u32 y, u32 width, u32 height, Vector3f color, UIElement* parent = nullptr, bool visible = true) noexcept
        : UIButton(clickHandler, x, y, parent, visible), _width(width), _height(height), _color(color),
          _vbo(IBuffer::create(context, 1, IBuffer::Type::ArrayBuffer)), _vao(context.createVertexArray(1))
    {
        const float xx = static_cast<float>(x);
        const float yy = static_cast<float>(y);
        const float ww = static_cast<float>(width);
        const float hh = static_cast<float>(height);

        float model[] = {
            xx,      yy + hh,
            xx,      yy,
            xx + ww, yy,

            xx,      yy + hh,
            xx + ww, yy,
            xx + ww, yy + hh
        };

        _vbo->fillBuffer(context, 12 * sizeof(float), model);
        _vbo->descriptor().addDescriptor({ ShaderDataType::Vector2Float });
        _vao->addVertexBuffer(context, _vbo);
        _vao->drawCount() = 6;
        // _vao->addAttribute(_vbo, 2, DataType::Float, false, 0, nullptr);
    }

    [[nodiscard]] u32 width() const noexcept { return _width; }
    [[nodiscard]] u32& width() noexcept { return _width; }
    [[nodiscard]] u32 height() const noexcept { return _height; }
    [[nodiscard]] u32& height() noexcept { return _height; }
    [[nodiscard]] Vector3f color() const noexcept { return _color; }
    [[nodiscard]] Vector3f& color() noexcept { return _color; }

    void onRender(float delta) noexcept override;
};
