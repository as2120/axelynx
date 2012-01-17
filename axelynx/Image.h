#pragma once

#include "axlib.h"

#include "math/vec2.h"

namespace axelynx
{
	class ImageSystem;

	class Image
	{
	public:
		virtual void Draw() const=0;

		virtual void Position(const vec2& position)=0;
		virtual void Scale(const vec2& position)=0;
		virtual void Rotate(float angle)=0;

		virtual void Region(const vec2& corner0, const vec2& corner1)=0;

		virtual ~Image(){};
	};
}