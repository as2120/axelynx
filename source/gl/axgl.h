#pragma once

//#include <windows.h>
#ifdef WIN32
#include <windows.h>
#include <gl/GL.h>
#include "wglext.h"
#endif

#ifdef UNIX
#include "gl.h"
#include <X11/X.h>
#include "glx.h"
#include "glxext.h"
#endif

#ifdef GLES2
#include "gl2.h"
#include "gl2ext.h"
#include "gl2platform.h"
#else
	#undef GL_GLEXT_PROTOTYPES
	#include "glext.h"
#endif

#include <axelynx/Logger.h>

extern GLenum g_OpenGLError;

inline std::wstring ErrorDescription(GLenum code)
{
	switch(code)
	{
		case(GL_INVALID_ENUM):
		return L"invalid enum";
		case(GL_INVALID_VALUE):
		return L"invalid value";
		case(GL_INVALID_OPERATION):
		return L"invalid operation";
		case(GL_STACK_OVERFLOW):
		return L"stack overflow";
		case(GL_STACK_UNDERFLOW):
		return L"stack underflow";
		case(GL_OUT_OF_MEMORY):
		return L"out of memory";
		case(GL_INVALID_FRAMEBUFFER_OPERATION):
		return L"invalid framebuffer operation";
	default:
		return L"unknow error";
	};
}

#ifdef _DEBUG
#define OPENGL_CHECK_FOR_ERRORS() \
	if ((g_OpenGLError = glGetError()) != GL_NO_ERROR) \
	LOG_SOURCE_ERROR((L"OpenGL error 0x%X\n desc:"+ErrorDescription(g_OpenGLError)).c_str(), (unsigned)g_OpenGLError);
#else
#define OPENGL_CHECK_FOR_ERRORS()
#endif

void AxelynxBlendMode();
// расширения OpenGL
// Texture
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLTEXIMAGE2DMULTISAMPLECOVERAGENVPROC glTexImage2DMultisampleCoverageNV;
extern PFNGLTEXIMAGE2DMULTISAMPLEPROC	glTexImage2DMultisample;
extern PFNGLTEXIMAGE3DPROC		glTexImage3D;
extern PFNGLTEXSUBIMAGE3DPROC	glTexSubImage3D;
extern PFNGLTEXBUFFERPROC	glTexBuffer;
extern PFNGLGETCOMPRESSEDTEXIMAGEPROC	glGetCompressedTexImage;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
extern PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D;
extern PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D;
// VAO
extern PFNGLGENVERTEXARRAYSPROC    glGenVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC    glBindVertexArray;
// VBO
extern PFNGLGENBUFFERSPROC    glGenBuffers;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLBINDBUFFERPROC    glBindBuffer;
extern PFNGLBUFFERDATAPROC    glBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLMAPBUFFERPROC     glMapBuffer;
extern PFNGLUNMAPBUFFERPROC   glUnmapBuffer;
// Shaders
extern PFNGLCREATEPROGRAMPROC     glCreateProgram;
extern PFNGLDELETEPROGRAMPROC     glDeleteProgram;
extern PFNGLLINKPROGRAMPROC       glLinkProgram;
extern PFNGLVALIDATEPROGRAMPROC   glValidateProgram;
extern PFNGLUSEPROGRAMPROC        glUseProgram;
extern PFNGLGETPROGRAMIVPROC      glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLCREATESHADERPROC      glCreateShader;
extern PFNGLDELETESHADERPROC      glDeleteShader;
extern PFNGLSHADERSOURCEPROC      glShaderSource;
extern PFNGLCOMPILESHADERPROC     glCompileShader;
extern PFNGLATTACHSHADERPROC      glAttachShader;
extern PFNGLDETACHSHADERPROC      glDetachShader;
extern PFNGLGETSHADERIVPROC       glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC  glGetShaderInfoLog;
extern PFNGLGETPROGRAMBINARYPROC glGetProgramBinary;
extern PFNGLPROGRAMBINARYPROC	 glProgramBinary;
extern PFNGLPROGRAMPARAMETERIPROC glProgramParameteri;
// Attributes
extern PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation;
extern PFNGLBINDATTRIBLOCATIONPROC        glBindAttribLocation;
extern PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLVERTEXATTRIBDIVISORPROC      glVertexAttribDivisor;
// Uniforms
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORMMATRIX3FVPROC   glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC   glUniformMatrix4fv;
extern PFNGLUNIFORM1IPROC          glUniform1i;
extern PFNGLUNIFORM1FVPROC         glUniform1fv;
extern PFNGLUNIFORM2FVPROC         glUniform2fv;
extern PFNGLUNIFORM3FVPROC         glUniform3fv;
extern PFNGLUNIFORM4FVPROC         glUniform4fv;
extern PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv;
extern PFNGLGETACTIVEUNIFORMSIVPROC       glGetActiveUniformsiv;
extern PFNGLGETACTIVEUNIFORMNAMEPROC      glGetActiveUniformName;
extern PFNGLGETUNIFORMINDICESPROC         glGetUniformIndices;
extern PFNGLGETUNIFORMBLOCKINDEXPROC      glGetUniformBlockIndex;
extern PFNGLUNIFORMBLOCKBINDINGPROC       glUniformBlockBinding;
extern PFNGLBINDBUFFERRANGEPROC           glBindBufferRange;
extern PFNGLBINDBUFFERBASEPROC            glBindBufferBase;
extern PFNGLGETACTIVEUNIFORMBLOCKIVPROC   glGetActiveUniformBlockiv;
extern PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName;
// FBO
extern PFNGLBINDFRAMEBUFFERPROC        glBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSPROC     glDeleteFramebuffers;
extern PFNGLGENFRAMEBUFFERSPROC        glGenFramebuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERTEXTUREPROC     glFramebufferTexture;
extern PFNGLFRAMEBUFFERTEXTURE3DPROC   glFramebufferTexture3D;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC   glFramebufferTexture2D;
// Instancing
extern PFNGLDRAWARRAYSINSTANCEDPROC   glDrawArraysInstanced;
extern PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
//MISC
extern PFNGLDRAWRANGEELEMENTSPROC		glDrawRangeElements;
extern PFNGLCLAMPCOLORPROC				glClampColor;
extern PFNGLGENERATEMIPMAPPROC				glGenerateMipmap;
extern PFNGLBLENDEQUATIONPROC			glBlendEquation;
extern PFNGLBLENDEQUATIONSEPARATEPROC	glBlendEquationSeparate;
extern PFNGLBLENDFUNCSEPARATEPROC		glBlendFuncSeparate;

//SEPARATE SHADER OBJECT

extern PFNGLUSEPROGRAMSTAGESPROC		glUseProgramStages;
extern PFNGLACTIVESHADERPROGRAMPROC		glActiveShaderProgram;
extern PFNGLCREATESHADERPROGRAMVPROC	glCreateShaderProgramv;
extern PFNGLBINDPROGRAMPIPELINEPROC		glBindProgramPipeline;
extern PFNGLDELETEPROGRAMPIPELINESPROC	glDeleteProgramPipelines;
extern PFNGLGENPROGRAMPIPELINESPROC		glGenProgramPipelines;
extern PFNGLISPROGRAMPIPELINEPROC		glIsProgramPipeline;
extern PFNGLGETPROGRAMPIPELINEIVPROC	glGetProgramPipelineiv;
extern PFNGLVALIDATEPROGRAMPIPELINEPROC glValidateProgramPipeline;
extern PFNGLGETPROGRAMPIPELINEINFOLOGPROC glGetProgramPipelineInfoLog;

extern PFNGLPROGRAMUNIFORM1IPROC glProgramUniform1i;
extern PFNGLPROGRAMUNIFORM2IPROC glProgramUniform2i;
extern PFNGLPROGRAMUNIFORM3IPROC glProgramUniform3i;
extern PFNGLPROGRAMUNIFORM4IPROC glProgramUniform4i;

extern PFNGLPROGRAMUNIFORM1UIPROC glProgramUniform1ui;
extern PFNGLPROGRAMUNIFORM2UIPROC glProgramUniform2ui;
extern PFNGLPROGRAMUNIFORM3UIPROC glProgramUniform3ui;
extern PFNGLPROGRAMUNIFORM4UIPROC glProgramUniform4ui;

extern PFNGLPROGRAMUNIFORM1FPROC glProgramUniform1f;
extern PFNGLPROGRAMUNIFORM2FPROC glProgramUniform2f;
extern PFNGLPROGRAMUNIFORM3FPROC glProgramUniform3f;
extern PFNGLPROGRAMUNIFORM4FPROC glProgramUniform4f;

extern PFNGLPROGRAMUNIFORM1DPROC glProgramUniform1d;
extern PFNGLPROGRAMUNIFORM2DPROC  glProgramUniform2d;
extern PFNGLPROGRAMUNIFORM3DPROC  glProgramUniform3d;
extern PFNGLPROGRAMUNIFORM4DPROC  glProgramUniform4d;

extern PFNGLPROGRAMUNIFORM1IVPROC  glProgramUniform1iv;
extern PFNGLPROGRAMUNIFORM2IVPROC glProgramUniform2iv;
extern PFNGLPROGRAMUNIFORM3IVPROC glProgramUniform3iv;
extern PFNGLPROGRAMUNIFORM4IVPROC glProgramUniform4iv;

extern PFNGLPROGRAMUNIFORM2UIVPROC glProgramUniform1uiv;
extern PFNGLPROGRAMUNIFORM3UIVPROC glProgramUniform2uiv;
extern PFNGLPROGRAMUNIFORM3UIVPROC glProgramUniform3uiv;
extern PFNGLPROGRAMUNIFORM4UIVPROC glProgramUniform4uiv;

extern PFNGLPROGRAMUNIFORM1FVPROC glProgramUniform1fv;
extern PFNGLPROGRAMUNIFORM2FVPROC glProgramUniform2fv;
extern PFNGLPROGRAMUNIFORM3FVPROC glProgramUniform3fv;
extern PFNGLPROGRAMUNIFORM4FVPROC glProgramUniform4fv;

extern PFNGLPROGRAMUNIFORM1DVPROC glProgramUniform1dv;
extern PFNGLPROGRAMUNIFORM2DVPROC glProgramUniform2dv;
extern PFNGLPROGRAMUNIFORM3DVPROC glProgramUniform3dv;
extern PFNGLPROGRAMUNIFORM4DVPROC glProgramUniform4dv;

extern PFNGLPROGRAMUNIFORMMATRIX2FVPROC glProgramUniformMatrix2fv;
extern PFNGLPROGRAMUNIFORMMATRIX3FVPROC glProgramUniformMatrix3fv;
extern PFNGLPROGRAMUNIFORMMATRIX4FVPROC glProgramUniformMatrix4fv;

extern PFNGLPROGRAMUNIFORMMATRIX2DVPROC glProgramUniformMatrix2dv;
extern PFNGLPROGRAMUNIFORMMATRIX3DVPROC glProgramUniformMatrix3dv;
extern PFNGLPROGRAMUNIFORMMATRIX4DVPROC glProgramUniformMatrix4dv;

extern PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glProgramUniformMatrix2x3fv;
extern PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glProgramUniformMatrix3x2fv;
extern PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glProgramUniformMatrix2x4fv;
extern PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glProgramUniformMatrix4x2fv;
extern PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glProgramUniformMatrix3x4fv;
extern PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glProgramUniformMatrix4x3fv;

extern PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glProgramUniformMatrix2x3dv;
extern PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glProgramUniformMatrix3x2dv;
extern PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glProgramUniformMatrix2x4dv;
extern PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glProgramUniformMatrix4x2dv;
extern PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glProgramUniformMatrix3x4dv;
extern PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glProgramUniformMatrix4x3dv;

#ifdef WIN32
extern PFNWGLSWAPINTERVALEXTPROC     wglSwapInterval;
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormat;
#endif

void AxelynxInitExtensions();
