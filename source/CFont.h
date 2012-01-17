#pragma once

#include "axelynx/Font.h"

class CFont : public axelynx::Font
{
public:
	CFont(const axelynx::Font::Desc &desc);

	virtual bool Bind() const;
	virtual bool UnBind() const;

	virtual void FAST_CALL Draw(const axelynx::Canvas *c,const char *ansi_string, bool center = false) const;
	virtual void FAST_CALL Draw(const axelynx::Canvas *c,const wchar_t *wide_string, bool center = false) const;

	virtual int GetHeight() const;
	virtual int FAST_CALL GetWidth(const char *ansi_string) const;
	virtual int FAST_CALL GetWidth(const wchar_t *wide_string) const;

	virtual void StartDraw(const axelynx::Canvas *c) const;
	virtual void EndDraw() const;

	virtual ~CFont();
};