#pragma once

#include "axelynx/ImageSystem.h"
#include "CImageGS.h"
#include "axelynx/Texture.h"
#include "axelynx/Shader.h"
#include "gl/axgl.h"
#include <vector>
#include <list>

class CImageSystemGS :public axelynx::ImageSystem
{
	RectData *src_buffer_;
	
	axelynx::Texture* texture_;
	axelynx::Texture* sincos_;
	axelynx::Shader* shader_;

	GLuint vao_;
	GLuint vbo_;
	GLuint ibo_;

	int maximages_;
	mutable int curimages_;

	GLubyte *buffer_;
	int current_buffer_lenght_;
public:
	virtual void Draw(const axelynx::Canvas *canvas) const;

	virtual CImageGS * GetImage();
	virtual CImageGS * GetImage(const axelynx::vec2& corner0, const axelynx::vec2& corner1);

	CImageSystemGS(axelynx::Texture *tex, int maximages);
	virtual ~CImageSystemGS();

	void AddGeom(const RectData &rd);
};