#pragma once

#include "Cookie/Core/Core.h"

namespace Cookie {

	class COOKIE_API Input
	{
	public:
		static bool GetKeyDown(int keycode);

		static bool GetMouseButtonDown(int button);
		static std::pair<float, float> GetMousePos();
		static float GetMouseX();
		static float GetMouseY();
	};
	
}