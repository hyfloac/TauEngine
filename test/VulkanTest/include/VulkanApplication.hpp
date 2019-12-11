#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <GLFW/glfw3.h>
#include "DynArray.hpp"
#include <optional>

class VulkanApplication
{
    DELETE_COPY(VulkanApplication);
public:
    enum Error
    {
        Success,
        ValidationLayersUnavailable,
        VulkanError,
        ZeroGraphicsDevices,
        ZeroSuitableGraphicsDevices
    };

    struct QueueFamilyIndices final
    {
        ::std::optional<u32> graphicsFamily;
        ::std::optional<u32> presentFamily;

        bool isComplete() const noexcept;
    };
private:
    u32 _width;
    u32 _height;
    bool _useValidationLayers;
    GLFWwindow* _window;
    VkInstance _instance;
    VkDebugUtilsMessengerEXT _debugMessenger;
    VkSurfaceKHR _surface;
    VkPhysicalDevice _physicalDevice;
    VkDevice _logicalDevice;
    VkQueue _graphicsQueue;
    VkQueue _presentQueue;
public:
    VulkanApplication(u32 width, u32 height, bool useValidationLayers) noexcept;

    ~VulkanApplication() noexcept;

    void mainLoop() noexcept;

    [[nodiscard]] Error initVulkan([[tau::out]] VkResult* vkRes) noexcept;

    void setupDebugMessenger([[tau::out]] VkResult* vkRes) noexcept;

    [[nodiscard]] Error selectPhysicalDevice() noexcept;

    [[nodiscard]] Error createLogicalDevice([[tau::out]] VkResult* vkRes) noexcept;

    [[nodiscard]] Error createSurface([[tau::out]] VkResult* vkRes) noexcept;
private:
    void initWindow() noexcept;

    [[nodiscard]] bool checkValidationLayerSupport() noexcept;

    [[nodiscard]] RefDynArray<const char*> getRequiredExtensions() const noexcept;

    [[nodiscard]] u32 rateDeviceSuitability(VkPhysicalDevice device) const noexcept;

    [[nodiscard]] bool checkDeviceExtensionSupport(VkPhysicalDevice device) const noexcept;

    [[nodiscard]] QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const noexcept;
};
