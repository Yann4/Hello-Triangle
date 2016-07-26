#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm\glm.hpp>
#include <array>

#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <iostream>
#include <algorithm>

#include "Deleter.h"

struct Vertex
{
	glm::vec2 Pos;
	glm::vec3 Colour;

	static VkVertexInputBindingDescription getBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof Vertex;
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, Pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, Colour);
		return attributeDescriptions;
	}
};

class Graphics
{

public:
	Graphics();
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
	const int width = 800;
	const int height = 800;
	const std::string title = "Vulkan";

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

	VDeleter<VkBuffer> vertexBuffer = { device, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> vertexBufferMemory{ device, vkFreeMemory };

	std::vector<Vertex> vertices = {
		{ { 0.0f, -0.5f },{ 1.0f, 0.0f, 0.0f } },
		{ { 0.5f, 0.5f },{ 0.0f, 1.0f, 0.0f } },
		{ { -0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } }
	};

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

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VDeleter<VkBuffer>& buffer, VDeleter<VkDeviceMemory>& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer destBuffer, VkDeviceSize size);
	void createVertexBuffers();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

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