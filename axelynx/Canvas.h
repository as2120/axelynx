#pragma once

#include "Shape.h"
#include "Drawable.h"
#include "math/mat3x2.h"
#include "Shader.h"

namespace axelynx
{
	class Font;

	enum BlendMode
	{
		BM_NONE
		,BM_ALPHA
		,BM_MULTIPLY
		,BM_ADD
	};

	class Canvas
	{
		public:

		virtual void Clear() const =0;
		virtual void ClearColor(float red, float green, float blue, float alpha)=0;

		virtual void SetColor(float red, float green, float blue)=0;
		virtual vec4 GetColor() const =0;
		virtual void SetAlpha(float alpha)=0;

		virtual void Rect(float x, float y, float width, float height)=0;

		virtual int GetHeight() const =0;
		virtual int GetWidth() const =0;

		virtual int Draw(const axDrawable* some) const =0;

		virtual mat3x2 GetTransform() const = 0;

		virtual vec2 GetPosition() const =0;
		virtual float GetRotate() const =0;
		virtual vec2 GetScale() const =0;

		virtual void SetPosition(const vec2& position)=0;
		void SetPosition(float x, float y)
		{
			SetPosition(vec2(x,y));
		}
		virtual void SetRotate(float angle)=0;
		virtual void SetScale(const vec2& scale)=0;
		void SetScale(float x, float y)
		{
			SetScale(vec2(x,y));
		}
		void SetScale(float value)
		{
			SetScale(vec2(value));
		}
		virtual BlendMode CurrentBlendMode()=0;
		virtual void SetBlendMode(BlendMode blend)=0;

		virtual bool ApplyTransform(axelynx::Shader *shader) const=0;
		
		virtual void SetFont(const Font *fnt) = 0;

		virtual void Text(const wchar_t *text, bool xcenter = false)=0;
		virtual void Text(const char *text, bool xcenter = false)=0;

		virtual void Text(vec2 pos,const wchar_t *text, bool xcenter = false)
		{
			vec2 oldpos = GetPosition();
			SetPosition(pos);
			Text(text,xcenter);
			SetPosition(oldpos);
		}

		virtual void Text(vec2 pos,const char *text, bool xcenter = false)
		{
			vec2 oldpos = GetPosition();
			SetPosition(pos);
			Text(text,xcenter);
			SetPosition(oldpos);
		}


		virtual void Text(float x, float y,const wchar_t *text, bool xcenter = false)
		{
			Text(vec2(x,y),text,xcenter);
		}

		virtual void Text(float x, float y,const char *text, bool xcenter = false)
		{
			Text(vec2(x,y),text,xcenter);
		}

		virtual ~Canvas(){};
	};
}
