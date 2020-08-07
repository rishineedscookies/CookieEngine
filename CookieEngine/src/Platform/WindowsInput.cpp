#include "ckpch.h"
#include "Cookie/Input/Input.h"

#include "Cookie/Application.h"
#include <GLFW/glfw3.h>

namespace Cookie {

	bool Input::GetKeyDown(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto keyState = glfwGetKey(window, static_cast<int32_t>(keycode));
		return keyState == GLFW_PRESS || keyState == GLFW_REPEAT;
	}

	bool Input::GetMouseButtonDown(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto buttonState = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return buttonState == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePos()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);

		return { (float)x, (float)y };
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePos();
		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePos();
		return y;
	}

}