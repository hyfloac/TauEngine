#include <GLFW/glfw3.h>
#include <NumTypes.hpp>
#include <cstdio>

#include "VulkanApplication.hpp"

int main(int argCount, char* args[])
{
    VulkanApplication* app = new VulkanApplication(800, 600);

    if(app->initVulkan() != VK_SUCCESS)
    {
        delete app;
        printf("Failed to create Vulkan instance.\n");
    }

    app->mainLoop();

    delete app;

    // glfwInit();
    //
    // glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan Test", nullptr, nullptr);
    //
    // u32 extensionCount = 0;
    // vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    //
    // printf("# Extensions Supported: %u\n", extensionCount);
    //
    // while(!glfwWindowShouldClose(window))
    // {
    //     glfwPollEvents();
    // }
    //
    // glfwDestroyWindow(window);
    //
    // glfwTerminate();

    return 0;
}
