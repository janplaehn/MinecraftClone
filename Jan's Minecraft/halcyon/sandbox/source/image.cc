// image.cc

#include "image.h"

#pragma warning(push)
#pragma warning(disable: 4100) // unreferenced formal parameter
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#pragma warning(pop)

#include <halcyon.h>

image::image()
   : width_(0)
   , height_(0)
   , data_(nullptr)
{
}

image::~image()
{
}

bool image::is_valid() const
{
   return width_ > 0 && height_ > 0 && data_;
}

bool image::load_from_file(const char *filename)
{
   if (is_valid()) { release(); }

   file_content image_content = {};
   if (!read_file_from_disk_binary(filename, &image_content))
      return false;

   int width = 0, height = 0, components = 0;
   uint8_t *data = (uint8_t *)stbi_load_from_memory((const stbi_uc *)image_content.data_,
                                                    (int)image_content.size_,
                                                    &width,
                                                    &height,
                                                    &components,
                                                    STBI_rgb_alpha);
   //const char *error_message = stbi_failure_reason();
   //(void)error_message;
   HC_ASSERT(data);

   width_ = width;
   height_ = height;
   data_ = data;

   release_file_content(&image_content);

   return true;
}

void image::release()
{
   if (data_)
   {
      stbi_image_free(data_);
   }

   width_ = 0;
   height_ = 0;
   data_ = nullptr;
}
