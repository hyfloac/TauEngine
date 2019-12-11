#include <GLFW/glfw3.h>
#include <NumTypes.hpp>
#include <cstdio>

#include "VulkanApplication.hpp"

#if defined(_DEBUG)
  #define USE_VALIDATION_LAYERS 1
#else
  #define USE_VALIDATION_LAYERS 0
#endif

int main(int argCount, char* args[])
{
    VulkanApplication* app = new VulkanApplication(800, 600, USE_VALIDATION_LAYERS);

    VkResult vkRes;
    VulkanApplication::Error vaError = app->initVulkan(&vkRes);
    if(vaError != VulkanApplication::Success)
    { 
        delete app;

        if(vaError == VulkanApplication::ValidationLayersUnavailable)
        {
            fprintf(stderr, "Validation layers were attempted to be used despite being unavailable.\n");
        }
        else if(vaError == VulkanApplication::VulkanError)
        {
            fprintf(stderr, "Failed to create Vulkan instance.\n");
        }
        return 1;
    }

    app->setupDebugMessenger(&vkRes);
    if(vkRes != VK_SUCCESS)
    {
        delete app;
        fprintf(stderr, "Failed to setup debug messenger.\n");
        return 1;
    }

    vaError = app->createSurface(&vkRes);
    if(vaError != VulkanApplication::Success)
    {
        delete app;

        if(vaError == VulkanApplication::VulkanError)
        {
            fprintf(stderr, "Failed to create Vulkan rendering surface.\n");
        }

        return 1;
    }

    vaError = app->selectPhysicalDevice();
    if(vaError != VulkanApplication::Success)
    {
        delete app;
        if(vaError == VulkanApplication::ZeroGraphicsDevices)
        {
            fprintf(stderr, "System does not have any graphics devices.\n");
        }
        else if(vaError == VulkanApplication::ZeroSuitableGraphicsDevices)
        {
            fprintf(stderr, "System does not have any _suitable_ graphics devices.\n");
        }
        return 1;
    }

    vaError = app->createLogicalDevice(&vkRes);
    if(vaError != VulkanApplication::Success)
    {
        delete app;

        if(vaError == VulkanApplication::VulkanError)
        {
            fprintf(stderr, "Failed to create Vulkan logical device.\n");
        }
        return 1;
    }

    app->mainLoop();

    delete app;

    return 0;
}
