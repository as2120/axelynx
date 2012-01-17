

#include "CPixMap.h"
#include "gl/axgl.h"
#include "axelynx/Logger.h"
#include <cstring>

AXELYNX_API axelynx::PixMap* axelynx::PixMap::Create(int width, int height, int bpp)
{
	return new CPixMap(width,height,bpp);
}

AXELYNX_API axelynx::PixMap* axelynx::PixMap::Create(int width, int height, int bpp, const void* data)
{
	return new CPixMap(width,height,bpp,data);
}

CPixMap::CPixMap(int width, int height, int bpp)
{
	width_ = width;
	height_ = height;
	bpp_ = bpp;

	data_ = new unsigned char[width_ * height_ * bpp_];
}

CPixMap::CPixMap(int width, int height, int bpp, const void* data)
{
	width_ = width;
	height_ = height;
	bpp_ = bpp;

	data_ = new unsigned char[width_ * height_ * bpp_];

	memcpy(data_,data,width_ * height_ * bpp_);
}

CPixMap::~CPixMap()
{
	delete[] data_;
	data_=0;
}

void CPixMap::Draw(int x, int y)
{
	GLenum format = GL_RGB;
	if(bpp_==4) format = GL_RGBA;
	if(bpp_==1) format = GL_R;
	if(bpp_==2) format = GL_RG;

	glRasterPos2i(x,y+height_);
	glDrawPixels(width_,height_,format,GL_UNSIGNED_BYTE,data_);
}

void CPixMap::SetPixel(int x, int y, unsigned int value)
{
	memcpy(data_ + (y*width_ + x) * bpp_,&value,bpp_);
}

unsigned int CPixMap::GetPixel(int x, int y) const
{
	unsigned int result=0;
	memcpy(&result,data_ + (y*width_ + x)* bpp_,bpp_);
	return result;
}


void CPixMap::SetPixel(int x, int y, int channel, unsigned char value)
{
	data_[(y*width_ + x)*bpp_ + channel] = value;
}

unsigned char CPixMap::GetPixel(int x, int y, int channel) const
{
	return data_[(y*width_ + x)*bpp_ + channel];
}

axelynx::PixMap::pixel CPixMap::operator()(int x,int y)
{
	return axelynx::PixMap::pixel(data_ + (y*width_ + x)*bpp_);
}

axelynx::PixMap::readonlypixel CPixMap::operator()(int x,int y) const
{
	return axelynx::PixMap::readonlypixel(data_ + (y*width_ + x)*bpp_);
}

axelynx::PixMap * CPixMap::ExtractChannel(int channel) const
{
	CPixMap *cpm = new CPixMap(width_,height_,1);

	for(int i=0;i<width_;++i)
	{
		for(int j=0;j<height_;++j)
		{
			cpm->SetPixel(i,j,0,GetPixel(i,j,channel));
		}
	}

	return cpm;
}