#define GLAD_C
#include "glad.h"
#include <stdlib.h>

static void* load(GLADloadproc loader, const char *name) {
    return loader(name);
}

int gladLoadGL(GLADloadproc loader) {
    if (!loader) return 0;

#define GLAD_LOAD_UNION(func, type, name) { \
    union { void *p; type f; } u; \
    u.p = load(loader, name); \
    func = u.f; \
}

    GLAD_LOAD_UNION(glClear, PFNGLCLEARPROC, "glClear");
    GLAD_LOAD_UNION(glClearColor, PFNGLCLEARCOLORPROC, "glClearColor");
    GLAD_LOAD_UNION(glEnable, PFNGLENABLEPROC, "glEnable");
    GLAD_LOAD_UNION(glDisable, PFNGLDISABLEPROC, "glDisable");
    GLAD_LOAD_UNION(glFrontFace, PFNGLFRONTFACEPROC, "glFrontFace");
    GLAD_LOAD_UNION(glCullFace, PFNGLCULLFACEPROC, "glCullFace");
    GLAD_LOAD_UNION(glDrawArrays, PFNGLDRAWARRAYSPROC, "glDrawArrays");
    GLAD_LOAD_UNION(glDrawElements, PFNGLDRAWELEMENTSPROC, "glDrawElements");
    GLAD_LOAD_UNION(glGenBuffers, PFNGLGENBUFFERSPROC, "glGenBuffers");
    GLAD_LOAD_UNION(glDeleteBuffers, PFNGLDELETEBUFFERSPROC, "glDeleteBuffers");
    GLAD_LOAD_UNION(glBindBuffer, PFNGLBINDBUFFERPROC, "glBindBuffer");
    GLAD_LOAD_UNION(glBufferData, PFNGLBUFFERDATAPROC, "glBufferData");
    GLAD_LOAD_UNION(glGenVertexArrays, PFNGLGENVERTEXARRAYSPROC, "glGenVertexArrays");
    GLAD_LOAD_UNION(glDeleteVertexArrays, PFNGLDELETEVERTEXARRAYSPROC, "glDeleteVertexArrays");
    GLAD_LOAD_UNION(glBindVertexArray, PFNGLBINDVERTEXARRAYPROC, "glBindVertexArray");
    GLAD_LOAD_UNION(glVertexAttribPointer, PFNGLVERTEXATTRIBPOINTERPROC, "glVertexAttribPointer");
    GLAD_LOAD_UNION(glEnableVertexAttribArray, PFNGLENABLEVERTEXATTRIBARRAYPROC, "glEnableVertexAttribArray");
    GLAD_LOAD_UNION(glDisableVertexAttribArray, PFNGLDISABLEVERTEXATTRIBARRAYPROC, "glDisableVertexAttribArray");
    GLAD_LOAD_UNION(glGenTextures, PFNGLGENTEXTURESPROC, "glGenTextures");
    GLAD_LOAD_UNION(glDeleteTextures, PFNGLDELETETEXTURESPROC, "glDeleteTextures");
    GLAD_LOAD_UNION(glBindTexture, PFNGLBINDTEXTUREPROC, "glBindTexture");
    GLAD_LOAD_UNION(glTexImage2D, PFNGLTEXIMAGE2DPROC, "glTexImage2D");
    GLAD_LOAD_UNION(glTexParameteri, PFNGLTEXPARAMETERIPROC, "glTexParameteri");
    GLAD_LOAD_UNION(glTexParameterf, PFNGLTEXPARAMETERFPROC, "glTexParameterf");
    GLAD_LOAD_UNION(glGenFramebuffers, PFNGLGENFRAMEBUFFERSPROC, "glGenFramebuffers");
    GLAD_LOAD_UNION(glDeleteFramebuffers, PFNGLDELETEFRAMEBUFFERSPROC, "glDeleteFramebuffers");
    GLAD_LOAD_UNION(glBindFramebuffer, PFNGLBINDBUFFERPROC, "glBindFramebuffer");
    GLAD_LOAD_UNION(glCheckFramebufferStatus, PFNGLCHECKFRAMEBUFFERSTATUSPROC, "glCheckFramebufferStatus");
    GLAD_LOAD_UNION(glFramebufferTexture2D, PFNGLFRAMEBUFFERTEXTURE2DPROC, "glFramebufferTexture2D");
    GLAD_LOAD_UNION(glBlendFunc, PFNGLBLENDFUNCPROC, "glBlendFunc");
    GLAD_LOAD_UNION(glViewport, PFNGLVIEWPORTPROC, "glViewport");
    GLAD_LOAD_UNION(glCreateShader, PFNGLCREATESHADERPROC, "glCreateShader");
    GLAD_LOAD_UNION(glDeleteShader, PFNGLDELETESHADERPROC, "glDeleteShader");
    GLAD_LOAD_UNION(glShaderSource, PFNGLSHADERSOURCEPROC, "glShaderSource");
    GLAD_LOAD_UNION(glCompileShader, PFNGLCOMPILESHADERPROC, "glCompileShader");
    GLAD_LOAD_UNION(glGetShaderiv, PFNGLGETSHADERIVPROC, "glGetShaderiv");
    GLAD_LOAD_UNION(glGetShaderInfoLog, PFNGLGETSHADERINFOLOGPROC, "glGetShaderInfoLog");
    GLAD_LOAD_UNION(glCreateProgram, PFNGLCREATEPROGRAMPROC, "glCreateProgram");
    GLAD_LOAD_UNION(glDeleteProgram, PFNGLDELETEPROGRAMPROC, "glDeleteProgram");
    GLAD_LOAD_UNION(glAttachShader, PFNGLATTACHSHADERPROC, "glAttachShader");
    GLAD_LOAD_UNION(glLinkProgram, PFNGLLINKPROGRAMPROC, "glLinkProgram");
    GLAD_LOAD_UNION(glUseProgram, PFNGLUSEPROGRAMPROC, "glUseProgram");
    GLAD_LOAD_UNION(glGetProgramiv, PFNGLGETPROGRAMIVPROC, "glGetProgramiv");
    GLAD_LOAD_UNION(glGetProgramInfoLog, PFNGLGETPROGRAMINFOLOGPROC, "glGetProgramInfoLog");
    GLAD_LOAD_UNION(glDetachShader, PFNGLDETACHSHADERPROC, "glDetachShader");
    GLAD_LOAD_UNION(glGetUniformLocation, PFNGLGETUNIFORMLOCATIONPROC, "glGetUniformLocation");
    GLAD_LOAD_UNION(glUniform1i, PFNGLUNIFORM1IPROC, "glUniform1i");
    GLAD_LOAD_UNION(glUniform1f, PFNGLUNIFORM1FPROC, "glUniform1f");
    GLAD_LOAD_UNION(glUniform2f, PFNGLUNIFORM2FPROC, "glUniform2f");
    GLAD_LOAD_UNION(glUniform3f, PFNGLUNIFORM3FPROC, "glUniform3f");
    GLAD_LOAD_UNION(glUniform4f, PFNGLUNIFORM4FPROC, "glUniform4f");
    GLAD_LOAD_UNION(glUniformMatrix4fv, PFNGLUNIFORMMATRIX4FVPROC, "glUniformMatrix4fv");
    GLAD_LOAD_UNION(glVertexAttribDivisor, PFNGLVERTEXATTRIBDIVISORPROC, "glVertexAttribDivisor");
    GLAD_LOAD_UNION(glGenRenderbuffers, PFNGLGENRENDERBUFFERSPROC, "glGenRenderbuffers");
    GLAD_LOAD_UNION(glDeleteRenderbuffers, PFNGLDELETERENDERBUFFERSPROC, "glDeleteRenderbuffers");
    GLAD_LOAD_UNION(glBindRenderbuffer, PFNGLBINDRENDERBUFFERPROC, "glBindRenderbuffer");
    GLAD_LOAD_UNION(glRenderbufferStorage, PFNGLRENDERBUFFERSTORAGEPROC, "glRenderbufferStorage");
    GLAD_LOAD_UNION(glGenerateMipmap, PFNGLGENERATEMIPMAPPROC, "glGenerateMipmap");
    GLAD_LOAD_UNION(glActiveTexture, PFNGLACTIVETEXTUREPROC, "glActiveTexture");
    GLAD_LOAD_UNION(glBlendEquation, PFNGLBLENDEQUATIONPROC, "glBlendEquation");
    GLAD_LOAD_UNION(glMapBuffer, PFNGLMAPBUFFERPROC, "glMapBuffer");
    GLAD_LOAD_UNION(glUnmapBuffer, PFNGLUNMAPBUFFERPROC, "glUnmapBuffer");
    GLAD_LOAD_UNION(glGetAttribLocation, PFNGLGETATTRIBLOCATIONPROC, "glGetAttribLocation");
    GLAD_LOAD_UNION(glScissor, PFNGLSCISSORPROC, "glScissor");



    if (!glClear || !glDrawArrays || !glCreateShader || !glCreateProgram) {
        return 0;
    }

    return 1;
}


