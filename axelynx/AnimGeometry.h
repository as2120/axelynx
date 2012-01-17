#pragma once

#include "Geometry.h"

namespace axelynx
{
	class AnimGeometry : public Geometry
	{
	public:
		virtual bool Draw(float frame) const =0;

		virtual bool Draw()
		{
			return Draw(0.0f);
		}

		virtual ~AnimGeometry(){};

		virtual std::wstring GetClass()
		{
			return L"animunknow";
		}
	};
}