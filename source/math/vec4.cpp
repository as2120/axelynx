#include <axelynx/math/vec4.h>

AXELYNX_API axelynx::vec4 axelynx::vec4::Identity()
{
	axelynx::vec4 v;

	v.x = v.y = v.z = v.w = 1.0f;
	return v;
}