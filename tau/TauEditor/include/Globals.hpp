#pragma once

#include "State.hpp"
#include <openvr.h>

class Window;
class IGraphicsInterface;
class IRenderingContext;
class GameRecorder;
class VRFreeCamCamera3DController;
class TERenderer;

struct VRHandles final
{
    vr::VRActionHandle_t moveUp;
    vr::VRActionHandle_t moveDown;
    vr::VRActionHandle_t movePlane;
    vr::VRActionHandle_t triggerLeft;
    vr::VRActionHandle_t triggerRight;
    vr::VRActionHandle_t hapticLeft;
    vr::VRActionHandle_t hapticRight;

    vr::VRActionSetHandle_t actionSet;

    vr::VRInputValueHandle_t sourceLeft;
    vr::VRInputValueHandle_t sourceRight;

    VRHandles() noexcept
        : moveUp(vr::k_ulInvalidActionHandle), moveDown(vr::k_ulInvalidActionHandle),
          movePlane(vr::k_ulInvalidActionHandle),
          triggerLeft(vr::k_ulInvalidActionHandle), triggerRight(vr::k_ulInvalidActionHandle),
          hapticLeft(vr::k_ulInvalidActionHandle), hapticRight(vr::k_ulInvalidActionHandle),
          actionSet(vr::k_ulInvalidActionSetHandle),
          sourceLeft(vr::k_ulInvalidInputValueHandle), sourceRight(vr::k_ulInvalidInputValueHandle)
    { }
};

struct Globals final
{
    Window& window;
    IGraphicsInterface& gi;
    IRenderingContext& rc;
    GameRecorder& gr;
    vr::IVRSystem* vr;
    VRFreeCamCamera3DController* vrCamera;
    VRHandles vrHandles;
    State gameState;
    TERenderer* renderer;

    bool currentLeftEye;

    Globals(Window& window, IGraphicsInterface& gi, IRenderingContext& rc, GameRecorder& gr, const State gameState) noexcept
        : window(window),
          gi(gi), rc(rc), gr(gr), vr(null), vrCamera(null), vrHandles(),
          gameState(gameState), currentLeftEye(false), renderer(null)
    { }
};
