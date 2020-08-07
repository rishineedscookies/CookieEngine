#pragma once

#include "Cookie/Input/InputAction.h"

namespace Cookie {

	class COOKIE_API BoolInputAction : public InputAction
	{
	public:
		BoolInputAction(std::string name)
			: InputAction(name) {}
		
		INPUT_CLASS_TYPE(Boolean)

		virtual bool GetBoolValue() override;

	protected:
		
	};

}