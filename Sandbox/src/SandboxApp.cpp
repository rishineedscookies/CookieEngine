#include <Cookie.h>

class Sandbox : public Cookie::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Cookie::Application* Cookie::CreateApplication()
{
	return new Sandbox();
}