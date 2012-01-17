#pragma once

#include "axelynx/TextCache.h"
#include "CRenderTarget.h"
#include "axelynx/Texture.h"
#include "axelynx/Shader.h"
#include <map>
#include <vector>
#include "gl/axgl.h"

class CTextCache : public axelynx::TextCache
{
	CRenderTarget *rt_;
	axelynx::Texture *tex_;
	axelynx::Shader *shader_;

	GLuint vao_;
	GLuint vbo_;
	GLuint ibo_;

	struct UV
	{
		unsigned short u0,v0,u1,v1;
	};

	struct StaticVertex
	{
		axelynx::vec2 position;
		unsigned short u,v;
		unsigned char r,g,b,a;
	};

	mutable std::vector<StaticVertex> static_;
	mutable const axelynx::Canvas *cur_canvas_;

	std::map<std::string,UV> ansi_text_;
	std::map<std::wstring,UV> unicode_text_;
public:
	CTextCache(const axelynx::Font *fnt);
	virtual void Resize(int newsidth, int newheight);
		
	virtual void AddText(const char* ansi_string);
	virtual void AddText(const wchar_t* unicode_string);
	virtual void RemoveText(const char* ansi_string);
	virtual void RemoveText(const wchar_t* unicode_string);

	virtual void DrawText(const char* ansi_string);
	virtual void DrawText(const wchar_t* unicode_string);

	virtual void StartDraw(const axelynx::Canvas *c);
	virtual void EndDraw();

	virtual ~CTextCache();
};