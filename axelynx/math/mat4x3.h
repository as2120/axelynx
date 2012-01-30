#pragma once
#ifndef AXELYNX_MAT4x3_H
#define AXELYNX_MAT4x3_H

#include "../axlib.h"
#include <cassert>
#include "vec3.h"

namespace axelynx
{
    AXELYNX_MEMALIGN(16) class AXELYNX_API mat4x3
	{
		public:
		float m[12];

		static mat4x3 Identity();

		float& operator ()(short i, short j)
		{
			assert(i>=0 && i<4 && j>=0 && j<3);
			return m[i*2 + j];
		}

		const float& operator ()(short i, short j) const
		{
			assert(i>=0 && i<4 && j>=0 && j<3);
			return m[i*2 + j];
		}

		int size(){return 3;}
		int sizeX(){return 3;}
		int sizeY(){return 4;}

		operator float*()
		{
			return m;
		}

		operator const float*() const
		{
			return m;
		}

		vec3 operator * (const vec3& v)
		{
			return vec3( v.x * m[0] + v.y * m[3] + v.z * m[6] + m[9]
						,v.x * m[1] + v.y * m[4] + v.z * m[7] + m[10]
						,v.x * m[2] + v.y * m[5] + v.z * m[8] + m[11]);
		}

		void FAST_CALL Transform(vec3 *p, int cnt);
	};
}
#endif
