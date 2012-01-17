#include <axelynx/math/vec2.h>

AXELYNX_API axelynx::vec2 axelynx::vec2::Identity()
{
	axelynx::vec2 v;

	v.x = v.y = 1.0f;
	return v;
}