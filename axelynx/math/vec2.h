#pragma once

#include "../axlib.h"

namespace axelynx
{
	class AXELYNX_API vec2
	{
		public:

		float x,y;

		static vec2 Identity();

		operator float*()
		{
			return reinterpret_cast<float*>(this);
		}

		operator const float*() const
		{
			return reinterpret_cast<const float*>(this);
		}

		vec2()
		:x(0),y(0){}

		vec2(float value)
			:x(value),y(value){}

		vec2(float x_, float y_)
			:x(x_),y(y_){}

		const vec2& operator += (float value)
		{
			x+=value; y+= value;
			return *this;
		}

		const vec2& operator += (const vec2& vector)
		{
			x+=vector.x; y+= vector.y;
			return *this;
		}

		vec2 operator-() const	{return vec2(-x,-y);}
	};


}