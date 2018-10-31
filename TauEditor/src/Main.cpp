#include "Maths.hpp"
#include <cstdio>
#include <TauEngine.hpp>
#include <Window.hpp>

int main(int argCount, char* args[])
{
    printf("Starting Tau Engine Editor\n");

    Window window(800, 600, "Test Window");
    window.createWindow();
    window.showWindow();

    while(!tauRunMessageLoop())
    {
    }

    return 0;
}
