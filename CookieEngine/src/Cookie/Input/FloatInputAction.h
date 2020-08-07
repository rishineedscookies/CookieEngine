#pragma once

#include "Cookie/Input/InputAction.h"

namespace Cookie {

	class COOKIE_API FloatInputAction : public InputAction
	{
	public:
		FloatInputAction(std::string name)
			: InputAction(name) {}

		INPUT_CLASS_TYPE(Float)

		virtual float GetFloatValue() override;

	protected:

	};

}