#pragma once

#include "Pivot.h"
#include "Geometry.h"

namespace axelynx
{
	class SpriteSystem : public Geometry
	{
	public:
		class Sprite : public Pivot
		{
		public:
			virtual void SetColor(float r, float g, float b, float a = 1)=0;
			virtual void SetSize(float size)=0;
			virtual ~Sprite(){};	
		};

		enum DrawMode
		{
			DM_SPRITE
			,DM_BILLBOARD
		};

		enum RecalcMode
		{
			RM_ONDRAW
			,RM_MANUAL
		};

		virtual Sprite* AddInstance()=0;
		virtual bool RemoveInstance(Sprite* pivot)=0;

		virtual int SetDrawMode(DrawMode) = 0;
		virtual int SetRecalcMode(RecalcMode) = 0;
		virtual bool Recalc()const = 0 ;
		virtual ~SpriteSystem(){};
	};
}