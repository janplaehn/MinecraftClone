// halcyon_render_system.h

#ifndef HALCYON_RENDER_SYSTEM_H_INCLUDED
#define HALCYON_RENDER_SYSTEM_H_INCLUDED

#include <stdint.h>

// note: config
#define RENDER_SYSTEM_MAX_VERTEX_ATTRIBUTES        8
#define RENDER_SYSTEM_MAX_TEXTURE_BINDINGS         4
#define RENDER_SYSTEM_MAX_UNIFORM_BINGINGS         16
#define RENDER_SYSTEM_MAX_FRAMEBUFFER_ATTACHMENTS  4

// note: render system enums
enum render_system_uniform_type {
	RENDER_SYSTEM_UNIFORM_TYPE_FLOAT,
	RENDER_SYSTEM_UNIFORM_TYPE_VEC2,
	RENDER_SYSTEM_UNIFORM_TYPE_VEC3,
	RENDER_SYSTEM_UNIFORM_TYPE_VEC4,
	RENDER_SYSTEM_UNIFORM_TYPE_INT,
	RENDER_SYSTEM_UNIFORM_TYPE_BOOL,
	RENDER_SYSTEM_UNIFORM_TYPE_SAMPLER,
	RENDER_SYSTEM_UNIFORM_TYPE_MATRIX,
};

enum render_system_texture_format {
	RENDER_SYSTEM_TEXTURE_FORMAT_RGB8,
	RENDER_SYSTEM_TEXTURE_FORMAT_RGBA8,
};

enum render_system_cube_map_face {
	RENDER_SYSTEM_CUBE_MAP_FACE_POSITIVE_X,
	RENDER_SYSTEM_CUBE_MAP_FACE_NEGATIVE_X,
	RENDER_SYSTEM_CUBE_MAP_FACE_POSITIVE_Y,
	RENDER_SYSTEM_CUBE_MAP_FACE_NEGATIVE_Y,
	RENDER_SYSTEM_CUBE_MAP_FACE_POSITIVE_Z,
	RENDER_SYSTEM_CUBE_MAP_FACE_NEGATIVE_Z,
	RENDER_SYSTEM_CUBE_MAP_FACE_COUNT,
};

enum render_system_texture_target {
	RENDER_SYSTEM_TEXTURE_TARGET_TEXTURE_2D,
	RENDER_SYSTEM_TEXTURE_TARGET_TEXTURE_CUBEMAP,
};

enum render_system_sampler_filter {
	RENDER_SYSTEM_SAMPLER_FILTER_NEAREST,
	RENDER_SYSTEM_SAMPLER_FILTER_LINEAR,
};

enum render_system_sampler_address_mode {
	RENDER_SYSTEM_SAMPLER_ADDRESS_MODE_CLAMP,
	RENDER_SYSTEM_SAMPLER_ADDRESS_MODE_REPEAT,
	RENDER_SYSTEM_SAMPLER_ADDRESS_MODE_MIRROR,
};

enum render_system_buffer_type {
	RENDER_SYSTEM_BUFFER_TYPE_VERTEX,
	RENDER_SYSTEM_BUFFER_TYPE_INDEX,
};

enum render_system_buffer_access {
	RENDER_SYSTEM_BUFFER_ACCESS_STATIC,
	RENDER_SYSTEM_BUFFER_ACCESS_DYNAMIC,
};

enum render_system_blend_equation {
	RENDER_SYSTEM_BLEND_EQUATION_ADD,
	RENDER_SYSTEM_BLEND_EQUATION_SUBTRACT,
	RENDER_SYSTEM_BLEND_EQUATION_REVERSE_SUBTRACT,
	RENDER_SYSTEM_BLEND_EQUATION_MIN,
	RENDER_SYSTEM_BLEND_EQUATION_MAX,
};

enum render_system_blend_factor {
	RENDER_SYSTEM_BLEND_FACTOR_ZERO,
	RENDER_SYSTEM_BLEND_FACTOR_ONE,
	RENDER_SYSTEM_BLEND_FACTOR_SRC_COLOR,
	RENDER_SYSTEM_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
	RENDER_SYSTEM_BLEND_FACTOR_DST_COLOR,
	RENDER_SYSTEM_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
	RENDER_SYSTEM_BLEND_FACTOR_SRC_ALPHA,
	RENDER_SYSTEM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
	RENDER_SYSTEM_BLEND_FACTOR_DST_ALPHA,
	RENDER_SYSTEM_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
	RENDER_SYSTEM_BLEND_FACTOR_CONSTANT_COLOR,
	RENDER_SYSTEM_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
	RENDER_SYSTEM_BLEND_FACTOR_CONSTANT_ALPHA,
	RENDER_SYSTEM_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
	RENDER_SYSTEM_BLEND_FACTOR_SRC_ALPHA_SATURATE,
};

enum render_system_compare_func {
	RENDER_SYSTEM_COMPARE_FUNC_NEVER,
	RENDER_SYSTEM_COMPARE_FUNC_LESS,
	RENDER_SYSTEM_COMPARE_FUNC_EQUAL,
	RENDER_SYSTEM_COMPARE_FUNC_LEQUAL,
	RENDER_SYSTEM_COMPARE_FUNC_GREATER,
	RENDER_SYSTEM_COMPARE_FUNC_NOTEQUAL,
	RENDER_SYSTEM_COMPARE_FUNC_GEQUAL,
	RENDER_SYSTEM_COMPARE_FUNC_ALWAYS,
};

enum render_system_stencil_op {
	RENDER_SYSTEM_STENCIL_OP_KEEP,
	RENDER_SYSTEM_STENCIL_OP_ZERO,
	RENDER_SYSTEM_STENCIL_OP_REPLACE,
	RENDER_SYSTEM_STENCIL_OP_INCR,
	RENDER_SYSTEM_STENCIL_OP_INCR_WRAP,
	RENDER_SYSTEM_STENCIL_OP_DECR,
	RENDER_SYSTEM_STENCIL_OP_DECR_WRAP,
	RENDER_SYSTEM_STENCIL_OP_INVERT,
};

enum render_system_cull_mode {
	RENDER_SYSTEM_CULL_MODE_NONE,
	RENDER_SYSTEM_CULL_MODE_BACK,
	RENDER_SYSTEM_CULL_MODE_FRONT,
	RENDER_SYSTEM_CULL_MODE_BOTH,
};

enum render_system_front_face {
	RENDER_SYSTEM_FRONT_FACE_CCW,
	RENDER_SYSTEM_FRONT_FACE_CW,
};

enum render_system_primitive_topology {
	RENDER_SYSTEM_PRIMITIVE_TOPOLOGY_POINT_LIST,
	RENDER_SYSTEM_PRIMITIVE_TOPOLOGY_LINE_LIST,
	RENDER_SYSTEM_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
};

enum render_system_index_type {
	RENDER_SYSTEM_INDEX_TYPE_UNSIGNED_BYTE,
	RENDER_SYSTEM_INDEX_TYPE_UNSIGNED_SHORT,
	RENDER_SYSTEM_INDEX_TYPE_UNSIGNED_INT,
};

// note: render system resource types
struct vertex_format {
	enum attribute_sematic {
		ATTRIBUTE_SEMANTIC_POSITION,
		ATTRIBUTE_SEMANTIC_TEXCOORD,
		ATTRIBUTE_SEMANTIC_NORMAL,
		ATTRIBUTE_SEMANTIC_COLOR,
	};

	enum attribute_format {
		ATTRIBUTE_FORMAT_FLOAT,
		ATTRIBUTE_FORMAT_BYTE,
	};

	struct attribute {
		attribute_sematic semantic_;
		attribute_format format_;
		int count_;
		int offset_;
		bool normalized_;
	};

	vertex_format();

	void add_attribute(attribute_sematic semantic,
		attribute_format format,
		int count,
		bool normalized);

	int stride_;
	int attribute_count_;
	attribute attributes_[RENDER_SYSTEM_MAX_VERTEX_ATTRIBUTES];
};

// note: render system resources handles
struct shader_program { uint32_t id_; };
struct texture { uint32_t id_; };
struct sampler_state { uint32_t id_; };
struct vertex_buffer { uint32_t id_; };
struct index_buffer { uint32_t id_; };

// note: framebuffer
enum render_system_framebuffer_format {
	RENDER_SYSTEM_FRAMEBUFFER_FORMAT_NONE,
	RENDER_SYSTEM_FRAMEBUFFER_FORMAT_RGB8,
	RENDER_SYSTEM_FRAMEBUFFER_FORMAT_RGBA8,
	RENDER_SYSTEM_FRAMEBUFFER_FORMAT_D32,
	RENDER_SYSTEM_FRAMEBUFFER_FORMAT_COUNT,
	RENDER_SYSTEM_FRAMEBUFFER_FORMAT_INVALID,
};

struct framebuffer {
	int width_;
	int height_;
	uint32_t id_;
	uint32_t depth_attachment_;
	uint32_t color_attachments_[RENDER_SYSTEM_MAX_FRAMEBUFFER_ATTACHMENTS];
};

// note: constant resources handles
const shader_program INVALID_SHADER_PROGRAM = {};
const texture INVALID_TEXTURE = {};
const sampler_state INVALID_SAMPLER_STATE = {};
const vertex_buffer INVALID_VERTEX_BUFFER = {};
const index_buffer INVALID_INDEX_BUFFER = {};

// note: render system information
struct render_system_info {
	const char *version_;
	const char *renderer_;
	const char *vendor_;
};

// note: render system api
struct render_system {
	render_system();
	~render_system();

	void get_render_system_info(render_system_info &info);

	bool create_shader(shader_program &handle,
		const char *vertex_shader_source,
		const char *fragment_shader_source);
	void destroy_shader(shader_program &handle);

	bool create_texture(texture &handle,
		render_system_texture_format format,
		int width, int height, const void *data);
	bool create_cubemap(texture &handle,
		render_system_texture_format format,
		int width, int height,
		const void *data[6]);
	void update_texture(texture &handle,
		render_system_texture_format format,
		int width, int height, const void *data);
	void destroy_texture(texture &handle);

	bool create_sampler_state(sampler_state &handle,
		render_system_sampler_filter filter,
		render_system_sampler_address_mode addr_u,
		render_system_sampler_address_mode addr_v,
		render_system_sampler_address_mode addr_w = RENDER_SYSTEM_SAMPLER_ADDRESS_MODE_CLAMP);
	void destroy_sampler_state(sampler_state &handle);

	bool create_vertex_buffer(vertex_buffer &handle,
		render_system_buffer_access access,
		int size, const void *data);
	void update_vertex_buffer(vertex_buffer &handle,
		int size, const void *data);
	void destroy_vertex_buffer(vertex_buffer &handle);

	bool create_index_buffer(index_buffer &handle,
		int size, const void *data);
	void destroy_index_buffer(index_buffer &handle);

	bool create_framebuffer(framebuffer &handle, int width, int height,
		int color_attachment_format_count,
		const render_system_framebuffer_format *color_attachment_formats,
		render_system_framebuffer_format depth_attachment_format = RENDER_SYSTEM_FRAMEBUFFER_FORMAT_NONE);
	void destroy_framebuffer(framebuffer &handle);

	void clear(v4 clear_color);
	void set_viewport(int x, int y, int width, int height);
	void set_framebuffer(framebuffer &handle);
	void set_shader_program(shader_program &handle);
	void set_shader_uniform(shader_program &handle,
		render_system_uniform_type type,
		const char *name, int count, const void *value);
	void set_index_buffer(index_buffer &handle);
	void set_vertex_buffer(vertex_buffer &handle);
	void set_vertex_format(vertex_format &format);
	void set_texture(texture &handle, int unit = 0, render_system_texture_target target = RENDER_SYSTEM_TEXTURE_TARGET_TEXTURE_2D);
	void set_sampler_state(sampler_state &handle, int unit = 0);
	void set_blend_state(bool enabled,
		render_system_blend_equation eq_rgb = RENDER_SYSTEM_BLEND_EQUATION_ADD,
		render_system_blend_factor src_rgb = RENDER_SYSTEM_BLEND_FACTOR_SRC_ALPHA,
		render_system_blend_factor dst_rgb = RENDER_SYSTEM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		render_system_blend_equation eq_alpha = RENDER_SYSTEM_BLEND_EQUATION_ADD,
		render_system_blend_factor src_alpha = RENDER_SYSTEM_BLEND_FACTOR_ONE,
		render_system_blend_factor dst_alpha = RENDER_SYSTEM_BLEND_FACTOR_ONE);
	void set_depth_state(bool write_enabled,
		bool read_enabled,
		float range_near = -1.0f,
		float range_far = 1.0f,
		render_system_compare_func func = RENDER_SYSTEM_COMPARE_FUNC_LESS);
	void set_rasterizer_state(render_system_cull_mode cull_mode,
		render_system_front_face front_face = RENDER_SYSTEM_FRONT_FACE_CW);
	void draw(render_system_primitive_topology topology,
		int start_index,
		int primitive_count);
	void draw_indexed(render_system_primitive_topology topology,
		render_system_index_type type,
		int start_index,
		int primitive_count);
};

#endif // HALCYON_RENDER_SYSTEM_H_INCLUDED
