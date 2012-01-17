#include <axelynx/math/mat2.h>

AXELYNX_API axelynx::mat2 axelynx::mat2::Identity()
{
	axelynx::mat2 m;

	m.m[1] = m.m[2] = 0;
	m.m[0] = m.m[3] = 1;

	return m;
}