#pragma once

#include "Texture.h"
#include "Shader.h"
#include "Drawable.h"
#include "math/vec4.h"

namespace axelynx
{
	class PostEffect : axDrawable
	{
	public:
		virtual bool SetColor(const vec4& color)=0;
		virtual bool SetColor(float red, float green, float blue, float alpha = 1.0f)=0;
		virtual bool BindShader(Shader *shader)=0;
		virtual Shader* GetShader() const=0;

		virtual bool Bind() const = 0;
		virtual bool UnBind(bool restore = false) const = 0;

		virtual void Draw(const axelynx::Canvas *canvas = 0) const =0;

		virtual PostEffect* Add(PostEffect *afx)=0;
		virtual PostEffect* Remove(PostEffect *afx)=0;

		virtual ~PostEffect(){};
	};
}