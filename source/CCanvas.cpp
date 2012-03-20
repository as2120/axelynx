#undef __STRICT_ANSI__

#include "CCanvas.h"
#include "gl/axgl.h"
#include "CShader.h"

#include "axelynx/AxelynxSystemShaders.h"
#include "axelynx/Logger.h"
#include "axelynx/Font.h"

#include <cstdarg>

#include <iostream>

#include <cstdio>

CCanvas * CCanvas::instance_=0;
axelynx::Shader * CCanvas::current_shader_ =0;

void CCanvas::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void CCanvas::ClearColor(float red, float green, float blue, float alpha)
{
	glClearColor(red,green,blue,alpha);
}


int CCanvas::GetHeight() const
{
	return height_;
}

int CCanvas::GetWidth() const
{
	return width_;
}

CCanvas::CCanvas()
	:width_(0),height_(0),red_(1),green_(1),blue_(1),alpha_(1),ortho_changed_(true)
{
	sh_positioncolor = axelynx::StandartShaders::Draw::PositionColor();
	position_ = vec2(0,0);
	angle_ = 0;
	scale_ = vec2(1,1);
	current_transform_ = mat3x2::FromTransform(position_,angle_,scale_);
	font_ = 0;
	SetBlendMode(axelynx::BM_ALPHA);
}

CCanvas* CCanvas::Create(int width, int height)
{
	if(!instance_)
		instance_ = new CCanvas();

	instance_->width_ = width;
	instance_->height_ = height;
	instance_->Recalc();

	return instance_;
}

void CCanvas::SetColor(float red, float green, float blue)
{
	red_ = red;
	green_ = green;
	blue_ = blue;
}
void CCanvas::SetAlpha(float alpha)
{
	alpha_ = alpha;
}

void CCanvas::Rect(float x, float y, float width, float height)
{

}

void CCanvas::Recalc()
{
	ortho_ = axelynx::mat4::Ortho(0,static_cast<float>(width_),0,static_cast<float>(height_),-1,1);
	glViewport(0,0,width_, height_);
	ortho_changed_ = true;
}

bool CCanvas::ApplyTransform(axelynx::Shader *sh) const
{
	sh->SetUniform("projection",ortho_);

	axelynx::mat3x2::FromTransform(current_transform_,position_,angle_,scale_);
	sh->SetUniform("transform",current_transform_);

	sh->SetUniform("canvascolor",vec4(red_,green_,blue_,alpha_));
	return true;
}

int CCanvas::Draw(const axDrawable* shape) const
{
	if(!CShader::Current())
	{
		//std::cout<<"no shader"<<std::endl;
		sh_positioncolor->Bind();
		if(!CShader::Current())
		{
			return -1;
		}

	}

	shape->Draw(this);

	return 1;
}

mat3x2 CCanvas::GetTransform() const
{
	axelynx::mat3x2::FromTransform(current_transform_,position_,angle_,scale_);
	return current_transform_;
}

vec2 CCanvas::GetPosition() const
{
	return position_;
}

float CCanvas::GetRotate() const
{
	return angle_;
}

vec2 CCanvas::GetScale() const
{
	return scale_;
}

void CCanvas::SetPosition(const vec2& position)
{
	position_ = position;
}

void CCanvas::SetRotate(float angle)
{
	angle_ = angle;
}

void CCanvas::SetScale(const vec2& scale)
{
	scale_ = scale;
}

void CCanvas::SetBlendMode(axelynx::BlendMode blend)
{
	current_bm_ = blend;

	switch(current_bm_)
	{
	case axelynx::BM_NONE:
		glDisable(GL_BLEND);
		break;
	case axelynx::BM_ALPHA:
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		break;
	case axelynx::BM_MULTIPLY:
		glEnable(GL_BLEND);
		glBlendFunc(GL_DST_COLOR,GL_NONE);
		break;
	case axelynx::BM_ADD:
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		break;
	}
}

void CCanvas::SetFont(const Font *fnt)
{
	font_ = fnt;
}

void CCanvas::Text(const wchar_t *text, bool xcenter)
{
	font_->Draw(this,text,xcenter);
}

void CCanvas::Text(const char *text, bool xcenter)
{
	font_->Draw(this,text,xcenter);
}

axelynx::vec4 CCanvas::GetColor() const
{
	return axelynx::vec4(red_,green_,blue_,alpha_);
}

void CCanvas::Print(const wchar_t *format,...)
{
	if(!font_)
	{
		#ifdef _DEBUG
				LOG_ERROR(L"font not binding to canvas!");
		#endif
		return;
	}

	va_list va;
	va_start(va,format);
	wchar_t buffer[1024];

	vswprintf(buffer,format,va);
	va_end(va);
	font_->Draw(this,buffer);
}

void CCanvas::Print(const char *format,...)
{
	if(!font_)
	{
		#ifdef _DEBUG
				LOG_ERROR(L"font not binding to canvas!");
		#endif
		return;
	}

	va_list va;
	va_start(va,format);
	char buffer[1024];
	vsprintf(buffer,format,va);
	va_end(va);
	font_->Draw(this,buffer);
}
