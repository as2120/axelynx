

#include "CImageGS.h"
#include "CImageSystemGS.h"

CImageGS::CImageGS(CImageSystemGS* is)
	:imagesystem_(is)
{

}

CImageGS::CImageGS(CImageSystemGS* is,const axelynx::vec2& corner0,const axelynx::vec2& corner1)
	:imagesystem_(is)
{

}

void CImageGS::Draw() const
{
	RectData geom;
	geom.position = position_;
	geom.scale = scale_;
	geom.angle = angle_;

	imagesystem_->AddGeom(geom);
}

void CImageGS::Position(const axelynx::vec2& position)
{
	position_ = position;
}

void CImageGS::Scale(const axelynx::vec2& scale)
{
	scale_ = scale;
}


void CImageGS::Rotate(float angle)
{
	angle_ = angle;
}

void CImageGS::Region(const axelynx::vec2& corner0, const axelynx::vec2& corner1)
{
}

CImageGS::~CImageGS()
{
}