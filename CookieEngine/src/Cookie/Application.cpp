#include "ckpch.h"
#include "Application.h"

#include "Cookie/Events/ApplicationEvent.h"
#include "Cookie/Log.h"

#include <GLFW/glfw3.h>

namespace Cookie {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 0.5);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

}