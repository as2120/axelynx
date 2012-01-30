#pragma once
#ifndef AXELYNX_AXDRAWABLE_H
#define AXELYNX_AXDRAWABLE_H

namespace axelynx
{
	class Canvas;

	class axDrawable
	{
	public:
		virtual void Draw(const axelynx::Canvas *canvas) const =0;
		virtual ~axDrawable(){};
	};
}

#endif
