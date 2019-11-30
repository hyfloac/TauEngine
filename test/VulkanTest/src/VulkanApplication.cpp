#include "VulkanApplication.hpp"
#include <DynArray.hpp>
#include <cstdio>

VulkanApplication::VulkanApplication(u32 width, u32 height) noexcept
    : _window(nullptr), _width(width), _height(height), _instance(nullptr)
{
    initWindow();
}

VulkanApplication::~VulkanApplication() noexcept
{
    if(_instance)
    {
        vkDestroyInstance(_instance, nullptr);
    }

    glfwDestroyWindow(_window);
    glfwTerminate();
}

void VulkanApplication::mainLoop()
{
    while(!glfwWindowShouldClose(_window))
    {
        glfwPollEvents();
    }
}

void VulkanApplication::initWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _window = glfwCreateWindow(_width, _height, "Vulkan Test", nullptr, nullptr);
}

static void printExtensions(const DynArray<VkExtensionProperties>& ext) noexcept;
static void printExtensions(u32 count, const char** ext) noexcept;

VkResult VulkanApplication::initVulkan()
{
    VkApplicationInfo appInfo = { };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Test";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    u32 glfwExtCount;
    const char** glfwExt = glfwGetRequiredInstanceExtensions(&glfwExtCount);

    printExtensions(glfwExtCount, glfwExt);

    VkInstanceCreateInfo createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtCount;
    createInfo.ppEnabledExtensionNames = glfwExt;
    createInfo.enabledLayerCount = 0;

    u32 vulkanExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &vulkanExtensionCount, nullptr);

    DynArray<VkExtensionProperties> vulkanExt(vulkanExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &vulkanExtensionCount, vulkanExt.arr());

    printExtensions(vulkanExt);

    return vkCreateInstance(&createInfo, nullptr, &_instance);
}

static void printExtensions(const DynArray<VkExtensionProperties>& ext) noexcept
{
    printf("Vulkan Extensions:\n");

    for(u32 i = 0; i < ext.count(); ++i)
    {
        printf("\t%s\n", ext[i].extensionName);
    }
}

static void printExtensions(u32 count, const char** ext) noexcept
{
    printf("GLFW Required Extensions:\n");

    for(u32 i = 0; i < count; ++i)
    {
        printf("\t%s\n", ext[i]);
    }
}
