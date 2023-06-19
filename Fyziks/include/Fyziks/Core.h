#pragma once

#ifdef _WIN32
#define PLATFORM_WINDOWS
#endif

#ifdef linux
#define PLATFORM_LINUX
#endif

#ifdef __APPLE__
#define PLATFORM_MACOS
#endif

#if defined(PLATFORM_WINDOWS)
#if defined(FYZIKS_EXPORTS)
#define FYZIKS_API __declspec(dllexport)
#else
#define FYZIKS_API __declspec(dllimport)
#endif
#endif

#if defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS)
#ifdef FYZIKS_EXPORTS
#define FYZIKS_API __attribute__((visibility("default")))
#else
#define FYZIKS_API
#endif
#endif