#pragma once
#ifndef AXELYNX_MAT3_H
#define AXELYNX_MAT3_H

#include "../axlib.h"
#include "vec3.h"
#include <cassert>

namespace axelynx
{
	class AXELYNX_API mat3
	{
		public:
		float m[9];
		
		mat3(float m00, float m01, float m02,float m10, float m11, float m12,float m20, float m21, float m22)
		{
			m[0] = m00;
			m[1] = m01;
			m[2] = m02;

			m[3] = m10;
			m[4] = m11;
			m[5] = m12;

			m[6] = m20;
			m[7] = m21;
			m[8] = m22;
			
		}

		mat3()
		{
			m[0] = m[1] = m[2] = m[3] = m[4] = m[5] = m[6] = m[7] = m[8] = 0;
		}

		explicit mat3(float value)
		{
			m[0] = m[1] = m[2] = m[3] = m[4] = m[5] = m[6] = m[7] = m[8] = value;
		}

		static mat3 Identity();

		const float& operator ()(short i, short j) const
		{
			assert(i>=0 && i<3 && j>=0 && j<3);

			return m[i*3 + j];
		}

		float& operator ()(short i, short j)
		{
			assert(i>=0 && i<3 && j>=0 && j<3);

			return m[i*3 + j];
		}

		int size(){return 3;}

		operator float*()
		{
			return m;
		}
		operator const float*() const
		{
			return m;
		}

		vec3 operator*(const vec3&) const;

		vec3 ztransform(float zvalue);
	};
}
#endif
