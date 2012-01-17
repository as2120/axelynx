#pragma once

#include "axelynx/ImageSystem.h"
#include "CImage.h"
#include "axelynx/Texture.h"
#include "axelynx/Shader.h"
#include "gl/axgl.h"
#include <vector>
#include <list>

class CImageSystem :public axelynx::ImageSystem
{
	RectData *src_buffer_;
	
	axelynx::Texture* texture_;
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

	virtual CImage * GetImage();
	virtual CImage * GetImage(const axelynx::vec2& corner0, const axelynx::vec2& corner1);

	CImageSystem(axelynx::Texture *tex, int maximages);
	virtual ~CImageSystem();

	void AddGeom(const RectData &rd);
};