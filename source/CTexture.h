#pragma once

#include "axelynx/Texture.h"
#include "axelynx/PixMap.h"

#include "gl/axgl.h"

class CTexture : public axelynx::Texture
{
	GLuint handle_;

	static CTexture * current_[32];
	static CTexture *fake_;

	TextureType axelynx_type_;
	GLuint gl_type_;

	bool Init();

	mutable int current_layer_;
	int width_,height_,depth_;
	int cpp_; // channels per pixel
	int bpc_; // bytes per channels

	int compressed_;
	bool use_mipmaps_;

	float anisotropic_;

	mutable CTexture *restored_;
	mutable int restored_layer_;

	CTexture *inner_tex_;

	void init(axelynx::Texture::Desc &desc);
public:
	CTexture(axelynx::Texture::Desc &desc);
	CTexture(axelynx::File file);
	CTexture(GLuint handele,axelynx::Texture::Desc &desc);

	virtual void Build(const void* data,GLuint format,GLuint internalFormat,const void *data1=0,const void *data2=0,const void *data3=0,const void *data4=0,const void *data5=0);

	virtual int GetWidth() const;
	virtual int GetHeight() const;
	virtual int GetDepth() const;

	virtual TextureType GetType() const;
		
	virtual bool Bind(int layer = 0);
	virtual bool UnBind(bool restore = false);

	GLuint GetHandle()
	{
		if(inner_tex_)
			return inner_tex_->handle_;

		return handle_;
	}

	GLuint GetGLTexType()
	{
		return gl_type_;
	}

	static CTexture * Current(int layer = 0)
	{
		return current_[layer];
	}

	static CTexture * FromPixMap(const axelynx::PixMap *pm,axelynx::Texture::Desc desc = axelynx::Texture::Desc());

	bool SetInnerTex(CTexture *inner);
	CTexture* GetInnerTex();
	static CTexture *GetFake();

	virtual bool Save(axelynx::File file) const;

	virtual bool Grab(int x, int y);
};


static void GetGLTextures(GLenum &format, GLenum &internalformat, int channels, int channel_size, bool srgb = false)
{

	switch(channels)
	{
	case 1:
		format = GL_RED;

		switch(channel_size)
		{
		case 1:
			 internalformat = GL_R8;break;
		case 2:
			internalformat = GL_R16F;break;
		case 4:
			internalformat = GL_R32F;break;
		};
		break;
	case 2:
		format = GL_RG;

		switch(channel_size)
		{
		case 1:
			 internalformat = GL_RG8;break;
		case 2:
			internalformat = GL_RG16F;break;
		case 3:
			internalformat = GL_RG32F;break;
		};
		break;
	case 3:
		format = srgb?GL_SRGB:GL_RGB;

		switch(channel_size)
		{
		case 1:
				internalformat = srgb?GL_SRGB8:GL_RGB8;break;
		case 2:
			internalformat = GL_RGB16F;break; 
		case 4:
			internalformat = GL_RGB32F;break;
		};
		break;
	case 4:
		format = srgb?GL_SRGB_ALPHA:GL_RGBA;
		switch(channel_size)
		{
		case 1:
			 internalformat = srgb?GL_SRGB8_ALPHA8:GL_RGBA8;break;
		case 2:
			internalformat = GL_RGBA16F;break;
		case 4:
			internalformat = GL_RGBA32F;break;
		};
		break;
	default:
		format = GL_RGBA;
	};
}