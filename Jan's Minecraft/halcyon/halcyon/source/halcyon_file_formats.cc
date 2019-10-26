// halcyon_file_formats.cc

#include <halcyon.h>
#include <halcyon_file_formats.h>

void halcyon_release_resource(void *pointer)
{
   // note: "Trust me, I know what Im doin'!"
   uint8_t *base = (uint8_t *)pointer;
   delete [] base;
}

bool halcyon_load_model(const char *filename, halcyon_model_file_header **header)
{
   file_content content = {};
   if (!read_file_from_disk_binary(filename, &content))
      return false;

   (*header) = (halcyon_model_file_header *)content.data_;

   return true;
}

uint32_t halcyon_model_data_size(halcyon_model_file_header *header)
{
   return header->primitive_count_ * header->format_.stride_;
}

const void *halcyon_model_data(halcyon_model_file_header *header)
{
   return (const void *)(&header[1]);
}
