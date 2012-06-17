

#include "CPostEffect.h"
#include "CRenderTarget.h"
#include "CCanvas.h"

CPostEffect * CPostEffect::current_=0;

CPostEffect::CPostEffect(int width, int height, int channels, int channelsize)
{
	RTT_ = new CRenderTarget(width,height);
	depth_texture_ = RTT_->CreateDepthTexture();
	texture_ = RTT_->CreateColorTexture(channels,channelsize);

	width_ = width;
	height_ = height;
	quad_ = 0;
	binded_ = false;
	restored_ = 0;
}

bool CPostEffect::Bind() const
{			
	restored_ = 0;
	RTT_->Bind();
	binded_ = true;
	return true;
}

bool CPostEffect::UnBind(bool restore) const
{
	if(restore)
		if(restored_)
			restored_->Bind();

	RTT_->UnBind();
	binded_ = false;
	return true;
}

void CPostEffect::Draw(const axelynx::Canvas *canvas) const
{
	if(!shader_)
		return;

	if(binded_)
		UnBind();

	if(effects_.empty())
	{
		shader_->Bind();
		shader_->SetUniform("effectcolor",color_);
		shader_->SetUniform("texture0",0);
		shader_->SetUniform("texture1",1);
		const axelynx::Canvas *c = (canvas)?canvas:CCanvas::Instance();
		texture_->Bind(0);
		depth_texture_->Bind(1);
		quad_->Draw(c);
		texture_->UnBind();
		depth_texture_->UnBind();
		shader_->UnBind();
	}
	else
	{
		Effects::const_iterator ci = effects_.begin();
		Effects::const_iterator ei = effects_.end();

		const axelynx::PostEffect *cureffect = this;

		for(;ci!=ei;++ci)
		{
			(*ci)->Bind();
			cureffect->Draw();
			(*ci)->UnBind();
			cureffect = (*ci);
		}

		cureffect->Draw();
	}
}

PostEffect* CPostEffect::Add(PostEffect *afx)
{
	effects_.push_back(afx);
	return this;
}

PostEffect* CPostEffect::Remove(PostEffect *afx)
{
	effects_.remove(afx);
	return this;
}

CPostEffect::~CPostEffect()
{
	delete RTT_;
	delete shader_;
}

bool CPostEffect::BindShader(axelynx::Shader *shader)
{
	shader_ = shader;
	shader_->Bind();

	if(!quad_)
		quad_ = axelynx::Shape2D::Rect(width_, height_,false);

	shader_->UnBind();
	return shader_->Valid();
}