#ifndef GLAD_H
#define GLAD_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
#ifndef APIENTRY
#define APIENTRY __stdcall
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif
#else
#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif
#endif

#include <stddef.h>

/* Prevent including gl.h */
#ifndef __gl_h_
#define __gl_h_
#endif
#ifndef __GL_H__
#define __GL_H__
#endif
#ifndef __glext_h_
#define __glext_h_
#endif
#ifndef __GLEXT_H_
#define __GLEXT_H_
#endif
#ifndef __gl_glcorearb_h_
#define __gl_glcorearb_h_
#endif

typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long long GLuint64;
typedef long long GLint64;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
typedef char GLchar;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

#define GL_TRUE 1
#define GL_FALSE 0
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_TRIANGLES 0x0004
#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_INT 0x1403
#define GL_FLOAT 0x1406
#define GL_RENDERBUFFER 0x8D41
#define GL_DEPTH_COMPONENT 0x1902
#define GL_RGBA 0x1908
#define GL_RGB 0x1907
#define GL_RG 0x8227
#define GL_RED 0x1903
#define GL_TEXTURE_2D 0x0DE1
#define GL_FRAMEBUFFER 0x8D40
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS 0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED 0x8CDD
#define GL_BLEND 0x0BE2
#define GL_CULL_FACE 0x0B45
#define GL_DEPTH_TEST 0x0B71
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_CCW 0x0901
#define GL_CW 0x0900
#define GL_LINEAR 0x2601
#define GL_NEAREST 0x2600
#define GL_REPEAT 0x2901
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_RGBA8 0x8058
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE0 0x84C0
#define GL_ACTIVE_TEXTURE 0x84E1
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_DEPTH_ATTACHMENT 0x8D00

/* Function pointer types */
typedef void (APIENTRYP PFNGLCLEARPROC)(GLbitfield mask);
typedef void (APIENTRYP PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (APIENTRYP PFNGLENABLEPROC)(GLenum cap);
typedef void (APIENTRYP PFNGLDISABLEPROC)(GLenum cap);
typedef void (APIENTRYP PFNGLFRONTFACEPROC)(GLenum mode);
typedef void (APIENTRYP PFNGLCULLFACEPROC)(GLenum mode);
typedef void (APIENTRYP PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRYP PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices);
typedef void (APIENTRYP PFNGLGENBUFFERSPROC)(GLsizei n, GLuint *buffers);
typedef void (APIENTRYP PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint *buffers);
typedef void (APIENTRYP PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (APIENTRYP PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void (APIENTRYP PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint *arrays);
typedef void (APIENTRYP PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint *arrays);
typedef void (APIENTRYP PFNGLBINDVERTEXARRAYPROC)(GLuint array);
typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (APIENTRYP PFNGLGENTEXTURESPROC)(GLsizei n, GLuint *textures);
typedef void (APIENTRYP PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint *textures);
typedef void (APIENTRYP PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
typedef void (APIENTRYP PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRYP PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
typedef void (APIENTRYP PFNGLTEXPARAMETERFPROC)(GLenum target, GLenum pname, GLfloat param);
typedef void (APIENTRYP PFNGLGENFRAMEBUFFERSPROC)(GLsizei n, GLuint *framebuffers);
typedef void (APIENTRYP PFNGLDELETEFRAMEBUFFERSPROC)(GLsizei n, const GLuint *framebuffers);
typedef void (APIENTRYP PFNGLBINDFRAMEBUFFERPROC)(GLenum target, GLuint framebuffer);
typedef GLenum (APIENTRYP PFNGLCHECKFRAMEBUFFERSTATUSPROC)(GLenum target);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (APIENTRYP PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
typedef void (APIENTRYP PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef GLuint (APIENTRYP PFNGLCREATESHADERPROC)(GLenum type);
typedef void (APIENTRYP PFNGLDELETESHADERPROC)(GLuint shader);
typedef void (APIENTRYP PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (APIENTRYP PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void (APIENTRYP PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef GLuint (APIENTRYP PFNGLCREATEPROGRAMPROC)(void);
typedef void (APIENTRYP PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef void (APIENTRYP PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (APIENTRYP PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void (APIENTRYP PFNGLUSEPROGRAMPROC)(GLuint program);
typedef void (APIENTRYP PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRYP PFNGLDETACHSHADERPROC)(GLuint program, GLuint shader);
typedef GLint (APIENTRYP PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar *name);
typedef void (APIENTRYP PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void (APIENTRYP PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void (APIENTRYP PFNGLUNIFORM2FPROC)(GLint location, GLfloat v0, GLfloat v1);
typedef void (APIENTRYP PFNGLUNIFORM3FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRYP PFNGLUNIFORM4FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRYP PFNGLVERTEXATTRIBDIVISORPROC)(GLuint index, GLuint divisor);
typedef void (APIENTRYP PFNGLGENRENDERBUFFERSPROC)(GLsizei n, GLuint *renderbuffers);
typedef void (APIENTRYP PFNGLDELETERENDERBUFFERSPROC)(GLsizei n, const GLuint *renderbuffers);
typedef void (APIENTRYP PFNGLBINDRENDERBUFFERPROC)(GLenum target, GLuint renderbuffer);
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEPROC)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLGENERATEMIPMAPPROC)(GLenum target);
typedef void (APIENTRYP PFNGLBLENDEQUATIONPROC)(GLenum mode);
typedef void* (APIENTRYP PFNGLMAPBUFFERPROC)(GLenum target, GLenum access);
typedef GLboolean (APIENTRYP PFNGLUNMAPBUFFERPROC)(GLenum target);
typedef void (APIENTRYP PFNGLACTIVETEXTUREPROC)(GLenum texture);

/* Function declarations */
#ifdef GLAD_C
#define GLAD_EXT
#else
#define NK_GLFW_TEXT_MAX 256
#define GLAD_EXT extern
#endif

typedef GLint (APIENTRYP PFNGLGETATTRIBLOCATIONPROC)(GLuint program, const GLchar *name);
typedef void (APIENTRYP PFNGLSCISSORPROC)(GLint x, GLint y, GLsizei width, GLsizei height);

GLAD_EXT PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
GLAD_EXT PFNGLSCISSORPROC glScissor;
GLAD_EXT PFNGLBLENDEQUATIONPROC glBlendEquation;

GLAD_EXT PFNGLMAPBUFFERPROC glMapBuffer;
GLAD_EXT PFNGLUNMAPBUFFERPROC glUnmapBuffer;
GLAD_EXT PFNGLCLEARPROC glClear;

GLAD_EXT PFNGLCLEARCOLORPROC glClearColor;
GLAD_EXT PFNGLENABLEPROC glEnable;
GLAD_EXT PFNGLDISABLEPROC glDisable;
GLAD_EXT PFNGLFRONTFACEPROC glFrontFace;
GLAD_EXT PFNGLCULLFACEPROC glCullFace;
GLAD_EXT PFNGLDRAWARRAYSPROC glDrawArrays;
GLAD_EXT PFNGLDRAWELEMENTSPROC glDrawElements;
GLAD_EXT PFNGLGENBUFFERSPROC glGenBuffers;
GLAD_EXT PFNGLDELETEBUFFERSPROC glDeleteBuffers;
GLAD_EXT PFNGLBINDBUFFERPROC glBindBuffer;
GLAD_EXT PFNGLBUFFERDATAPROC glBufferData;
GLAD_EXT PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
GLAD_EXT PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
GLAD_EXT PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
GLAD_EXT PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
GLAD_EXT PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
GLAD_EXT PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
GLAD_EXT PFNGLGENTEXTURESPROC glGenTextures;
GLAD_EXT PFNGLDELETETEXTURESPROC glDeleteTextures;
GLAD_EXT PFNGLBINDTEXTUREPROC glBindTexture;
GLAD_EXT PFNGLTEXIMAGE2DPROC glTexImage2D;
GLAD_EXT PFNGLTEXPARAMETERIPROC glTexParameteri;
GLAD_EXT PFNGLTEXPARAMETERFPROC glTexParameterf;
GLAD_EXT PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
GLAD_EXT PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
GLAD_EXT PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
GLAD_EXT PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
GLAD_EXT PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
GLAD_EXT PFNGLBLENDFUNCPROC glBlendFunc;
GLAD_EXT PFNGLVIEWPORTPROC glViewport;
GLAD_EXT PFNGLCREATESHADERPROC glCreateShader;
GLAD_EXT PFNGLDELETESHADERPROC glDeleteShader;
GLAD_EXT PFNGLSHADERSOURCEPROC glShaderSource;
GLAD_EXT PFNGLCOMPILESHADERPROC glCompileShader;
GLAD_EXT PFNGLGETSHADERIVPROC glGetShaderiv;
GLAD_EXT PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
GLAD_EXT PFNGLCREATEPROGRAMPROC glCreateProgram;
GLAD_EXT PFNGLDELETEPROGRAMPROC glDeleteProgram;
GLAD_EXT PFNGLATTACHSHADERPROC glAttachShader;
GLAD_EXT PFNGLLINKPROGRAMPROC glLinkProgram;
GLAD_EXT PFNGLUSEPROGRAMPROC glUseProgram;
GLAD_EXT PFNGLGETPROGRAMIVPROC glGetProgramiv;
GLAD_EXT PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
GLAD_EXT PFNGLDETACHSHADERPROC glDetachShader;
GLAD_EXT PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
GLAD_EXT PFNGLUNIFORM1IPROC glUniform1i;
GLAD_EXT PFNGLUNIFORM1FPROC glUniform1f;
GLAD_EXT PFNGLUNIFORM2FPROC glUniform2f;
GLAD_EXT PFNGLUNIFORM3FPROC glUniform3f;
GLAD_EXT PFNGLUNIFORM4FPROC glUniform4f;
GLAD_EXT PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
GLAD_EXT PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;
GLAD_EXT PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
GLAD_EXT PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
GLAD_EXT PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
GLAD_EXT PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
GLAD_EXT PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
GLAD_EXT PFNGLACTIVETEXTUREPROC glActiveTexture;

typedef void* (*GLADloadproc)(const char *name);
int gladLoadGL(GLADloadproc loader);

#ifdef __cplusplus
}
#endif

#endif
