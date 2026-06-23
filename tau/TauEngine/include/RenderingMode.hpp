#pragma once

#include <NumTypes.hpp>
#include <Objects.hpp>
#include <ArrayList.hpp>
#include <algorithm>

class RenderingMode final
{
    DEFAULT_DESTRUCT(RenderingMode);
    DEFAULT_CM_PU(RenderingMode);
public:
    /**
     * Describes the various possible rendering modes.
     *
     *   Not all modes are listed, and unlisted modes don't
     * add any new features worth supporting, thus specific
     * support for it was skipped.
     */
    enum Mode : u8
    {
        DirectX10 = 0,
        DirectX11,
        DirectX12,
        DirectX12_1,
        Vulkan1_0,
        Vulkan1_1,
        Vulkan1_2,
        Vulkan1_3,
        Vulkan1_4,
        OpenGL4_1,
        OpenGL4_2,
        OpenGL4_3,
        OpenGL4_4,
        OpenGL4_5,
        OpenGL4_6,
        Metal,
        MAX_VALUE_INTERNAL,
        MAX_VALUE = MAX_VALUE_INTERNAL - 1
    };

    typedef void(__cdecl * modeChange_f)(Mode oldMode, bool oldDebugMode, Mode newMode, bool newDebugMode);

    static RenderingMode& getGlobalMode() noexcept
    {
        static RenderingMode _currentGlobalMode(RenderingMode::Mode::OpenGL4_2);
        return _currentGlobalMode; 
    }
private:
    Mode m_CurrentMode;
    bool m_DebugMode;
    ArrayList<modeChange_f> m_ChangeHandlers;
public:
    RenderingMode(const Mode initialMode, const bool debugMode = false) noexcept
        : m_CurrentMode(initialMode)
        , m_DebugMode(debugMode)
        , m_ChangeHandlers(4096)
    { }

    [[nodiscard]] Mode currentMode() const noexcept { return m_CurrentMode; }
    operator Mode() const noexcept { return m_CurrentMode; }

    [[nodiscard]] bool debugMode() const noexcept { return m_DebugMode; }

    void setMode(const Mode mode) noexcept
    {
        for(const auto handler : m_ChangeHandlers)
        {
            handler(m_CurrentMode, m_DebugMode, mode, m_DebugMode);
        }

        m_CurrentMode = mode;
    }

    void setDebugMode(const bool debugMode) noexcept
    {
        for(const auto handler : m_ChangeHandlers)
        {
            handler(m_CurrentMode, m_DebugMode, m_CurrentMode, debugMode);
        }

        m_DebugMode = debugMode;
    }

    void setMode(const Mode mode, const bool debugMode) noexcept
    {
        for(const auto handler : m_ChangeHandlers)
        {
            handler(m_CurrentMode, m_DebugMode, mode, debugMode);
        }

        m_CurrentMode = mode;
        m_DebugMode = debugMode;
    }

    void registerChangeHandler(const modeChange_f handler) noexcept
    {
        m_ChangeHandlers.add(handler);
    }

    void unregisterChangeHandler(const modeChange_f handler) noexcept
    {
        const auto it = std::find(m_ChangeHandlers.begin(), m_ChangeHandlers.end(), handler);
        if(it != m_ChangeHandlers.end())
        {
            m_ChangeHandlers.removeFast(it.index());
        }
    }

    [[nodiscard]] bool isOpenGL() const noexcept
    {
        switch(m_CurrentMode)
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

    [[nodiscard]] bool isVulkan() const noexcept
    {
        switch(m_CurrentMode)
        {
            case Mode::Vulkan1_0:
            case Mode::Vulkan1_1:
            case Mode::Vulkan1_2:
            case Mode::Vulkan1_3:
            case Mode::Vulkan1_4:
                return true;
            default: return false;
        }
    }

    [[nodiscard]] bool isDirectX() const noexcept
    {
        switch(m_CurrentMode)
        {
            case DirectX10:
            case DirectX11:
            case DirectX12:
            case DirectX12_1:
                return true;
            default: return false;
        }
    }

    [[nodiscard]] bool isMetal() const noexcept
    {
        switch(m_CurrentMode)
        {
            case Metal:
                return true;
            default: return false;
        }
    }
};

