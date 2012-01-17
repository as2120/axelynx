#pragma once


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
