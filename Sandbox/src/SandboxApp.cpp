#include <Cookie.h>

class ExampleLayer : public Cookie::Layer
{
public:
	ExampleLayer()
		: Layer("Example") {}

	void OnUpdate() override
	{
		CK_INFO("ExampleLayer::Update");
	}

	void OnEvent(Cookie::Event& event) override
	{
		CK_TRACE(event);
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