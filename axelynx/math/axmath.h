#pragma once

#include "../axlib.h"

#include "vectormath.h"

namespace axelynx
{
	AXELYNX_API int FAST_CALL rand(int max);
	AXELYNX_API int FAST_CALL rand(int min, int max);
	AXELYNX_API float FAST_CALL rnd(float max);
	AXELYNX_API float FAST_CALL rnd(float min, float max);

	AXELYNX_API void sincos(float a,float & s,float & c );

	AXELYNX_API bool FAST_CALL LineSphereIntersect(const vec3& base, const vec3& dir, const vec4& sphere);
}