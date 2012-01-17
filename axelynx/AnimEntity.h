#pragma once

#include "AnimGeometry.h"
#include "Entity.h"

namespace axelynx
{
	class AnimEntity : public virtual Entity
	{
	public:
		virtual void SetSpeedFactor(float speedfactor) = 0;

		virtual const AnimGeometry *GetGeometry() const =0;
		~AnimEntity()
		{
		}


	};
}