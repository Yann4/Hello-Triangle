#include "Application.h"

void HelloTriangleApplication::run()
{
	initWindow();
	initVulkan();
	mainLoop();
}

void HelloTriangleApplication::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(mWidth, mHeight, title.c_str(), nullptr, nullptr);
}

void HelloTriangleApplication::initVulkan()
{

}

void HelloTriangleApplication::mainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
}