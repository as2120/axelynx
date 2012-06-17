#pragma once

#include "axelynx/PostEffect.h"
#include "axelynx/RenderTarget.h"
#include "axelynx/Shape2D.h"
#include <list>

class CPostEffect : public axelynx::PostEffect
{
	axelynx::Shader *shader_;
	axelynx::Texture *texture_;
	axelynx::Texture *depth_texture_;
	axelynx::RenderTarget *RTT_;
	axelynx::vec4 color_;
	axelynx::Shape2D *quad_;
	mutable bool binded_;

	typedef std::list<axelynx::PostEffect*> Effects;
	Effects effects_;

	int width_, height_;

	mutable CPostEffect *restored_;
	static CPostEffect *current_;
public:
	CPostEffect(int width, int height, int channels = 3, int channelsize = 1);

	virtual bool SetColor(const axelynx::vec4& color)
	{
		color_ = color;
		return true;
	}

	virtual bool SetColor(float red, float green, float blue, float alpha = 1.0f)
	{
		color_ = axelynx::vec4(red,green,blue,alpha);
		return true;
	}

	virtual bool BindShader(axelynx::Shader *shader);
	virtual axelynx::Shader* GetShader() const {return shader_;}

	virtual bool Bind() const;
	virtual bool UnBind(bool restore = false) const;

	virtual void Draw(const axelynx::Canvas *canvas = 0) const;

	virtual PostEffect* Add(PostEffect *afx);
	virtual PostEffect* Remove(PostEffect *afx);

	virtual ~CPostEffect();
};