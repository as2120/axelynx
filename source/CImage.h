#pragma once

#include "axelynx/Image.h"
#include "axelynx/math/mat3x2.h"

class CImageSystem;

struct RectVertex
{
	axelynx::vec2 position;
	unsigned short u,v;
	unsigned char r,g,b,a;
};

struct RectData
{
	RectVertex corners[4];

	RectData()
	{
		corners[0].position.x = -0.5f;
		corners[0].position.y = -0.5f;
		corners[0].u = 0;
		corners[0].v = 0;

		corners[1].position.x = 0.5f;
		corners[1].position.y = -0.5f;
		corners[1].u = 65535;
		corners[1].v = 0;

		corners[2].position.x = 0.5f;
		corners[2].position.y = 0.5f;
		corners[2].u = 65535;
		corners[2].v = 65535;

		corners[3].position.x = -0.5f;
		corners[3].position.y = 0.5f;
		corners[3].u = 0;
		corners[3].v = 65535;

		for(int i=0;i<4;++i)
		{
			corners[i].a = 255;
			corners[i].r = 255;
			corners[i].g = 255;
			corners[i].b = 255;
		}
	}
};

class CImage : public axelynx::Image
{
	CImageSystem *imagesystem_;

	axelynx::vec2 position_;
	axelynx::vec2 scale_;
	float angle_;
	bool need_recalc_;
	mutable axelynx::mat3x2 transform_;

	RectData rdata_;

public:

	CImage(CImageSystem* is);
	CImage(CImageSystem* is, const axelynx::vec2& corner0,const axelynx::vec2& corner1);
	virtual void Draw() const;

	virtual void Position(const axelynx::vec2& position);
	virtual void Scale(const axelynx::vec2& position);
	virtual void Rotate(float angle);

	virtual void Region(const axelynx::vec2& corner0, const axelynx::vec2& corner1);

	virtual ~CImage();
};