#pragma once

#include <memory>

#ifdef CK_PLATFORM_WINDOWS
#if CK_DYNAMIC_LINK
	#ifdef CK_BUILD_DLL
		#define COOKIE_API _declspec(dllexport)
	#else
		#define COOKIE_API _declspec(dllimport)
	#endif
#else
	#define COOKIE_API
#endif
#else
	#error Cookie Engine only supports Windows!
#endif

#ifdef CK_DEBUG
	#define CK_ENABLE_ASSERTS
#endif

#ifdef CK_ENABLE_ASSERTS
	#define CK_ASSERT(x, ...) { if(!x)) { CK_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CK_CORE_ASSERT(x, ...) { if(!(x)) { CK_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CK_ASSERT(x, ...) 
	#define CK_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define CK_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Cookie {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}