#pragma once

namespace Cookie {

	class COOKIE_API InputProcessor
	{
	public:

		virtual bool GetBoolValue() = 0;
		virtual float GetFloatValue() = 0;
	};

}