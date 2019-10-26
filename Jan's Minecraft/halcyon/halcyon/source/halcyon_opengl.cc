// halcyon_opengl.cc

#include "halcyon.h"

#define GLF(ret, name, ...) type_##name *name;
GL_FUNCLIST_1_0;
GL_FUNCLIST_1_1;
GL_FUNCLIST_1_2;
GL_FUNCLIST_1_3;
GL_FUNCLIST_1_4;
GL_FUNCLIST_1_5;
GL_FUNCLIST_2_0;
GL_FUNCLIST_2_1;
GL_FUNCLIST_3_0;
GL_FUNCLIST_3_1;
GL_FUNCLIST_3_2;
GL_FUNCLIST_3_3;
#undef GLF

#if HALCYON_PLATFORM_WINDOWS

#define WIN32_MEAN_AND_LEAN
#include <Windows.h>

extern "C"
{
	// note: tell windows to use discrete nvidia gpu
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

	// note: tell windows to use discrete amd gpu
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_ACCELERATION_ARB 0x2003
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_PIXEL_TYPE_ARB 0x2013
#define WGL_COLOR_BITS_ARB 0x2014
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_STENCIL_BITS_ARB 0x2023
#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_TYPE_RGBA_ARB 0x202B
typedef BOOL WINAPI wglChoosePixelFormatARB_t(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
static wglChoosePixelFormatARB_t *wglChoosePixelFormatARB;

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB 0x2093
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_DEBUG_BIT_ARB 0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
typedef HGLRC WINAPI wglCreateContextAttribsARB_t(HDC hDC, HGLRC hshareContext, const int *attribList);
static wglCreateContextAttribsARB_t *wglCreateContextAttribsARB;

typedef BOOL WINAPI wglSwapIntervalEXT_t(int interval);
static wglSwapIntervalEXT_t *wglSwapIntervalEXT;

typedef HGLRC WINAPI wglCreateContext_t(HDC hDC);
static wglCreateContext_t *wglCreateContext_;

typedef BOOL WINAPI wglMakeCurrent_t(HDC hDC, HGLRC hGLRC);
static wglMakeCurrent_t *wglMakeCurrent_;

typedef BOOL WINAPI wglDeleteContext_t(HGLRC hGLRC);
static wglDeleteContext_t *wglDeleteContext_;

typedef HGLRC WINAPI wglGetCurrentContext_t(VOID);
static wglGetCurrentContext_t *wglGetCurrentContext_;

static bool
win32_load_opengl_dll(HMODULE *handle) {
	*handle = LoadLibraryA("OpenGL32.dll");

	return (*handle) != 0;
}

static bool
win32_create_fake_opengl_context(HWND *window_handle) {
	WNDCLASSEXA wc = { sizeof(wc) };
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = "fakeOpenGLContextWindowClass";
	wc.lpfnWndProc = DefWindowProcA;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	if (!RegisterClassExA(&wc))
		return 0;

	HWND fake_window_handle = CreateWindowExA(0, wc.lpszClassName,
		"title",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		0, 0, wc.hInstance, 0);
	if (!fake_window_handle)
		return false;

	HDC device_context_handle = GetDC(fake_window_handle);

	PIXELFORMATDESCRIPTOR pfd = { sizeof(pfd) };
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pixel_format_index = ChoosePixelFormat(device_context_handle, &pfd);
	if (!SetPixelFormat(device_context_handle, pixel_format_index, &pfd))
		return false;

	HGLRC render_context_handle = wglCreateContext_(device_context_handle);
	if (!wglMakeCurrent_(device_context_handle, render_context_handle))
		return false;

	if (window_handle)
		*window_handle = fake_window_handle;

	return true;
}

static void
win32_destroy_fake_opengl_context(HWND window_handle) {
	HGLRC render_context_handle = wglGetCurrentContext_();
	wglMakeCurrent_(NULL, NULL);
	wglDeleteContext_(render_context_handle);
	DestroyWindow(window_handle);
}

static bool
win32_load_opengl_functions(HMODULE handle, int major, int minor) {
	wglCreateContext_ = (wglCreateContext_t *)GetProcAddress(handle, "wglCreateContext");
	wglMakeCurrent_ = (wglMakeCurrent_t *)GetProcAddress(handle, "wglMakeCurrent");
	wglDeleteContext_ = (wglDeleteContext_t *)GetProcAddress(handle, "wglDeleteContext");
	wglGetCurrentContext_ = (wglGetCurrentContext_t *)GetProcAddress(handle, "wglGetCurrentContext");

	HWND fake_window_handle = 0;
	if (!win32_create_fake_opengl_context(&fake_window_handle)) {
		return false;
	}

	typedef PROC WINAPI wglGetProcAddress_t(LPCSTR lpszProc);
	wglGetProcAddress_t *wgl_get_proc_address = (wglGetProcAddress_t *)GetProcAddress(handle, "wglGetProcAddress");
	if (!wgl_get_proc_address) {
		return false;
	}

	// load wgl extensions
	wglChoosePixelFormatARB = (wglChoosePixelFormatARB_t *)wgl_get_proc_address("wglChoosePixelFormatARB");
	wglCreateContextAttribsARB = (wglCreateContextAttribsARB_t *)wgl_get_proc_address("wglCreateContextAttribsARB");
	wglSwapIntervalEXT = (wglSwapIntervalEXT_t *)wgl_get_proc_address("wglSwapIntervalEXT");

	bool result = true;
	// GetProcAddress
#define GLF(ret, name, ...)                             \
   name = (type_##name *)GetProcAddress(handle, #name); \
   if (!name) { result = false; }

	GL_FUNCLIST_1_0;
	if (major == 1 && minor == 0)
		goto done;

	GL_FUNCLIST_1_1;
	if (major == 1 && minor == 1)
		goto done;
#undef GLF

	// wglGetProcAddress
#define GLF(ret, name, ...)                           \
   name = (type_##name *)wgl_get_proc_address(#name); \
   if (!name) { result = false; }

	GL_FUNCLIST_1_2;
	if (major == 1 && minor == 2)
		goto done;
	GL_FUNCLIST_1_3;
	if (major == 1 && minor == 3)
		goto done;
	GL_FUNCLIST_1_4;
	if (major == 1 && minor == 4)
		goto done;
	GL_FUNCLIST_1_5;
	if (major == 1 && minor == 5)
		goto done;
	GL_FUNCLIST_2_0;
	if (major == 2 && minor == 0)
		goto done;
	GL_FUNCLIST_2_1;
	if (major == 2 && minor == 1)
		goto done;
	GL_FUNCLIST_3_0;
	if (major == 3 && minor == 0)
		goto done;
	GL_FUNCLIST_3_1;
	if (major == 3 && minor == 1)
		goto done;
	GL_FUNCLIST_3_2;
	if (major == 3 && minor == 2)
		goto done;
	GL_FUNCLIST_3_3;

#undef GLF

	done :
		 win32_destroy_fake_opengl_context(fake_window_handle);

		 return result;
}

static bool
platform_create_core_opengl_context(void *handle, int major, int minor) {
	HMODULE opengl_dll_handle = 0;
	if (!win32_load_opengl_dll(&opengl_dll_handle)) {
		return false;
	}

	if (!win32_load_opengl_functions(opengl_dll_handle, major, minor)) {
		return false;
	}

	HWND window_handle = (HWND)handle;
	HDC device_context_handle = GetDC(window_handle);
	const int pixel_format_attribs[] =
	{
	   WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
	   WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
	   WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
	   WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
	   WGL_COLOR_BITS_ARB, 32,
	   WGL_DEPTH_BITS_ARB, 24,
	   WGL_STENCIL_BITS_ARB, 8,
	   0
	};
	int pixel_format_index = 0;
	UINT num_pixel_formats = 0;
	wglChoosePixelFormatARB(device_context_handle,
		pixel_format_attribs,
		0,
		1,
		&pixel_format_index,
		&num_pixel_formats);

	PIXELFORMATDESCRIPTOR pfd = { sizeof(pfd) };
	DescribePixelFormat(device_context_handle,
		pixel_format_index,
		sizeof(pfd),
		&pfd);
	SetPixelFormat(device_context_handle,
		pixel_format_index,
		&pfd);

#pragma warning(push)
#pragma warning(disable:4204) // nonstandard extension used: non-constant aggregate initializer
	int context_attrib_list[] =
	{
	   WGL_CONTEXT_MAJOR_VERSION_ARB, major,
	   WGL_CONTEXT_MINOR_VERSION_ARB, minor,
	   WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	   WGL_CONTEXT_FLAGS_ARB, 0,
	   0
	};
#pragma warning(pop)

	HGLRC render_context_handle = wglCreateContextAttribsARB(device_context_handle,
		NULL,
		context_attrib_list);
	if (!wglMakeCurrent_(device_context_handle, render_context_handle))
		return false;

	if (wglSwapIntervalEXT)
		wglSwapIntervalEXT(0);

	return true;
}

#endif // PLATFORM_WINDOWS

bool halcyon_opengl_init(void *window_handle, int major, int minor) {
	if (!platform_create_core_opengl_context(window_handle, major, minor))
		return false;

	return true;
}
