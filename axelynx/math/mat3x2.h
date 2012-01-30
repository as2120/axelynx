#pragma once
#ifndef AXELYNX_MAT3x2_H
#define AXELYNX_MAT3x2_H

#include "../axlib.h"

#include "vec2.h"
#include "mat4.h"

#include <cassert>

namespace axelynx
{
	class AXELYNX_API mat3x2
	{
		public:
		float m[6];

		static mat3x2 FAST_CALL Identity();
		static mat3x2 FAST_CALL FromTransform(const vec2& position, float angle = 0, const vec2& transform = vec2(1,1));
		static void FAST_CALL FromTransform(mat3x2& result,const vec2& position, float angle = 0, const vec2& transform = vec2(1,1));

		float& operator ()(short i, short j)
		{
			assert(i>=0 && i<3 && j>=0 && j<2);
			return m[i*2 + j];
		}

		const float& operator ()(short i, short j) const
		{
			assert(i>=0 && i<3 && j>=0 && j<2);
			return m[i*2 + j];
		}

		int size(){return 2;}
		int sizeC(){return 2;}
		int sizeY(){return 3;}

		operator float*()
		{
			return m;
		}

		operator const float*() const
		{
			return m;
		}

		vec2 operator * (const vec2& v)
		{
			return vec2( v.x * m[0] + v.y * m[2] + m[4]
						,v.x * m[1] + v.y * m[3] + m[5]);
		}


		mat4 FAST_CALL operator * (const mat4& other);
		void FAST_CALL Transform(vec2 *p, int cnt);

		void FAST_CALL transform(vec2 *v)
		{
			float nx = v->x *m[0] + v->y * m[2] + m[4];
			float ny = v->x *m[1] + v->y * m[3] + m[5];
			v->x = nx;
			v->y = ny;
		}

		void FAST_CALL transform(vec2 *to, const vec2& from)
		{
			to->x = from.x *m[0] + from.y * m[2] + m[4];
			to->y = from.x *m[1] + from.y * m[3] + m[5];;
		}

	};
}
#endif