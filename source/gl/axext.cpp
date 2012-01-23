
#include "axelynx/Logger.h"

#include "axgl.h"

GLenum g_OpenGLError = GL_NO_ERROR;

template <typename T>
void axGetProcAddress(T &p, const char * name)
{
	std::string core = name;
	std::string arb = core + std::string("ARB");
	std::string ext = core + std::string("EXT");

    #ifdef WIN32
	p = reinterpret_cast<T>(wglGetProcAddress(core.c_str()));
	if(!p)
	{
		p = reinterpret_cast<T>(wglGetProcAddress(arb.c_str()));
	}
	if(!p)
		p = reinterpret_cast<T>(wglGetProcAddress(ext.c_str()));
	#endif
	#ifdef UNIX
	p = reinterpret_cast<T>(glXGetProcAddress(reinterpret_cast<const GLubyte*>(core.c_str())));
	if(!p)
		p = reinterpret_cast<T>(glXGetProcAddress(reinterpret_cast<const GLubyte*>(arb.c_str())));
	if(!p)
		p = reinterpret_cast<T>(glXGetProcAddress(reinterpret_cast<const GLubyte*>(ext.c_str())));
	#endif
}

#ifdef WIN32
	#ifdef _MSC_VER
		#define OPENGL_GET_PROC(n) \
		axGetProcAddress(n,#n); \
		if (NULL == n) \
			 { \
			 LOG_ERROR(std::wstring(L"Loading extension fail:")+std::wstring(L#n)); \
			 } \
			 else \
			 { \
			 LOG_HINT(std::wstring(L"Loading extension success:")+std::wstring(L#n)); \
			 }
	#else
		#define OPENGL_GET_PROC(n) \
		axGetProcAddress(n,#n); \
		if (NULL == n) \
			{ \
			LOG_ERROR(std::wstring(L"Loading extension fail:")+std::wstring(L""#n)); \
			} \
			else \
			{ \
			LOG_HINT(std::wstring(L"Loading extension success:")+std::wstring(L""#n)); \
			}
    #endif
#endif

#ifdef UNIX
	#define OPENGL_GET_PROC(n) \
	axGetProcAddress(n,#n); \
	if (NULL == n) \
		 { \
		 LOG_ERROR(std::wstring(L"Loading extension fail:")+std::wstring(L""#n)); \
		 } \
		 else \
		 { \
		 LOG_HINT(std::wstring(L"Loading extension success:")+std::wstring(L""#n)); \
		 }
#endif

// объявим расширения OpenGL
// Texture
PFNGLACTIVETEXTUREPROC glActiveTexture = 0;
PFNGLTEXIMAGE2DMULTISAMPLECOVERAGENVPROC glTexImage2DMultisampleCoverageNV=0;
PFNGLTEXIMAGE2DMULTISAMPLEPROC	glTexImage2DMultisample=0;
PFNGLTEXIMAGE3DPROC		glTexImage3D=0;
PFNGLTEXSUBIMAGE3DPROC	glTexSubImage3D=0;
PFNGLTEXBUFFERPROC	glTexBuffer=0;
PFNGLGETCOMPRESSEDTEXIMAGEPROC	glGetCompressedTexImage=0;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D =0;
PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D=0;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D=0;

// VAO
PFNGLGENVERTEXARRAYSPROC    glGenVertexArrays    = 0;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = 0;
PFNGLBINDVERTEXARRAYPROC    glBindVertexArray    = 0;
// VBO
PFNGLGENBUFFERSPROC    glGenBuffers    = 0;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = 0;
PFNGLBINDBUFFERPROC    glBindBuffer    = 0;
PFNGLBUFFERDATAPROC    glBufferData    = 0;
PFNGLBUFFERSUBDATAPROC glBufferSubData = 0;
PFNGLMAPBUFFERPROC     glMapBuffer     = 0;
PFNGLUNMAPBUFFERPROC   glUnmapBuffer   = 0;
// Shaders
PFNGLCREATEPROGRAMPROC     glCreateProgram     = 0;
PFNGLDELETEPROGRAMPROC     glDeleteProgram     = 0;
PFNGLLINKPROGRAMPROC       glLinkProgram       = 0;
PFNGLVALIDATEPROGRAMPROC   glValidateProgram   = 0;
PFNGLUSEPROGRAMPROC        glUseProgram        = 0;
PFNGLGETPROGRAMIVPROC      glGetProgramiv      = 0;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = 0;
PFNGLCREATESHADERPROC      glCreateShader      = 0;
PFNGLDELETESHADERPROC      glDeleteShader      = 0;
PFNGLSHADERSOURCEPROC      glShaderSource      = 0;
PFNGLCOMPILESHADERPROC     glCompileShader     = 0;
PFNGLATTACHSHADERPROC      glAttachShader      = 0;
PFNGLDETACHSHADERPROC      glDetachShader      = 0;
PFNGLGETSHADERIVPROC       glGetShaderiv       = 0;
PFNGLGETSHADERINFOLOGPROC  glGetShaderInfoLog  = 0;
PFNGLGETPROGRAMBINARYPROC glGetProgramBinary	=0;
PFNGLPROGRAMBINARYPROC	 glProgramBinary		=0;
PFNGLPROGRAMPARAMETERIPROC glProgramParameteri =0;

// Attributes
PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation        = 0;
PFNGLBINDATTRIBLOCATIONPROC        glBindAttribLocation =0;
PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer      = 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray  = 0;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = 0;
PFNGLVERTEXATTRIBDIVISORPROC      glVertexAttribDivisor = 0;
// Uniforms
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0;
PFNGLUNIFORMMATRIX3FVPROC   glUniformMatrix3fv   = 0;
PFNGLUNIFORMMATRIX4FVPROC   glUniformMatrix4fv   = 0;
PFNGLUNIFORM1IPROC          glUniform1i          = 0;
PFNGLUNIFORM1FVPROC         glUniform1fv         = 0;
PFNGLUNIFORM2FVPROC         glUniform2fv         = 0;
PFNGLUNIFORM3FVPROC         glUniform3fv         = 0;
PFNGLUNIFORM4FVPROC         glUniform4fv         = 0;
PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv = 0;
PFNGLGETACTIVEUNIFORMSIVPROC       glGetActiveUniformsiv=0;
PFNGLGETACTIVEUNIFORMNAMEPROC      glGetActiveUniformName=0;
PFNGLGETUNIFORMINDICESPROC         glGetUniformIndices=0;
PFNGLGETUNIFORMBLOCKINDEXPROC      glGetUniformBlockIndex=0;
PFNGLUNIFORMBLOCKBINDINGPROC       glUniformBlockBinding=0;
PFNGLBINDBUFFERRANGEPROC           glBindBufferRange=0;
PFNGLBINDBUFFERBASEPROC            glBindBufferBase=0;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC   glGetActiveUniformBlockiv=0;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName=0;
// FBO
PFNGLBINDFRAMEBUFFERPROC        glBindFramebuffer        = 0;
PFNGLDELETEFRAMEBUFFERSPROC     glDeleteFramebuffers     = 0;
PFNGLGENFRAMEBUFFERSPROC        glGenFramebuffers        = 0;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = 0;
PFNGLFRAMEBUFFERTEXTUREPROC     glFramebufferTexture     = 0;
PFNGLFRAMEBUFFERTEXTURE3DPROC   glFramebufferTexture3D = 0;
PFNGLFRAMEBUFFERTEXTURE2DPROC	glFramebufferTexture2D = 0;
// Instancing
PFNGLDRAWARRAYSINSTANCEDPROC   glDrawArraysInstanced=0;
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced=0;
//MISC
PFNGLDRAWRANGEELEMENTSPROC		glDrawRangeElements = 0;
PFNGLCLAMPCOLORPROC				glClampColor =0;
PFNGLGENERATEMIPMAPPROC				glGenerateMipmap=0;
PFNGLBLENDEQUATIONPROC			glBlendEquation =0;
PFNGLBLENDEQUATIONSEPARATEPROC	glBlendEquationSeparate =0;
PFNGLBLENDFUNCSEPARATEPROC		glBlendFuncSeparate =0;

//SEPARATE SHADERS

PFNGLUSEPROGRAMSTAGESPROC		glUseProgramStages=0;
PFNGLACTIVESHADERPROGRAMPROC		glActiveShaderProgram=0;
PFNGLCREATESHADERPROGRAMVPROC	glCreateShaderProgramv=0;
PFNGLBINDPROGRAMPIPELINEPROC		glBindProgramPipeline=0;
PFNGLDELETEPROGRAMPIPELINESPROC	glDeleteProgramPipelines=0;
PFNGLGENPROGRAMPIPELINESPROC		glGenProgramPipelines=0;
PFNGLISPROGRAMPIPELINEPROC		glIsProgramPipeline=0;
PFNGLGETPROGRAMPIPELINEIVPROC	glGetProgramPipelineiv=0;
PFNGLVALIDATEPROGRAMPIPELINEPROC glValidateProgramPipeline=0;
PFNGLGETPROGRAMPIPELINEINFOLOGPROC glGetProgramPipelineInfoLog=0;

PFNGLPROGRAMUNIFORM1IPROC glProgramUniform1i=0;
PFNGLPROGRAMUNIFORM2IPROC glProgramUniform2i=0;
PFNGLPROGRAMUNIFORM3IPROC glProgramUniform3i=0;
PFNGLPROGRAMUNIFORM4IPROC glProgramUniform4i=0;

PFNGLPROGRAMUNIFORM1UIPROC glProgramUniform1ui=0;
PFNGLPROGRAMUNIFORM2UIPROC glProgramUniform2ui=0;
PFNGLPROGRAMUNIFORM3UIPROC glProgramUniform3ui=0;
PFNGLPROGRAMUNIFORM4UIPROC glProgramUniform4ui=0;

PFNGLPROGRAMUNIFORM1FPROC glProgramUniform1f=0;
PFNGLPROGRAMUNIFORM2FPROC glProgramUniform2f=0;
PFNGLPROGRAMUNIFORM3FPROC glProgramUniform3f=0;
PFNGLPROGRAMUNIFORM4FPROC glProgramUniform4f=0;

PFNGLPROGRAMUNIFORM1DPROC glProgramUniform1d=0;
PFNGLPROGRAMUNIFORM2DPROC  glProgramUniform2d=0;
PFNGLPROGRAMUNIFORM3DPROC  glProgramUniform3d=0;
PFNGLPROGRAMUNIFORM4DPROC  glProgramUniform4d=0;

PFNGLPROGRAMUNIFORM1IVPROC  glProgramUniform1iv=0;
PFNGLPROGRAMUNIFORM2IVPROC glProgramUniform2iv=0;
PFNGLPROGRAMUNIFORM3IVPROC glProgramUniform3iv=0;
PFNGLPROGRAMUNIFORM4IVPROC glProgramUniform4iv=0;

PFNGLPROGRAMUNIFORM2UIVPROC glProgramUniform1uiv=0;
PFNGLPROGRAMUNIFORM3UIVPROC glProgramUniform2uiv=0;
PFNGLPROGRAMUNIFORM3UIVPROC glProgramUniform3uiv=0;
PFNGLPROGRAMUNIFORM4UIVPROC glProgramUniform4uiv=0;

PFNGLPROGRAMUNIFORM1FVPROC glProgramUniform1fv=0;
PFNGLPROGRAMUNIFORM2FVPROC glProgramUniform2fv=0;
PFNGLPROGRAMUNIFORM3FVPROC glProgramUniform3fv=0;
PFNGLPROGRAMUNIFORM4FVPROC glProgramUniform4fv=0;

PFNGLPROGRAMUNIFORM1DVPROC glProgramUniform1dv=0;
PFNGLPROGRAMUNIFORM2DVPROC glProgramUniform2dv=0;
PFNGLPROGRAMUNIFORM3DVPROC glProgramUniform3dv=0;
PFNGLPROGRAMUNIFORM4DVPROC glProgramUniform4dv=0;

PFNGLPROGRAMUNIFORMMATRIX2FVPROC glProgramUniformMatrix2fv=0;
PFNGLPROGRAMUNIFORMMATRIX3FVPROC glProgramUniformMatrix3fv=0;
PFNGLPROGRAMUNIFORMMATRIX4FVPROC glProgramUniformMatrix4fv=0;

PFNGLPROGRAMUNIFORMMATRIX2DVPROC glProgramUniformMatrix2dv=0;
PFNGLPROGRAMUNIFORMMATRIX3DVPROC glProgramUniformMatrix3dv=0;
PFNGLPROGRAMUNIFORMMATRIX4DVPROC glProgramUniformMatrix4dv=0;

PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glProgramUniformMatrix2x3fv=0;
PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glProgramUniformMatrix3x2fv=0;
PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glProgramUniformMatrix2x4fv=0;
PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glProgramUniformMatrix4x2fv=0;
PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glProgramUniformMatrix3x4fv=0;
PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glProgramUniformMatrix4x3fv=0;

PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glProgramUniformMatrix2x3dv=0;
PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glProgramUniformMatrix3x2dv=0;
PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glProgramUniformMatrix2x4dv=0;
PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glProgramUniformMatrix4x2dv=0;
PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glProgramUniformMatrix3x4dv=0;
PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glProgramUniformMatrix4x3dv=0;

#ifdef WIN32
PFNWGLSWAPINTERVALEXTPROC     wglSwapInterval =0;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormat =0;
#endif

void AxelynxInitExtensions()
{
// Texture
	OPENGL_GET_PROC(glActiveTexture);
	OPENGL_GET_PROC(glTexBuffer);
	OPENGL_GET_PROC(glTexImage2DMultisample);
	OPENGL_GET_PROC(glTexImage3D);
	OPENGL_GET_PROC(glTexSubImage3D);
	OPENGL_GET_PROC(glGetCompressedTexImage);
	OPENGL_GET_PROC(glCompressedTexImage1D);
	OPENGL_GET_PROC(glCompressedTexImage2D);
	OPENGL_GET_PROC(glCompressedTexImage3D);
	// VAO
	OPENGL_GET_PROC(glGenVertexArrays);
	OPENGL_GET_PROC(glDeleteVertexArrays);
	OPENGL_GET_PROC(glBindVertexArray);
	// VBO
	OPENGL_GET_PROC(glGenBuffers);
	OPENGL_GET_PROC(glDeleteBuffers);
	OPENGL_GET_PROC(glBindBuffer);
	OPENGL_GET_PROC(glBufferData);
	OPENGL_GET_PROC(glBufferSubData);
	OPENGL_GET_PROC(glMapBuffer);
	OPENGL_GET_PROC(glUnmapBuffer);
	// Shaders
	OPENGL_GET_PROC(glCreateProgram);
	OPENGL_GET_PROC(glDeleteProgram);
	OPENGL_GET_PROC(glLinkProgram);
	OPENGL_GET_PROC(glValidateProgram);
	OPENGL_GET_PROC(glUseProgram);
	OPENGL_GET_PROC(glGetProgramiv);
	OPENGL_GET_PROC(glGetProgramInfoLog);
	OPENGL_GET_PROC(glCreateShader);
	OPENGL_GET_PROC(glDeleteShader);
	OPENGL_GET_PROC(glShaderSource);
	OPENGL_GET_PROC(glCompileShader);
	OPENGL_GET_PROC(glAttachShader);
	OPENGL_GET_PROC(glDetachShader);
	OPENGL_GET_PROC(glGetShaderiv);
	OPENGL_GET_PROC(glGetShaderInfoLog);
	OPENGL_GET_PROC(glGetProgramBinary);
	OPENGL_GET_PROC(glProgramBinary);
	OPENGL_GET_PROC(glProgramParameteri);
	// Attributes
	OPENGL_GET_PROC(glGetAttribLocation);
	OPENGL_GET_PROC(glBindAttribLocation);
	OPENGL_GET_PROC(glVertexAttribPointer);
	OPENGL_GET_PROC(glEnableVertexAttribArray);
	OPENGL_GET_PROC(glDisableVertexAttribArray);
	OPENGL_GET_PROC(glVertexAttribDivisor);
	// Uniforms
	OPENGL_GET_PROC(glGetUniformLocation);
	OPENGL_GET_PROC(glUniformMatrix3fv);
	OPENGL_GET_PROC(glUniformMatrix4fv);
	OPENGL_GET_PROC(glUniform1i);
	OPENGL_GET_PROC(glUniform1fv);
	OPENGL_GET_PROC(glUniform2fv);
	OPENGL_GET_PROC(glUniform3fv);
	OPENGL_GET_PROC(glUniform4fv);
	OPENGL_GET_PROC(glUniformMatrix3x2fv);
	OPENGL_GET_PROC(glGetActiveUniformsiv);
	OPENGL_GET_PROC(glGetActiveUniformName);
	OPENGL_GET_PROC(glGetUniformIndices);
	OPENGL_GET_PROC(glGetUniformBlockIndex);
	OPENGL_GET_PROC(glUniformBlockBinding);
	OPENGL_GET_PROC(glBindBufferRange);
	OPENGL_GET_PROC(glBindBufferBase);
	OPENGL_GET_PROC(glGetActiveUniformBlockiv);
	OPENGL_GET_PROC(glGetActiveUniformBlockName);
	// FBO
	OPENGL_GET_PROC(glBindFramebuffer);
	OPENGL_GET_PROC(glDeleteFramebuffers);
	OPENGL_GET_PROC(glGenFramebuffers);
	OPENGL_GET_PROC(glCheckFramebufferStatus);
	OPENGL_GET_PROC(glFramebufferTexture);
	OPENGL_GET_PROC(glFramebufferTexture3D);
	OPENGL_GET_PROC(glFramebufferTexture2D);
	// Instancing
	OPENGL_GET_PROC(glDrawArraysInstanced);
	OPENGL_GET_PROC(glDrawElementsInstanced);
	//MISC
	OPENGL_GET_PROC(glDrawRangeElements);
	OPENGL_GET_PROC(glClampColor);
	OPENGL_GET_PROC(glGenerateMipmap);
	OPENGL_GET_PROC(glBlendEquation);
	OPENGL_GET_PROC(glBlendEquationSeparate);
	OPENGL_GET_PROC(glBlendFuncSeparate);
	//SEPARATE SHADERS

	OPENGL_GET_PROC(glUseProgramStages);
	OPENGL_GET_PROC(glActiveShaderProgram);
	OPENGL_GET_PROC(glCreateShaderProgramv);
	OPENGL_GET_PROC(glBindProgramPipeline);
	OPENGL_GET_PROC(glDeleteProgramPipelines);
	OPENGL_GET_PROC(glGenProgramPipelines);
	OPENGL_GET_PROC(glIsProgramPipeline);
	OPENGL_GET_PROC(glProgramParameteri);
	OPENGL_GET_PROC(glGetProgramPipelineiv);
	OPENGL_GET_PROC(glValidateProgramPipeline);
	OPENGL_GET_PROC(glGetProgramPipelineInfoLog);
	
	OPENGL_GET_PROC(glProgramUniform1i);
	OPENGL_GET_PROC(glProgramUniform2i);
	OPENGL_GET_PROC(glProgramUniform3i);
	OPENGL_GET_PROC(glProgramUniform4i);
	
	OPENGL_GET_PROC(glProgramUniform1ui);
	OPENGL_GET_PROC(glProgramUniform2ui);
	OPENGL_GET_PROC(glProgramUniform3ui);
	OPENGL_GET_PROC(glProgramUniform4ui);
	
	OPENGL_GET_PROC(glProgramUniform1f);
	OPENGL_GET_PROC(glProgramUniform2f);
	OPENGL_GET_PROC(glProgramUniform3f);
	OPENGL_GET_PROC(glProgramUniform4f);
	
	OPENGL_GET_PROC(glProgramUniform1d);
	OPENGL_GET_PROC(glProgramUniform2d);
	OPENGL_GET_PROC(glProgramUniform3d);
	OPENGL_GET_PROC(glProgramUniform4d);
	
	OPENGL_GET_PROC(glProgramUniform1iv);
	OPENGL_GET_PROC(glProgramUniform2iv);
	OPENGL_GET_PROC(glProgramUniform3iv);
	OPENGL_GET_PROC(glProgramUniform4iv);
	
	OPENGL_GET_PROC(glProgramUniform1uiv);
	OPENGL_GET_PROC(glProgramUniform2uiv);
	OPENGL_GET_PROC(glProgramUniform3uiv);
	OPENGL_GET_PROC(glProgramUniform4uiv);
	
	OPENGL_GET_PROC(glProgramUniform1fv);
	OPENGL_GET_PROC(glProgramUniform2fv);
	OPENGL_GET_PROC(glProgramUniform3fv);
	OPENGL_GET_PROC(glProgramUniform4fv);
	
	OPENGL_GET_PROC(glProgramUniform1dv);
	OPENGL_GET_PROC(glProgramUniform2dv);
	OPENGL_GET_PROC(glProgramUniform3dv);
	OPENGL_GET_PROC(glProgramUniform4dv);
	
	OPENGL_GET_PROC(glProgramUniformMatrix2fv);
	OPENGL_GET_PROC(glProgramUniformMatrix3fv);
	OPENGL_GET_PROC(glProgramUniformMatrix4fv);
	
	OPENGL_GET_PROC(glProgramUniformMatrix2dv);
	OPENGL_GET_PROC(glProgramUniformMatrix3dv);
	OPENGL_GET_PROC(glProgramUniformMatrix4dv);
	
	OPENGL_GET_PROC(glProgramUniformMatrix2x3fv);
	OPENGL_GET_PROC(glProgramUniformMatrix3x2fv);
	OPENGL_GET_PROC(glProgramUniformMatrix2x4fv);
	OPENGL_GET_PROC(glProgramUniformMatrix4x2fv);
	OPENGL_GET_PROC(glProgramUniformMatrix3x4fv);
	OPENGL_GET_PROC(glProgramUniformMatrix4x3fv);
	
	OPENGL_GET_PROC(glProgramUniformMatrix2x3dv);
	OPENGL_GET_PROC(glProgramUniformMatrix3x2dv);
	OPENGL_GET_PROC(glProgramUniformMatrix2x4dv);
	OPENGL_GET_PROC(glProgramUniformMatrix4x2dv);
	OPENGL_GET_PROC(glProgramUniformMatrix3x4dv);
	OPENGL_GET_PROC(glProgramUniformMatrix4x3dv);
	//windows

#ifdef WIN32
	OPENGL_GET_PROC(wglSwapInterval);
	OPENGL_GET_PROC(wglChoosePixelFormat);

	//wglGetProcAddress("wglSwapIntervalEXT");
#endif
}