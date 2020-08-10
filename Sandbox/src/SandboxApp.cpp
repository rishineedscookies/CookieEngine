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
		mathfu::Vector<float, 3> v1(1.0f, 2.0f, 3.0f), v2(3.0f, 2.5f, 0.5f), v3;
		v3 = v1 + v2;
		CK_TRACE("{0} {1} {2}", v3.x, v3.y, v3.z);

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
		PushOverlay(new Cookie::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Cookie::Application* Cookie::CreateApplication()
{
	return new Sandbox();
}