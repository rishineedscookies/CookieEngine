#pragma once

#include <mathfu/vector.h>
#include <mathfu/glsl_mappings.h>

namespace Cookie {

	class Input
	{
	public:
		static bool GetKeyDown(int keycode);

		static bool GetMouseButtonDown(int button);
		static mathfu::vec2 GetMousePos();
		static float GetMouseX();
		static float GetMouseY();
	};
	
}