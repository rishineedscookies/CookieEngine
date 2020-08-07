#include "ckpch.h"
#include "InputManager.h"

namespace Cookie {

	bool InputManager::GetBoolValue(bool& inBool, std::string actionName)
	{
		InputAction* action = m_InputActionMap.at(actionName);
		inBool = action->GetBoolValue();
		return action->GetValueType() == InputValueType::Boolean;
	}

	bool InputManager::GetFloatValue(float& inFloat, std::string actionName)
	{
		InputAction* action = m_InputActionMap.at(actionName);
		inFloat = action->GetFloatValue();
		return action->GetValueType() == InputValueType::Float;
	}

	bool InputManager::GetVec2FloatValue(bool& inVec2Float, std::string actionName)
	{
		return false;
	}

}
