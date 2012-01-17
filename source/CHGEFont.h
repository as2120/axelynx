#pragma once

#include "axelynx/Font.h"
#include "axelynx/Texture.h"
#include "axelynx/Shader.h"
#include <map>
#include <vector>
#include "gl/axgl.h"

class CHGEFont : public axelynx::Font
{
	struct Glyf
	{
		unsigned short u0,v0;
		unsigned short u1,v1;

		unsigned short width;
		unsigned short height;

		int kerning;
		int x_shift;
	};

	axelynx::Texture *tex_;
	axelynx::Shader *shader_;

	axelynx::Texture *cache_;
	axelynx::Shader *shader_static_;

	typedef std::vector<Glyf> Glyfs;
	mutable Glyfs glyfs_;

	GLuint vao_;
	GLuint vbo_;
	GLuint ibo_;

	GLuint vao_static_;
	GLuint vbo_static_;
	GLuint ibo_static_;

	struct Vertex
	{
		float x,y;
		unsigned short u,v;
	};

	struct StaticVertex
	{
		axelynx::vec2 position;
		unsigned short u,v;
		unsigned char r,g,b,a;
	};

	mutable std::vector<StaticVertex> static_;
	mutable const axelynx::Canvas *cur_canvas_;
public:
	CHGEFont(const axelynx::Font::Desc &desc);

	virtual bool Bind() const;
	virtual bool UnBind() const;

	virtual void FAST_CALL Draw(const axelynx::Canvas *c,const char *ansi_string, bool center = false) const;
	virtual void FAST_CALL Draw(const axelynx::Canvas *c,const wchar_t *wide_string, bool center = false) const;

	virtual int GetHeight() const;
	virtual int FAST_CALL GetWidth(const char *ansi_string) const;
	virtual int FAST_CALL GetWidth(const wchar_t *wide_string) const;

	virtual void StartDraw(const axelynx::Canvas *c) const;
	virtual void EndDraw() const;

	virtual ~CHGEFont();
};