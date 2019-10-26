// main.cc

#include <halcyon.h>

// note: more platform headers that we need
#include <windowsx.h>

// note: helper function to display an error before exit
static int
exit_error_message(const char *message)
{
   MessageBoxA(NULL, message, "ERROR!", MB_OK | MB_ICONERROR);
   return -1;
}

#pragma warning(disable: 4505) // unreferenced local function has been removed

struct input_state
{
   struct
   {
      bool current_button_state_[MOUSE_BUTTON_COUNT];
      bool previous_button_state_[MOUSE_BUTTON_COUNT];
      int x_;
      int y_;
   } mouse_;
   struct
   {
      bool current_key_state_[KEYCODE_COUNT];
      bool previous_key_state_[KEYCODE_COUNT];
   } keyboard_;
   struct
   {
      int character_count_;
      int characters_[16];
   } text_;
};

static void
input_state_process_event(input_state &input, MSG &msg)
{
   switch (msg.message)
   {
      case WM_MOUSEMOVE:
      {
         input.mouse_.x_ = GET_X_LPARAM(msg.lParam);
         input.mouse_.y_ = GET_Y_LPARAM(msg.lParam);
      } break;
      case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
      {
         bool is_down = msg.message == WM_LBUTTONDOWN ? true : false;
         input.mouse_.current_button_state_[MOUSE_BUTTON_LEFT] = is_down;
      } break;
      case WM_RBUTTONDOWN:
      case WM_RBUTTONUP:
      {
         bool is_down = msg.message == WM_RBUTTONDOWN ? true : false;
         input.mouse_.current_button_state_[MOUSE_BUTTON_RIGHT] = is_down;
      } break;
      case WM_MBUTTONDOWN:
      case WM_MBUTTONUP:
      {
         bool is_down = msg.message == WM_MBUTTONDOWN ? true : false;
         input.mouse_.current_button_state_[MOUSE_BUTTON_MIDDLE] = is_down;
      } break;
      case WM_XBUTTONDOWN:
      case WM_XBUTTONUP:
      {
         bool is_extra_1 = (GET_XBUTTON_WPARAM(msg.wParam) & XBUTTON1) > 0;
         bool is_down = msg.message == WM_XBUTTONDOWN ? true : false;
         int index = is_extra_1 ? MOUSE_BUTTON_EXTRA_1 : MOUSE_BUTTON_EXTRA_2;
         input.mouse_.current_button_state_[index] = is_down;
      } break;
      case WM_KEYDOWN:
      case WM_KEYUP:
      {
         bool is_down = msg.message == WM_KEYDOWN ? true : false;
         int index = msg.wParam & 0xff;
         input.keyboard_.current_key_state_[index] = is_down;
      } break;
      case WM_UNICHAR:
      case WM_CHAR:
      {
         if (input.text_.character_count_ < 16)
         {

         }
      } break;
   }
}

static void
input_state_post_process(input_state &input, keyboard_device &keyboard, mouse_device &mouse)
{
   for (int i = 0; i < MOUSE_BUTTON_COUNT; i++)
   {
      mouse.buttons_[i].pressed_ = false;
      mouse.buttons_[i].released_ = false;

      if (input.mouse_.previous_button_state_[i] != input.mouse_.current_button_state_[i])
      {
         if (input.mouse_.current_button_state_[i])
         {
            mouse.buttons_[i].down_ = true;
            mouse.buttons_[i].pressed_ = true;
         }
         else
         {
            mouse.buttons_[i].down_ = false;
            mouse.buttons_[i].released_ = true;
         }
      }

      input.mouse_.previous_button_state_[i] = input.mouse_.current_button_state_[i];
   }

   mouse.x_ = input.mouse_.x_;
   mouse.y_ = input.mouse_.y_;

   for (int i = 0; i < KEYCODE_COUNT; i++)
   {
      keyboard.keys_[i].pressed_ = false;
      keyboard.keys_[i].released_ = false;

      if (input.keyboard_.previous_key_state_[i] != input.keyboard_.current_key_state_[i])
      {
         if (input.keyboard_.current_key_state_[i])
         {
            keyboard.keys_[i].down_ = true;
            keyboard.keys_[i].pressed_ = true;
         }
         else
         {
            keyboard.keys_[i].down_ = false;
            keyboard.keys_[i].released_ = true;
         }
      }

      input.keyboard_.previous_key_state_[i] = input.keyboard_.current_key_state_[i];
   }

   // todo: text input
   const int CHARCTER_COUNT = input.text_.character_count_;
   for (int i = 0; i < CHARCTER_COUNT; i++)
   {
      input.text_.characters_[i] = 0;
   }

   input.text_.character_count_ = 0;
}

static LRESULT CALLBACK
WinMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg)
   {
      case WM_CLOSE:
         PostQuitMessage(0);
         break;
      default:
         return DefWindowProc(hWnd, uMsg, wParam, lParam);
   }
   return 0;
}

extern bool halcyon_opengl_init(void *window_handle, int major, int minor);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
   const wchar_t *window_title = L"Jan's Minecraft";
   const int client_width = 1600;
   const int client_height = 900;

   const wchar_t *class_name = L"halcyonClassName";
   {
      // note: register window class
      WNDCLASSEX window_class = {};
      window_class.cbSize = sizeof(WNDCLASSEX);
      window_class.hInstance = hInstance;
      window_class.hbrBackground = CreateSolidBrush(0x00000000);
      window_class.lpfnWndProc = WinMainProc;
      window_class.lpszClassName = class_name;
      window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
      window_class.hCursor = LoadCursor(NULL, IDC_ARROW);

      if (!RegisterClassEx(&window_class))
        return exit_error_message("could not register window class!");
   }

   HWND window_handle = NULL;
   {
      DWORD window_style = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU);
      RECT window_rect = { 0, 0, client_width, client_height };
      if (!AdjustWindowRect(&window_rect, window_style, 0))
         return exit_error_message("Could adjust window rect.");

      int window_width = window_rect.right - window_rect.left;
      int window_height = window_rect.bottom - window_rect.top;

      window_handle = CreateWindowEx(0,
                                     class_name,
                                     window_title,
                                     window_style,
                                     CW_USEDEFAULT,
                                     CW_USEDEFAULT,
                                     window_width,
                                     window_height,
                                     NULL,
                                     NULL,
                                     hInstance,
                                     NULL);
      if (!window_handle)
         return exit_error_message("Could not create window.");

      if (!halcyon_opengl_init(window_handle, 3, 3))
         return exit_error_message("could not initialize opengl!");
   }

   // note: keyboard, mouse and helper input state 
   keyboard_device keyboard = {};
   mouse_device mouse = {};
   input_state input = {};

   HDC device_context_handle = GetDC(window_handle);
   ShowWindow(window_handle, SW_NORMAL);

   application *app = create_application(__argc, __argv);
   if (!app->init())
      return exit_error_message("could not create application!");

   // todo: query screen refresh rate
   const double monitor_refresh_rate = 60.0;
   const double frame_target_time = 1.0 / monitor_refresh_rate;
   double frame_accumulator = 0.0f;

   int64_t timing_start_tick = 0;
   QueryPerformanceCounter((LARGE_INTEGER *)&timing_start_tick);
   int64_t timing_previous_tick = timing_start_tick;

   double timing_factor = 0.0;
   {
      LARGE_INTEGER frequency = {};
      QueryPerformanceFrequency(&frequency);
      timing_factor = frequency.QuadPart / 1000.0;
   }

   bool running = true;
   while (running)
   {
      MSG msg = { 0 };
      while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
      {
         if (msg.message == WM_QUIT)
            running = false;

         input_state_process_event(input, msg);

         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }
      
      input_state_post_process(input, keyboard, mouse);

      // note: timing calculations
      int64_t timing_current_tick = 0;
      QueryPerformanceCounter((LARGE_INTEGER *)&timing_current_tick);
      
      int64_t app_ticks = timing_current_tick - timing_start_tick;
      double app_time = (app_ticks / timing_factor) * 0.001;
      
      int64_t delta_ticks = timing_current_tick - timing_previous_tick;
      double deltatime = (delta_ticks / timing_factor) * 0.001;
      timing_previous_tick = timing_current_tick;

      // note: limit frame delta time
      if (deltatime > 0.2)
         deltatime = 0.2;

      frame_accumulator += deltatime;
      while (frame_accumulator >= frame_target_time && running)
      {
         frame_accumulator -= frame_target_time;
         running = running && app->tick(app_time, frame_target_time, &keyboard, &mouse);
      }

      app->render();
      SwapBuffers(device_context_handle);
   }

   app->exit();

   return 0;
}
