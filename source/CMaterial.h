#pragma once

#include "axelynx/Material.h"

class CMaterial : public axelynx::Material
{
	axelynx::Shader *shader_;
	axelynx::Shader *zerlypass_shader_;
	axelynx::Texture *textures_[8];
	axelynx::BlendMode blend_;
	mutable int modellocation_;
	mutable int modelviewlocation_;
	mutable int modelviewprojlocation_;
	mutable int normallocation_;

	axelynx::DepthWriteMode dw_mode_;
	axelynx::DepthTestMode dt_mode_;
	axelynx::CullingMode c_mode_;
	static const CMaterial *current_;
	bool using_entity_textures_;
public:
	CMaterial();

	virtual void SetShader(axelynx::Shader *sh);
	virtual void SetTexture(axelynx::Texture *tex, int layer =0);
	virtual void SetBlend(axelynx::BlendMode bm);

	virtual void Bind(const axelynx::Camera *cam) const;
	virtual void UnBind() const;

	virtual axelynx::Shader * GetShader() const;

	virtual int GetLocationModelViewProj() const;
	virtual int GetLocationModelView() const;
	virtual int GetLocationModel() const;
	virtual int GetLocationNormal() const;

	virtual bool isUsingEntityTextures() const;
	virtual void UseEntityTextures(bool value) ;

	virtual axelynx::BlendMode GetBlendMode() const;

	static int Free()
	{
		if(current_)
			current_->UnBind();

		current_ = 0;
		return 1;
	}

	virtual void SetDepthWriteMode(axelynx::DepthWriteMode dwm);
	virtual void SetDepthTestMode(axelynx::DepthTestMode dwm);
	virtual void SetCullingMode(axelynx::CullingMode cm);

	virtual void SetZEarlyPassShader(axelynx::Shader *sh);
};
