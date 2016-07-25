#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <iostream>
#include <algorithm>

#include "Deleter.h"

class Graphics
{

public:
	Graphics();
	Graphics(int width, int height, std::string title);
	~Graphics();

	void Draw();
	bool windowClosed();

private:
	struct QueueFamilyIndices
	{
		int graphicsFamily = -1;
		int presentFamily = -1;

		bool isComplete()
		{
			return graphicsFamily >= 0 && presentFamily >= 0;
		}
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;

		bool isAdequate()
		{
			return !formats.empty() && !presentModes.empty();
		}
	};

private:
	GLFWwindow* window;
	int width = 600;
	int height = 800;
	std::string title = "Vulkan";

	VDeleter<VkInstance> instance{ vkDestroyInstance };

	VDeleter<VkDevice> device{ vkDestroyDevice };
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	VDeleter<VkCommandPool> commandPool{ device, vkDestroyCommandPool };
	std::vector<VkCommandBuffer> commandBuffers;

	VDeleter<VkSemaphore> imageAvailableSemaphore{ device, vkDestroySemaphore };
	VDeleter<VkSemaphore> renderFinishedSemaphore{ device, vkDestroySemaphore };

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VDeleter<VkSurfaceKHR> surface{ instance, vkDestroySurfaceKHR };

	VDeleter<VkPipelineLayout> pipelineLayout{ device, vkDestroyPipelineLayout };
	VDeleter<VkPipeline> graphicsPipeline{ device, vkDestroyPipeline };
	VDeleter<VkRenderPass> renderPass{ device, vkDestroyRenderPass };

	VDeleter<VkSwapchainKHR> swapChain{ device, vkDestroySwapchainKHR };
	std::vector<VkImage> swapChainImages;
	std::vector<VDeleter<VkFramebuffer>> swapChainFrameBuffers;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	std::vector<VDeleter<VkImage>> swapChainImageViews;

	VDeleter<VkShaderModule> vertShaderModule = { device, vkDestroyShaderModule };
	VDeleter<VkShaderModule> fragShaderModule = { device, vkDestroyShaderModule };

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	};

	//Validation Layer variables
	VDeleter<VkDebugReportCallbackEXT> callback{ instance, Graphics::DestroyDebugReportCallbackEXT };

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation",
	};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

private:
	//High level creation functions
	void initWindow();
	void initVulkan();

	void drawFrame();
	
	//Callback function
	static void onWindowResized(GLFWwindow* window, int width, int height);

	//Functions to initialise Vulkan
	void createVkInstance();

	//Physical/Locgical Device creation & selection
	void createLogicalDevice();
	//TODO: Rate devices and select based upon that
	void selectPhysicalDevice();

	//Selection function
	bool isDeviceSuitable(const VkPhysicalDevice& dev);
	bool checkDeviceExtensionSupport(VkPhysicalDevice dev);

	void createSurface();

	void createSemaphores();

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	//Swap chain selection
	void recreateSwapchain();
	void createSwapChain();
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice dev);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities); //Chooses the resolution of the swap chain images

	//Image views
	void createImageViews();

	//Graphics pipeline
	void createGraphicsPipeline();
	void createRenderPass();

	void createFrameBuffers();

	//Shader loaders
	static std::vector<char> readFile(const std::string& fileName);
	void createShaderModule(const std::vector<char>& code, VDeleter<VkShaderModule>& shaderModule);

	//Command pool
	void createCommandPool();
	void createCommandBuffers();

	//Validation layer
	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();
	
	//Validation layer callbacks
	void setupDebugCallback();
	VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
	static VkBool32 debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);
	static void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
};