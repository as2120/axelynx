#pragma once
#ifndef AXELYNX_VEC3_H
#define AXELYNX_VEC3_H
#include "../axlib.h"

namespace axelynx
{
	class AXELYNX_API vec3
	{
		public:

		float x,y,z;

		operator float*()
		{
			return reinterpret_cast<float*>(this);
		}

		operator const float*() const
		{
			return reinterpret_cast<const float*>(this);
		}

		static vec3 Identity();

		vec3()
		:x(0),y(0),z(0){}

		explicit vec3(float value)
				:x(value),y(value),z(value){}

		vec3(float x_, float y_,float z_)
				:x(x_),y(y_),z(z_){}

		const vec3& operator += (float value)
		{
			x+=value; y+= value; z+=value;
			return *this;
		}

		const vec3& operator += (const vec3& vector)
		{
			x+=vector.x; y+= vector.y; z+=vector.z;
			return *this;
		}


		vec3 operator * (float value) const
		{
			return vec3(x * value, y* value, z * value);
		}

		vec3 operator * (const vec3& other) const
		{
			return vec3(x * other.x, y* other.y, z * other.z);
		}

		const vec3& operator *= (float value) 
		{
			x *= value;
			y *= value;
			z *= value;
			return (*this);
		}

		vec3 FAST_CALL cross(const vec3& other) const
		{
			return vec3(y*other.z-z*other.y, z*other.x-x*other.z, x*other.y-y*other.x);
		}

		float FAST_CALL dot(const vec3& other) const
		{
			return x*other.x + y*other.y + z*other.z;
		}
		float FAST_CALL length() const;
		vec3 FAST_CALL normalize();
		vec3 FAST_CALL normalize() const
		{
			vec3 result(x,y,z);
			result.normalize();
			return result;
		}


		vec3 operator + (const vec3& other) const
		{
			return vec3(x+other.x, y+other.y, z+other.z);
		}

		vec3 operator + (float value) const
		{
			return vec3(x+value, y+value, z+value);
		}

		vec3 operator - (float value) const
		{
			return vec3(x-value, y-value, z-value);
		}

		vec3 operator - (const vec3& other) const
		{
			return vec3(x-other.x, y-other.y, z-other.z);
		}

		vec3 operator-() const
		{
			return vec3(-x,-y,-z);
		}
	};
}

#endif