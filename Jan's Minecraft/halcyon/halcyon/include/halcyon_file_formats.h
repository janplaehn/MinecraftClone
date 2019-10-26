// halcyon_file_formats.h

#ifndef HALCYON_FILE_FORMATS_H_INCLUDED
#define HALCYON_FILE_FORMATS_H_INCLUDED

#include <stdint.h>

#include <halcyon_types.h>
#include <halcyon_render_system.h>

#define HALCYON_MODEL_MAGIC               0x666D6268  // hbmf - halcyon binary model file
#define HALCYON_MODEL_VERSION             0x01000000  // 01 - major, 00 - minor, 0000 - patch

struct halcyon_model_vertex_format
{
   uint32_t stride_;             // total size of a vertex in bytes
   uint32_t attribute_count_;
   uint8_t attributes_[32];
};

struct halcyon_model_file_header
{
   uint32_t magic_;
   uint32_t version_;
   uint32_t primitive_count_;
   halcyon_model_vertex_format format_;
};

void halcyon_release_resource(void *pointer);
bool halcyon_load_model(const char *filename, halcyon_model_file_header **header);
uint32_t halcyon_model_data_size(halcyon_model_file_header *header);
const void *halcyon_model_data(halcyon_model_file_header *header);


#endif // HALCYON_FILE_FORMATS_H_INCLUDED
