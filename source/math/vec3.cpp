#include <axelynx/math/vec3.h>
#include <cmath>

AXELYNX_API axelynx::vec3 axelynx::vec3::Identity()
{
	axelynx::vec3 v;

	v.x = v.y = v.z = 1.0f;
	return v;
}

float axelynx::vec3::length() const
{
	return sqrtf(x*x + y*y + z*z);
}

float axelynx::vec3::sqlength() const
{
	return x*x + y*y + z*z;
}

axelynx::vec3 axelynx::vec3::normalize()
{
	float nval = length();
	if(nval>0)
	{
		nval = 1.0f / nval;

		x *= nval;
		y *= nval;
		z *= nval;
	}

	return axelynx::vec3(x,y,z);
}