// halcyon_opengl.h

#ifndef HALCYON_OPENGL_H_INCLUDED
#define HALCYON_OPENGL_H_INCLUDED

//
// source: 
//   - https://www.khronos.org/registry/OpenGL/api/GL/glcorearb.h
// notes:
//   - target context 3.3 (directx 10-ish)
//   - add more as we need them
//

#if HALCYON_PLATFORM_WINDOWS
#endif // HALCYON_PLATFORM_WINDOWS

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define GLDECL 
#define GLF(ret, name, ...) \
   typedef ret GLDECL type_##name (__VA_ARGS__); \
   extern type_##name *name;

	// GL_VERSION_1_0
	typedef void GLvoid;
	typedef unsigned int GLenum;
	typedef float GLfloat;
	typedef int GLint;
	typedef int GLsizei;
	typedef unsigned int GLbitfield;
	typedef double GLdouble;
	typedef unsigned int GLuint;
	typedef unsigned char GLboolean;
	typedef unsigned char GLubyte;
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_FALSE                          0
#define GL_TRUE                           1
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006
#define GL_NEVER                          0x0200
#define GL_LESS                           0x0201
#define GL_EQUAL                          0x0202
#define GL_LEQUAL                         0x0203
#define GL_GREATER                        0x0204
#define GL_NOTEQUAL                       0x0205
#define GL_GEQUAL                         0x0206
#define GL_ALWAYS                         0x0207
#define GL_ZERO                           0
#define GL_ONE                            1
#define GL_SRC_COLOR                      0x0300
#define GL_ONE_MINUS_SRC_COLOR            0x0301
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_DST_ALPHA                      0x0304
#define GL_ONE_MINUS_DST_ALPHA            0x0305
#define GL_DST_COLOR                      0x0306
#define GL_ONE_MINUS_DST_COLOR            0x0307
#define GL_SRC_ALPHA_SATURATE             0x0308
#define GL_NONE                           0
#define GL_FRONT                          0x0404
#define GL_BACK                           0x0405
#define GL_FRONT_AND_BACK                 0x0408
#define GL_NO_ERROR                       0
#define GL_INVALID_ENUM                   0x0500
#define GL_INVALID_VALUE                  0x0501
#define GL_INVALID_OPERATION              0x0502
#define GL_OUT_OF_MEMORY                  0x0505
#define GL_CW                             0x0900
#define GL_CCW                            0x0901
#define GL_CULL_FACE                      0x0B44
#define GL_FRONT_FACE                     0x0B46
#define GL_DEPTH_TEST                     0x0B71
#define GL_DEPTH_FUNC                     0x0B74
#define GL_BLEND                          0x0BE2
#define GL_TEXTURE_2D                     0x0DE1
#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_INVERT                         0x150A
#define GL_DEPTH_COMPONENT                0x1902
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
#define GL_LINE                           0x1B01
#define GL_FILL                           0x1B02
#define GL_KEEP                           0x1E00
#define GL_REPLACE                        0x1E01
#define GL_INCR                           0x1E02
#define GL_DECR                           0x1E03
#define GL_VENDOR                         0x1F00
#define GL_RENDERER                       0x1F01
#define GL_VERSION                        0x1F02
#define GL_NEAREST                        0x2600
#define GL_LINEAR                         0x2601
#define GL_NEAREST_MIPMAP_NEAREST         0x2700
#define GL_LINEAR_MIPMAP_NEAREST          0x2701
#define GL_NEAREST_MIPMAP_LINEAR          0x2702
#define GL_LINEAR_MIPMAP_LINEAR           0x2703
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_REPEAT                         0x2901

#define GL_FUNCLIST_1_0 \
   GLF(void, glCullFace, GLenum mode) \
   GLF(void, glFrontFace, GLenum mode) \
   GLF(void, glPolygonMode, GLenum face, GLenum mode) \
   GLF(void, glScissor, GLint x, GLint y, GLsizei width, GLsizei height) \
   GLF(void, glTexParameteri, GLenum target, GLenum pname, GLint param) \
   GLF(void, glTexParameteriv, GLenum target, GLenum pname, const GLint *params) \
   GLF(void, glTexImage2D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) \
   GLF(void, glDrawBuffer, GLenum buf) \
   GLF(void, glClear, GLbitfield mask) \
   GLF(void, glClearColor, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) \
   GLF(void, glClearStencil, GLint s) \
   GLF(void, glClearDepth, GLdouble depth) \
   GLF(void, glDepthMask, GLboolean flag) \
   GLF(void, glDisable, GLenum cap) \
   GLF(void, glEnable, GLenum cap) \
   GLF(void, glBlendFunc, GLenum sfactor, GLenum dfactor) \
   GLF(void, glDepthFunc, GLenum func) \
   GLF(GLenum, glGetError, void) \
   GLF(void, glGetIntegerv, GLenum pname, GLint *data) \
   GLF(GLboolean, glIsEnabled, GLenum cap) \
   GLF(void, glDepthRange, GLdouble n, GLdouble f) \
   GLF(void, glViewport, GLint x, GLint y, GLsizei width, GLsizei height) \
   GLF(const GLubyte *, glGetString, GLenum name)
	GL_FUNCLIST_1_0;

	// GL_VERSION_1_1
	typedef float GLclampf;
	typedef double GLclampd;
#define GL_DOUBLE                         0x140A
#define GL_RGB8                           0x8051
#define GL_RGBA8                          0x8058
#define GL_VERTEX_ARRAY                   0x8074

#define GL_FUNCLIST_1_1 \
   GLF(void, glDrawArrays, GLenum mode, GLint first, GLsizei count) \
   GLF(void, glDrawElements, GLenum mode, GLsizei count, GLenum type, const void *indices) \
   GLF(void, glBindTexture, GLenum target, GLuint texture) \
   GLF(void, glDeleteTextures, GLsizei n, const GLuint *textures) \
   GLF(void, glGenTextures, GLsizei n, GLuint *textures) \
   GLF(GLboolean, glIsTexture, GLuint texture) 
	GL_FUNCLIST_1_1;

	// GL_VERSION_1_2
#define GL_TEXTURE_WRAP_R                 0x8072
#define GL_BGR                            0x80E0
#define GL_BGRA                           0x80E1
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_TEXTURE_MIN_LOD                0x813A
#define GL_TEXTURE_MAX_LOD                0x813B
#define GL_TEXTURE_BASE_LEVEL             0x813C
#define GL_TEXTURE_MAX_LEVEL              0x813D

#define GL_FUNCLIST_1_2 \
   GLF(void, glDrawRangeElements, GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices)
	GL_FUNCLIST_1_2;

	// GL_VERSION_1_3
#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE_CUBE_MAP               0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP       0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A
#define GL_COMPRESSED_RGB                 0x84ED
#define GL_COMPRESSED_RGBA                0x84EE
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE  0x86A0
#define GL_TEXTURE_COMPRESSED             0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS     0x86A3

#define GL_FUNCLIST_1_3 \
   GLF(void, glActiveTexture, GLenum texture) \
   GLF(void, glCompressedTexImage2D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data) \
   GLF(void, glGetCompressedTexImage, GLenum target, GLint level, void *img) 
	GL_FUNCLIST_1_3;

	// GL_VERSION_1_4
#define GL_BLEND_DST_RGB                  0x80C8
#define GL_BLEND_SRC_RGB                  0x80C9
#define GL_BLEND_DST_ALPHA                0x80CA
#define GL_BLEND_SRC_ALPHA                0x80CB
#define GL_POINT_FADE_THRESHOLD_SIZE      0x8128
#define GL_DEPTH_COMPONENT16              0x81A5
#define GL_DEPTH_COMPONENT24              0x81A6
#define GL_DEPTH_COMPONENT32              0x81A7
#define GL_MIRRORED_REPEAT                0x8370
#define GL_MAX_TEXTURE_LOD_BIAS           0x84FD
#define GL_TEXTURE_LOD_BIAS               0x8501
#define GL_INCR_WRAP                      0x8507
#define GL_DECR_WRAP                      0x8508
#define GL_TEXTURE_DEPTH_SIZE             0x884A
#define GL_TEXTURE_COMPARE_MODE           0x884C
#define GL_TEXTURE_COMPARE_FUNC           0x884D
#define GL_BLEND_COLOR                    0x8005
#define GL_BLEND_EQUATION                 0x8009
#define GL_CONSTANT_COLOR                 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR       0x8002
#define GL_CONSTANT_ALPHA                 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA       0x8004
#define GL_FUNC_ADD                       0x8006
#define GL_FUNC_REVERSE_SUBTRACT          0x800B
#define GL_FUNC_SUBTRACT                  0x800A
#define GL_MIN                            0x8007
#define GL_MAX                            0x8008

#define GL_FUNCLIST_1_4 \
   GLF(void, glBlendFuncSeparate, GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha) \
   GLF(void, glMultiDrawArrays, GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount) \
   GLF(void, glMultiDrawElements, GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount) \
   GLF(void, glBlendEquation, GLenum mode) 
	GL_FUNCLIST_1_4;

	// GL_VERSION_1_5
#include <stddef.h>
	typedef ptrdiff_t GLsizeiptr;
	typedef ptrdiff_t GLintptr;
#define GL_BUFFER_SIZE                    0x8764
#define GL_BUFFER_USAGE                   0x8765
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_STREAM_DRAW                    0x88E0
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8

#define GL_FUNCLIST_1_5 \
   GLF(void, glBindBuffer, GLenum target, GLuint buffer) \
   GLF(void, glDeleteBuffers, GLsizei n, const GLuint *buffers) \
   GLF(void, glGenBuffers, GLsizei n, GLuint *buffers) \
   GLF(GLboolean, glIsBuffer, GLuint buffer) \
   GLF(void, glBufferData, GLenum target, GLsizeiptr size, const void *data, GLenum usage) \
   GLF(void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const void *data) \
   GLF(void*, glMapBuffer, GLenum target, GLenum access) \
   GLF(GLboolean, glUnmapBuffer, GLenum target) 
	GL_FUNCLIST_1_5;

	// GL_VERSION_2_0
	typedef char GLchar;
	typedef short GLshort;
	typedef signed char GLbyte;
	typedef unsigned short GLushort;
#define GL_MAX_DRAW_BUFFERS               0x8824
#define GL_DRAW_BUFFER0                   0x8825
#define GL_MAX_TEXTURE_IMAGE_UNITS        0x8872
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_FLOAT_VEC2                     0x8B50
#define GL_FLOAT_VEC3                     0x8B51
#define GL_FLOAT_VEC4                     0x8B52
#define GL_INT_VEC2                       0x8B53
#define GL_INT_VEC3                       0x8B54
#define GL_INT_VEC4                       0x8B55
#define GL_BOOL                           0x8B56
#define GL_BOOL_VEC2                      0x8B57
#define GL_BOOL_VEC3                      0x8B58
#define GL_BOOL_VEC4                      0x8B59
#define GL_FLOAT_MAT2                     0x8B5A
#define GL_FLOAT_MAT3                     0x8B5B
#define GL_FLOAT_MAT4                     0x8B5C
#define GL_SAMPLER_1D                     0x8B5D
#define GL_SAMPLER_2D                     0x8B5E
#define GL_SAMPLER_3D                     0x8B5F
#define GL_SAMPLER_CUBE                   0x8B60
#define GL_SAMPLER_1D_SHADOW              0x8B61
#define GL_SAMPLER_2D_SHADOW              0x8B62
#define GL_DELETE_STATUS                  0x8B80
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_ACTIVE_UNIFORMS                0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH      0x8B87
#define GL_ACTIVE_ATTRIBUTES              0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH    0x8B8A
#define GL_CURRENT_PROGRAM                0x8B8D

#define GL_FUNCLIST_2_0 \
   GLF(void, glBlendEquationSeparate, GLenum modeRGB, GLenum modeAlpha) \
   GLF(void, glDrawBuffers, GLsizei n, const GLenum *bufs) \
   GLF(void, glAttachShader, GLuint program, GLuint shader) \
   GLF(void, glBindAttribLocation, GLuint program, GLuint index, const GLchar *name) \
   GLF(void, glCompileShader, GLuint shader) \
   GLF(GLuint, glCreateProgram, void) \
   GLF(GLuint, glCreateShader, GLenum type) \
   GLF(void, glDeleteProgram, GLuint program) \
   GLF(void, glDeleteShader, GLuint shader) \
   GLF(void, glDetachShader, GLuint program, GLuint shader) \
   GLF(void, glDisableVertexAttribArray, GLuint index) \
   GLF(void, glEnableVertexAttribArray, GLuint index) \
   GLF(GLint, glGetAttribLocation, GLuint program, const GLchar *name) \
   GLF(void, glGetProgramiv, GLuint program, GLenum pname, GLint *params) \
   GLF(void, glGetProgramInfoLog, GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) \
   GLF(void, glGetShaderiv, GLuint shader, GLenum pname, GLint *params) \
   GLF(void, glGetShaderInfoLog, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) \
   GLF(GLint, glGetUniformLocation, GLuint program, const GLchar *name) \
   GLF(GLboolean, glIsProgram, GLuint program) \
   GLF(GLboolean, glIsShader, GLuint shader) \
   GLF(void, glLinkProgram, GLuint program) \
   GLF(void, glShaderSource, GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length) \
   GLF(void, glUseProgram, GLuint program) \
   GLF(void, glUniform1i, GLint location, const GLint value) \
   GLF(void, glUniform1fv, GLint location, GLsizei count, const GLfloat *value) \
   GLF(void, glUniform2fv, GLint location, GLsizei count, const GLfloat *value) \
   GLF(void, glUniform3fv, GLint location, GLsizei count, const GLfloat *value) \
   GLF(void, glUniform4fv, GLint location, GLsizei count, const GLfloat *value) \
   GLF(void, glUniform1iv, GLint location, GLsizei count, const GLint *value) \
   GLF(void, glUniform2iv, GLint location, GLsizei count, const GLint *value) \
   GLF(void, glUniform3iv, GLint location, GLsizei count, const GLint *value) \
   GLF(void, glUniform4iv, GLint location, GLsizei count, const GLint *value) \
   GLF(void, glUniformMatrix2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
   GLF(void, glUniformMatrix3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
   GLF(void, glUniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
   GLF(void, glValidateProgram, GLuint program) \
   GLF(void, glVertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) 
	GL_FUNCLIST_2_0;

	// GL_VERSION_2_1
#define GL_SRGB                           0x8C40
#define GL_SRGB8                          0x8C41
#define GL_SRGB_ALPHA                     0x8C42
#define GL_SRGB8_ALPHA8                   0x8C43
#define GL_COMPRESSED_SRGB                0x8C48
#define GL_COMPRESSED_SRGB_ALPHA          0x8C49

#define GL_FUNCLIST_2_1
	GL_FUNCLIST_2_1;

	// GL_VERSION_3_0
	typedef unsigned short GLhalf;
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C
#define GL_NUM_EXTENSIONS                 0x821D
#define GL_CONTEXT_FLAGS                  0x821E
#define GL_TEXTURE_2D_ARRAY               0x8C1A
#define GL_SAMPLER_2D_ARRAY               0x8DC1
#define GL_SAMPLER_2D_ARRAY_SHADOW        0x8DC4
#define GL_SAMPLER_CUBE_SHADOW            0x8DC5
#define GL_INT_SAMPLER_2D                 0x8DCA
#define GL_UNSIGNED_INT_SAMPLER_2D        0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_CUBE      0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY  0x8DD7
#define GL_INVALID_FRAMEBUFFER_OPERATION  0x0506
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_DEPTH_STENCIL                  0x84F9
#define GL_UNSIGNED_INT_24_8              0x84FA
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED        0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS          0x8CDF
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41
#define GL_FRAMEBUFFER_SRGB               0x8DB9

#define GL_FUNCLIST_3_0 \
   GLF(const GLubyte *, glGetStringi, GLenum name, GLuint index) \
   GLF(void, glBindBufferRange, GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) \
   GLF(void, glBindBufferBase, GLenum target, GLuint index, GLuint buffer) \
   GLF(void, glUniform1uiv, GLint location, GLsizei count, const GLuint *value) \
   GLF(void, glUniform2uiv, GLint location, GLsizei count, const GLuint *value) \
   GLF(void, glUniform3uiv, GLint location, GLsizei count, const GLuint *value) \
   GLF(void, glUniform4uiv, GLint location, GLsizei count, const GLuint *value) \
   GLF(void, glClearBufferfi, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil) \
   GLF(GLboolean, glIsRenderbuffer, GLuint renderbuffer) \
   GLF(void, glBindRenderbuffer, GLenum target, GLuint renderbuffer) \
   GLF(void, glDeleteRenderbuffers, GLsizei n, const GLuint *renderbuffers) \
   GLF(void, glGenRenderbuffers, GLsizei n, GLuint *renderbuffers) \
   GLF(void, glRenderbufferStorage, GLenum target, GLenum internalformat, GLsizei width, GLsizei height) \
   GLF(GLboolean, glIsFramebuffer, GLuint framebuffer) \
   GLF(void, glBindFramebuffer, GLenum target, GLuint framebuffer) \
   GLF(void, glDeleteFramebuffers, GLsizei n, const GLuint *framebuffers) \
   GLF(void, glGenFramebuffers, GLsizei n, GLuint *framebuffers) \
   GLF(GLenum, glCheckFramebufferStatus, GLenum target) \
   GLF(void, glFramebufferTexture2D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) \
   GLF(void, glFramebufferRenderbuffer, GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) \
   GLF(void, glBlitFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) \
   GLF(void, glRenderbufferStorageMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) \
   GLF(void, glBindVertexArray, GLuint array) \
   GLF(void, glDeleteVertexArrays, GLsizei n, const GLuint *arrays) \
   GLF(void, glGenVertexArrays, GLsizei n, GLuint *arrays) \
   GLF(GLboolean, glIsVertexArray, GLuint array) 
	GL_FUNCLIST_3_0;

	// GL_VERSION_3_1
#define GL_UNIFORM_BUFFER                 0x8A11

#define GL_FUNCLIST_3_1 \
   GLF(void, glDrawArraysInstanced, GLenum mode, GLint first, GLsizei count, GLsizei instancecount) \
   GLF(void, glDrawElementsInstanced, GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount) \
   GLF(void, glTexBuffer, GLenum target, GLenum internalformat, GLuint buffer) \
   GLF(void, glUniformBlockBinding, GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding) 
	GL_FUNCLIST_3_1;

	// GL_VERSION_3_2
#define GL_CONTEXT_CORE_PROFILE_BIT       0x00000001

#define GL_FUNCLIST_3_2 \
   GLF(void, glDrawElementsBaseVertex, GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex) \
   GLF(void, glDrawRangeElementsBaseVertex, GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex) \
   GLF(void, glDrawElementsInstancedBaseVertex, GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex) \
   GLF(void, glMultiDrawElementsBaseVertex, GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount, const GLint *basevertex) \
   GLF(void, glFramebufferTexture, GLenum target, GLenum attachment, GLuint texture, GLint level) \
   GLF(void, glTexImage2DMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) 
	GL_FUNCLIST_3_2;

	// GL_VERSION_3_3
#define GL_FUNCLIST_3_3 \
   GLF(void, glGenSamplers, GLsizei count, GLuint *samplers) \
   GLF(void, glDeleteSamplers, GLsizei count, const GLuint *samplers) \
   GLF(GLboolean, glIsSampler, GLuint sampler) \
   GLF(void, glBindSampler, GLuint unit, GLuint sampler) \
   GLF(void, glSamplerParameteri, GLuint sampler, GLenum pname, GLint param) \
   GLF(void, glVertexAttribDivisor, GLuint index, GLuint divisor) 
	GL_FUNCLIST_3_3;

#undef GLF

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HALCYON_OPENGL_H_INCLUDED
