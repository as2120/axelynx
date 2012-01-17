#pragma once

#include "axelynx/Sprite.h"

class CSprite : public axelynx::Sprite
{
public:
	virtual bool Draw(void *additionalinfo) const;

	virtual int CountTriangles()const;
	virtual int CountVertices() const;

	virtual ~CSprite(){};

	virtual std::wstring GetClass()
	{
		return L"sprite";
	}
};