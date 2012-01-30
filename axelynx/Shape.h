#pragma once
#ifndef AXELYNX_SHAPE_H
#define AXELYNX_SHAPE_H

#include "math/vectormath.h"
#include "axlib.h"
#include "Drawable.h"

namespace axelynx
{
	class Shape : public axDrawable
	{
	public:
		virtual void Draw( const axelynx::Canvas *canvas) const = 0;

		virtual int TrianglesCount() const =0;
		virtual int VerticesCount() const = 0;

		virtual vec2 GetPosition(int index) const = 0;
		virtual vec4 GetColor(int index) const = 0;
		virtual vec2 GetTexCoords(int index) const = 0;

		virtual int GetTrisIndex(int tris, int corner) const=0;

		virtual void SetPosition(int index, const vec2& position) = 0;
		virtual void SetColor(int index, const vec4& color) = 0;
		virtual void SetTexCoords(int index, const vec2& uv) = 0;

		virtual void SetTrisIndex(int tris, int corner, int vertexindex)=0;
		virtual void SetTrisIndex(int tris, int corner0, int corner1, int corner2)=0;

		static AXELYNX_API Shape* Rect(int width, int height, bool centred = true);

		virtual ~Shape(){};
	};
}

#endif
