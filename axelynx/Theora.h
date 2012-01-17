#pragma once

#include "Texture.h"

namespace axelynx
{
	class Theora
	{

	public:
		enum DiffuseCalcMode
		{
			DCM_AUTO
			,DCM_SOFTWARE
			,DCM_HARDWARE
			,DCM_NONE
		};

		virtual int Width() = 0;
		virtual int Height() = 0;

		virtual bool Play() = 0;
		virtual bool Stop() = 0;
		virtual bool Pause() = 0;

		virtual int CountFrames() = 0;
		virtual int CurrentFrame() = 0;

		virtual Texture* GetDiffuseTexture() = 0;
		virtual Texture* GetTextureY() = 0;
		virtual Texture* GetTextureU() = 0;
		virtual Texture* GetTextureV() = 0;

		virtual void SetDiffuseCalcMode(DiffuseCalcMode mode) = 0;

		virtual float dWidthY()=0;
		virtual float dWidthUV()=0;
		
		virtual void Update() = 0;
		virtual ~Theora(){};
	};
}