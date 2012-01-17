#pragma once

#include "axelynx/Shape.h"
#include "CShader.h"

#include "gl/axgl.h"

using namespace axelynx;

class CShape : public axelynx::Shape
{
	GLuint vbo_;
	GLuint ibo_;
	GLuint vao_;

	vec2* positions_;
	vec4* colors_;
	vec2* uv_;

	bool uv_use_;
	bool color_use_;

	int cntvertices_;
	int cntindices_;

	GLenum drawmode_;

	int indexsize_;
	int *indices_;

	CShader *building;
	bool builded;
	int vertexsize_;

	GLubyte *vbuffer_; //video card buffer (for VBO)
public:
	CShape(int cnttris, int cntverts);
	virtual void Draw(const axelynx::Canvas *canvas) const;

	virtual int TrianglesCount() const;
	virtual int VerticesCount() const;

	virtual vec2 GetPosition(int index) const;
	virtual vec4 GetColor(int index) const;
	virtual vec2 GetTexCoords(int index) const;

	virtual int GetTrisIndex(int tris, int corner) const;

	virtual void SetPosition(int index, const vec2& position);
	virtual void SetColor(int index, const vec4& color);
	virtual void SetTexCoords(int index, const vec2& uv);

	virtual void SetTrisIndex(int tris, int corner, int vertexindex);
	virtual void SetTrisIndex(int tris, int corner0, int corner1, int corner2);
	bool Build();
};
