#include <axelynx/math/mat3x2.h>
#include <cmath>

namespace axelynx
{
	AXELYNX_API void sincos(float a,float & s,float & c );
}

AXELYNX_API axelynx::mat3x2 axelynx::mat3x2::Identity()
{
	axelynx::mat3x2 m;

	m.m[1] = m.m[2] = 0;
	m.m[0] = m.m[3] = 1;
	m.m[4] = m.m[5] = 0;

	return m;
}

AXELYNX_API void axelynx::mat3x2::FromTransform(axelynx::mat3x2& result,const axelynx::vec2& position, float angle, const axelynx::vec2& scale)
{
	result[4] = position.x;
	result[5] = position.y;

	float sinx,cosx;

	axelynx::sincos(angle,sinx,cosx);
	result[0] = cosx * scale.x;
	result[1] = -sinx * scale.x;
	result[2] = sinx * scale.y;
	result[3] = cosx * scale.y;
}

AXELYNX_API axelynx::mat3x2 axelynx::mat3x2::FromTransform(const axelynx::vec2& position, float angle, const axelynx::vec2& scale)
{
	axelynx::mat3x2 m;

	m.m[4] = position.x;
	m.m[5] = position.y;

	float sinx,cosx;

	axelynx::sincos(angle * 3.1415f / 180.0f,sinx,cosx);
	m.m[0] = cosx * scale.x;
	m.m[1] = -sinx * scale.x;
	m.m[2] = sinx * scale.y;
	m.m[3] = cosx * scale.y;

	return m;
}

AXELYNX_API axelynx::mat4 operator * (const axelynx::mat4& other)
{
	return axelynx::mat4::Identity();
}