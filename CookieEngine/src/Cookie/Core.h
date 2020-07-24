#pragma once

#ifdef CK_PLATFORM_WINDOWS
	#ifdef CK_BUILD_DLL
		#define COOKIE_API _declspec(dllexport)
	#else
		#define COOKIE_API _declspec(dllimport)
	#endif
#else
	#error Cookie Engine only supports Windows!
#endif