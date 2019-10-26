// image.h

#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <stdint.h>

struct image
{
   image();
   ~image();

   bool is_valid() const;
   bool load_from_file(const char *filename);
   void release();

   int width_;
   int height_;
   uint8_t *data_;
};

#endif // IMAGE_H_INCLUDED
