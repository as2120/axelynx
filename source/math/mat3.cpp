#include <axelynx/math/mat3.h>

AXELYNX_API axelynx::mat3 axelynx::mat3::Identity()
{
	axelynx::mat3 m;

	m.m[1] = m.m[2] = m.m[3] = m.m[5] = m.m[6] = m.m[7] = 0;
	m.m[0] = m.m[4] = m.m[8] = 1;

	return m;
}

axelynx::vec3 axelynx::mat3::operator*(const axelynx::vec3& v) const
{
	axelynx::vec3 result;

	result.x = m[0] * v.x + m[1] * v.y + m[2] * v.z;
	result.y = m[3] * v.x + m[4] * v.y + m[5] * v.z;
	result.z = m[6] * v.x + m[7] * v.y + m[8] * v.z;

	return result;
}

axelynx::vec3 axelynx::mat3::ztransform(float zvalue)
{
	axelynx::vec3 result;

	result.x = m[2] * zvalue;
	result.y = m[5] * zvalue;
	result.z = m[8] * zvalue;

	return result;
}