#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <GLFW/glfw3.h>

class VulkanApplication
{
    DELETE_COPY(VulkanApplication);
private:
    GLFWwindow* _window;
    u32 _width;
    u32 _height;
    VkInstance _instance;
public:
    VulkanApplication(u32 width, u32 height) noexcept;

    ~VulkanApplication() noexcept;

    void mainLoop();

    VkResult initVulkan();
private:
    void initWindow();
};
