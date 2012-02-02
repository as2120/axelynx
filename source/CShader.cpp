
#include "CShader.h"
#include "axelynx//Logger.h"
#include "axelynx/utils/axstring.h"
#include <string.h>
#include <iostream>
#include "CStatistics.h"
#include "CFile.h"
#include "CEngine.h"
#include "axelynx/Shell.h"
#include "saSpec.h"

CShader * CShader::current =0;
std::string shaderprolog = "";

#define SHADER_SOURCE_MAX_SIZE 65536
GLint CShader::ShaderProgramStatus(GLuint program, GLenum param)
{
	GLint status, length;
	GLchar buffer[1024];

	glGetProgramiv(program, param, &status);

	if (status != GL_TRUE)
	{
		glGetProgramInfoLog(program, 1024, &length, buffer);

		LOG_ERROR(L"link shader failed!");
		wchar_t * result = axelynx::utils::MultiByteToWideChar(buffer);
		LOG_ERROR(result);
		delete[] result;
	}
	OPENGL_CHECK_FOR_ERRORS();
	return status;
}

GLint CShader::ShaderStatus(GLuint shader, GLenum param, const char *source)
{
	GLint status, length;
	GLchar buffer[1024];
	OPENGL_CHECK_FOR_ERRORS();
	glGetShaderiv(shader, param, &status);
	OPENGL_CHECK_FOR_ERRORS();
	if (status != GL_TRUE)
	{
		glGetShaderInfoLog(shader, 1024, &length, buffer);
		OPENGL_CHECK_FOR_ERRORS();
		bool error_line[1024] = {false};

		//highlight lines with errors

		//ATI
		for(unsigned int i=0;i<length;++i)
		{
			if(buffer[i] == ':')
			{
				int line_number = 0;
				sscanf(buffer + i,":%d:",&line_number);
				if(line_number>0 && line_number<256)
					error_line[line_number] = true;

			}
		}

		//NVIDIA
		for(unsigned int i=0;i<length;++i)
		{
			if(buffer[i] == '(')
			{
				int line_number = 0;
				sscanf(buffer + i,"(%d)",&line_number);
				if(line_number>0 && line_number<256)
					error_line[line_number] = true;

			}
		}

		std::string logerror = "";

		for(unsigned int i = 0;i<length;++i)
		{
			if(buffer[i] != 10)
			{
				logerror += buffer[i];
			}
			else
			{
				logerror += "<br>";
			}
		}

		wchar_t * result = axelynx::utils::MultiByteToWideChar(logerror.c_str());

		LOG_ERROR(result);
		delete[] result;

		LOG_WRITE(L"<br><table border = 1><tr><td><font color = black>");
		std::string src = "";

		int curline = 1;
		for(unsigned int i = 0;i<strlen(source);++i)
		{
			if(source[i] != 10)
			{
				if(error_line[curline])
					src+= "<font color = red>";
				src += source[i];
				if(error_line[curline])
					src+= "</font>";
			}
			else
			{
				src += "<br>";
				curline ++;
			}
		}

		result = axelynx::utils::MultiByteToWideChar(src.c_str());
		LOG_WRITE(result);
		delete[] result;
		LOG_WRITE(L"</font></td></tr></table>");

		if(CEngine::Instance()->Settings().Shaders.ShowLogOnFailCompile)
		{
			axelynx::shell::Execute(axelynx::Logger::Instance()->GetLogFileName().c_str());
		}
	}
	OPENGL_CHECK_FOR_ERRORS();
	return status;
}

bool CShader::VertexSource(const char *src)
{
	if(!src)
		return false;

	if(!vertexshader_)
	{
		vertexshader_ = glCreateShader(GL_VERTEX_SHADER);
	}

	std::string glslversion = ShaderVersionProlog();
	vertexsource_ = glslversion + std::string(src);
	int len = vertexsource_.length();

	const char *source = vertexsource_.c_str();
	glShaderSource(vertexshader_,1,&source,&len);
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}

bool CShader::FragmentSource(const char *src)
{
	if(!src)
		return false;

	if(!fragmentshader_)
	{
		fragmentshader_ = glCreateShader(GL_FRAGMENT_SHADER);
	}

	std::string glslversion = ShaderVersionProlog();
	fragmentsource_ = glslversion + std::string(src);
	int len = fragmentsource_.length();

	const char *source = fragmentsource_.c_str();

	glShaderSource(fragmentshader_,1,&source,&len);
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}

bool CShader::GeometrySource(const char *src)
{
	if(!src)
		return false;

	if(!geometryshader_)
	{
		geometryshader_ = glCreateShader(GL_GEOMETRY_SHADER);
	}

	std::string glslversion = ShaderVersionProlog();
	geometrysource_ = glslversion + std::string(src);
	int len = geometrysource_.length();
	const char *source = geometrysource_.c_str();
	glShaderSource(geometryshader_,1,&source,&len);
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}

bool CShader::TesselationSource(const char *control, const char *evaluation)
{
	if(!control && !evaluation)
		return false;

	if(!tesscontrolshader_ && control)
	{
		tesscontrolshader_ = glCreateShader(GL_TESS_CONTROL_SHADER);
	}

	if(!tessevaluationshader_ && evaluation)
	{
		tessevaluationshader_ = glCreateShader(GL_TESS_EVALUATION_SHADER);
	}

	std::string glslversion = ShaderVersionProlog();
	tesscontrolsource_ = glslversion + std::string(control);
	tessevaluationsource_ =glslversion + std::string(evaluation);

	if(control)
	{
		int len = tesscontrolsource_.length();
		glShaderSource(tesscontrolshader_,1,&control,&len);
	}

	if(evaluation)
	{
		int len = tessevaluationsource_.length();
		glShaderSource(tessevaluationshader_,1,&evaluation,&len);
	}
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}


bool CShader::Compile()
{
	if(compiled_)
		return true;

	compiled_ = true;

	if(vertexshader_)
	{
		glCompileShader(vertexshader_);
			OPENGL_CHECK_FOR_ERRORS();
		if(ShaderStatus(vertexshader_, GL_COMPILE_STATUS,vertexsource_.c_str()))
		{
			OPENGL_CHECK_FOR_ERRORS();
			glAttachShader(programm_,vertexshader_);
		}			
		else
			compiled_ = false;
	}
	OPENGL_CHECK_FOR_ERRORS();
	if(fragmentshader_)
	{
		glCompileShader(fragmentshader_);
			OPENGL_CHECK_FOR_ERRORS();
		if(ShaderStatus(fragmentshader_, GL_COMPILE_STATUS,fragmentsource_.c_str()))
		{
			OPENGL_CHECK_FOR_ERRORS();
			glAttachShader(programm_,fragmentshader_);
		}
		else
			compiled_ = false;
	}
	OPENGL_CHECK_FOR_ERRORS();
	if(geometryshader_)
	{
		glCompileShader(geometryshader_);
		if(ShaderStatus(geometryshader_, GL_COMPILE_STATUS,geometrysource_.c_str()))
			glAttachShader(programm_,geometryshader_);
		else
			compiled_ = false;
	}
	OPENGL_CHECK_FOR_ERRORS();
	if(tesscontrolshader_)
	{
		glCompileShader(tesscontrolshader_);
		if(ShaderStatus(tesscontrolshader_, GL_COMPILE_STATUS,tesscontrolsource_.c_str()))
			glAttachShader(programm_,tesscontrolshader_);
		else
			compiled_ = false;
	}
	OPENGL_CHECK_FOR_ERRORS();
	if(tessevaluationshader_)
	{
		glCompileShader(tessevaluationshader_);
		if(ShaderStatus(tessevaluationshader_, GL_COMPILE_STATUS,tesscontrolsource_.c_str()))
			glAttachShader(programm_,tessevaluationshader_);
		else
			compiled_ = false;
	}
	OPENGL_CHECK_FOR_ERRORS();
	if(CEngine::Instance()->Settings().Shaders.UseCash)
		if(glProgramParameteri)
			glProgramParameteri(programm_,GL_PROGRAM_BINARY_RETRIEVABLE_HINT,GL_TRUE);
	OPENGL_CHECK_FOR_ERRORS();
	glLinkProgram(programm_);
	if(ShaderProgramStatus(programm_, GL_LINK_STATUS))
	{
	}
	else
	{
		compiled_ = false;
	}
	OPENGL_CHECK_FOR_ERRORS();
	for(int i = 0;i<32;++i)
	{
		stdlocations[i] = -1;
	}
	OPENGL_CHECK_FOR_ERRORS();
	Bind();
	OPENGL_CHECK_FOR_ERRORS();
	stdlocations[SU_MODEL] = GetUniformLocation("model");
	stdlocations[SU_MODELVIEW] = GetUniformLocation("modelview");
	stdlocations[SU_MODELVIEWPROJ] = GetUniformLocation("modelviewproj");
	stdlocations[SU_COLOR] = GetUniformLocation("entitycolor");
	stdlocations[SU_NORMALMATRIX] = GetUniformLocation("normalmatrix");
	stdlocations[SU_VIEWDIR] = GetUniformLocation("viewdir");
	stdlocations[SU_LIGHTPOS] = GetUniformLocation("lightpos");
	stdlocations[SU_LIGHTDIR] = GetUniformLocation("lightdir");
	stdlocations[SU_ENTITYCOLOR] = GetUniformLocation("entitycolor");
	stdlocations[SU_EYEPOS] = GetUniformLocation("eyepos");
	OPENGL_CHECK_FOR_ERRORS();
	for(int i=0;i<8;++i)
	{
		char buff[9] = "texture0";
		buff[7] = '0' + i;
		int loc = GetUniformLocation(buff);
		if(loc >= 0)
			SetUniform(loc,i);
	}
	OPENGL_CHECK_FOR_ERRORS();
	UnBind();
	OPENGL_CHECK_FOR_ERRORS();
	return compiled_;
}

bool CShader::SetUniform(int location, int value)
{
	if (location != -1)
	{
		if(current == this)
			glUniform1i(location,value);
		else
		{
			CShader *old = current;
			Bind();
			glUniform1i(location,value);
			UnBind();
			if(old)
				old->Bind();
		}
	}

	CStatistics::uniforms++;
	CStatistics::uniformsdata += sizeof(value);
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}

bool CShader::SetUniform(int location, float value)
{
	if (location != -1)
	{
		if(current == this)
			glUniform1fv(location,1,&value);
		else
		{
			CShader *old = current;
			Bind();
			glUniform1fv(location,1,&value);
			UnBind();
			if(old)
				old->Bind();
		}
	}


	CStatistics::uniforms++;
	CStatistics::uniformsdata += sizeof(value);
	return true;
}

bool CShader::SetUniform(int location, const axelynx::vec2& value)
{
	CStatistics::uniforms++;
	CStatistics::uniformsdata += sizeof(value);

	if (location != -1)
	{
		if(current == this)
			glUniform2fv(location,1,value);
		else
		{
			CShader *old = current;
			Bind();
			glUniform2fv(location,1,value);
			UnBind();
			if(old)
				old->Bind();
		}
	}

	return true;
}

bool CShader::SetUniform(int location, const axelynx::vec3& value)
{
	CStatistics::uniforms++;
	CStatistics::uniformsdata += sizeof(value);

	if (location != -1)
	{
		if(current == this)
			glUniform3fv(location,1,value);
		else
		{
			CShader *old = current;
			Bind();
			glUniform3fv(location,1,value);
			UnBind();
			if(old)
				old->Bind();
		}
	}


	return true;
}

bool CShader::SetUniform(int location, const axelynx::vec4& value)
{
	if (location != -1)
	{
		if(current == this)
			glUniform4fv(location,1,value);
		else
		{
			CShader *old = current;
			Bind();
			glUniform4fv(location,1,value);
			UnBind();
			if(old)
				old->Bind();
		}
	}


	CStatistics::uniforms++;
	CStatistics::uniformsdata += sizeof(value);
	return true;
}

bool CShader::SetUniform(int location, const axelynx::mat2& value)
{
	CStatistics::uniforms++;
	CStatistics::uniformsdata += sizeof(value);
	return true;
}

bool CShader::SetUniform(int location, const axelynx::mat3& value)
{
	if (location != -1)
	{
		if(current == this)
			glUniformMatrix3fv(location, 1, GL_FALSE, value);
		else
		{
			CShader *old = current;
			Bind();
			glUniformMatrix3fv(location, 1, GL_FALSE, value);
			UnBind();
			if(old)
				old->Bind();
		}
	}
		
	CStatistics::uniforms++;
	CStatistics::uniformsdata += sizeof(value);
	return true;
}

bool CShader::SetUniform(int location, const axelynx::mat4& value)
{
	if (location != -1)
	{
		if(current == this)
			glUniformMatrix4fv(location, 1, GL_FALSE, value.m);
		else
		{
			CShader *old = current;
			Bind();
			glUniformMatrix4fv(location, 1, GL_FALSE, value);
			UnBind();
			if(old)
				old->Bind();
		}
	}

	CStatistics::uniforms++;
	CStatistics::uniformsdata += sizeof(value);
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}

bool CShader::SetUniform(int location, const axelynx::mat3x2& value)
{
	if (location != -1)
	{
		if(current == this)
			glUniformMatrix3x2fv(location, 1, GL_FALSE, value);
		else
		{
			CShader *old = current;
			Bind();
			glUniformMatrix3x2fv(location, 1, GL_FALSE, value);
			UnBind();
			if(old)
				old->Bind();
		}
	}

	CStatistics::uniforms++;
	CStatistics::uniformsdata += sizeof(value);
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}



bool CShader::SetUniformArray(int location, int count, int *value)
{
	if (location != -1)
	{
		if(current == this)
			glUniform1i(location,*value);
		else
		{
			CShader *old = current;
			Bind();
			glUniform1i(location,*value);
			UnBind();
			if(old)
				old->Bind();
		}
	}

	CStatistics::uniforms++;
	CStatistics::uniformsdata += sizeof(value);
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}

bool CShader::SetUniformArray(int location, int count, float *value)
{
	if (location != -1)
	{
		if(current == this)
			glUniform1fv(location,count,value);
		else
		{
			CShader *old = current;
			Bind();
			glUniform1fv(location,count,value);
			UnBind();
			if(old)
				old->Bind();
		}
	}


	CStatistics::uniforms++;
	CStatistics::uniformsdata += sizeof(value);
	return true;
}

bool CShader::SetUniformArray(int location, int count, const axelynx::vec2* value)
{
	CStatistics::uniforms++;
	CStatistics::uniformsdata += sizeof(value);

	if (location != -1)
	{
		if(current == this)
			glUniform2fv(location,count,*value);
		else
		{
			CShader *old = current;
			Bind();
			glUniform2fv(location,count,*value);
			UnBind();
			if(old)
				old->Bind();
		}
	}

	return true;
}

bool CShader::SetUniformArray(int location, int count, const axelynx::vec3* value)
{
	CStatistics::uniforms++;
	CStatistics::uniformsdata += sizeof(value);

	if (location != -1)
	{
		if(current == this)
			glUniform3fv(location,count,*value);
		else
		{
			CShader *old = current;
			Bind();
			glUniform3fv(location,count,*value);
			UnBind();
			if(old)
				old->Bind();
		}
	}


	return true;
}

bool CShader::SetUniformArray(int location, int count, const axelynx::vec4* value)
{
	if (location != -1)
	{
		if(current == this)
			glUniform4fv(location,count,*value);
		else
		{
			CShader *old = current;
			Bind();
			glUniform4fv(location,count,*value);
			UnBind();
			if(old)
				old->Bind();
		}
	}


	CStatistics::uniforms++;
	CStatistics::uniformsdata += sizeof(value);
	return true;
}

bool CShader::SetUniformArray(int location, int count, const axelynx::mat2* value)
{
	CStatistics::uniforms++;
	CStatistics::uniformsdata += sizeof(value);
	return true;
}

bool CShader::SetUniformArray(int location, int count, const axelynx::mat3* value)
{
	if (location != -1)
	{
		if(current == this)
			glUniformMatrix3fv(location, count, GL_FALSE, *value);
		else
		{
			CShader *old = current;
			Bind();
			glUniformMatrix3fv(location, count, GL_FALSE, *value);
			UnBind();
			if(old)
				old->Bind();
		}
	}
		
	CStatistics::uniforms++;
	CStatistics::uniformsdata += sizeof(value);
	return true;
}

bool CShader::SetUniformArray(int location, int count, const axelynx::mat4* value)
{
	if (location != -1)
	{
		if(current == this)
			glUniformMatrix4fv(location, count, GL_FALSE, *value);
		else
		{
			CShader *old = current;
			Bind();
			glUniformMatrix4fv(location, count, GL_FALSE, *value);
			UnBind();
			if(old)
				old->Bind();
		}
	}

	CStatistics::uniforms++;
	CStatistics::uniformsdata += sizeof(value);
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}

bool CShader::SetUniformArray(int location, int count, const axelynx::mat3x2* value)
{
	if (location != -1)
	{
		if(current == this)
			glUniformMatrix3x2fv(location, count, GL_FALSE, *value);
		else
		{
			CShader *old = current;
			Bind();
			glUniformMatrix3x2fv(location, count, GL_FALSE, *value);
			UnBind();
			if(old)
				old->Bind();
		}
	}

	CStatistics::uniforms++;
	CStatistics::uniformsdata += sizeof(value);
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}


bool CShader::Bind()
{

	if(compiled_)
	{


		if(current != this)
		{
			restored_ = current;

			CStatistics::shaders++;
			glUseProgram(programm_);
		}

		current = this;
	}
	else
	{
		glUseProgram(0);
		current = 0;
	}

	OPENGL_CHECK_FOR_ERRORS();
	return true;
}

bool CShader::UnBind(bool restore)
{
	if(restore)
		if(restored_)
			restored_->Bind();

	if(current == this)
	{
		glUseProgram(0);
	}
	OPENGL_CHECK_FOR_ERRORS();
	current = 0;
	return true;
}

AXELYNX_API axelynx::Shader * axelynx::Shader::Create() //static
{
	return new CShader();
}

CShader::CShader()
	:programm_(0)
	,vertexsource_("")
	,fragmentsource_("")
	,geometrysource_("")
	,tesscontrolsource_("")
	,tessevaluationsource_("")
	,vertexshader_(0)
	,fragmentshader_(0)
	,geometryshader_(0)
	,tesscontrolshader_(0)
	,tessevaluationshader_(0)
	,compiled_(false)
{
	programm_ = glCreateProgram();
	restored_ = 0;
	OPENGL_CHECK_FOR_ERRORS();
}

int CShader::GetAttribLocation(const char *name)
{
	return glGetAttribLocation(programm_, name);
}

int CShader::GetUniformLocation(const char *name)
{
	if(!compiled_)
		return -1;

	int result = glGetUniformLocation(programm_, name);

	return result;
}

int CShader::BindAttribLocation(const char* name, int location)
{
	std::cout<<"name:"<<name<<std::endl;
	std::cout<<"location:"<<location<<std::endl;
	glBindAttribLocation(programm_,static_cast<GLuint>(location),name);

	OPENGL_CHECK_FOR_ERRORS();

	if(glGetError() == GL_NO_ERROR)
	{
		return 0;
	}

	return 1;
}

bool InitShaderVersion()
{
	const GLubyte * txt = glGetString(GL_SHADING_LANGUAGE_VERSION);
	char *vs = "#version xxx \n";
	shaderprolog = std::string(vs);

	shaderprolog[9] = char(txt[0]);
	shaderprolog[10] = char(txt[2]);
	shaderprolog[11] = char(txt[3]);

	shaderprolog += std::string("precision highp float; \n");

	int cnt_va = sizeof(sysattribs) / sizeof(sysattribs[0]);

	for(int i=0;i<cnt_va;++i)
	{
		char buff[255];
		sprintf(buff,"\n#define %s %d",sysattribs[i].definename,i);
		shaderprolog += buff;
	}

	shaderprolog += "\n";

	std::cout<<shaderprolog<<std::endl;
	return true;
}

std::string ShaderVersionProlog()
{
	return shaderprolog;
}

bool CShader::LoadVertexSource(axelynx::File file)
{
	file.Open();

	long size = file.Size();

	char buff[SHADER_SOURCE_MAX_SIZE];
	if(size>SHADER_SOURCE_MAX_SIZE)
		size = SHADER_SOURCE_MAX_SIZE;
	file.Read(buff,size);
	std::cout.write(buff,size);
	buff[size] = 0;
	file.Close();
	return VertexSource(buff);

}

bool CShader::LoadFragmentSource(axelynx::File file)
{
	file.Open();

	long size = file.Size();

	char buff[SHADER_SOURCE_MAX_SIZE];
	if(size>SHADER_SOURCE_MAX_SIZE)
		size = SHADER_SOURCE_MAX_SIZE;
	file.Read(buff,size);
	std::cout.write(buff,size);
	buff[size] = 0;
	file.Close();
	return FragmentSource(buff);
}

bool CShader::LoadGeometrySource(axelynx::File file)
{
	file.Open();

	long size = file.Size();

	char buff[SHADER_SOURCE_MAX_SIZE];
	if(size>SHADER_SOURCE_MAX_SIZE)
		size = SHADER_SOURCE_MAX_SIZE;
	file.Read(buff,size);
	std::cout.write(buff,size);
	buff[size] = 0;
	file.Close();
	return GeometrySource(buff);
}

bool CShader::LoadTesselationControlSource(axelynx::File file)
{
	file.Open();

	long size = file.Size();

	char buff[SHADER_SOURCE_MAX_SIZE];
	if(size>SHADER_SOURCE_MAX_SIZE)
		size = SHADER_SOURCE_MAX_SIZE;
	file.Read(buff,size);
	std::cout.write(buff,size);
	buff[size] = 0;
	file.Close();
	return TesselationSource(buff,0);
}

bool CShader::LoadTesselationEvaluationSource(axelynx::File file)
{
	file.Open();

	long size = file.Size();

	char buff[SHADER_SOURCE_MAX_SIZE];
	if(size>SHADER_SOURCE_MAX_SIZE)
		size = SHADER_SOURCE_MAX_SIZE;
	file.Read(buff,size);
	std::cout.write(buff,size);
	buff[size] = 0;
	file.Close();
	return TesselationSource(0,buff);
}

 int CShader::GetStandartUniformLocation(axelynx::Shader::StandartUniform uniform)
{
	return stdlocations[uniform];
}

bool CShader::SaveBinary(const wchar_t *filename)
{
	if(!glGetProgramBinary)
		return false;

	FILE *f = axWriteFile(filename);

	
	GLenum type;
	GLsizei sz;

	glGetProgramiv(programm_,GL_PROGRAM_BINARY_LENGTH,&sz);
	
	GLbyte *data = new GLbyte[sz];
	glGetProgramBinary(programm_,65536,&sz,&type,data);

	fwrite(&type,4,1,f);
	fwrite(&sz,4,1,f);
	fwrite(data,1,sz,f);
	fclose(f);

	return true;
}

bool CShader::LoadBinary(axelynx::File file)
{
	if(!glProgramBinary)
		return false;

	GLenum type;
	GLsizei sz;
	
	file.Open();
	file.Read(type);
	file.Read(sz);

	GLbyte *data = new GLbyte[sz];

	file.Read(data,sz);
	file.Close();
	glProgramBinary(programm_,type,data,sz);
	delete[] data;
	compiled_ = true;
	if(ShaderProgramStatus(programm_, GL_LINK_STATUS))
	{
	}
	else
	{
		compiled_ = false;
		return compiled_;
	}

	for(int i = 0;i<32;++i)
	{
		stdlocations[i] = -1;
	}

	Bind();
	stdlocations[SU_MODEL] = GetUniformLocation("model");
	stdlocations[SU_MODELVIEW] = GetUniformLocation("modelview");
	stdlocations[SU_MODELVIEWPROJ] = GetUniformLocation("modelviewproj");
	stdlocations[SU_COLOR] = GetUniformLocation("entitycolor");
	stdlocations[SU_NORMALMATRIX] = GetUniformLocation("normalmatrix");
	stdlocations[SU_VIEWDIR] = GetUniformLocation("viewdir");
	stdlocations[SU_LIGHTPOS] = GetUniformLocation("lightpos");
	stdlocations[SU_LIGHTDIR] = GetUniformLocation("lightdir");

	for(int i=0;i<8;++i)
	{
		char buff[9] = "texture0";
		buff[7] = '0' + i;
		int loc = GetUniformLocation(buff);
		if(loc >= 0)
			SetUniform(loc,i);
	}

	UnBind();

	return true;
}

bool CShader::Valid() const
{
	return compiled_;
}

bool CShader::SaveFile(const wchar_t* filename_s) const
{
	if(""!=vertexsource_)
	{
		std::wstring filename = filename_s;
		filename += L".vs";

		axelynx::File file(filename);
		file.Create();
		file.WriteString(vertexsource_);
		file.Close();
	}

	if(""!=fragmentsource_)
	{
		std::wstring filename = filename_s;
		filename += L".fs";

		axelynx::File file(filename);
		file.Create();
		file.WriteString(fragmentsource_);
		file.Close();
	}

	if(""!=geometrysource_)
	{
		std::wstring filename = filename_s;
		filename += L".gs";

		axelynx::File file(filename);
		file.Create();
		file.WriteString(geometrysource_);
		file.Close();
	}

	if(""!=tesscontrolsource_)
	{
		std::wstring filename = filename_s;
		filename += L".tcs";

		axelynx::File file(filename);
		file.Create();
		file.WriteString(tesscontrolsource_);
		file.Close();
	}

	if(""!=tessevaluationsource_)
	{
		std::wstring filename = filename_s;
		filename += L".tes";

		axelynx::File file(filename);
		file.Create();
		file.WriteString(tessevaluationsource_);
		file.Close();
	}

	return true;
}

axelynx::Shader* axelynx::Shader::GetCurrent()
{
	return CShader::Current();
}