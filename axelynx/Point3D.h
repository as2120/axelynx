#pragma once

#include "math/vectormath.h"

namespace axelynx
{
	class Point3D
	{
	public:
		vec3 position_;
		quat orient_;

		Point3D(const vec3 &position)
			:position_(position_),orient_()
		{}
	};
}
