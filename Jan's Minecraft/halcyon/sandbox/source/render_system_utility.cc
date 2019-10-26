// render_system_utility.cc

#include "render_system_utility.h"
#include "image.h"

bool load_shader_program(render_system *render_sys, 
                         shader_program &handle,
                         const char *vertex_shader_source_filename,
                         const char *fragment_shader_source_filename)
{
   bool result = true;

   // note: load vertex shader
   file_content vertex_shader_file_content = {};
   if (!read_file_from_disk(vertex_shader_source_filename, &vertex_shader_file_content)) { HC_ASSERT(false); }

   // note: load fragment shader
   file_content fragment_shader_file_content = {};
   if (!read_file_from_disk(fragment_shader_source_filename, &fragment_shader_file_content)) { HC_ASSERT(false); }

   // note: create shader program
   if (!render_sys->create_shader(handle,
                                 (const char *)vertex_shader_file_content.data_,
                                 (const char *)fragment_shader_file_content.data_))
   {
      result = false;
   }

   // note: release shader sources
   release_file_content(&vertex_shader_file_content);
   release_file_content(&fragment_shader_file_content);

   return result;
}

bool load_texture(render_system *render_sys,
                  texture &handle,
                  render_system_texture_format format,
                  const char *filename)
{
   bool result = true;

   image img;
   if (!img.load_from_file(filename))
   {
      return false;
   }

   if (!render_sys->create_texture(handle,
                                   format,
                                   img.width_,
                                   img.height_,
                                   img.data_))
   {
      result = false;
   }

   img.release();

   return result;
}
