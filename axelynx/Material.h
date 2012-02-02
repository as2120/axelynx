#pragma once
#ifndef AXELYNX_MATERIAL_H
#define AXELYNX_MATERIAL_H

#include "Shader.h"
#include "Texture.h"
#include "Canvas.h"
#include "Camera.h"

namespace axelynx
{
	enum DepthWriteMode
	{
		DW_AUTO
		,DW_WRITE
		,DW_NONE
	};

	enum DepthTestMode
	{
		DT_AUTO
		,DT_TEST
		,DT_NONE
	};

	enum CullingMode
	{
		CM_BACK
		,CM_FRONT
		,CM_DISABLE
	};

	class Material
	{
	public:
		virtual void SetShader(Shader *sh)=0;
		virtual void SetTexture(Texture *tex, int layer =0)=0;
		virtual void SetBlend(BlendMode bm)=0;

		virtual void Bind(const Camera *cam) const =0;
		virtual void UnBind() const =0;

		virtual Shader * GetShader() const =0;

		virtual int GetLocationModelViewProj() const =0;
		virtual int GetLocationModelView() const =0;
		virtual int GetLocationModel() const =0;
		virtual int GetLocationNormal() const =0;

		virtual bool isUsingEntityTextures() const = 0;
		virtual void UseEntityTextures(bool value) = 0;

		virtual BlendMode GetBlendMode() const =0;

		virtual void SetDepthWriteMode(DepthWriteMode dwm) = 0;
		virtual void SetDepthTestMode(DepthTestMode dwm) = 0;
		virtual void SetCullingMode(CullingMode cm) = 0;

		virtual void SetZEarlyPassShader(Shader *sh)=0;
	};
}

#endif