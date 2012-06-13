#pragma once
#ifndef AXELYNX_ANIMENTITY_H
#define AXELYNX_ANIMENTITY_H
#include "AnimGeometry.h"
#include "Entity.h"

namespace axelynx
{
	class AnimEntity : public virtual Entity
	{
	public:
		virtual void SetSpeedFactor(float speedfactor) = 0;

		virtual void SetFrame(float frame) = 0;

		virtual const AnimGeometry *GetGeometry() const =0;
		~AnimEntity()
		{
		}


	};
}
#endif