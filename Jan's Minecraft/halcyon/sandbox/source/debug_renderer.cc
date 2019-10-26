// debug_renderer.cc

#include "debug_renderer.h"
#include "image.h"

#include <stdarg.h>
#include <stdio.h>

debug_renderer::debug_renderer(render_system &render_sys)
   : render_sys_(&render_sys)
{
   // note: calculate glyph texture coordinates
   const float x_scale = 1.0f / 16.0f;
   const float y_scale = 1.0f / 16.0f;

   for (int y = 0, index = 0; y < 16; y++)
   {
      float v0 = (y * y_scale);
      float v1 = (y * y_scale) + y_scale;

      for (int x = 0; x < 16; x++, index++)
      {
         float u0 = (x * x_scale);
         float u1 = (x * x_scale) + x_scale;
         glyph_uvs_[index] = { u0, v0, u1, v1 };
      }
   }

   projection_ = glm::mat4(1);
   vertex_cache_capacity_ = 0;
   vertex_cache_count_ = 0;
   vertex_cache_ = nullptr;

   text_shader_program_ = {};
   text_vertex_buffer_ = {};
   text_vertex_format_ = {};
   text_texture_ = {};
   text_sampler_ = {};
}

debug_renderer::~debug_renderer()
{
}

bool debug_renderer::init(int width, int height)
{
   // note: construct orthographic projection matrix
   projection_ = glm::orthoLH_ZO(0.0f, (float)width, (float)height, 0.0f, 0.0f, 1.0f);

   // note: allocate vertex cache cpu side
   vertex_cache_capacity_ = 4096 * 6;
   vertex_cache_count_ = 0;
   vertex_cache_ = new vertex[vertex_cache_capacity_];

   // note: create the vertex buffer
   if (!render_sys_->create_vertex_buffer(text_vertex_buffer_,
                                          RENDER_SYSTEM_BUFFER_ACCESS_DYNAMIC,
                                          sizeof(vertex) * vertex_cache_capacity_,
                                          nullptr))
   {
      HC_ASSERT(false);
   }

   // note: load vertex shader
   file_content vertex_shader_file_content = {};
   if (!read_file_from_disk("assets/font_vertex_shader.txt", &vertex_shader_file_content)) { HC_ASSERT(false); }

   // note: load fragment shader
   file_content fragment_shader_file_content = {};
   if (!read_file_from_disk("assets/font_fragment_shader.txt", &fragment_shader_file_content)) { HC_ASSERT(false); }

   // note: create shader program
   if (!render_sys_->create_shader(text_shader_program_,
      (const char *)vertex_shader_file_content.data_,
                                   (const char *)fragment_shader_file_content.data_))
   {
      HC_ASSERT(false);
   }

   // note: release shader sources
   release_file_content(&vertex_shader_file_content);
   release_file_content(&fragment_shader_file_content);

   // note: load image and create texture
   image font_image;
   if (!font_image.load_from_file("assets/debug_font.png")) { HC_ASSERT(false); }
   if (!render_sys_->create_texture(text_texture_,
                                    RENDER_SYSTEM_TEXTURE_FORMAT_RGBA8,
                                    font_image.width_,
                                    font_image.height_,
                                    font_image.data_))
   {
      HC_ASSERT(false);
   }
   font_image.release();

   // note: create sampler state
   if (!render_sys_->create_sampler_state(text_sampler_,
                                          RENDER_SYSTEM_SAMPLER_FILTER_NEAREST,
                                          RENDER_SYSTEM_SAMPLER_ADDRESS_MODE_CLAMP,
                                          RENDER_SYSTEM_SAMPLER_ADDRESS_MODE_CLAMP))
   {
      HC_ASSERT(false);
   }

   // note: build vertex format
   text_vertex_format_.add_attribute(vertex_format::ATTRIBUTE_SEMANTIC_POSITION,
                                     vertex_format::ATTRIBUTE_FORMAT_FLOAT,
                                     2,
                                     false);
   text_vertex_format_.add_attribute(vertex_format::ATTRIBUTE_SEMANTIC_TEXCOORD,
                                     vertex_format::ATTRIBUTE_FORMAT_FLOAT,
                                     2,
                                     false);

   return true;
}

void debug_renderer::exit()
{
   render_sys_->destroy_shader(text_shader_program_);
   render_sys_->destroy_texture(text_texture_);
   render_sys_->destroy_sampler_state(text_sampler_);
   render_sys_->destroy_vertex_buffer(text_vertex_buffer_);
}

void debug_renderer::render()
{
   render_sys_->update_vertex_buffer(text_vertex_buffer_,
                                    sizeof(vertex) * vertex_cache_count_,
                                    vertex_cache_);

   // note: pipeline settings
   render_sys_->set_shader_program(text_shader_program_);
   render_sys_->set_blend_state(true);
   render_sys_->set_depth_state(false, false);
   render_sys_->set_rasterizer_state(RENDER_SYSTEM_CULL_MODE_BACK);

   // note: shader uniforms
   render_sys_->set_shader_uniform(text_shader_program_,
                                  RENDER_SYSTEM_UNIFORM_TYPE_MATRIX,
                                  "u_projection",
                                  1,
                                  glm::value_ptr(projection_));

   // note: input assembly
   render_sys_->set_vertex_buffer(text_vertex_buffer_);
   render_sys_->set_vertex_format(text_vertex_format_);

   // note: resource bindings
   render_sys_->set_texture(text_texture_, 0);
   render_sys_->set_sampler_state(text_sampler_, 0);

   // note: draw call
   render_sys_->draw(RENDER_SYSTEM_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                     0,
                     vertex_cache_count_);


   vertex_cache_count_ = 0;
}

void debug_renderer::draw_text(const v2 &origin, const char *text)
{
   HC_ASSERT(vertex_cache_count_ < vertex_cache_capacity_);

   // note: font rendering settings
   const float glyph_width = 8.0f;
   const float glyph_height = 8.0f;
   const int invalid_character_index = (int)'?';

   float x = origin.x_;
   float y = origin.y_;

   // note: construct vertex buffer for text on the cpu
   const char *src = text;
   while (*src)
   {
      int character_index = (int)*src++;
      if (character_index == (int)'\n')
      {
         x = origin.x_;
         y += glyph_height + 2.0f;
         continue;
      }
      else if (character_index == (int)'\t')
      {
         x += glyph_width * 3;
         continue;
      }

      if (character_index < 0 || character_index > 255)
         character_index = invalid_character_index;

      v2 t0 = glyph_uvs_[character_index].xy_;
      v2 t1 = glyph_uvs_[character_index].zw_;

      v2 p0 = { x, y };
      v2 p1 = { x + glyph_width, y + glyph_height };

      x += glyph_width;

      vertex *vert = vertex_cache_ + vertex_cache_count_;
      vertex_cache_count_ += 6;

      vert[0].position_ = { p0.x_, p0.y_ };
      vert[0].texcoord_ = { t0.x_, t0.y_ };

      vert[1].position_ = { p1.x_, p0.y_ };
      vert[1].texcoord_ = { t1.x_, t0.y_ };

      vert[2].position_ = { p1.x_, p1.y_ };
      vert[2].texcoord_ = { t1.x_, t1.y_ };

      vert[3].position_ = { p1.x_, p1.y_ };
      vert[3].texcoord_ = { t1.x_, t1.y_ };

      vert[4].position_ = { p0.x_, p1.y_ };
      vert[4].texcoord_ = { t0.x_, t1.y_ };

      vert[5].position_ = { p0.x_, p0.y_ };
      vert[5].texcoord_ = { t0.x_, t0.y_ };
   }
}

void debug_renderer::draw_text_format(const v2 &origin, const char *format, ...)
{
   char text[1024] = {};
   va_list args;
   va_start(args, format);
   vsprintf_s(text, format, args);
   va_end(args);

   draw_text(origin, text);
}
