#pragma once
#ifndef AXELYNX_VEC4_H
#define AXELYNX_VEC4_H
#include "../axlib.h"
#include "vec3.h"

namespace axelynx
{
	class AXELYNX_API vec4
	{
		public:

		float x,y,z,w;

		operator float*()
		{
			return reinterpret_cast<float*>(this);
		}

		operator const float*() const
		{
			return reinterpret_cast<const float*>(this);
		}

		static vec4 Identity();

		vec4()
		:x(0),y(0),z(0),w(1){}

		vec4(float value)
				:x(value),y(value),z(value),w(1){}

		vec4(float x_, float y_,float z_)
				:x(x_),y(y_),z(z_),w(1){}
		vec4(float x_, float y_,float z_, float w_)
				:x(x_),y(y_),z(z_),w(w_){}

		vec4(const vec3& vec, float scalar = 0)
			:x(vec.x),y(vec.y),z(vec.z),w(scalar){}
		const vec4& operator += (float value)
		{
			x+=value; y+= value; z+=value; w+=value;
			return *this;
		}

		const vec4& operator += (const vec4& vector)
		{
			x+=vector.x; y+= vector.y; z+=vector.z;w+=vector.w;
			return *this;
		}

	};
}
#endif
