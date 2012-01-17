#include <axelynx/math/mat4x3.h>

AXELYNX_API axelynx::mat4x3 axelynx::mat4x3::Identity()
{
	axelynx::mat4x3 m;

	m.m[1] = m.m[2] = m.m[3] = m.m[5] = m.m[6] = m.m[7] = m.m[8] = 0;
	m.m[0] = m.m[4] = m.m[8] = 1;

	return m;
}