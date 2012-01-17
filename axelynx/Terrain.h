#pragma once

#include "Geometry.h"

namespace axelynx
{
	class Terrain : public axelynx::Geometry
	{
	public:
		float GetHeight(float x, float z);
		float GridValue(int i, int j);
		int GetWidth();
		int GetHeight();
	};
}