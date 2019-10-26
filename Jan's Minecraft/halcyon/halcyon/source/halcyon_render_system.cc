// halcyon_render_system.cc

#include <halcyon.h>

#define HC_ARRAYCOUNT(x) (sizeof(x) / sizeof(x[0]))

vertex_format::vertex_format()
	: stride_(0)
	, attribute_count_(0) {
	//for (int i = 0; i < RENDER_SYSTEM_MAX_VERTEX_ATTRIBUTES; i++)
	for (auto &attrib : attributes_) {
		attrib = {};
	}
}

void vertex_format::add_attribute(attribute_sematic semantic,
	attribute_format format,
	int count,
	bool normalized) {
	static int format_sizes[] = { sizeof(float), sizeof(uint8_t), };
	HC_ASSERT(attribute_count_ < RENDER_SYSTEM_MAX_VERTEX_ATTRIBUTES);

	int index = attribute_count_++;
	attributes_[index].semantic_ = semantic;
	attributes_[index].format_ = format;
	attributes_[index].count_ = count;
	attributes_[index].offset_ = stride_;
	attributes_[index].normalized_ = normalized;

	stride_ += count * format_sizes[format];
}

// render system
static void
opengl_error_check() {
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		HC_ASSERT(!"opengl error code!");
	}

}

static const GLenum gl_texture_format_internal[] =
{
   GL_RGB,
   GL_RGBA,
};

static const GLenum gl_texture_format[] =
{
   GL_RGB,
   GL_RGBA,
};

static const GLenum gl_texture_target[] =
{
   GL_TEXTURE_2D,
   GL_TEXTURE_CUBE_MAP,
};

static const GLenum gl_sampler_filter[] =
{
   GL_NEAREST,
   GL_LINEAR,
   GL_NEAREST_MIPMAP_NEAREST,
   GL_NEAREST_MIPMAP_LINEAR,
   GL_LINEAR_MIPMAP_NEAREST,
   GL_LINEAR_MIPMAP_LINEAR,
};

static const GLenum gl_sampler_address[] =
{
   GL_CLAMP_TO_EDGE,
   GL_REPEAT,
   GL_MIRRORED_REPEAT
};

static const GLenum gl_blend_eq[] =
{
   GL_FUNC_ADD,
   GL_FUNC_SUBTRACT,
   GL_FUNC_REVERSE_SUBTRACT,
   GL_MIN,
   GL_MAX,
};

static const GLenum gl_blend_ft[] =
{
   GL_ZERO,
   GL_ONE,
   GL_SRC_COLOR,
   GL_ONE_MINUS_SRC_COLOR,
   GL_DST_COLOR,
   GL_ONE_MINUS_DST_COLOR,
   GL_SRC_ALPHA,
   GL_ONE_MINUS_SRC_ALPHA,
   GL_DST_ALPHA,
   GL_ONE_MINUS_DST_ALPHA,
   GL_CONSTANT_COLOR,
   GL_ONE_MINUS_CONSTANT_COLOR,
   GL_CONSTANT_ALPHA,
   GL_ONE_MINUS_CONSTANT_ALPHA,
   GL_SRC_ALPHA_SATURATE,
};

static const GLenum gl_compare_func[] =
{
   GL_NEVER,
   GL_LESS,
   GL_EQUAL,
   GL_LEQUAL,
   GL_GREATER,
   GL_NOTEQUAL,
   GL_GEQUAL,
   GL_ALWAYS,
};

static const GLenum gl_stencil_op[] =
{
   GL_KEEP,
   GL_ZERO,
   GL_REPLACE,
   GL_INCR,
   GL_INCR_WRAP,
   GL_DECR,
   GL_DECR_WRAP,
   GL_INVERT,
};

static const GLenum gl_cull_mode[] =
{
   GL_NONE,
   GL_BACK,
   GL_FRONT,
   GL_FRONT_AND_BACK,
};

static const GLenum gl_front_face[] =
{
   GL_CCW,
   GL_CW,
};

static const GLenum gl_primitive_topology[] =
{
   GL_POINTS,
   GL_LINES,
   GL_TRIANGLES,
};

static const GLenum gl_index_type[] =
{
   GL_UNSIGNED_BYTE,
   GL_UNSIGNED_SHORT,
   GL_UNSIGNED_INT,
};

static const GLuint gl_index_size[] =
{
   sizeof(uint8_t),
   sizeof(uint16_t),
   sizeof(uint32_t),
};

static const GLenum gl_attribute_type[] =
{
   GL_FLOAT,
   GL_UNSIGNED_BYTE,
};

static const GLuint gl_attribute_size[] =
{
   sizeof(float),
   sizeof(char),
};

static const GLenum gl_uniform_type[] =
{
   GL_FLOAT,
   GL_FLOAT_VEC2,
   GL_FLOAT_VEC3,
   GL_FLOAT_VEC4,
   GL_INT,
   GL_BOOL,
   GL_SAMPLER_2D,
   GL_FLOAT_MAT4,
};

static const GLuint gl_uniform_size[] =
{
   sizeof(float),
   sizeof(int),
   sizeof(int),
   sizeof(int),
   sizeof(float) * 16,
};

const GLenum gl_buffer_type[] =
{
   GL_ARRAY_BUFFER,
   GL_ELEMENT_ARRAY_BUFFER,
};

const GLenum gl_buffer_access[] =
{
   GL_STATIC_DRAW,
   GL_STREAM_DRAW,
};

const GLenum gl_framebuffer_format_internal[] =
{
   GL_NONE,
   GL_RGB,
   GL_RGBA,
   GL_DEPTH24_STENCIL8,
};

const GLenum gl_framebuffer_format[] =
{
   GL_NONE,
   GL_RGB,
   GL_RGBA,
   GL_DEPTH_STENCIL,
};

static GLenum gl_framebuffer_type[] =
{
   GL_NONE,
   GL_UNSIGNED_BYTE,
   GL_UNSIGNED_BYTE,
   GL_UNSIGNED_INT_24_8,
};


static GLuint gl_vertex_array_object = 0;

render_system::render_system() {
	gl_vertex_array_object = 0;
	glGenVertexArrays(1, &gl_vertex_array_object);
	glBindVertexArray(gl_vertex_array_object);

	opengl_error_check();
}

render_system::~render_system() {
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &gl_vertex_array_object);
}

void render_system::get_render_system_info(render_system_info &info) {
	info.version_ = (const char *)glGetString(GL_VERSION);
	info.renderer_ = (const char *)glGetString(GL_RENDERER);
	info.vendor_ = (const char *)glGetString(GL_VENDOR);
}

bool render_system::create_shader(shader_program &handle,
	const char *vertex_shader_source,
	const char *fragment_shader_source) {
	GLuint vid = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vid, 1, &vertex_shader_source, NULL);
	glCompileShader(vid);

	GLuint fid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fid, 1, &fragment_shader_source, NULL);
	glCompileShader(fid);

	GLuint pid = glCreateProgram();
	glAttachShader(pid, vid);
	glAttachShader(pid, fid);
	glLinkProgram(pid);

	GLint link_status = 0;
	glGetProgramiv(pid, GL_LINK_STATUS, &link_status);
	if (link_status == GL_FALSE) {
		GLchar vertex_error[1024];
		GLchar fragment_error[1024];
		GLchar program_error[1024];

		glGetShaderInfoLog(vid, sizeof(vertex_error), NULL, vertex_error);
		glGetShaderInfoLog(fid, sizeof(fragment_error), NULL, fragment_error);
		glGetProgramInfoLog(pid, sizeof(program_error), NULL, program_error);

		HC_ASSERT(!"shader program error");

		glDetachShader(pid, vid);
		glDetachShader(pid, fid);
		glDeleteProgram(pid);
	}
	else {
		handle.id_ = pid;
	}

	glDeleteShader(vid);
	glDeleteShader(fid);

	opengl_error_check();

	return true;
}

void render_system::destroy_shader(shader_program &handle) {
	glDeleteProgram(handle.id_);
	handle.id_ = 0;

	opengl_error_check();
}

bool render_system::create_texture(texture &handle,
	render_system_texture_format format,
	int width, int height, const void *data) {
	GLuint id = 0;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D,
		0, // mip level
		gl_texture_format_internal[format],
		width,
		height,
		0,
		gl_texture_format[format],
		GL_UNSIGNED_BYTE,
		data);
	glBindTexture(GL_TEXTURE_2D, 0);

	opengl_error_check();

	handle.id_ = id;

	return true;
}

bool render_system::create_cubemap(texture &handle,
	render_system_texture_format format,
	int width, int height,
	const void *data[6]) {
	GLuint id = 0;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	for (int index = 0; index < 6; index++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index,
			0,
			gl_texture_format_internal[format],
			width,
			height,
			0,
			gl_texture_format[format],
			GL_UNSIGNED_BYTE,
			data[index]);

		opengl_error_check();
	}

	handle.id_ = id;

	return true;
}

void render_system::update_texture(texture &handle,
	render_system_texture_format format,
	int width, int height, const void *data) {
	GLuint id = handle.id_;

	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		gl_texture_format_internal[format],
		width,
		height,
		0,
		gl_texture_format[format],
		GL_UNSIGNED_BYTE,
		data);
	glBindTexture(GL_TEXTURE_2D, 0);

	opengl_error_check();
}

void render_system::destroy_texture(texture &handle) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &handle.id_);
	handle.id_ = 0;

	opengl_error_check();
}

bool render_system::create_sampler_state(sampler_state &handle,
	render_system_sampler_filter filter,
	render_system_sampler_address_mode addr_u,
	render_system_sampler_address_mode addr_v,
	render_system_sampler_address_mode addr_w/* = RENDER_SYSTEM_SAMPLER_ADDRESS_MODE_CLAMP*/)

{
	GLuint id = 0;

	glGenSamplers(1, &id);
	glBindSampler(0, id);
	glSamplerParameteri(id,
		GL_TEXTURE_MIN_FILTER,
		gl_sampler_filter[filter]);
	glSamplerParameteri(id,
		GL_TEXTURE_MAG_FILTER,
		filter == RENDER_SYSTEM_SAMPLER_FILTER_NEAREST ? GL_NEAREST : GL_LINEAR);
	glSamplerParameteri(id,
		GL_TEXTURE_WRAP_S,
		gl_sampler_address[addr_u]);
	glSamplerParameteri(id,
		GL_TEXTURE_WRAP_T,
		gl_sampler_address[addr_v]);
	glSamplerParameteri(id,
		GL_TEXTURE_WRAP_R,
		gl_sampler_address[addr_w]);
	glBindSampler(0, 0);

	opengl_error_check();

	handle.id_ = id;

	return true;
}

void render_system::destroy_sampler_state(sampler_state &handle) {
	glDeleteSamplers(1, &handle.id_);
	handle.id_ = 0;

	opengl_error_check();
}

bool render_system::create_vertex_buffer(vertex_buffer &handle,
	render_system_buffer_access access,
	int size, const void *data) {
	GLuint id = 0;

	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, gl_buffer_access[access]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	opengl_error_check();

	handle.id_ = id;

	return true;
}

void render_system::update_vertex_buffer(vertex_buffer &handle,
	int size, const void *data) {
	glBindBuffer(GL_ARRAY_BUFFER, handle.id_);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	opengl_error_check();
}

void render_system::destroy_vertex_buffer(vertex_buffer &handle) {
	glDeleteBuffers(1, &handle.id_);
	handle.id_ = 0;

	opengl_error_check();
}

bool render_system::create_index_buffer(index_buffer &handle,
	int size, const void *data) {
	GLuint id = 0;

	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	opengl_error_check();

	handle.id_ = id;

	return true;
}

void render_system::destroy_index_buffer(index_buffer &handle) {
	glDeleteBuffers(1, &handle.id_);
	handle.id_ = 0;

	opengl_error_check();
}

bool render_system::create_framebuffer(framebuffer &handle, int width, int height,
	int color_attachment_format_count,
	const render_system_framebuffer_format *color_attachment_formats,
	render_system_framebuffer_format depth_attachment_format /* = RENDER_SYSTEM_FRAMEBUFFER_FORMAT_NONE */) {
	HC_ASSERT(width > 0);
	HC_ASSERT(height > 0);
	HC_ASSERT(color_attachment_format_count > 0);
	HC_ASSERT(color_attachment_format_count < RENDER_SYSTEM_MAX_FRAMEBUFFER_ATTACHMENTS);

	GLuint id = 0;
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	GLuint textures[RENDER_SYSTEM_MAX_FRAMEBUFFER_ATTACHMENTS] = {};
	glGenTextures(color_attachment_format_count, textures);

	int color_attachment_count = 0;
	int depth_attachment_count = 0;

	for (int attachment_index = 0;
		attachment_index < color_attachment_format_count;
		attachment_index++) {
		const render_system_framebuffer_format format = color_attachment_formats[attachment_index];

		glBindTexture(GL_TEXTURE_2D, textures[attachment_index]);
		glTexImage2D(GL_TEXTURE_2D,
			0,
			gl_framebuffer_format_internal[format],
			width,
			height,
			0,
			gl_framebuffer_format[format],
			gl_framebuffer_type[format],
			nullptr);

		opengl_error_check();

		if (format == RENDER_SYSTEM_FRAMEBUFFER_FORMAT_D32) {
			HC_ASSERT(depth_attachment_count < 1);

			glFramebufferTexture2D(GL_FRAMEBUFFER,
				GL_DEPTH_STENCIL_ATTACHMENT,
				GL_TEXTURE_2D,
				textures[attachment_index],
				0);

			depth_attachment_count++;
		}
		else {
			glFramebufferTexture2D(GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0 + color_attachment_count,
				GL_TEXTURE_2D,
				textures[attachment_index],
				0);

			color_attachment_count++;
		}

		opengl_error_check();
	}

	GLuint rbo = 0;
	if (depth_attachment_format != RENDER_SYSTEM_FRAMEBUFFER_FORMAT_NONE) {
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);

		glRenderbufferStorage(GL_RENDERBUFFER,
			gl_framebuffer_format_internal[depth_attachment_format],
			width,
			height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,
			GL_DEPTH_STENCIL_ATTACHMENT,
			GL_RENDERBUFFER,
			rbo);
	}

	GLenum complete_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (complete_status != GL_FRAMEBUFFER_COMPLETE) {
		HC_ASSERT(false);
	}

	opengl_error_check();

	handle.width_ = width;
	handle.height_ = height;
	handle.id_ = id;
	handle.depth_attachment_ = rbo;
	for (int index = 0; index < RENDER_SYSTEM_MAX_FRAMEBUFFER_ATTACHMENTS; index++) {
		handle.color_attachments_[index] = textures[index];
	}

	GLenum attachment_indices[RENDER_SYSTEM_MAX_FRAMEBUFFER_ATTACHMENTS] =
	{
	   GL_COLOR_ATTACHMENT0 + 0,
	   GL_COLOR_ATTACHMENT0 + 1,
	   GL_COLOR_ATTACHMENT0 + 2,
	   GL_COLOR_ATTACHMENT0 + 3,
	};
	glDrawBuffers(color_attachment_count, attachment_indices);

	return true;
}

void render_system::destroy_framebuffer(framebuffer &handle) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	handle.width_ = 0;
	handle.height_ = 0;

	glDeleteFramebuffers(1, &handle.id_);
	handle.id_ = 0;

	if (handle.depth_attachment_)
		glDeleteRenderbuffers(1, &handle.depth_attachment_);
	handle.depth_attachment_ = 0;

	for (uint32_t index = 0; index < RENDER_SYSTEM_MAX_FRAMEBUFFER_ATTACHMENTS; index++) {
		if (handle.color_attachments_[index]) {
			glDeleteTextures(1, handle.color_attachments_ + index);
			handle.color_attachments_[index] = 0;
		}
	}

	opengl_error_check();
}

void render_system::clear(v4 clear_color) {
	glClearColor(clear_color.r_,
		clear_color.g_,
		clear_color.b_,
		clear_color.a_);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_system::set_viewport(int x, int y, int width, int height) {
	glViewport(x, y, width, height);
}

void render_system::set_framebuffer(framebuffer &handle) {
	glBindFramebuffer(GL_FRAMEBUFFER, handle.id_);
	set_viewport(0, 0, handle.width_, handle.height_);

	if (!handle.id_)
		return;
}

void render_system::set_shader_program(shader_program &handle) {
	glUseProgram(handle.id_);
}

void render_system::set_shader_uniform(shader_program &handle,
	render_system_uniform_type type,
	const char *name, int count, const void *value) {
	GLint location = glGetUniformLocation(handle.id_, name);
	if (location == -1)
		return;

	switch (type) {
	case RENDER_SYSTEM_UNIFORM_TYPE_FLOAT:
	{
		glUniform1fv(location, count, (const GLfloat *)value);
		opengl_error_check();
	} break;
	case RENDER_SYSTEM_UNIFORM_TYPE_VEC2:
	{
		glUniform2fv(location, count, (const GLfloat *)value);
		opengl_error_check();
	} break;
	case RENDER_SYSTEM_UNIFORM_TYPE_VEC3:
	{
		glUniform3fv(location, count, (const GLfloat *)value);
		opengl_error_check();
	} break;
	case RENDER_SYSTEM_UNIFORM_TYPE_VEC4:
	{
		glUniform4fv(location, count, (const GLfloat *)value);
		opengl_error_check();
	} break;
	case RENDER_SYSTEM_UNIFORM_TYPE_INT:
	case RENDER_SYSTEM_UNIFORM_TYPE_BOOL:
	case RENDER_SYSTEM_UNIFORM_TYPE_SAMPLER:
	{
		glUniform1iv(location, count, (const GLint *)value);
		opengl_error_check();
	} break;
	case RENDER_SYSTEM_UNIFORM_TYPE_MATRIX:
	{
		glUniformMatrix4fv(location, count, GL_FALSE, (const GLfloat *)value);
		opengl_error_check();
	} break;
	}
}

void render_system::set_index_buffer(index_buffer &handle) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle.id_);

	opengl_error_check();
}

void render_system::set_vertex_buffer(vertex_buffer &handle) {
	glBindBuffer(GL_ARRAY_BUFFER, handle.id_);

	opengl_error_check();
}

void render_system::set_vertex_format(vertex_format &format) {
	// note: disable all attributes
	for (int i = 0; i < RENDER_SYSTEM_MAX_VERTEX_ATTRIBUTES; i++) {
		glDisableVertexAttribArray(i);
	}

	const int vertex_stride = format.stride_;
	const int attribute_count = format.attribute_count_;
	for (int attribute_index = 0;
		attribute_index < attribute_count;
		attribute_index++) {
		const vertex_format::attribute &attribute = format.attributes_[attribute_index];

		glEnableVertexAttribArray(attribute.semantic_);
		glVertexAttribPointer(attribute.semantic_,
			attribute.count_,
			gl_attribute_type[attribute.format_],
			attribute.normalized_,
			vertex_stride,
			(const void *)(uintptr_t)attribute.offset_);
	}

	opengl_error_check();
}

void render_system::set_texture(texture &handle, int unit/* = 0 */, render_system_texture_target target/* = RENDER_SYSTEM_TEXTURE_TARGET_TEXTURE_2D*/) {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(gl_texture_target[target], handle.id_);

	opengl_error_check();
}

void render_system::set_sampler_state(sampler_state &handle, int unit/* = 0 */) {
	glBindSampler(unit, handle.id_);

	opengl_error_check();
}

void render_system::set_blend_state(bool enabled,
	render_system_blend_equation eq_rgb,
	render_system_blend_factor src_rgb,
	render_system_blend_factor dst_rgb,
	render_system_blend_equation eq_alpha,
	render_system_blend_factor src_alpha,
	render_system_blend_factor dst_alpha) {
	if (enabled) {
		glEnable(GL_BLEND);
		glBlendFuncSeparate(gl_blend_ft[src_rgb],
			gl_blend_ft[dst_rgb],
			gl_blend_ft[src_alpha],
			gl_blend_ft[dst_alpha]);
		glBlendEquationSeparate(gl_blend_eq[eq_rgb],
			gl_blend_eq[eq_alpha]);
	}
	else {
		glDisable(GL_BLEND);
	}

	opengl_error_check();
}

void render_system::set_depth_state(bool write_enabled,
	bool read_enabled,
	float range_near/* = -1.0f */,
	float range_far/* = 1.0f */,
	render_system_compare_func func/* = RENDER_SYSTEM_COMPARE_FUNC_LESS */) {
	if (read_enabled) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(gl_compare_func[func]);

		if (write_enabled)
			glDepthMask(GL_TRUE);
		else
			glDepthMask(GL_FALSE);
	}
	else {
		glDisable(GL_DEPTH_TEST);
		//glDepthMask(GL_FALSE);
	}

	glDepthRange(range_near, range_far);

	opengl_error_check();
}

void render_system::set_rasterizer_state(render_system_cull_mode cull_mode,
	render_system_front_face front_face/* = RENDER_SYSTEM_FRONT_FACE_CW */) {
	if (cull_mode != RENDER_SYSTEM_CULL_MODE_NONE) {
		glEnable(GL_CULL_FACE);
		glCullFace(gl_cull_mode[cull_mode]);
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	glFrontFace(gl_front_face[front_face]);

	opengl_error_check();
}

void render_system::draw(render_system_primitive_topology topology,
	int start_index,
	int primitive_count) {
	glDrawArrays(gl_primitive_topology[topology],
		start_index,
		primitive_count);

	opengl_error_check();
}

void render_system::draw_indexed(render_system_primitive_topology topology,
	render_system_index_type type,
	int start_index,
	int primitive_count) {
	glDrawElements(gl_primitive_topology[topology],
		primitive_count,
		gl_index_type[type],
		(const void *)(uintptr_t)(gl_index_size[type] * start_index));

	opengl_error_check();
}
