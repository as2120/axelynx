#pragma once

#include "axelynx/RenderTarget.h"
#include "gl/axgl.h"

class CRenderTarget : public axelynx::RenderTarget
{
	axelynx::Texture* color_[16];
	axelynx::Texture* depth_;
	GLuint fbo_;
	
	bool binded_;
	static const CRenderTarget* current_;
	int width_, height_;

	mutable const CRenderTarget *restored_;

	mutable int old_width_,old_height_;
public:
	CRenderTarget(int width, int height);

	virtual bool Bind() const;
	virtual bool UnBind(bool restore = false) const;
	virtual bool Bind3DTexture(axelynx::Texture* tex, int texlayer, int rendertargetlayer = 0);
	virtual bool BindCubemap(axelynx::Texture* tex, int side, int rendertargetlayer = 0);
	virtual bool BindColorTexture(axelynx::Texture* tex, int layer = 0);
	virtual bool BindDepthTexture(axelynx::Texture* tex);

	virtual bool ClearTextures();
	virtual bool Clear() const;

	virtual int GetWidth() const;
	virtual int GetHeight() const;

	virtual axelynx::Texture* CreateColorTexture(int channels=3, int channel_size=1, int layer =0);
	virtual axelynx::Texture* CreateDepthTexture();

	virtual axelynx::Texture* GetColorTexture(int layer =0) const;
	virtual axelynx::Texture* GetDepthTexture() const;

	

	static const CRenderTarget * Current()
	{
		return current_;
	}
};