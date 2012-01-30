#pragma once
#ifndef AXELYNX_MAT2_H
#define AXELYNX_MAT2_H
#include "../axlib.h"
#include <cassert>

namespace axelynx
{
    AXELYNX_MEMALIGN(16) class AXELYNX_API mat2
	{
		public:
		float m[4];
		mat2()
		{
			m[0] = m[1] = m[2] = m[3] = 0;
		}

		mat2(float value)
		{
			m[0] = m[1] = m[2] = m[3] = value;
		}

		mat2(float m00, float m01, float m10, float m11)
		{
			m[0] = m00;
			m[1] = m01;
			m[2] = m10;
			m[3] = m11;
		}

		static mat2 Identity();

		float& FAST_CALL operator ()(short i, short j)
		{
			assert(i>=0 && i<2 && j>=0 && j<2);
			return m[i*2 + j];
		}

		const float& FAST_CALL operator ()(short i, short j) const
		{
			assert(i>=0 && i<2 && j>=0 && j<2);
			return m[i*2 + j];
		}

		int size(){return 2;}

		operator float*()
		{
			return m;
		}
		operator const float*() const
		{
			return m;
		}
	};
}
#endif
