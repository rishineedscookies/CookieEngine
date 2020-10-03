#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Cookie {

	class COOKIE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define CK_CORE_TRACE(...)	::Cookie::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CK_CORE_INFO(...)	::Cookie::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CK_CORE_WARN(...)	::Cookie::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CK_CORE_ERROR(...)	::Cookie::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CK_CORE_FATAL(...)	::Cookie::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define CK_TRACE(...)		::Cookie::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CK_INFO(...)		::Cookie::Log::GetClientLogger()->info(__VA_ARGS__)
#define CK_WARN(...)		::Cookie::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CK_ERROR(...)		::Cookie::Log::GetClientLogger()->error(__VA_ARGS__)
#define CK_FATAL(...)		::Cookie::Log::GetClientLogger()->critical(__VA_ARGS__)