#pragma once

#include "State.hpp"

class Window;
class IGraphicsInterface;
class IRenderingContext;
class GameRecorder;

struct Globals final
{
    Window& window;
    IGraphicsInterface& gi;
    IRenderingContext& rc;
    GameRecorder& gr;
    State gameState;


    Globals(Window& window, IGraphicsInterface& gi, IRenderingContext& rc, GameRecorder& gr, const State gameState)
        : window(window),
          gi(gi),
          rc(rc),
          gr(gr),
          gameState(gameState)
    {
    }
};
