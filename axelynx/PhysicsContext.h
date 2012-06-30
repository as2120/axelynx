#pragma once
#ifndef AXELYNX_PHYSICSCONTEXT_H
#define AXELYNX_PHYSICSCONTEXT_H

#include "File.h"
#include <string>

#include "Shape.h"


#include "math/vec3.h"

namespace axelynx
{
	class PhysicsContext
	{
	public:

		virtual Shape* AddInfinityPlane() = 0;
		virtual Shape* AddSphere(float radius) = 0;
		virtual Shape* AddBox(const vec3& dimesions) = 0;

		virtual ~PhysicsContext()
		{
		}


	};
}
#endif