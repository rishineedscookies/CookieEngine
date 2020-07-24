#pragma once

#ifdef CK_PLATFORM_WINDOWS

extern Cookie::Application* Cookie::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Cookie::CreateApplication();
	app->Run();
	delete app;
}

#endif