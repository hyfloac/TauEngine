#include "VulkanExtensions.h"

#define GET_FUNC(_NAME) static PFN_##_NAME func = reinterpret_cast<PFN_##_NAME>(vkGetInstanceProcAddr(instance, #_NAME));\
                        if(!func) { return VK_ERROR_EXTENSION_NOT_PRESENT; }

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDebugUtilsMessengerEXT* pDebugMessenger) NOEXCEPT
{
    GET_FUNC(vkCreateDebugUtilsMessengerEXT);
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
}

VkResult DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) NOEXCEPT
{
    GET_FUNC(vkDestroyDebugUtilsMessengerEXT);
    func(instance, debugMessenger, pAllocator);
    return VK_SUCCESS;
}
