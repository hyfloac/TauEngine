#include "Window.hpp"
#include "Renderer.hpp"
#include "Utils.hpp"
#include "DeltaTime.hpp"

void __cdecl onResize(UINT width, UINT height, void* userParam);
int gameLoop(UINT targetUPS, Renderer& renderer, const Window& window);
u64 microTime() noexcept;

int main(int argCount, char* args[])
{
    UNUSED2(argCount, args);

    Renderer renderer;

    Window window(720, 720, &renderer);
    window.create();
    window.show();
    window.setOnResize(onResize);

    RetCode ret = renderer.loadPipeline(window);
    if(ret != RC_Success)
    {
        if(ret != RC_ErrorReported)
        {
            MessageBoxW(NULL, L"Unknown error occurred while loading pipeline.", NULL, MB_OK | MB_ICONERROR);
        }
        return 1;
    }

    ret = renderer.loadAssets(window);
    if(ret != RC_Success)
    {
        if(ret != RC_ErrorReported)
        {
            MessageBoxW(NULL, L"Unknown error occurred while loading assets.", NULL, MB_OK | MB_ICONERROR);
        }
        return 1;
    }

    gameLoop(128, renderer, window);

    MSG msg;
    while(!window.shouldClose())
    {
        while(PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        ret = renderer.render();
        if(ret != RC_Success)
        {
            if(ret != RC_ErrorReported)
            {
                MessageBoxW(NULL, L"Unknown error occurred while rendering.", NULL, MB_OK | MB_ICONERROR);
            }
            return 1;
        }
    }

    return 0;
}

void __cdecl onResize(const UINT width, const UINT height, void* const userParam)
{
    Renderer* const renderer = reinterpret_cast<Renderer*>(userParam);
    renderer->onWindowSizeChange(width, height);
}

int gameLoop(const UINT targetUPS, Renderer& renderer, const Window& window)
{
    const float Mu_PER_UPDATE = 1000000.0f / targetUPS;
    u64 lastTime = microTime();
    float lag = 0.0f;

    u64 counterTime = lastTime;
    u32 fps = 0;
    u32 ups = 0;

    DeltaTime deltaTime;

    MSG msg;

    while(!window.shouldClose())
    {
        const u64 currentTime = microTime();
        const u64 elapsed = currentTime - lastTime;
        lastTime = currentTime;
        lag += static_cast<float>(elapsed);

        while(lag >= Mu_PER_UPDATE)
        {
            deltaTime.onUpdate();

            while(PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }

            const RetCode ret = renderer.update();
            if(ret != RC_Success)
            {
                if(ret != RC_ErrorReported)
                {
                    MessageBoxW(NULL, L"Unknown error occurred while updating.", NULL, MB_OK | MB_ICONERROR);
                }
                return 1;
            }
            lag -= Mu_PER_UPDATE;
            ++ups;
        }

        if(elapsed != 0)
        {
            deltaTime.setDeltaMicro(elapsed);

            const RetCode ret = renderer.render();
            if(ret != RC_Success)
            {
                if(ret != RC_ErrorReported)
                {
                    MessageBoxW(NULL, L"Unknown error occurred while rendering.", NULL, MB_OK | MB_ICONERROR);
                }
                return 1;
            }
            ++fps;
        }

        if(currentTime - counterTime >= 1000000)
        {
            counterTime = currentTime;

            printf("FPS/UPS: %d/%d\n", fps, ups);

            ups = 0;
            fps = 0;
        }
    }

    return 0;
}

u64 microTime() noexcept
{
#ifdef _WIN32
    /* Windows */
    FILETIME fileTime;
    ULARGE_INTEGER fileTimeInt;

    /* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
     * to a LARGE_INTEGER structure. */
    GetSystemTimeAsFileTime(&fileTime);
    fileTimeInt.LowPart = fileTime.dwLowDateTime;
    fileTimeInt.HighPart = fileTime.dwHighDateTime;

    u64 ret = static_cast<u64>(fileTimeInt.QuadPart);
    ret /= 10; /* From 100 nano seconds (10^-7) to microseconds (10^-6) intervals */

    return ret;
#else
    /* Linux */
    struct timeval timeVal;

    gettimeofday(&timeVal, null);

    return timeVal.tv_usec;
#endif
}
