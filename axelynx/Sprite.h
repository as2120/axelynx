#pragma once

#include "Geometry.h"

namespace axelynx
{
	class Sprite : public Geometry
	{
	public:
		virtual bool Draw(void *additionalinfo) const=0;

		virtual int CountTriangles()const =0;
		virtual int CountVertices() const =0;

		virtual ~Sprite(){};
	};
}