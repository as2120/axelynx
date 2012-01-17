#pragma once

#include "Geometry.h"
#include "Pivot.h"
#include "math\vec3.h"

namespace axelynx
{
	class RaySystem : public Geometry
	{
	public:
		enum RecalcMode
		{
			RM_ONDRAW
			,RM_MANUAL
		};

		enum RenderType
		{
			RT_FAKE
			,RT_VOLUMETRIC
		};

		class LightRay
		{
		public:
			virtual void SetWidth(float width)=0;
			virtual void SetLenght(float lenght)=0;
			virtual void Place(vec3 from, vec3 to)=0;
			virtual void SetColor(float r, float g, float b, float a = 1)=0;

			virtual const vec3& StartPoint() const =0;
			virtual const vec3& EndPoint() const =0;

			virtual float GetLenght() const = 0;
			virtual const vec4& GetColor() const = 0;
			virtual float GetWidth() const = 0;
		};

		class Ray : public Pivot, public LightRay
		{
		public:
			enum Start
			{
				S_START,S_END,S_MIDDLE
			};
			virtual void Orign(Start sp)=0;
		};

		virtual Ray* AddInstance()=0;
		virtual LightRay* AddLightRay()=0;
		virtual bool RemoveInstance(LightRay * pivot)=0;

		virtual int SetRecalcMode(RecalcMode) = 0;
		virtual int SetRenderType(RenderType) = 0;
		virtual void SetDefaultOrign(Ray::Start mode) = 0;
		virtual bool Recalc()const = 0;
		virtual ~RaySystem(){};
	};
}