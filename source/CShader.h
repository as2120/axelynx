#pragma once

#include "axelynx/Shader.h"
#include "gl/axgl.h"
#include <string>

class CShader : public axelynx::Shader
{
	GLuint programm_;

	std::string vertexsource_;
	std::string fragmentsource_;
	std::string geometrysource_;
	std::string tesscontrolsource_;
	std::string tessevaluationsource_;

	GLuint vertexshader_;
	GLuint fragmentshader_;
	GLuint geometryshader_;
	GLuint tesscontrolshader_;
	GLuint tessevaluationshader_;

	static CShader * current;
	bool compiled_;

	int stdlocations[32];

	mutable CShader *restored_;
public:
	CShader();

	virtual bool VertexSource(const char *src);
	virtual bool FragmentSource(const char *src);
	virtual bool GeometrySource(const char *src);
	virtual bool TesselationSource(const char *control, const char *evaluation);

	virtual bool Compile();

	virtual bool SetUniform(int location, int value);
	virtual bool SetUniform(int location, float value);
	virtual bool SetUniform(int location, const axelynx::vec2& value);
	virtual bool SetUniform(int location, const axelynx::vec3& value);
	virtual bool SetUniform(int location, const axelynx::vec4& value);
	virtual bool SetUniform(int location, const axelynx::mat2& value);
	virtual bool SetUniform(int location, const axelynx::mat3& value);
	virtual bool SetUniform(int location, const axelynx::mat4& value);
	virtual bool SetUniform(int location, const axelynx::mat3x2& value);

	virtual bool SetUniformArray(int location, int count, int* value);
	virtual bool SetUniformArray(int location, int count, float* value);
	virtual bool SetUniformArray(int location, int count, const axelynx::vec2* value);
	virtual bool SetUniformArray(int location, int count, const axelynx::vec3* value);
	virtual bool SetUniformArray(int location, int count, const axelynx::vec4* value);
	virtual bool SetUniformArray(int location, int count, const axelynx::mat2* value);
	virtual bool SetUniformArray(int location, int count, const axelynx::mat3* value);
	virtual bool SetUniformArray(int location, int count, const axelynx::mat4* value);
	virtual bool SetUniformArray(int location, int count, const axelynx::mat3x2* value);

	virtual bool Bind();
	virtual bool UnBind(bool restore = false);

	virtual int GetAttribLocation(const char *name);
	virtual int GetUniformLocation(const char *name);

	static CShader* Current()
	{
		return current;
	}

	GLint ShaderProgramStatus(GLuint program, GLenum param);
	GLint ShaderStatus(GLuint shader, GLenum param, const char *source);

	virtual int BindAttribLocation(const char* name, int location);

	virtual bool LoadVertexSource(axelynx::File file);
	virtual bool LoadFragmentSource(axelynx::File file);
	virtual bool LoadGeometrySource(axelynx::File file);
	virtual bool LoadTesselationControlSource(axelynx::File file);
	virtual bool LoadTesselationEvaluationSource(axelynx::File file);

	virtual bool SaveBinary(const wchar_t *filename);
	virtual bool LoadBinary(axelynx::File file);

	virtual bool Valid() const;
	virtual int GetStandartUniformLocation(axelynx::Shader::StandartUniform uniform);

	virtual bool SaveFile(const wchar_t* filename) const;
};

bool InitShaderVersion();
std::string ShaderVersionProlog();

extern std::string shaderprolog;