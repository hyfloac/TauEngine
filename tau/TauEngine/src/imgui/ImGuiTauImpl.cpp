#include "imgui/ImGuiTauImpl.hpp"
#include "model/Buffer.hpp"
#include "model/VertexArray.hpp"
#include "system/RenderingContext.hpp"
#include "shader/IShaderProgram.hpp"
#include "events/WindowEvent.hpp"
#include "system/Keyboard.hpp"
#include <EnumBitFields.hpp>

void ImGuiTau::render(IRenderingContext& ctx, ImDrawData* const drawData) noexcept
{

    const float fbWidthF  = drawData->DisplaySize.x * drawData->FramebufferScale.x;
    const float fbHeightF = drawData->DisplaySize.y * drawData->FramebufferScale.y;
    if(fbWidthF <= 0.0f || fbHeightF <= 0.0f)
    {
        return;
    }

    const uSys fbWidth  = static_cast<uSys>(fbWidthF);
    const uSys fbHeight = static_cast<uSys>(fbHeightF);

    const ImVec2 clipOffset = drawData->DisplayPos;
    const ImVec2 clipScale = drawData->FramebufferScale;

    initBuffers(ctx, drawData);

    _shader->bind(ctx);

    for(uSys i = 0; i < static_cast<uSys>(drawData->CmdListsCount); ++i)
    {
        const ImDrawList* const cmdList = drawData->CmdLists[i];

        _vertexBuffer->beginModification(ctx);
        _vertexBuffer->modifyBuffer(0, cmdList->VtxBuffer.Size * sizeof(ImDrawVert), cmdList->VtxBuffer.Data);
        _vertexBuffer->endModification(ctx);

        _indexBuffer->beginModification(ctx);
        _indexBuffer->modifyBuffer(0, cmdList->IdxBuffer.Size * sizeof(ImDrawIdx), cmdList->IdxBuffer.Data);
        _indexBuffer->endModification(ctx);

        for(uSys j = 0; j < static_cast<uSys>(cmdList->CmdBuffer.Size); ++j)
        {
            const ImDrawCmd& drawCmd = cmdList->CmdBuffer[j];

            if(drawCmd.UserCallback)
            {
                if(drawCmd.UserCallback == ImDrawCallback_ResetRenderState)
                {
                    // TODO: Reset Render State
                }
                else
                {
                    drawCmd.UserCallback(cmdList, &drawCmd);
                }
            }
            else
            {
                ImVec4 clipRect;
                clipRect.x = (drawCmd.ClipRect.x - clipOffset.x) * clipScale.x;
                clipRect.y = (drawCmd.ClipRect.y - clipOffset.y) * clipScale.y;
                clipRect.z = (drawCmd.ClipRect.z - clipOffset.x) * clipScale.x;
                clipRect.w = (drawCmd.ClipRect.w - clipOffset.y) * clipScale.y;

                if(clipRect.x < fbWidth && clipRect.y < fbHeight && 
                   clipRect.z >= 0.0f   && clipRect.w >= 0.0f)
                {
                    // TODO: Setup Scissor Test
                }

                drawCmd.TextureId->bind(ctx, 0, EShader::Stage::Pixel);

                _va->draw(ctx, drawCmd.ElemCount, drawCmd.IdxOffset * sizeof(ImDrawIdx));
            }
        }
    }
}

void ImGuiTau::update() noexcept
{
    ImGuiIO& io = ImGui::GetIO();

    const Keyboard::Flags kFlags = Keyboard::keyboardFlags();
    io.KeyCtrl = static_cast<bool>(kFlags & Keyboard::Flags::Ctrl);
    io.KeyShift = static_cast<bool>(kFlags & Keyboard::Flags::Shift);
    io.KeyAlt = static_cast<bool>(kFlags & Keyboard::Flags::Alt);
    io.KeySuper = false;

    if(io.WantSetMousePos)
    {
        Mouse::mousePos(_window, Mouse::Pos { static_cast<u32>(io.MousePos.x), static_cast<u32>(io.MousePos.y) });
    }

    const Mouse::Pos mPos = Mouse::mousePos();
    io.MousePos.x = static_cast<float>(mPos.x);
    io.MousePos.y = static_cast<float>(mPos.y);
}

void ImGuiTau::onEvent(Event& e) noexcept
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowKeyEvent>(this, &ImGuiTau::onKeyPress);
    dispatcher.dispatch<WindowAsciiKeyEvent>(this, &ImGuiTau::onAsciiKeyPress);
    dispatcher.dispatch<WindowMouseClickEvent>(this, &ImGuiTau::onMouseClick);
}

void ImGuiTau::initBuffers(IRenderingContext& ctx, ImDrawData* const drawData) noexcept
{
    uSys maxVertexBufferSize = 0;
    uSys maxIndexBufferSize = 0;

    for(uSys i = 0; i < static_cast<uSys>(drawData->CmdListsCount); ++i)
    {
        const ImDrawList* cmdList = drawData->CmdLists[i];
        if(static_cast<uSys>(cmdList->VtxBuffer.Size) > maxVertexBufferSize)
        {
            maxVertexBufferSize = cmdList->VtxBuffer.Size;
        }
        if(static_cast<uSys>(cmdList->IdxBuffer.Size) > maxIndexBufferSize)
        {
            maxIndexBufferSize = cmdList->IdxBuffer.Size;
        }
    }

    bool createVA = false;

    if(!_vertexBuffer || _vertexBuffer->elementCount() < maxVertexBufferSize)
    {
        BufferArgs vArgs(3);
        vArgs.type = EBuffer::Type::ArrayBuffer;
        vArgs.usage = EBuffer::UsageType::StreamDraw;
        vArgs.elementCount = maxVertexBufferSize + 4096;
        vArgs.initialBuffer = nullptr;
        vArgs.instanced = false;
        vArgs.descriptor.addDescriptor(ShaderSemantic::Position, ShaderDataType::Vector2Float);
        vArgs.descriptor.addDescriptor(ShaderSemantic::TextureCoord, ShaderDataType::Vector2Float);
        vArgs.descriptor.addDescriptor(ShaderSemantic::Color, ShaderDataType::Vector4Float);

        _vertexBuffer = ctx.createBuffer().buildCPPRef(vArgs, null);
        createVA = true;
    }

    if(!_indexBuffer || _indexBuffer->bufferSize() < maxIndexBufferSize)
    {
        IndexBufferArgs iArgs;
        iArgs.usage = EBuffer::UsageType::StreamDraw;
        iArgs.elementCount = maxIndexBufferSize + 8192;
        iArgs.initialBuffer = nullptr;

        _indexBuffer = ctx.createIndexBuffer().buildCPPRef(iArgs, null);
        createVA = true;
    }

    if(createVA)
    {
        VertexArrayArgs vaArgs(1);
        vaArgs.shader = _vertexShader;
        vaArgs.buffers[0] = _vertexBuffer;
        vaArgs.indexBuffer = _indexBuffer;
        vaArgs.drawCount = 0;
        vaArgs.drawType = DrawType::SeparatedTriangles;

        _va = ctx.createVertexArray().buildCPPRef(vaArgs, null);
    }
}

void ImGuiTau::initKeys() noexcept
{
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.BackendPlatformName = "TauEngine";

    io.KeyMap[ImGuiKey_Tab] = static_cast<int>(Keyboard::Key::Tab);
    io.KeyMap[ImGuiKey_LeftArrow] = static_cast<int>(Keyboard::Key::Left);
    io.KeyMap[ImGuiKey_RightArrow] = static_cast<int>(Keyboard::Key::Right);
    io.KeyMap[ImGuiKey_UpArrow] = static_cast<int>(Keyboard::Key::Up);
    io.KeyMap[ImGuiKey_DownArrow] = static_cast<int>(Keyboard::Key::Down);
    io.KeyMap[ImGuiKey_PageUp] = static_cast<int>(Keyboard::Key::PageUp);
    io.KeyMap[ImGuiKey_PageDown] = static_cast<int>(Keyboard::Key::PageDown);
    io.KeyMap[ImGuiKey_Home] = static_cast<int>(Keyboard::Key::Home);
    io.KeyMap[ImGuiKey_End] = static_cast<int>(Keyboard::Key::End);
    io.KeyMap[ImGuiKey_Insert] = static_cast<int>(Keyboard::Key::Insert);
    io.KeyMap[ImGuiKey_Delete] = static_cast<int>(Keyboard::Key::Delete);
    io.KeyMap[ImGuiKey_Backspace] = static_cast<int>(Keyboard::Key::BackSpace);
    io.KeyMap[ImGuiKey_Space] = static_cast<int>(Keyboard::Key::Space);
    io.KeyMap[ImGuiKey_Enter] = static_cast<int>(Keyboard::Key::Enter);
    io.KeyMap[ImGuiKey_Escape] = static_cast<int>(Keyboard::Key::Esc);
    io.KeyMap[ImGuiKey_KeyPadEnter] = static_cast<int>(Keyboard::Key::Enter);
    io.KeyMap[ImGuiKey_A] = static_cast<int>(Keyboard::Key::A);
    io.KeyMap[ImGuiKey_C] = static_cast<int>(Keyboard::Key::C);
    io.KeyMap[ImGuiKey_V] = static_cast<int>(Keyboard::Key::V);
    io.KeyMap[ImGuiKey_X] = static_cast<int>(Keyboard::Key::X);
    io.KeyMap[ImGuiKey_Y] = static_cast<int>(Keyboard::Key::Y);
    io.KeyMap[ImGuiKey_Z] = static_cast<int>(Keyboard::Key::Z);
}

bool ImGuiTau::onKeyPress(WindowKeyEvent& e) noexcept
{
    ImGuiIO& io = ImGui::GetIO();

    if(e.event() == Keyboard::Event::Held)
    {
        return false;
    }

    const bool keyActive = e.event() != Keyboard::Event::Released;

    io.KeysDown[static_cast<int>(e.key())] = keyActive;

    return false;
}

bool ImGuiTau::onAsciiKeyPress(WindowAsciiKeyEvent& e) noexcept
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(e.wideChar());
    return false;
}

bool ImGuiTau::onMouseClick(WindowMouseClickEvent& e) noexcept
{
    ImGuiIO& io = ImGui::GetIO();

    switch(e.mouseEvent())
    {
        case Mouse::Event::LeftButtonDown:   io.MouseDown[0] = true;  break;
        case Mouse::Event::LeftButtonUp:     io.MouseDown[0] = false; break;
        case Mouse::Event::RightButtonDown:  io.MouseDown[1] = true;  break;
        case Mouse::Event::RightButtonUp:    io.MouseDown[1] = false; break;
        case Mouse::Event::MiddleButtonDown: io.MouseDown[2] = true;  break;
        case Mouse::Event::MiddleButtonUp:   io.MouseDown[2] = false; break;
        case Mouse::Event::XButton1Down:     io.MouseDown[3] = true;  break;
        case Mouse::Event::XButton1Up:       io.MouseDown[3] = false; break;
        case Mouse::Event::XButton2Down:     io.MouseDown[4] = true;  break;
        case Mouse::Event::XButton2Up:       io.MouseDown[4] = false; break;
        default: break;
    }

    return false;
}
