#include "ckpch.h"
#include "Timer.h"
#include <GLFW/glfw3.h>

namespace Cookie
{

	double GetCurrentTime()
	{
		return glfwGetTime();
	}

}
