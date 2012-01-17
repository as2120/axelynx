#pragma once

#include "axelynx/Canvas.h"
#include "axelynx/math/vectormath.h"
#include "axelynx/Shader.h"

using namespace axelynx;

class CCanvas : public axelynx::Canvas
{
	int width_;
	int height_;

	float red_,green_,blue_,alpha_;

	static CCanvas *instance_;

	CCanvas();

	mat4 ortho_;
	vec2 position_;
	float angle_;
	vec2 scale_;

	vec4 color_;
	mutable mat3x2 current_transform_;

	Shader * sh_position;
	Shader * sh_positionuv;
	Shader * sh_positioncolor;
	Shader * sh_positioncoloruv;

	static Shader * current_shader_;
	axelynx::BlendMode current_bm_;

	mutable bool ortho_changed_;
	const axelynx::Font *font_;
public:
	virtual void Clear() const ;
	virtual void ClearColor(float red, float green, float blue, float alpha);
	virtual int GetHeight() const;
	virtual int GetWidth() const;

	virtual void SetColor(float red, float green, float blue);
	virtual axelynx::vec4 GetColor() const;
	virtual void SetAlpha(float alpha);

	virtual void Rect(float x, float y, float width, float height);

	void Recalc();
	static CCanvas* Create(int width, int height);
	static CCanvas* Instance()
	{
		if(!instance_)
			return Create(1,1);

		return instance_;
	}
	static void ResizeCanvas(int width, int height)
	{
		if(instance_)
		{
			instance_ -> width_ = width;
			instance_ -> height_ = height;
			instance_ -> Recalc();
		}
	}

	virtual int Draw(const axDrawable* shape) const;

	virtual mat3x2 GetTransform() const;

	virtual vec2 GetPosition() const;
	virtual float GetRotate() const;
	virtual vec2 GetScale() const;

	virtual void SetPosition(const vec2& position);
	virtual void SetRotate(float angle);
	virtual void SetScale(const vec2& scale);

	virtual axelynx::BlendMode CurrentBlendMode()
	{
		return current_bm_;
	}

	virtual void SetBlendMode(axelynx::BlendMode blend);
	virtual bool ApplyTransform(axelynx::Shader *shader) const;

	virtual void SetFont(const Font *fnt);
	virtual void Text(const wchar_t *text, bool xcenter = false);
	virtual void Text(const char *text, bool xcenter = false);

	virtual ~CCanvas()
	{
	}
};
