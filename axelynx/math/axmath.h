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

	AXELYNX_API bool LineSphereIntersect(const vec3& base, const vec3& dir, const vec4& sphere);



	AXELYNX_API void CalcTriangleBasis( const axelynx::vec3& E, const axelynx::vec3& F, const axelynx::vec3& G, float sE,
			float tE, float sF, float tF, float sG, float tG, axelynx::vec3& tangentX,
			axelynx::vec3& tangentY );
	AXELYNX_API axelynx::vec3 ClosestPointOnLine( const axelynx::vec3& a, const axelynx::vec3& b, const axelynx::vec3& p );
	AXELYNX_API axelynx::vec3 Ortogonalize( const axelynx::vec3& v1, const axelynx::vec3& v2 );
}