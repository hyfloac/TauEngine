#pragma once

#include <spdlog/spdlog.h>
#include <Application.hpp>
#include <system/Window.hpp>
#include <NumTypes.hpp>
#include <events/WindowEvent.hpp>
#include <system/RenderingContext.hpp>
#include <Objects.hpp>
#include "Config.h"
#include <shader/IShaderProgram.hpp>
#include <model/VertexArray.hpp>
#include <shader/Uniform.hpp>
#include <Color.hpp>
#include <glm/mat4x4.hpp>
#include <camera/Camera2D.hpp>
#include <DirectXMath.h>
#include <shader/TextureUploader.hpp>

static void onWindowEvent(void* param, WindowEvent& e) noexcept;

class DX10Application final : public Application
{
    DELETE_COPY(DX10Application);
public:
    struct Uniforms final
    {
        RGBAColor color;

        Uniforms(const RGBAColor& color)
            : color(color)
        {
        }
    };
    struct Matrices final
    {
        glm::mat4 projection;
        glm::mat4 viewMatrix;
        glm::mat4 viewMatrixTrans;
        glm::mat4 model;
        DirectX::XMMATRIX dxProjection;
    };
private:
    static constexpr const char* CONFIG_PATH = "|game/config.bin";
    Config _config;
    Window* _window;
    Ref<spdlog::logger> _logger;
    u8 _r, _g, _b;
    int _colorState;
    u8 _aa;
    Ref<IShaderProgram> _shader;
    Ref<IVertexArray> _va;
    UniformBlockS<Uniforms>* _uni;
    UniformBlockS<Matrices>* _matrices;
    Camera2DController* _camera;
    Ref<ISingleTextureUploader> _texUploader;
public:
    DX10Application() noexcept;

    ~DX10Application() noexcept override;

    [[nodiscard]] Ref<spdlog::logger> logger() const noexcept { return _logger; }

    bool init(int argCount, char* args[]) noexcept override;

    void finalize() noexcept override;

    void onException(ExceptionData& ex) noexcept override;
protected:
    void update(float fixedDelta) noexcept override;

    void render(const DeltaTime& delta) noexcept override;

    void renderFPS(u32 ups, u32 fps) noexcept override;

    void runMessageLoop() noexcept override;
private:
    [[nodiscard]] const IRenderingContext& ctx() const noexcept { return *_window->renderingContext(); }
    [[nodiscard]] IRenderingContext& ctx() noexcept { return *_window->renderingContext(); }

    void setupConfig() noexcept;

    void writeConfig() noexcept;

    void onWindowEvent(WindowEvent& e) noexcept;

    bool onCharPress(WindowAsciiKeyEvent& e) noexcept;

    bool onKeyPress(WindowKeyEvent& e) noexcept;

    bool onWindowResize(WindowResizeEvent& e) const noexcept;

    void onIncorrectContext(IncorrectContextException& ex) const noexcept;
public:
    friend void onWindowEvent(void* param, WindowEvent& e) noexcept;
};
