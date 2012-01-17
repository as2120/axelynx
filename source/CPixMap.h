#pragma once

#include "axelynx/PixMap.h"

class CPixMap : public axelynx::PixMap
	{
		int width_;
		int height_;
		int bpp_;
		unsigned char *data_;
	public:
		CPixMap(int width, int height, int bpp);
		CPixMap(int width, int height, int bpp, const void* data);
		~CPixMap();
		virtual int Height() const
		{
			return height_;
		}

		virtual int Width() const
		{
			return width_;
		}

		virtual int Bpp() const
		{
			return bpp_;
		}

		virtual void Draw(int x, int y);

		virtual void SetPixel(int x, int y, unsigned int value);
		virtual unsigned int GetPixel(int x, int y) const;

		virtual void SetPixel(int x, int y, int channel, unsigned char value);
		virtual unsigned char GetPixel(int x, int y, int channel) const;

		virtual unsigned char* Data()
		{
			return data_;
		}
		virtual const unsigned char* Data() const
		{
			return data_;
		}

		virtual axelynx::PixMap::pixel operator ()(int x,int y);
		virtual axelynx::PixMap::readonlypixel operator ()(int x,int y) const;

		virtual PixMap *ExtractChannel(int channel) const;
	};