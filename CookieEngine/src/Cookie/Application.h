#pragma once

#include "Core.h"

namespace Cookie {

	class COOKIE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}

