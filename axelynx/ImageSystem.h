#pragma once

#include "axlib.h"

#include "Image.h"
#include "Drawable.h"

namespace axelynx
{
	class ImageSystem : public axDrawable
	{
	public:
		virtual Image * GetImage()=0;
		virtual Image * GetImage(const vec2& corner0, const vec2& corner1)=0;

		virtual void Draw(const axelynx::Canvas *canvas) const =0;
		virtual ~ImageSystem(){};
	};
}