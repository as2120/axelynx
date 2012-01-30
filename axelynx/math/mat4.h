#pragma once
#ifndef AXELYNX_MAT4_H
#define AXELYNX_MAT4_H

#include "../axlib.h"
#include <cassert>

#include "mat3.h"
#include "vec3.h"
#include "quat.h"

namespace axelynx
{
    AXELYNX_MEMALIGN(16) class AXELYNX_API mat4
	{
		public:
			
		mat4()
			:m0(0),m1(0),m2(0),m3(0),m4(0),m5(0),m6(0),m7(0),m8(0),m9(0),m10(0),m11(0),m12(0),m13(0),m14(0),m15(0)
		{
		}
		mat4(float _m0, float _m1, float _m2, float _m3
			,float _m4, float _m5, float _m6, float _m7
			,float _m8, float _m9, float _m10,float _m11
			,float _m12,float _m13,float _m14,float _m15)
			:m0(_m0),m1(_m1),m2(_m2),m3(_m3),m4(_m4),m5(_m5),m6(_m6),m7(_m7),m8(_m8),m9(_m9),m10(_m10),m11(_m11),m12(_m12),m13(_m13),m14(_m14),m15(_m15)
		{
		}

		union
		{
			float m[16];
			struct
			{
				float m0,m1,m2,m3,m4,m5,m6,m7,m8,m9,m10,m11,m12,m13,m14,m15;
			};
		};

		static mat4 Identity();

		const float& operator [](int i) const
		{
			assert(i>=0 && i<16);

			return m[i];
		}

		float& operator [](int i)
		{
			assert(i>=0 && i<16);

			return m[i];
		}

		const float& operator ()(short i, short j) const
		{
			assert(i>=0 && i<4 && j>=0 && j<4);

			return m[i*4 + j];
		}

		float& operator ()(short i, short j)
		{
			assert(i>=0 && i<4 && j>=0 && j<4);

			return m[i*4 + j];
		}

		int size(){return 4;}

		static mat4 FAST_CALL Make(const mat3& orient, const vec3& position);
		static mat4 FAST_CALL Make(const mat3& orient, const vec3& position, const vec3& scale);
		static mat4 FAST_CALL Make(const mat3& orient, const vec3& position, float scale);

		static mat4 FAST_CALL Make(const quat& orient, const vec3& position);
		static mat4 FAST_CALL Make(const quat& orient, const vec3& position, const vec3& scale);
		static mat4 FAST_CALL Make(const quat& orient, const vec3& position, float scale);

		static void FAST_CALL Make(axelynx::mat4& into,const axelynx::quat& orient, const axelynx::vec3& position, const axelynx::vec3& scale);

		operator float*()
		{
			return m;
		}

		operator const float*() const
		{
			return m;
		}

		static mat4 FAST_CALL Perspective(float fov, float aspect, float znear, float zfar);
		static mat4 FAST_CALL Ortho(float left, float right, float top, float bottom, float znear, float zfar);

		mat4 FAST_CALL operator*(const mat4& A) const;
		mat4 FAST_CALL operator*=(const mat4& A);

		mat3 ToMat3() const
		{
			return mat3(m[0],m[1],m[2],m[4],m[5],m[6],m[8],m[9],m[10]);
		}
	};
}
#endif
