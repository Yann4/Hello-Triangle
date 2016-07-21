#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <iostream>

#include "Deleter.h"

struct QueueFamilyIndices
{
	int graphicsFamily = -1;

	bool isComplete()
	{
		return graphicsFamily >= 0;
	}
};

class Application
{
public:
	void run();

private:
	GLFWwindow* window;
	const int mWidth = 600;
	const int mHeight = 800;
	const std::string title = "Vulkan";

	VDeleter<VkInstance> instance{ vkDestroyInstance };
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	//Validation Layer variables
	VDeleter<VkDebugReportCallbackEXT> callback{ instance, Application::DestroyDebugReportCallbackEXT };

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

private:
	void initWindow();
	void initVulkan();
	void mainLoop();

	//Functions to initialise Vulkan
	void createVkInstance();

	void selectPhysicalDevice();
	bool isDeviceSuitable(const VkPhysicalDevice& dev);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	//Validation layer functions
	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();
	static VkBool32 debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, 
		size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);

	void setupDebugCallback();

	VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, 
		const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);

	static void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
};
