#define APP_IMPL
#include <EditorApplication.hpp>

TauEditorApplication::TauEditorApplication() noexcept
    : Application(32), _window(null)
{ }

TauEditorApplication::~TauEditorApplication() noexcept
{
    delete _window;
}

void TauEditorApplication::init(int argCount, char* args[]) noexcept
{
    
}

void TauEditorApplication::render(const float delta) noexcept
{
    
}

void TauEditorApplication::update(const float fixedDelta) noexcept
{
    
}

void TauEditorApplication::renderFPS(const u32 ups, const u32 fps) noexcept
{
    
}

Application* startGame() noexcept
{
    return new TauEditorApplication;
}
