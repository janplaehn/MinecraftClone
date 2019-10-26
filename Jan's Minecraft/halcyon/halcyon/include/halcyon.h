// halcyon.h

#ifndef HALCYON_H_INCLUDED
#define HALCYON_H_INCLUDED

// note: config settings
#define HALCYON_PLATFORM_WINDOWS    1
#define HALCYON_DEVELOPMENT_BUILD   1

// note: config dependent settings
#if HALCYON_PLATFORM_WINDOWS
   #define WIN32_MEAN_AND_LEAN
   #include <Windows.h>
   #define HALCYON_API_EXPORT __declspec(dllexport)
   //#pragma comment(lib, "")
#endif // HALCYON_PLATFORM_WINDOWS

#ifndef HALCYON_API_EXPORT
   #define HALCYON_API_EXPORT
#endif // HALCYON_API_EXPORT

#if HALCYON_DEVELOPMENT_BUILD
#define HC_ASSERT(cond) if (!(cond)) { *(volatile int *)0 = 0; }
#else 
#define HC_ASSERT(cond) (void)(cond)
#endif // HALCYON_DEVELOPMENT_BUILD

#pragma warning(push)
#pragma warning(disable: 4201) // nonstandard extension used: nameless struct/union
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)


#include <halcyon_types.h>
#include <halcyon_platform.h>
#include <halcyon_opengl.h>
#include <halcyon_file_system.h>
#include <halcyon_input_system.h>
#include <halcyon_render_system.h>
#include <halcyon_application.h>
#include <halcyon_file_formats.h>

#endif // HALCYON_H_INCLUDED
