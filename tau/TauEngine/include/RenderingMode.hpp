#pragma once

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

#include <NumTypes.hpp>
#include <Objects.hpp>

class RenderingMode final
{
    DEFAULT_DESTRUCT(RenderingMode);
    DEFAULT_COPY(RenderingMode);
public:
    /**
     * Describes the various possible rendering modes.
     *
     *   Not all modes are listed, and unlisted mode doesn't
     * add any new features worth supporting, thus specific
     * support for it was skipped.
     */
    enum Mode : u8
    {
        DirectX9 = 0,
        DirectX10,
        DirectX11,
        DirectX12,
        DirectX12_1,
        Vulkan,
        OpenGL2,
        OpenGL3,
        OpenGL3_1,
        OpenGL3_2,
        OpenGL3_3,
        OpenGL4,
        OpenGL4_2,
        OpenGL4_3,
        OpenGL4_4,
        OpenGL4_5,
        OpenGL4_6,
    };

    typedef void(__cdecl * modeChange_f)(Mode oldMode, bool oldDebugMode, Mode newMode, bool newDebugMode);

    static RenderingMode& getGlobalMode() noexcept
    {
        static RenderingMode _currentGlobalMode(RenderingMode::Mode::OpenGL3);
        return _currentGlobalMode; 
    }
private:
    Mode _currentMode;
    bool _debugMode;
    std::vector<modeChange_f> _changeHandlers;
public:
    RenderingMode(Mode initialMode, bool debugMode = false) noexcept
        : _currentMode(initialMode), _debugMode(debugMode)
    { }

    [[nodiscard]] inline Mode currentMode() const noexcept { return _currentMode; }
    inline operator Mode() const noexcept { return _currentMode; }

    [[nodiscard]] inline bool debugMode() const noexcept { return _debugMode; }

    void setMode(Mode mode) noexcept
    {
        for(auto handler : _changeHandlers)
        { handler(_currentMode, _debugMode, mode, _debugMode); }

        _currentMode = mode;
    }

    void setDebugMode(bool debugMode) noexcept
    {
        for(auto handler : _changeHandlers)
        { handler(_currentMode, _debugMode, _currentMode, debugMode); }

        _debugMode = debugMode;
    }

    void setMode(Mode mode, bool debugMode) noexcept
    {
        for(auto handler : _changeHandlers)
        { handler(_currentMode, _debugMode, mode, debugMode); }

        _currentMode = mode;
        _debugMode = debugMode;
    }

    void registerChangeHandler(modeChange_f handler) noexcept
    {
        _changeHandlers.push_back(handler);
    }

    void unregisterChangeHandler(modeChange_f handler) noexcept
    {
        const auto it = std::find(_changeHandlers.begin(), _changeHandlers.end(), handler);
        if(it != _changeHandlers.end())
        { _changeHandlers.erase(it); }
    }
};

