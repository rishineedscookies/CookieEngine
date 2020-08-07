#include "ckpch.h"
#include "KeyInputProcessor.h"
#include "Cookie/Input/Input.h"

namespace Cookie {


	bool KeyInputProcessor::GetBoolValue()
	{
		return Input::GetKeyDown(m_Keycode);
	}

}