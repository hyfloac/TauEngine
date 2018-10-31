#include <TauEngine.hpp>
#include <Utils.hpp>

#ifdef _WIN32
#include <Windows.h>

bool tauRunMessageLoop() noexcept
{
    MSG msg;
    if(!GetMessageA(&msg, null, 0, 0)) { return true; }
    TranslateMessage(&msg);
    DispatchMessageA(&msg);

    return false;
}
#else
bool tauRunMessageLoop() noexcept
{
    return false;
}
#endif
