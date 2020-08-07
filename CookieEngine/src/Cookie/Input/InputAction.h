#pragma once

namespace Cookie {

	enum class InputValueType {

		None,
		Boolean, Integer, Float, Float2

	};

#define INPUT_CLASS_TYPE(type) static InputValueType GetStaticType() { return InputValueType::##type; }\
								virtual InputValueType GetValueType() const override { return GetStaticType(); }\
								virtual const char* GetTypeName() const override {return #type; }

	class COOKIE_API InputAction
	{
	public:
		InputAction(std::string name)
			: m_Name(name) {}

		virtual InputValueType GetValueType() const = 0;
		virtual const char* GetTypeName() const = 0;

		virtual bool GetBoolValue() = 0;
		virtual float GetFloatValue() = 0;
		virtual bool GetVec2FloatValue() = 0;

	protected:
		std::string m_Name;
	};

}

