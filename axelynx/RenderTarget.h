#pragma once

#include "axlib.h"
#include "Texture.h"
#include "Canvas.h"

namespace axelynx
{
	class RenderTarget
	{
	public:
		virtual bool Bind() const =0;
		virtual bool UnBind(bool restore = false) const =0;

		virtual bool Bind3DTexture(Texture* tex, int texlayer, int rendertargetlayer = 0)=0;
		virtual bool BindCubemap(Texture* tex, int side, int rendertargetlayer = 0)=0;
		virtual bool BindColorTexture(Texture* tex, int layer = 0)=0;
		virtual bool BindDepthTexture(Texture* tex)=0;

		virtual bool ClearTextures()=0;
		virtual bool Clear() const =0;

		virtual int GetWidth() const =0;
		virtual int GetHeight() const =0;

		virtual Texture* CreateColorTexture(int channels=3, int channel_size=1, int layer =0,bool use_mipmaps=false)=0;
		virtual Texture* CreateDepthTexture()=0;

		virtual Texture* GetColorTexture(int layer =0) const =0;
		virtual Texture* GetDepthTexture() const =0;

		virtual ~RenderTarget(){};
	};
}