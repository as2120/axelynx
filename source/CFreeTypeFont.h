#pragma once
#include "axelynx/Font.h"
#include "axelynx/Texture.h"
#include "axelynx/Shader.h"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>


#include <map>
#include <vector>
#include "gl/axgl.h"

class CFreeTypeFont : public axelynx::Font
{
	struct Glyf
	{
		unsigned short u0,v0;
		unsigned short u1,v1;

		signed char width;
		signed char height;

		signed char  kerning;
		signed char x_shift;

		signed char y_shift;

		bool generated;

		Glyf()
			:u0(0),v0(0),u1(0),v1(0),width(0),height(0),kerning(0),x_shift(0),y_shift(0),generated(false)
		{
		}
	};

	axelynx::Texture *tex_;
	axelynx::Shader *shader_;

	axelynx::Texture *cache_;
	axelynx::Shader *shader_static_;

	typedef Glyf* Glyfs;
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

	static FT_Library library_;
	FT_Face face_;
	static int count_fonts;

	CFreeTypeFont::Glyf FAST_CALL RenderGyph(wchar_t symbol) const; 

	int font_height_;

	mutable int cx_, cy_;

	unsigned char* file_buff;
public:
	CFreeTypeFont(const axelynx::Font::Desc &desc);

	virtual bool Bind() const;
	virtual bool UnBind() const;

	virtual void FAST_CALL Draw(const axelynx::Canvas *c,const char *ansi_string, bool center = false) const;
	virtual void FAST_CALL Draw(const axelynx::Canvas *c,const wchar_t *wide_string, bool center = false) const;

	virtual int GetHeight() const;
	virtual int FAST_CALL GetWidth(const char *ansi_string) const;
	virtual int FAST_CALL GetWidth(const wchar_t *wide_string) const;

	virtual void StartDraw(const axelynx::Canvas *c) const;
	virtual void EndDraw() const;

	virtual ~CFreeTypeFont();
};