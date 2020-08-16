#include <Cookie.h>
#include <mathfu/vector.h>
#include <vectorial/vec4f.h>

class ExampleLayer : public Cookie::Layer
{
public:
	ExampleLayer()
		: Layer("Example") {}

	void OnUpdate() override
	{
		if (Cookie::Input::GetKeyDown(CK_KEY_TAB))
		{
			CK_TRACE("Tab key is pressed (poll)!");
		}
	}

	void OnEvent(Cookie::Event& event) override
	{
		if(event.GetEventType() == Cookie::EventType::KeyPressed)
		{
			Cookie::KeyPressedEvent& e = (Cookie::KeyPressedEvent&)event;
			if (e.GetKeyCode() == CK_KEY_TAB)
			{
				CK_TRACE("Tab key is pressed (event)!");
			}
			CK_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public Cookie::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Cookie::Application* Cookie::CreateApplication()
{
	return new Sandbox();
}