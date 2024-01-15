#pragma once
#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
class Log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
private:
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

// Core log macros
#define AL_CORE_TRACE(...)    ::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AL_CORE_INFO(...)     ::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AL_CORE_WARN(...)     ::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AL_CORE_ERROR(...)    ::Log::GetCoreLogger()->error(__VA_ARGS__)
#define AL_CORE_CRITICAL(...) ::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define AL_TRACE(...)	      ::Log::GetClientLogger()->trace(__VA_ARGS__)
#define AL_INFO(...)	      ::Log::GetClientLogger()->info(__VA_ARGS__)
#define AL_WARN(...)	      ::Log::GetClientLogger()->warn(__VA_ARGS__)
#define AL_ERROR(...)	      ::Log::GetClientLogger()->error(__VA_ARGS__)
#define AL_CRITICAL(...)	  ::Log::GetClientLogger()->critical(__VA_ARGS__)