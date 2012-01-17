

#include "CImage.h"
#include "CImageSystem.h"

CImage::CImage(CImageSystem* is)
	:imagesystem_(is)
{

}

CImage::CImage(CImageSystem* is,const axelynx::vec2& corner0,const axelynx::vec2& corner1)
	:imagesystem_(is)
{

}

void CImage::Draw() const
{
	axelynx::mat3x2::FromTransform(transform_,position_,angle_,scale_);

	RectData geom = rdata_;
	for(int i=0;i<4;++i)
	{
		geom.corners[i].position = transform_ * rdata_.corners[i].position;
	}
	imagesystem_->AddGeom(geom);
}

void CImage::Position(const axelynx::vec2& position)
{
	position_ = position;
}

void CImage::Scale(const axelynx::vec2& scale)
{
	scale_ = scale;
}


void CImage::Rotate(float angle)
{
	angle_ = angle;
}

void CImage::Region(const axelynx::vec2& corner0, const axelynx::vec2& corner1)
{
}

CImage::~CImage()
{
}