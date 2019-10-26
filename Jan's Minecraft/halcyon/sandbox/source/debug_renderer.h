// debug_renderer.h

#ifndef DEBUG_RENDERER_H_INCLUDED
#define DEBUG_RENDERER_H_INCLUDED

#include <halcyon.h>

struct debug_renderer
{
   debug_renderer(render_system &render_sys);
   ~debug_renderer();

   bool init(int width, int height);
   void exit();
   void render();

   void draw_text(const v2 &origin, const char *text);
   void draw_text_format(const v2 &origin, const char *format, ...);

   struct vertex
   {
      v2 position_;
      v2 texcoord_;
   };

   glm::mat4 projection_;
   v4 glyph_uvs_[256];
   int vertex_cache_capacity_;
   int vertex_cache_count_;
   vertex *vertex_cache_;

   render_system *render_sys_;
   shader_program text_shader_program_;
   vertex_buffer text_vertex_buffer_;
   vertex_format text_vertex_format_;
   texture text_texture_;
   sampler_state text_sampler_;
};

#endif // DEBUG_RENDERER_H_INCLUDED
