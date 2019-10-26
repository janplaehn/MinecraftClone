// halcyon_platform.cc

#include <halcyon.h>

#define WIN32_MEAN_AND_LEAN
#define VC_EXTRALEAN
#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>

bool message_box(const char *caption, const char *text)
{
   return MessageBoxA(NULL,
                      text,
                      caption,
                      MB_OK | MB_ICONSTOP) == IDOK;
}

bool message_box_format(const char *caption, const char *format, ...)
{
   char text[1024] = {};
   
   va_list vargs;
   va_start(vargs, format);
   vsprintf_s(text, sizeof(text), format, vargs);
   va_end(vargs);

   return message_box(caption, text);
}
