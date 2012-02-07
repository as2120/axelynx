

#include "CMaterial.h"
#include "CCanvas.h"
#include "gl/axgl.h"
#include <iostream>
#include "CStatistics.h"
#include "CEarlyZ.h"

CMaterial const *CMaterial::current_ =0;

CMaterial::CMaterial()
{
	shader_ = 0;
	zerlypass_shader_ = 0;

	for(int i=0;i<8;++i)
		textures_[i] = 0;

	blend_ =axelynx::BM_NONE;
	using_entity_textures_  =false;

	dw_mode_ = axelynx::DW_AUTO;
	dt_mode_ = axelynx::DT_AUTO;
	c_mode_ = axelynx::CM_BACK;
}

void CMaterial::SetShader(axelynx::Shader *sh)
{
	shader_ =sh;
}

void CMaterial::SetTexture(axelynx::Texture *tex, int layer)
{
	textures_[layer] = tex;
}

void CMaterial::SetBlend(axelynx::BlendMode bm)
{
	blend_ = bm;
}

axelynx::BlendMode CMaterial::GetBlendMode() const
{
	return blend_;
}

void CMaterial::Bind(const axelynx::Camera *cam) const
{
	if(current_ == this)
	{
		return;
	}

	if(!shader_) //shader nai
		return ;

	switch(c_mode_)
	{
	case axelynx::CM_BACK:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		break;
	case axelynx::CM_FRONT:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		break;
	case axelynx::CM_DISABLE:
		glDisable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		break;
	};
	CStatistics::materials++;

	axelynx::Shader *shader = (zerlypass_shader_ && CEarlyZ::OnEarlyZPass())?zerlypass_shader_:shader_;

	shader->Bind();
	modellocation_ = shader->GetStandartUniformLocation(axelynx::Shader::SU_MODEL);
	modelviewlocation_ = shader->GetStandartUniformLocation(axelynx::Shader::SU_MODELVIEW);
	modelviewprojlocation_ = shader->GetStandartUniformLocation(axelynx::Shader::SU_MODELVIEWPROJ);
	normallocation_ = shader->GetStandartUniformLocation(axelynx::Shader::SU_NORMALMATRIX);

	if(!using_entity_textures_)
	{
		for(int i=0;i<8;++i)
		{
			if(textures_[i])
			{
				textures_[i]->Bind(i);
			}
		}
	}

	CCanvas::Instance()->SetBlendMode(blend_);
	

	if(!CEarlyZ::OnEarlyZPass())
	{
		if(dw_mode_ == axelynx::DW_NONE)
		{
			glDepthMask(GL_FALSE);
		}

		if(dw_mode_ == axelynx::DW_WRITE)
		{
				glDepthMask(GL_TRUE);
		}

		if(dw_mode_ == axelynx::DW_AUTO)
		{
			if(blend_ != axelynx::BM_NONE)
			{
			
				glDepthMask(GL_FALSE);
			}
			else
			{
				glDepthMask(GL_TRUE);
			}
		}

		if(dt_mode_ == axelynx::DT_NONE)
		{
			glDisable(GL_DEPTH_TEST);
		}
		else
		{
			glEnable(GL_DEPTH_TEST);
		}
	}
	current_ = this;
}
void CMaterial::UnBind() const
{
	if(zerlypass_shader_ && CEarlyZ::OnEarlyZPass())
		zerlypass_shader_->UnBind();
	else
		shader_->UnBind();

	for(int i=0;i<8;++i)
	{
		if(textures_[i])
		{
			textures_[i]->UnBind();
		}
	}

	glDepthMask(GL_TRUE);

	current_ = 0;
}

axelynx::Shader * CMaterial::GetShader() const
{
	return shader_;
}


int CMaterial::GetLocationModel() const
{
	return modellocation_;
}

int CMaterial::GetLocationNormal() const
{
	return normallocation_;
}

int CMaterial::GetLocationModelViewProj() const
{
	return modelviewprojlocation_;
}

int CMaterial::GetLocationModelView() const
{
	return modelviewlocation_;
}

bool CMaterial::isUsingEntityTextures() const
{
	return using_entity_textures_;
}

void CMaterial::UseEntityTextures(bool value)
{
	using_entity_textures_ = value;
}

void CMaterial::SetDepthWriteMode(axelynx::DepthWriteMode dwm)
{
	dw_mode_  =dwm;
}

void CMaterial::SetDepthTestMode(axelynx::DepthTestMode dtm)
{
	dt_mode_  =dtm;
}

void CMaterial::SetCullingMode(axelynx::CullingMode dtm)
{
	c_mode_  =dtm;
}

void CMaterial::SetZEarlyPassShader(axelynx::Shader *sh)
{
	zerlypass_shader_  =sh;
}