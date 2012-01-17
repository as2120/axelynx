#pragma once

#include "axlib.h"
#include "math/vectormath.h"
#include "File.h"

namespace axelynx
{
	class Shader
	{
	public:
		enum StandartUniform
		{
			SU_MODEL			=0
			,SU_MODELVIEW		=1
			,SU_MODELVIEWPROJ	=2
			,SU_COLOR			=3
			,SU_NORMALMATRIX	=4
			,SU_VIEWDIR			=5
			,SU_LIGHTPOS		=6
			,SU_LIGHTDIR		=7
			,SU_ENTITYCOLOR		=8
			,SU_EYEPOS			=9
		};

		static AXELYNX_API Shader * Create();

		virtual bool VertexSource(const char *src)=0;
		virtual bool FragmentSource(const char *src)=0;
		virtual bool GeometrySource(const char *src)=0;
		virtual bool TesselationSource(const char *control, const char *evaluation)=0;

		virtual bool Compile()=0;

		template<class T>
		bool SetUniform(const char *name, T value)
		{
			return SetUniform(GetUniformLocation(name),value);
		}

		template<class T>
		bool SetUniform(StandartUniform uniform, T value)
		{
			return SetUniform(GetStandartUniformLocation(uniform),value);
		}
		
		virtual bool SetUniform(int location, int value)=0;
		virtual bool SetUniform(int location, float value)=0;
		virtual bool SetUniform(int location, const vec2& value)=0;
		virtual bool SetUniform(int location, const vec3& value)=0;
		virtual bool SetUniform(int location, const vec4& value)=0;
		virtual bool SetUniform(int location, const mat2& value)=0;
		virtual bool SetUniform(int location, const mat3& value)=0;
		virtual bool SetUniform(int location, const mat4& value)=0;
		virtual bool SetUniform(int location, const mat3x2& value)=0;

		virtual bool SetUniformArray(int location, int count, int *value)=0;
		virtual bool SetUniformArray(int location, int count, float *value)=0;
		virtual bool SetUniformArray(int location, int count, const vec2* value)=0;
		virtual bool SetUniformArray(int location, int count, const vec3* value)=0;
		virtual bool SetUniformArray(int location, int count, const vec4* value)=0;
		virtual bool SetUniformArray(int location, int count, const mat2* value)=0;
		virtual bool SetUniformArray(int location, int count, const mat3* value)=0;
		virtual bool SetUniformArray(int location, int count, const mat4* value)=0;
		virtual bool SetUniformArray(int location, int count, const mat3x2* value)=0;

		virtual bool Bind()=0;
		virtual bool UnBind(bool restore = false)=0;

		virtual int GetAttribLocation(const char *name)=0;
		virtual int GetUniformLocation(const char *name)=0;

		virtual int BindAttribLocation(const char* name, int location)=0;

		virtual bool LoadVertexSource(File file)=0;
		virtual bool LoadFragmentSource(File file)=0;
		virtual bool LoadGeometrySource(File file)=0;
		virtual bool LoadTesselationControlSource(File file)=0;
		virtual bool LoadTesselationEvaluationSource(File file)=0;

		virtual bool SaveBinary(const wchar_t *filename)=0;
		virtual bool LoadBinary(File file)=0;

		virtual bool Valid() const = 0;
		virtual int GetStandartUniformLocation(StandartUniform uniform)=0;
		virtual ~Shader(){};

		virtual bool SaveFile(const wchar_t* filename) const =0;
	};
}
