#include "VulkanApplication.hpp"
#include <DynArray.hpp>
#include <cstdio>
#include <cstring>
#include <set>
#include "VulkanExtensions.h"

#define VALIDATION_LAYER_COUNT (1)
#define DEVICE_EXTENSION_COUNT (1)

static const char* VALIDATION_LAYERS[VALIDATION_LAYER_COUNT] = {
    "VK_LAYER_KHRONOS_validation"
};

static const char* DEVICE_EXTENSIONS[DEVICE_EXTENSION_COUNT] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

static void printExtensions(const DynArray<VkExtensionProperties>& ext) noexcept;
static void printExtensions(const RefDynArray<const char*>& ext) noexcept;
static void printLayers(const DynArray<VkLayerProperties>& layers) noexcept;

static VkDebugUtilsMessengerCreateInfoEXT getDebugCreateInfo(void* userParam) noexcept;

VulkanApplication::VulkanApplication(const u32 width, const u32 height, const bool useValidationLayers) noexcept
    : _width(width), _height(height), _useValidationLayers(useValidationLayers),
      _window(nullptr), _instance(nullptr), _debugMessenger(nullptr),
      _physicalDevice(VK_NULL_HANDLE), _logicalDevice(VK_NULL_HANDLE),
      _graphicsQueue(VK_NULL_HANDLE), _presentQueue(VK_NULL_HANDLE)
      
{
    initWindow();
}

VulkanApplication::~VulkanApplication() noexcept
{
    if(_logicalDevice)
    {
        vkDestroyDevice(_logicalDevice, nullptr);
    }

    if(_instance)
    {
        if(_useValidationLayers)
        {
            DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
        }

        if(_surface)
        {
            vkDestroySurfaceKHR(_instance, _surface, nullptr);
        }

        vkDestroyInstance(_instance, nullptr);
    }

    glfwDestroyWindow(_window);
    glfwTerminate();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void VulkanApplication::mainLoop() noexcept
{
    while(!glfwWindowShouldClose(_window))
    {
        glfwPollEvents();
    }
}

void VulkanApplication::initWindow() noexcept
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _window = glfwCreateWindow(_width, _height, "Vulkan Test", nullptr, nullptr);
}

VulkanApplication::Error VulkanApplication::initVulkan(VkResult* vkRes) noexcept
{
    if(_useValidationLayers && !checkValidationLayerSupport())
    {
        return Error::ValidationLayersUnavailable;
    }

    VkApplicationInfo appInfo = { };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Test";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    const RefDynArray<const char*> extensions = getRequiredExtensions();

#ifdef _DEBUG
    printExtensions(extensions);
#endif

    VkInstanceCreateInfo createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = extensions.count();
    createInfo.ppEnabledExtensionNames = extensions;

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if(_useValidationLayers)
    {
        createInfo.enabledLayerCount = VALIDATION_LAYER_COUNT;
        createInfo.ppEnabledLayerNames = VALIDATION_LAYERS;

        debugCreateInfo = getDebugCreateInfo(nullptr);
        createInfo.pNext = &debugCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    u32 vulkanExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &vulkanExtensionCount, nullptr);

    DynArray<VkExtensionProperties> vulkanExt(vulkanExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &vulkanExtensionCount, vulkanExt.arr());

#ifdef _DEBUG
    printExtensions(vulkanExt);
#endif

    const VkResult res = vkCreateInstance(&createInfo, nullptr, &_instance);
    if(vkRes) { *vkRes = res; }
    return res == VK_SUCCESS ? Error::Success : Error::VulkanError;
}

bool VulkanApplication::checkValidationLayerSupport() noexcept
{
    u32 layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    DynArray<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.arr());

    printLayers(availableLayers);

    for(uSys i = 0; i < VALIDATION_LAYER_COUNT; ++i)
    {
        bool layerFound = false;

        for(const auto& layerProperties : availableLayers)
        {
            if(strcmp(VALIDATION_LAYERS[0], layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if(!layerFound)
        {
            return false;
        }
    }

    return true;
}

RefDynArray<const char*> VulkanApplication::getRequiredExtensions() const noexcept
{
    u32 glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    RefDynArray<const char*> extensions(glfwExtensionCount + 1);

    uSys i = 0;
    for(; i < glfwExtensionCount; ++i)
    {
        extensions[i] = glfwExtensions[i];
    }

    if(_useValidationLayers)
    {
        extensions[i] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    }

    return extensions;
}

void VulkanApplication::setupDebugMessenger(VkResult* vkRes) noexcept
{
    if(!_useValidationLayers) { return; }

    auto createInfo = getDebugCreateInfo(nullptr);
    const VkResult res = CreateDebugUtilsMessengerEXT(_instance, &createInfo, nullptr, &_debugMessenger);
    if(vkRes) { *vkRes = res; }
}

[[nodiscard]] VulkanApplication::Error VulkanApplication::selectPhysicalDevice() noexcept
{
    u32 deviceCount = 0;
    vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

    if(!deviceCount)
    { return Error::ZeroGraphicsDevices; }

    DynArray<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(_instance, &deviceCount, devices);

    u32 mostSuitability = 0;

    for(const auto& device : devices)
    {
        const u32 suitability = rateDeviceSuitability(device);
        if(suitability > mostSuitability)
        {
            mostSuitability = suitability;
            _physicalDevice = device;
        }
    }

    if(_physicalDevice == VK_NULL_HANDLE)
    { return Error::ZeroSuitableGraphicsDevices; }

    return Error::Success;
}

u32 VulkanApplication::rateDeviceSuitability(VkPhysicalDevice device) const noexcept
{
#define UNSUITABLE(_EXPR) if((_EXPR)) { return 0; }

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    const QueueFamilyIndices indices = findQueueFamilies(device);

    // UNSUITABLE(!deviceFeatures.geometryShader);
    UNSUITABLE(!indices.isComplete());
    UNSUITABLE(!checkDeviceExtensionSupport(device));

    u32 suitability = 0;

    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    { suitability += 1000; }
    else if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
    { suitability += 800; }

    suitability += deviceProperties.limits.maxImageDimension2D;

    suitability += deviceFeatures.tessellationShader ? 200 : 0;

    return suitability;
}

bool VulkanApplication::checkDeviceExtensionSupport(VkPhysicalDevice device) const noexcept
{
    u32 extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    DynArray<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions);

    for(uSys i = 0; i < DEVICE_EXTENSION_COUNT; ++i)
    {
        bool found = false;
        for(const auto& extension : availableExtensions)
        {
            if(::std::strcmp(DEVICE_EXTENSIONS[i], extension.extensionName) == 0)
            {
                found = true;
                break;
            }
        }
        if(!found) { return false; }
    }

    return true;
}

VulkanApplication::QueueFamilyIndices VulkanApplication::findQueueFamilies(VkPhysicalDevice device) const noexcept
{
    QueueFamilyIndices indices;

    u32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, & queueFamilyCount, nullptr);

    DynArray<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    u32 i = 0;
    for(const auto& queueFamily : queueFamilies)
    {
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _surface, &presentSupport);

        if(presentSupport)
        {
            indices.presentFamily = i;
        }

        if(indices.isComplete())
        {
            break;
        }

        ++i;
    }

    return indices;
}

bool VulkanApplication::QueueFamilyIndices::isComplete() const noexcept
{
    return graphicsFamily.has_value() &&
            presentFamily.has_value();
}

VulkanApplication::Error VulkanApplication::createLogicalDevice(VkResult* vkRes) noexcept
{
    QueueFamilyIndices indices = findQueueFamilies(_physicalDevice);

    std::set<u32> uniqueQueueFamilies = 
    {
        indices.graphicsFamily.value(),
        indices. presentFamily.value()
    };

    DynArray<VkDeviceQueueCreateInfo> queueCreateInfos(uniqueQueueFamilies.size());

    float queuePriority = 1.0f;
    {
        uSys i = 0;
        for(const u32 queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo& queueCreateInfo = queueCreateInfos[i++];
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.pNext = nullptr;
            queueCreateInfo.flags = 0;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
        }
    }

    VkPhysicalDeviceFeatures deviceFeatures = { };

    VkDeviceCreateInfo createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.arr();
    createInfo.queueCreateInfoCount = queueCreateInfos.count();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = DEVICE_EXTENSION_COUNT;
    createInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS;

    if(_useValidationLayers)
    {
        createInfo.enabledLayerCount = VALIDATION_LAYER_COUNT;
        createInfo.ppEnabledLayerNames = VALIDATION_LAYERS;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    const VkResult res = vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_logicalDevice);
    if(vkRes) { *vkRes = res; }
    if(res != VK_SUCCESS)
    {
        return Error::VulkanError;
    }

    vkGetDeviceQueue(_logicalDevice, indices.graphicsFamily.value(), 0, &_graphicsQueue);
    vkGetDeviceQueue(_logicalDevice, indices. presentFamily.value(), 0, &_presentQueue);

    return Error::Success;
}

VulkanApplication::Error VulkanApplication::createSurface(VkResult* vkRes) noexcept
{
    const VkResult res = glfwCreateWindowSurface(_instance, _window, nullptr, &_surface);
    if(vkRes) { *vkRes = res; }
    if(res != VK_SUCCESS)
    { return Error::VulkanError; }

    return Error::Success;
}

static void printExtensions(const DynArray<VkExtensionProperties>& ext) noexcept
{
    printf("Vulkan Extensions:\n");

    for(const auto& ex : ext)
    {
        printf("\t%s\n", ex.extensionName);
    }
}

static void printExtensions(const RefDynArray<const char*>& ext) noexcept
{
    printf("Required Extensions:\n");

    for(const auto& ex : ext)
    {
        printf("\t%s\n", ex);
    }
}

static void printLayers(const DynArray<VkLayerProperties>& layers) noexcept
{
    printf("Vulkan Available Extensions:\n");

    for(const auto& layer : layers)
    {
        printf("\t%s\n", layer.layerName);
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                    void* pUserData) noexcept
{
    fprintf(stderr, "Validation Layer: %s\n", pCallbackData->pMessage);

    return VK_FALSE;
}

VkDebugUtilsMessengerCreateInfoEXT getDebugCreateInfo(void* userParam) noexcept
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;

    return createInfo;
}
