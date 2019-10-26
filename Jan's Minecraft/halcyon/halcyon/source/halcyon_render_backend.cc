// halcyon_render_backend.cc

#include <halcyon.h>
#include <halcyon_render_backend.h>

#define HC_ARRAYCOUNT(x) (sizeof(x) / sizeof(x[0]))

#define ENABLE_OPENGL_ERROR_CHECK 1

#if ENABLE_OPENGL_ERROR_CHECK
struct renderer_opengl_error_t
{
   const char *string;
   const GLenum code;
};

static renderer_opengl_error_t renderer_opengl_error_strings[] =
{
   { "GL_NO_ERROR", GL_NO_ERROR },
   { "GL_INVALID_ENUM", GL_INVALID_ENUM },
   { "GL_INVALID_VALUE", GL_INVALID_VALUE },
   { "GL_INVALID_OPERATION", GL_INVALID_OPERATION },
   { "GL_OUT_OF_MEMORY", GL_OUT_OF_MEMORY },
   { "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT", GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT },
   { "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT", GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT },
   { "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER", GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER },
   { "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER", GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER },
   { "GL_FRAMEBUFFER_UNSUPPORTED", GL_FRAMEBUFFER_UNSUPPORTED },
};

static const char *renderer_opengl_error_string(GLenum err)
{
   for (int i = 0; i < HC_ARRAYCOUNT(renderer_opengl_error_strings); i++)
   {
      if (renderer_opengl_error_strings[i].code == err)
      {
         return renderer_opengl_error_strings[i].string;
      }
   }

   return "GL_UNKNOWN_ERROR";
}

static void renderer_opengl_error_check()
{
   GLenum err = glGetError();
   if (err != GL_NO_ERROR)
   {
      const char *error_type = renderer_opengl_error_string(err);
      (void)error_type;
      //MessageBoxA(NULL, error_type, "OpenGL Error!", MB_OK);
      HC_ASSERT(!"opengl error code!");
   }
}
#else
#define renderer_opengl_error_check() 
#endif

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

void renderer_get_info(renderer_info *info)
{
   info->version_ = (const char *)glGetString(GL_VERSION);
   info->vendor_ = (const char *)glGetString(GL_VENDOR);
   info->renderer_ = (const char *)glGetString(GL_RENDERER);
}

void renderer_create_shader(renderer_shader_program *shader,
                            const char *vertex_shader_source,
                            const char *fragment_shader_source)
{
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
   if (link_status == GL_FALSE)
   {
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
   else
   {
      shader->id_ = pid;
   }

   glDeleteShader(vid);
   glDeleteShader(fid);

   renderer_opengl_error_check();
}

void renderer_destroy_shader(renderer_shader_program shader)
{
   glDeleteProgram(shader.id_);
   renderer_opengl_error_check();
}

void renderer_get_attribute_locations(renderer_shader_program shader, int count,
                                      const char **names, int *locations)
{
   //glUseProgram(shader.id_);
   for (int i = 0; i < count; i++)
      locations[i] = glGetAttribLocation(shader.id_, names[i]);

   renderer_opengl_error_check();
}

void renderer_get_uniform_locations(renderer_shader_program shader, int count,
                                    const char **names, int *locations)
{
   //glUseProgram(shader.id_);
   for (int i = 0; i < count; i++)
      locations[i] = glGetUniformLocation(shader.id_, names[i]);

   renderer_opengl_error_check();
}

void renderer_create_texture(renderer_texture *texture, renderer_texture_format format,
                             int width, int height, const void *data)
{
   GLuint id = 0;
   glGenTextures(1, &id);
   glBindTexture(GL_TEXTURE_2D, id);
#if 0
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                   gl_texture_filter[filter]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                   filter == RENDERER_TEXTURE_FILTER_NEAREST ? GL_NEAREST : GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_texture_address[addr_u]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_texture_address[addr_v]);
#endif
   glTexImage2D(GL_TEXTURE_2D,
                0, // mip level
                gl_texture_format_internal[format],
                width,
                height,
                0,
                gl_texture_format[format],
                GL_UNSIGNED_BYTE, data);
   glBindTexture(GL_TEXTURE_2D, 0);

   texture->id_ = id;

   renderer_opengl_error_check();
}

void renderer_destroy_texture(renderer_texture texture)
{
   glBindTexture(GL_TEXTURE_2D, 0);
   glDeleteTextures(1, &texture.id_);
   renderer_opengl_error_check();
}

void renderer_update_texture(renderer_texture texture, int level, 
                             renderer_texture_format format,
                             int width, int height, const void *data)
{
   GLuint id = texture.id_;

   glBindTexture(GL_TEXTURE_2D, id);
   glTexImage2D(GL_TEXTURE_2D, 
                level, 
                gl_texture_format_internal[format], 
                width, 
                height,
                0, 
                gl_texture_format[format], 
                GL_UNSIGNED_BYTE, 
                data);
   glBindTexture(GL_TEXTURE_2D, 0);

   renderer_opengl_error_check();
}

void renderer_create_sampler(renderer_sampler *sampler, renderer_sampler_filter filter,
                             renderer_sampler_address_mode addr_u, renderer_sampler_address_mode addr_v)
{
   GLuint id = 0;
   
   glGenSamplers(1, &id);
   glBindSampler(0, id);
   glSamplerParameteri(id, 
                       GL_TEXTURE_MIN_FILTER,
                       gl_sampler_filter[filter]);
   glSamplerParameteri(id,
                       GL_TEXTURE_MAG_FILTER,
                       filter == RENDERER_SAMPLER_FILTER_NEAREST ? GL_NEAREST : GL_LINEAR);
   glSamplerParameteri(id,
                       GL_TEXTURE_WRAP_S, 
                       gl_sampler_address[addr_u]);
   glSamplerParameteri(id,
                       GL_TEXTURE_WRAP_T, 
                       gl_sampler_address[addr_v]);
   glBindSampler(0, 0);

   sampler->id_ = id;

   renderer_opengl_error_check();
}

void renderer_destroy_sampler(renderer_sampler sampler)
{
   glDeleteSamplers(1, &sampler.id_);
   renderer_opengl_error_check();
}

void renderer_create_buffer(renderer_buffer *buffer,
                            renderer_buffer_type type, renderer_buffer_access access,
                            int size, const void *data)
{
   GLuint id = 0;

   glGenBuffers(1, &id);
   glBindBuffer(gl_buffer_type[type], id);
   glBufferData(gl_buffer_type[type], size, data, gl_buffer_access[access]);
   glBindBuffer(gl_buffer_type[type], 0);

   buffer->id_ = id;

   renderer_opengl_error_check();
}

void renderer_destroy_buffer(renderer_buffer buffer)
{
   glDeleteBuffers(1, &buffer.id_);
   renderer_opengl_error_check();
}

void renderer_update_buffer(renderer_buffer buffer, renderer_buffer_type type,
                            int size, const void *data)
{
   glBindBuffer(gl_buffer_type[type], buffer.id_);
   glBufferSubData(gl_buffer_type[type], 0, size, data);
   glBindBuffer(gl_buffer_type[type], 0);

   renderer_opengl_error_check();
}

void renderer_add_attribute(renderer_vertex_format *format, int index, int size,
                            renderer_attribute_type type, bool normalized)
{
   HC_ASSERT(format->attribute_count_ < RENDERER_MAX_VERTEX_ATTRIBUTES);

   renderer_vertex_attribute *attribute = format->attributes_ + format->attribute_count_++;
   attribute->index_ = index;
   attribute->size_ = size;
   attribute->offset_ = format->stride_;
   attribute->type_ = type;
   attribute->normalized_ = normalized;

   format->stride_ += gl_attribute_size[type] * size;
}

void renderer_add_texture_binding(renderer_resource_bindings *bindings, renderer_texture texture)
{
   HC_ASSERT(bindings->texture_count_ < RENDERER_MAX_TEXTURE_BINDINGS);

   bindings->textures_[bindings->texture_count_++] = texture;
}

void renderer_add_sampler_binding(renderer_resource_bindings *bindings, renderer_sampler sampler)
{
   HC_ASSERT(bindings->sampler_count_ < RENDERER_MAX_TEXTURE_BINDINGS);

   bindings->samplers_[bindings->sampler_count_++] = sampler;
}

void renderer_add_uniform_binding(renderer_resource_bindings *bindings, renderer_uniform_type type,
                                  int location, int size, const void *data)
{
   HC_ASSERT(bindings->uniform_count_ < RENDERER_MAX_UNIFORM_BINGINGS);

   renderer_uniform *uniform = bindings->uniforms_ + bindings->uniform_count_++;
   uniform->type_ = type;
   uniform->location_ = location;
   uniform->size_ = size;
   uniform->data_ = data;
}

void renderer_bind_pipeline(renderer_pipeline *pipeline, renderer_viewport *viewport)
{
   glClearColor(pipeline->clear_color_.r_,
                pipeline->clear_color_.g_,
                pipeline->clear_color_.b_, 
                pipeline->clear_color_.a_);
   glViewport(viewport->x_, viewport->y_, viewport->width_, viewport->height_);
   glUseProgram(pipeline->shader_program_.id_);

   renderer_blend_state *bs = &pipeline->blend_state_;
   if (bs->enabled_)
   {
      glEnable(GL_BLEND);
      glBlendFuncSeparate(gl_blend_ft[bs->src_rgb_],
                          gl_blend_ft[bs->dst_rgb_],
                          gl_blend_ft[bs->src_alpha_],
                          gl_blend_ft[bs->dst_alpha_]);
      glBlendEquationSeparate(gl_blend_eq[bs->eq_rgb_],
                              gl_blend_eq[bs->eq_alpha_]);
   }
   else
   {
      glDisable(GL_BLEND);
   }

   renderer_depth_stencil_state *dss = &pipeline->depth_stencil_state_;
   if (dss->depth_.write_enabled_)
      glDepthMask(GL_TRUE);
   else
      glDepthMask(GL_FALSE);

   if (dss->depth_.read_enabled_)
      glEnable(GL_DEPTH_TEST);
   else
      glDisable(GL_DEPTH_TEST);

   glDepthRange(dss->depth_.range_near_, dss->depth_.range_far_);
   glDepthFunc(gl_compare_func[dss->depth_.func_]);

   // todo: stencil stuff...
   // depth_stencil_state *dss = ...

   renderer_rasterizer_state *rs = &pipeline->rasterizer_state_;
   if (rs->cull_mode_ != RENDERER_CULL_MODE_NONE)
   {
      glEnable(GL_CULL_FACE);
      glCullFace(gl_cull_mode[rs->cull_mode_]);
   }
   else
   {
      glDisable(GL_CULL_FACE);
   }

   glFrontFace(gl_front_face[rs->front_face_]);

   renderer_opengl_error_check();
}

void renderer_bind_input_assembly(renderer_input_assembly *assembly)
{
   renderer_buffer *vertex_buffer = &assembly->vertex_buffer_;
   glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer->id_);

   renderer_buffer *index_buffer = &assembly->index_buffer_;
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer->id_);

   renderer_vertex_format *vf = &assembly->vertex_format_;
   int stride = vf->stride_;
   int count = vf->attribute_count_;
   for (int i = 0; i < count; i++)
   {
      renderer_vertex_attribute *va = vf->attributes_ + i;

      glEnableVertexAttribArray(va->index_);
      glVertexAttribPointer(va->index_,
                            va->size_,
                            gl_attribute_type[va->type_],
                            va->normalized_ ? GL_TRUE : GL_FALSE,
                            stride,
                            (const GLvoid *)(uintptr_t)va->offset_);
   }

   renderer_opengl_error_check();
}

void renderer_bind_resource_bindings(renderer_resource_bindings *bindings)
{
   renderer_texture *textures = bindings->textures_;
   renderer_sampler *samplers = bindings->samplers_;
   for (int i = 0; i < RENDERER_MAX_TEXTURE_BINDINGS; i++)
   {
      renderer_texture *texture = textures + i;
      renderer_sampler *sampler = samplers + i;

      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, texture->id_);
      glBindSampler(i, sampler->id_);

      renderer_opengl_error_check();
   }

   int uniform_count = bindings->uniform_count_;
   renderer_uniform *uniforms = bindings->uniforms_;
   for (int i = 0; i < uniform_count; i++)
   {
      renderer_uniform *uniform = uniforms + i;

      switch (uniform->type_)
      {
         case RENDERER_UNIFORM_TYPE_FLOAT:
         {
            glUniform1fv(uniform->location_, 
                         uniform->size_, 
                         (const GLfloat *)uniform->data_);
            renderer_opengl_error_check();
         } break;
         case RENDERER_UNIFORM_TYPE_INT:
         case RENDERER_UNIFORM_TYPE_BOOL:
         case RENDERER_UNIFORM_TYPE_SAMPLER:
         {
            glUniform1iv(uniform->location_, 
                         uniform->size_, 
                         (const GLint *)uniform->data_);
            renderer_opengl_error_check();
         } break;
         case RENDERER_UNIFORM_TYPE_MATRIX:
         {
            glUniformMatrix4fv(uniform->location_,
                               uniform->size_,
                               GL_FALSE,
                               (const GLfloat *)uniform->data_);
            renderer_opengl_error_check();
         } break;
      }
   }

   renderer_opengl_error_check();
}

void renderer_submit_drawcall(renderer_drawcall *drawcall)
{
   renderer_primitive_topology primitive_topology = drawcall->primitive_topology_;

   int vertex_count = drawcall->vertex_count_;
   int instance_count = drawcall->instance_count_; // note: always 1 for now
   int index_buffer_offset = drawcall->index_buffer_offset_;
   int vertex_buffer_offset = drawcall->vertex_buffer_offset_;

   (void)instance_count;

   if (index_buffer_offset >= 0)
   {
      glDrawElements(gl_primitive_topology[primitive_topology],
                     vertex_count,
                     GL_UNSIGNED_SHORT,
                     (const GLvoid *)(uintptr_t)index_buffer_offset);
   }
   else
   {
      glDrawArrays(gl_primitive_topology[primitive_topology],
                   vertex_buffer_offset,
                   vertex_count);
   }

   renderer_opengl_error_check();
}
