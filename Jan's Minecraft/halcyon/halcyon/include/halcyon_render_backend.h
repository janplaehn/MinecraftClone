// halcyon_render_backend.h

#ifndef HALCYON_RENDER_BACKEND_H_INCLUDED
#define HALCYON_RENDER_BACKEND_H_INCLUDED

#include <stdint.h>

// config
#define RENDERER_MAX_VERTEX_ATTRIBUTES    8
#define RENDERER_MAX_TEXTURE_BINDINGS     4
#define RENDERER_MAX_UNIFORM_BINGINGS     16

// info
struct renderer_info
{
   const char *version_;
   const char *vendor_;
   const char *renderer_;
};

// types
struct renderer_shader_program { uint32_t id_; };
struct renderer_texture { uint32_t id_; };
struct renderer_sampler { uint32_t id_; };
struct renderer_buffer { uint32_t id_; };

enum renderer_texture_format
{
   RENDERER_TEXTURE_FORMAT_RGB8,
   RENDERER_TEXTURE_FORMAT_RGBA8,
};

enum renderer_sampler_filter
{
   RENDERER_SAMPLER_FILTER_NEAREST,
   RENDERER_SAMPLER_FILTER_LINEAR,
};

enum renderer_sampler_address_mode
{
   RENDERER_SAMPLER_ADDRESS_MODE_CLAMP,
   RENDERER_SAMPLER_ADDRESS_MODE_REPEAT,
   RENDERER_SAMPLER_ADDRESS_MODE_MIRROR,
};

enum renderer_buffer_type
{
   RENDERER_BUFFER_TYPE_VERTEX,
   RENDERER_BUFFER_TYPE_INDEX,
};

enum renderer_buffer_access
{
   RENDERER_BUFFER_ACCESS_STATIC,
   RENDERER_BUFFER_ACCESS_DYNAMIC,
};

enum renderer_blend_equation
{
   RENDERER_BLEND_EQUATION_ADD,
   RENDERER_BLEND_EQUATION_SUBTRACT,
   RENDERER_BLEND_EQUATION_REVERSE_SUBTRACT,
   RENDERER_BLEND_EQUATION_MIN,
   RENDERER_BLEND_EQUATION_MAX,
};

enum renderer_blend_factor
{
   RENDERER_BLEND_FACTOR_ZERO,
   RENDERER_BLEND_FACTOR_ONE,
   RENDERER_BLEND_FACTOR_SRC_COLOR,
   RENDERER_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
   RENDERER_BLEND_FACTOR_DST_COLOR,
   RENDERER_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
   RENDERER_BLEND_FACTOR_SRC_ALPHA,
   RENDERER_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
   RENDERER_BLEND_FACTOR_DST_ALPHA,
   RENDERER_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
   RENDERER_BLEND_FACTOR_CONSTANT_COLOR,
   RENDERER_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
   RENDERER_BLEND_FACTOR_CONSTANT_ALPHA,
   RENDERER_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
   RENDERER_BLEND_FACTOR_SRC_ALPHA_SATURATE,
};

struct renderer_blend_state
{
   bool enabled_;
   renderer_blend_equation eq_rgb_;
   renderer_blend_factor src_rgb_;
   renderer_blend_factor dst_rgb_;
   renderer_blend_equation eq_alpha_;
   renderer_blend_factor src_alpha_;
   renderer_blend_factor dst_alpha_;
};

enum renderer_compare_func
{
   RENDERER_COMPARE_FUNC_NEVER,
   RENDERER_COMPARE_FUNC_LESS,
   RENDERER_COMPARE_FUNC_EQUAL,
   RENDERER_COMPARE_FUNC_LEQUAL,
   RENDERER_COMPARE_FUNC_GREATER,
   RENDERER_COMPARE_FUNC_NOTEQUAL,
   RENDERER_COMPARE_FUNC_GEQUAL,
   RENDERER_COMPARE_FUNC_ALWAYS,
};

enum renderer_stencil_op
{
   RENDERER_STENCIL_OP_KEEP,
   RENDERER_STENCIL_OP_ZERO,
   RENDERER_STENCIL_OP_REPLACE,
   RENDERER_STENCIL_OP_INCR,
   RENDERER_STENCIL_OP_INCR_WRAP,
   RENDERER_STENCIL_OP_DECR,
   RENDERER_STENCIL_OP_DECR_WRAP,
   RENDERER_STENCIL_OP_INVERT,
};

struct renderer_depth_stencil_state
{
   struct
   {
      bool write_enabled_;
      bool read_enabled_;
      float range_near_;
      float range_far_;
      renderer_compare_func func_;
   } depth_;
   struct
   {
      bool write_enabled_;
      bool read_enabled_;
      int mask_;
      int ref_;
      renderer_compare_func func_;
      renderer_stencil_op fail_;
      renderer_stencil_op depth_fail_;
      renderer_stencil_op depth_pass_;
   } stencil_;
};

enum renderer_cull_mode
{
   RENDERER_CULL_MODE_NONE,
   RENDERER_CULL_MODE_BACK,
   RENDERER_CULL_MODE_FRONT,
   RENDERER_CULL_MODE_BOTH,
};

enum renderer_front_face
{
   RENDERER_FRONT_FACE_CCW,
   RENDERER_FRONT_FACE_CW,
};

struct renderer_rasterizer_state
{
   renderer_cull_mode cull_mode_;
   renderer_front_face front_face_;
};

enum renderer_primitive_topology
{
   RENDERER_PRIMITIVE_TOPOLOGY_POINT_LIST,
   RENDERER_PRIMITIVE_TOPOLOGY_LINE_LIST,
   RENDERER_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
};

enum renderer_attribute_type
{
   RENDERER_ATTRIBUTE_TYPE_FLOAT,
   RENDERER_ATTRIBUTE_TYPE_BYTE,
};

struct renderer_vertex_attribute
{
   int index_;
   int size_;
   int offset_;
   renderer_attribute_type type_;
   bool normalized_;
};

struct renderer_vertex_format
{
   int stride_;
   int attribute_count_;
   renderer_vertex_attribute attributes_[RENDERER_MAX_VERTEX_ATTRIBUTES];
};

enum renderer_framebuffer_format
{
   RENDERER_FRAMEBUFFER_FORMAT_RGB8,
   RENDERER_FRAMEBUFFER_FORMAT_RGBA8,
   RENDERER_FRAMEBUFFER_FORMAT_R32F,
   RENDERER_FRAMEBUFFER_FORMAT_R24FG8,
};

struct renderer_framebuffer
{
   uint32_t id_;
   int width_;
   int height_;
   uint32_t color_id_;
   uint32_t depth_id_;
};

struct renderer_viewport
{
   int x_, y_;
   int width_;
   int height_;
};

enum renderer_uniform_type
{
   RENDERER_UNIFORM_TYPE_FLOAT,
   RENDERER_UNIFORM_TYPE_INT,
   RENDERER_UNIFORM_TYPE_BOOL,
   RENDERER_UNIFORM_TYPE_SAMPLER,
   RENDERER_UNIFORM_TYPE_MATRIX,
};

struct renderer_uniform
{
   renderer_uniform_type type_;
   int location_;
   int size_;
   union
   {
      float f32_;
      v2 v2_;
      v3 v3_;
      v4 v4_;
      m4 m4_;
      const void *data_;
   };
};

struct renderer_pipeline
{
   renderer_shader_program shader_program_;
   renderer_blend_state blend_state_;
   renderer_depth_stencil_state depth_stencil_state_;
   renderer_rasterizer_state rasterizer_state_;
   v4 clear_color_;
};

struct renderer_input_assembly
{
   renderer_vertex_format vertex_format_;
   renderer_buffer vertex_buffer_;
   renderer_buffer index_buffer_;
};

struct renderer_resource_bindings
{
   int texture_count_;
   renderer_texture textures_[RENDERER_MAX_TEXTURE_BINDINGS];
   int sampler_count_;
   renderer_sampler samplers_[RENDERER_MAX_TEXTURE_BINDINGS];
   int uniform_count_;
   renderer_uniform uniforms_[RENDERER_MAX_UNIFORM_BINGINGS];
};

struct renderer_drawcall
{
   renderer_primitive_topology primitive_topology_;
   int vertex_count_;
   int instance_count_;
   int index_buffer_offset_;
   int vertex_buffer_offset_;
};

// render backend
void renderer_get_info(renderer_info *info);

void renderer_create_shader(renderer_shader_program *shader,
                            const char *vertex_shader_source,
                            const char *fragment_shader_source);
void renderer_destroy_shader(renderer_shader_program shader);
void renderer_get_attribute_locations(renderer_shader_program shader, int count,
                                      const char **names, int *locations);
void renderer_get_uniform_locations(renderer_shader_program shader, int count,
                                    const char **names, int *locations);

void renderer_create_texture(renderer_texture *texture, renderer_texture_format format, 
                             int width, int height, const void *data);
void renderer_destroy_texture(renderer_texture texture);
void renderer_update_texture(renderer_texture texture, int level,
                             renderer_texture_format format,
                             int width, int height, const void *data);

void renderer_create_sampler(renderer_sampler *sampler, renderer_sampler_filter filter,
                             renderer_sampler_address_mode addr_u, renderer_sampler_address_mode addr_v);
void renderer_destroy_sampler(renderer_sampler sampler);

void renderer_create_buffer(renderer_buffer *buffer,
                            renderer_buffer_type type, renderer_buffer_access access,
                            int size, const void *data);
void renderer_destroy_buffer(renderer_buffer buffer);
void renderer_update_buffer(renderer_buffer buffer, renderer_buffer_type type,
                            int size, const void *data);

void renderer_add_attribute(renderer_vertex_format *format, int index, int size,
                            renderer_attribute_type type, bool normalized);

void renderer_add_texture_binding(renderer_resource_bindings *bindings, renderer_texture texture);
void renderer_add_sampler_binding(renderer_resource_bindings *bindings, renderer_sampler sampler);
void renderer_add_uniform_binding(renderer_resource_bindings *bindings, renderer_uniform_type type,
                                  int location, int size, const void *data);

void renderer_bind_pipeline(renderer_pipeline *pipeline, renderer_viewport *viewport);
void renderer_bind_input_assembly(renderer_input_assembly *assembly);
void renderer_bind_resource_bindings(renderer_resource_bindings *bindings);
void renderer_submit_drawcall(renderer_drawcall *drawcall);

#endif // HALCYON_RENDER_BACKEND_H_INCLUDED
