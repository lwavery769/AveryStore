#pragma once

#ifdef AL_PLATFORM_WINDOWS
#ifdef AL_BUILD_DLL
#define STORE_API __declspec(dllexport)
#else
#define STORE_API __declspec(dllimport)
#endif
#else
#error Hazel only supports Windows!
#endif
#define BIT(x) (1 << x)
#ifdef AL_ENABLE_ASSERTS
#define HZ_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define AL_ASSERT(x, ...)
#define AL_CORE_ASSERT(x, ...)
#endif
#define BIT(x) (1 << x) 

#define HZ_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
