#pragma once

#pragma once

#include "axlib.h"

#include "Shape.h"

namespace axelynx
{
	class StandartShapes
	{
	public:
		static AXELYNX_API Shape * Box(float width,float height,float depth);		
		static AXELYNX_API Shape * Sphere(float radius = 1);		
	};
}