#pragma once
#ifndef AXELYNX_TEXTURE_H
#define AXELYNX_TEXTURE_H

#include "axlib.h"
#include "File.h"

namespace axelynx
{
	class Texture
	{
	public:
		enum TextureType
		{
			TT_1D
			,TT_2D
			,TT_3D
			,TT_CUBE
			,TT_ARRAY
			,TT_DEPTH
		};

		struct Desc
		{
			TextureType TT;
			int width;
			int height;
			int depth;
			int cpp;
			int bpc;
			const void *data;
			bool use_mipmaps;
			int compressed;
			int internalbytes;
			float anisotropic;
			bool async;

			Desc()
			{
				TT = axelynx::Texture::TT_2D;
				width = 0;
				height = 0;
				depth = 0;
				cpp = 4;
				bpc = 1;
				data = 0;
				use_mipmaps = true;
				compressed = 0;
				internalbytes = 0; //auto
				anisotropic = 0;
				async = false;
			}	   

			Desc& TexType(TextureType tt) {TT = tt; return *this;}
			Desc& Size(int width_, int height_ = 1, int depth_ = 1) {width = width_; height = height_; depth = depth_; return *this;}
			Desc& UseMipmaps(bool use) {use_mipmaps = use; return *this;}
			Desc& Compressed(int value) {compressed = value; return *this;}
			Desc& Anisotropic(float value) {anisotropic = value; return *this;}
			Desc& Texels(const void* ptr) {data = ptr; return *this;}
			Desc& ASync(bool value){async = value; return *this;}
		};

		virtual int GetWidth() const =0;
		virtual int GetHeight() const =0;
		virtual int GetDepth() const =0;

		virtual TextureType GetType() const =0;
		
		virtual bool Bind(int layer = 0)=0;
		virtual bool UnBind(bool restore = false)=0;

		virtual bool Save(File file) const =0;

		
		virtual bool Grab(int x, int y) = 0;

		virtual void RegenerateMipmaps() = 0;
		bool Grab()
		{
			return Grab(0,0);
		}

		static AXELYNX_API Texture * Current();
		virtual ~Texture(){};
	};
}

#endif