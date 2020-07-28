#include "Application.h"

#include "Cookie/Events/ApplicationEvent.h"
#include "Cookie/Log.h"

namespace Cookie {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		CK_TRACE(e);

		while (true);
	}

}