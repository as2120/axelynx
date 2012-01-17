#pragma once

#include "axelynx/Image.h"

class CImageSystemGS;


struct RectData
{
	axelynx::vec2 position;
	axelynx::vec2 scale;
	float angle;
};

class CImageGS : public axelynx::Image
{
	CImageSystemGS *imagesystem_;

	axelynx::vec2 position_;
	axelynx::vec2 scale_;
	float angle_;

	RectData rdata_;

public:

	CImageGS(CImageSystemGS* is);
	CImageGS(CImageSystemGS* is, const axelynx::vec2& corner0,const axelynx::vec2& corner1);
	virtual void Draw() const;

	virtual void Position(const axelynx::vec2& position);
	virtual void Scale(const axelynx::vec2& position);
	virtual void Rotate(float angle);

	virtual void Region(const axelynx::vec2& corner0, const axelynx::vec2& corner1);

	virtual ~CImageGS();
};