#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Graphics.h"

class Application
{
public:
	void run();

private:
	
	void mainLoop();

private:
	Graphics graphics;
	
};
