#pragma once

#include "axlib.h"

namespace axelynx
{
	inline unsigned char RGBA_R(unsigned int value)
	{
		return static_cast<unsigned char>(value & 0x00FF0000);
	}

	inline unsigned char RGBA_G(unsigned int value)
	{
		return static_cast<unsigned char>(value & 0x0000FF00);
	}

	inline unsigned char RGBA_B(unsigned int value)
	{
		return static_cast<unsigned char>(value & 0x000000FF);
	}

	inline unsigned char RGBA_A(unsigned int value)
	{
		return static_cast<unsigned char>(value & 0xFF000000);
	}

	inline unsigned int RGBA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		return alpha << 24 | red << 16 | green <<8 | blue;
	}

	class PixMap
	{
	public:

		class pixel //for example comepixmap(i,i)[0] = red(i,i)
		{
			unsigned char* value;
		public:
			explicit pixel(unsigned char* mempos):value(mempos){};
			const unsigned char& operator[](int i) const {return *(value+i);};
			unsigned char& operator[](int i) {return *(value+i);};
		};

		class readonlypixel //for example comepixmap(i,i)[0] = red(i,i)
		{
			unsigned char* value;
		public:
			explicit readonlypixel(unsigned char* mempos):value(mempos){};
			const unsigned char& operator[](int i) const {return *(value+i);};
		};

		virtual int Height() const =0;
		virtual int Width() const =0;
		virtual int Bpp() const=0;

		virtual void Draw(int x, int y)=0;

		static AXELYNX_API PixMap* Create(int width, int height, int bpp);
		static AXELYNX_API PixMap* Create(int width, int height, int bpp, const void* data);

		virtual void SetPixel(int x, int y, unsigned int value)=0;
		virtual unsigned int GetPixel(int x, int y) const =0 ;

		virtual void SetPixel(int x, int y, int channel, unsigned char value)=0;
		virtual unsigned char GetPixel(int x, int y, int channel) const =0;

		virtual unsigned char* Data()=0;
		virtual const unsigned char* Data() const =0;

		virtual pixel operator ()(int x,int y) = 0;
		virtual readonlypixel operator ()(int x,int y) const = 0;

		virtual PixMap *ExtractChannel(int channel) const = 0;
		virtual ~PixMap(){};
	};
}