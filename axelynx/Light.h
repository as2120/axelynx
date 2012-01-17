#pragma once

#include "Pivot.h"

namespace axelynx
{
	class Light : public axelynx::Pivot
	{
	public:
		enum Mode
		{
			LM_SPOT,
			LM_DIR,
			LM_CONE
		};

		virtual void Color(float r, float g, float b)=0;
		virtual void SetMode(const Mode &m)=0;
		virtual void SetDistance(float distance)=0;
		virtual void SetAngle(float angle)=0;

		virtual ~Light(){};
	};
}