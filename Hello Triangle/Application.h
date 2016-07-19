#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

#include "Deleter.h"

class HelloTriangleApplication
{
public:
	void run();

private:
	GLFWwindow* window;
	const int mWidth = 600;
	const int mHeight = 800;
	const std::string title = "Vulkan";

	VDeleter<VkInstance> instance{ vkDestroyInstance };
private:
	void initWindow();
	void initVulkan();
	void mainLoop();

	//Functions to initialise Vulkan
	void createVkInstance();
};
