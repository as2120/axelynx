

#include "CFont.h"

CFont::CFont(const axelynx::Font::Desc &desc)
{
}

bool CFont::Bind() const
{
	return true;
}

bool CFont::UnBind() const
{
	return true;
}

void CFont::Draw(const axelynx::Canvas *c,const char *ansi_string, bool center) const
{
}

void CFont::Draw(const axelynx::Canvas *c,const wchar_t *wide_string, bool center) const
{
}

int CFont::GetHeight() const
{
	return 0;
}

int CFont::GetWidth(const char *ansi_string) const
{
	return 0;
}

int CFont::GetWidth(const wchar_t *wide_string) const
{
	return 0;
}

CFont::~CFont()
{
}

void CFont::StartDraw(const axelynx::Canvas *c) const
{
}

void CFont::EndDraw() const
{
}