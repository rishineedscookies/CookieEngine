#pragma once
#include "ckpch.h"
#include "Cookie/Input/InputAction.h"



namespace Cookie {

	class COOKIE_API InputManager
	{
	public: 
		InputManager();

		~InputManager();

		bool GetBoolValue(bool& inBool, std::string actionName);
		bool GetFloatValue(float& inFloat, std::string actionName);
		bool GetVec2FloatValue(bool& inVec2Float, std::string actionName);

	private: 
		std::unordered_map<std::string, InputAction*> m_InputActionMap;
	};

}