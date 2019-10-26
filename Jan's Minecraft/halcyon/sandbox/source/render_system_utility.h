// render_system_utility.h

#ifndef RENDER_SYSTEM_UTILITY_H_INCLUDED
#define RENDER_SYSTEM_UTILITY_H_INCLUDED

#include <halcyon.h>

bool load_shader_program(render_system *render_sys,
                         shader_program &handle,
                         const char *vertex_shader_source_filename,
                         const char *fragment_shader_source_filename);

bool load_texture(render_system *render_sys,
                  texture &handle,
                  render_system_texture_format format,
                  const char *filename);

#endif // RENDER_SYSTEM_UTILITY_H_INCLUDED
