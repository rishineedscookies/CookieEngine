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

#ifdef CK_ENABLE_ASSERTS
	#define CK_ASSERT(x, ...) { if(!x)) { CK_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CK_CORE_ASSERT(x, ...) { if(!x)) { CK_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CK_ASSERT(x, ...) 
	#define CK_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)