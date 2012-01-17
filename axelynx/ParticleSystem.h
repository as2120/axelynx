#pragma once

#include "Pivot.h"
#include "AnimGeometry.h"

namespace axelynx
{
	class ParticleSystem : public AnimGeometry
	{
	public:
		enum DrawMode
		{
			DM_SPRITE
			,DM_BILLBOARD
		};

		enum UpdateMode
		{
			UM_AUTO
			,UM_CPU
			,UM_GPU
		};

		virtual int SetDrawMode(DrawMode) = 0;
		virtual int SetUpdateMode(DrawMode) = 0;

		virtual ~ParticleSystem(){};
	};
}