

#include "CRenderTarget.h"
#include "axelynx/Logger.h"
#include "CTexture.h"
#include "CCanvas.h"

const CRenderTarget * CRenderTarget::current_ = 0;

CRenderTarget::CRenderTarget(int width, int height)
{
	for(int i=0;i<16;++i)
	{
		color_[i] = 0;
	}
	depth_ = 0;
	fbo_=0;
	
	binded_ = false;
	width_ = width;
	height_ = height;

// Framebuffer Object (FBO) для рендера в него буфера глубины
	glGenFramebuffers(1, &fbo_); // делаем созданный FBO текущим
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
	// отключаем вывод цвета в текущий FBO
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE); 
}

bool CRenderTarget::Bind() const
{
	old_width_ = CCanvas::Instance()->GetWidth();
	old_height_ = CCanvas::Instance()->GetHeight();

	CCanvas::ResizeCanvas(width_,height_);

	// проверим текущий FBO на корректность
	GLenum fboStatus;  // создаем FBO для рендера глубины в текстуру
	if ((fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR(L"glCheckFramebufferStatus error");
	}  // возвращаем FBO по-умолчанию glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if(current_ != this)
	{
		restored_ = current_;
		glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
	}

	current_ = this;

	return true;
}

bool CRenderTarget::UnBind(bool restore) const
{
	CCanvas::ResizeCanvas(old_width_,old_height_);
	if(restore)
		if(restored_)
			restored_->Bind();

	if(current_)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	current_ = 0;

	return true;
}

bool CRenderTarget::BindColorTexture(axelynx::Texture* tex, int layer)
{
	// указываем для текущего FBO текстуру, куда следует производить рендер глубины
	CTexture *ctex = dynamic_cast<CTexture*>(tex);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_); 
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + layer, ctex->GetHandle(), 0);
	//glEnable(GL_FRAMEBUFFER_SRGB);
	if(current_)
		current_->Bind();
	else
		glBindFramebuffer(GL_FRAMEBUFFER, 0); 
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}

bool CRenderTarget::Bind3DTexture(axelynx::Texture* tex, int texlayer, int rendertargetlayer)
{
	// указываем для текущего FBO текстуру, куда следует производить рендер глубины
	CTexture *ctex = dynamic_cast<CTexture*>(tex);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_); 
	glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + rendertargetlayer, GL_TEXTURE_3D, ctex->GetHandle(), 0, texlayer);

	if(current_)
		current_->Bind();
	else
		glBindFramebuffer(GL_FRAMEBUFFER, 0); 

	OPENGL_CHECK_FOR_ERRORS();
	return true;
}

bool CRenderTarget::BindCubemap(axelynx::Texture* tex, int side, int rendertargetlayer)
{
	// указываем для текущего FBO текстуру, куда следует производить рендер глубины
	CTexture *ctex = dynamic_cast<CTexture*>(tex);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + rendertargetlayer, GL_TEXTURE_CUBE_MAP_POSITIVE_X + side, ctex->GetHandle(), 0);
	
	if(current_)
		current_->Bind();
	else
		glBindFramebuffer(GL_FRAMEBUFFER, 0); 

	OPENGL_CHECK_FOR_ERRORS();
	return true;
}

bool CRenderTarget::BindDepthTexture(axelynx::Texture* tex)
{
	// указываем для текущего FBO текстуру, куда следует производить рендер глубины
	CTexture *ctex = dynamic_cast<CTexture*>(tex);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, ctex->GetHandle(), 0);

	if(current_)
		current_->Bind();
	else
		glBindFramebuffer(GL_FRAMEBUFFER, 0); 
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}

bool CRenderTarget::ClearTextures()
{

	return false;
}

bool CRenderTarget::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return false;
}

int CRenderTarget::GetWidth() const
{
	return width_;
}

int CRenderTarget::GetHeight() const
{
	return height_;
}

axelynx::Texture* CRenderTarget::CreateColorTexture(int channels, int channel_size, int layer,bool use_mipmaps)
{
	axelynx::Texture::Desc desc;
	desc.TT = axelynx::Texture::TT_2D;
	desc.width = width_;
	desc.height = height_;
	desc.cpp = channels;
	desc.bpc = channel_size;
	desc.use_mipmaps = use_mipmaps;

	CTexture *tex = new CTexture(desc);

	GLenum format;
	GLenum internalformat;
	GetGLTextures(format,internalformat,channels,channel_size);
	tex->Build(0,format,internalformat);

	BindColorTexture(tex,layer);

	return tex;
}

axelynx::Texture* CRenderTarget::CreateDepthTexture()
{
	axelynx::Texture::Desc desc;
	desc.TT = axelynx::Texture::TT_DEPTH;
	desc.width = width_;
	desc.height = height_;
	desc.cpp = 1;
	desc.bpc = 4;
	desc.use_mipmaps = false;

	CTexture *tex = new CTexture(desc);

	//tex->Bind();
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	//tex->UnBind();
	tex->Build(0,GL_DEPTH_COMPONENT,GL_DEPTH_COMPONENT32);

	BindDepthTexture(tex);

	return tex;
}

axelynx::Texture* CRenderTarget::GetColorTexture(int layer) const
{
	return 0;
}

axelynx::Texture* CRenderTarget::GetDepthTexture() const
{
	return 0;
}