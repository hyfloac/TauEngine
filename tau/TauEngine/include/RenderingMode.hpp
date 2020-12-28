#pragma once

#include <NumTypes.hpp>
#include <Objects.hpp>
#include <ArrayList.hpp>

class RenderingMode final
{
    DEFAULT_DESTRUCT(RenderingMode);
    DEFAULT_CM_PU(RenderingMode);
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
        DirectX10 = 0,
        DirectX11,
        DirectX12,
        DirectX12_1,
        Vulkan,
        OpenGL4_1,
        OpenGL4_2,
        OpenGL4_3,
        OpenGL4_4,
        OpenGL4_5,
        OpenGL4_6,
        MAX_VALUE = OpenGL4_6
    };

    typedef void(__cdecl * modeChange_f)(Mode oldMode, bool oldDebugMode, Mode newMode, bool newDebugMode);

    static RenderingMode& getGlobalMode() noexcept
    {
        static RenderingMode _currentGlobalMode(RenderingMode::Mode::OpenGL4_2);
        return _currentGlobalMode; 
    }
private:
    Mode _currentMode;
    bool _debugMode;
    ArrayList<modeChange_f> _changeHandlers;
public:
    RenderingMode(const Mode initialMode, const bool debugMode = false) noexcept
        : _currentMode(initialMode)
        , _debugMode(debugMode)
        , _changeHandlers(4096)
    { }

    [[nodiscard]] inline Mode currentMode() const noexcept { return _currentMode; }
    inline operator Mode() const noexcept { return _currentMode; }

    [[nodiscard]] inline bool debugMode() const noexcept { return _debugMode; }

    void setMode(const Mode mode) noexcept
    {
        for(auto handler : _changeHandlers)
        { handler(_currentMode, _debugMode, mode, _debugMode); }

        _currentMode = mode;
    }

    void setDebugMode(const bool debugMode) noexcept
    {
        for(auto handler : _changeHandlers)
        { handler(_currentMode, _debugMode, _currentMode, debugMode); }

        _debugMode = debugMode;
    }

    void setMode(const Mode mode, const bool debugMode) noexcept
    {
        for(auto handler : _changeHandlers)
        { handler(_currentMode, _debugMode, mode, debugMode); }

        _currentMode = mode;
        _debugMode = debugMode;
    }

    void registerChangeHandler(const modeChange_f handler) noexcept
    {
        _changeHandlers.add(handler);
    }

    void unregisterChangeHandler(const modeChange_f handler) noexcept
    {
        const auto it = std::find(_changeHandlers.begin(), _changeHandlers.end(), handler);
        if(it != _changeHandlers.end())
        { _changeHandlers.removeFast(it.index()); }
    }

    [[nodiscard]] inline bool isOpenGL() const noexcept
    {
        switch(_currentMode)
        {
            case Mode::OpenGL4_1:
            case Mode::OpenGL4_2:
            case Mode::OpenGL4_3:
            case Mode::OpenGL4_4:
            case Mode::OpenGL4_5:
            case Mode::OpenGL4_6:
                return true;
            default: return false;
        }
    }

    [[nodiscard]] inline bool isVulkan() const noexcept
    {
        switch(_currentMode)
        {
            case Mode::Vulkan:
                return true;
            default: return false;
        }
    }

    [[nodiscard]] inline bool isDirectX() const noexcept
    {
        switch(_currentMode)
        {
            case DirectX10:
            case DirectX11:
            case DirectX12:
            case DirectX12_1:
                return true;
            default: return false;
        }
    }
};

