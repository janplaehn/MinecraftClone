// halcyon_platform.h

#ifndef HALCYON_PLATFORM_H_INCLUDED
#define HALCYON_PLATFORM_H_INCLUDED

// note: this file contains mostly utility functions
//       that the os provides for us.

bool message_box(const char *caption, const char *text);
bool message_box_format(const char *caption, const char *format, ...);

#endif // HALCYON_PLATFORM_H_INCLUDED
