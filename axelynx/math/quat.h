#pragma once
#ifndef AXELYNX_QUAT_H
#define AXELYNX_QUAT_H
#include "../axlib.h"
#include "mat3.h"
//#include "mat4.h"
#include "vec3.h"

namespace axelynx
{
	class AXELYNX_API quat
	{
		public:
		float x,y,z,w;

		static quat Identity();
		quat();
		quat(const vec3& angles);
		quat(const vec3& axis, float angle);
		quat(const mat3& rotatematrix);

		mat3 ToMat3() const;
		quat operator*(quat other) const;
		quat operator*(float other) const;
		const quat& operator *=(const quat& other);

		quat slerp(const quat& other, float t) const;
		vec3 ztransform(float value) const;
		//AXELYNX_API void ToTransformMatrix(mat4& matrix);
	};
}
#endif