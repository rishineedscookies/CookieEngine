#pragma once

#ifdef CK_PLATFORM_WINDOWS

extern Cookie::Application* Cookie::CreateApplication();

int main(int argc, char** argv)
{
	Cookie::Log::Init();
	CK_CORE_WARN("Initialized Log!");

	auto app = Cookie::CreateApplication();
	app->Run();
	delete app;
}

#endif