#pragma once
#ifndef AXELYNX_ENTITY_H
#define AXELYNX_ENTITY_H

#include "Pivot.h"
#include "Geometry.h"

#include "Texture.h"
#include "Shader.h"
#include "Canvas.h"
#include "Camera.h"
#include "Material.h"

#include <string>


namespace axelynx
{
	enum PickMode
	{
		PM_NONE,
		PM_SPHERE,
		PM_BOX,
		PM_GEOMETRY,
		PM_STATIC
	};

	class Entity : public Pivot
	{
	public:
		class Action
		{
		public:
			virtual bool operator()(Entity* ent,float twin) = 0;
			virtual ~Action(){};
		};


		virtual std::wstring GetName() const =0;
		virtual const Geometry *GetGeometry() const =0;
		virtual bool Draw(const Camera *camera)=0;
		virtual bool Update(float twin = 1.0f)=0;
		virtual bool inFrustum(const frustum &f)=0;

		virtual ~Entity(){};

		virtual axelynx::Entity* Copy() const = 0;
		virtual Entity* Show(bool value = true) = 0;
		virtual Entity* SetMaterial(const Material *, bool by_hierarhy = true)=0;
		virtual Entity* SetTexture(Texture* texture, int layer=0, bool by_hierarhy = true)=0;
		virtual Entity* SetShader(Shader* shader, bool by_hierarhy = true)=0;
		virtual Entity* SetBlend(BlendMode bm, bool by_hierarhy = true)=0;
		virtual Entity* SetName(std::wstring name)=0;
		virtual Entity* ReciveShadows(bool value, bool by_hierarhy = true) = 0;
		virtual Entity* CastShadows(bool value, bool by_hierarhy = true) = 0;
		virtual Entity* OnUpdate(Action *action)=0;

		virtual Entity* FindByName(const std::wstring &name, bool by_hierarhy = true) = 0;
		Entity* Hide() { return Show(false); }

		virtual Entity* SetColor(const vec4& color)=0;
		virtual Entity* SetParent(Pivot * parent)
		{
			Pivot::SetParent(parent);
			return this;
		}
		virtual Entity* SetPosition(const vec3& position)
		{
			Pivot::SetPosition(position);
			return this;
		}
		virtual Entity* SetScale(const vec3& scale)
		{
			Pivot::SetScale(scale);
			return this;
		}
		virtual Entity* SetOrientation(const quat& orient)
		{
			Pivot::SetOrientation(orient);
			return this;
		}
		virtual Entity* SetRotate(const vec3& angles)
		{
			Pivot::SetRotate(angles);
			return this;
		}
		virtual Entity* Translate(const vec3& translate)
		{
			Pivot::Translate(translate);
			return this;
		}
		virtual Entity* Move(const vec3& translate)
		{
			Pivot::Move(translate);
			return this;
		}
		virtual Entity* Move(float value)
		{
			Pivot::Move(value);
			return this;
		}
		virtual Entity* MoveX(float value)
		{
			Pivot::MoveX(value);
			return this;
		}
		virtual Entity* MoveY(float value)
		{
			Pivot::MoveY(value);
			return this;
		}
		virtual Entity* MoveZ(float value)
		{
			Pivot::MoveZ(value);
			return this;
		}
		virtual Entity* Turn(const vec3& angles)
		{
			Pivot::Turn(angles);
			return this;
		}
		virtual Entity* Turn(const quat& orient)
		{
			Pivot::Turn(orient);
			return this;
		}
		virtual Entity* PointOn(const vec3& point)
		{
			Pivot::PointOn(point);
			return this;
		}


		Entity* SetPosition(const Pivot* pivot)
		{
			return SetPosition(pivot->GetPosition());
		}

		Entity* SetPosition(float x, float y, float z)
		{
			return SetPosition(vec3(x,y,z));
		}

		Entity* SetScale(float x, float y, float z)
		{
			return SetScale(vec3(x,y,z));
		}
		Entity* SetScale(float value)
		{
			return SetScale(vec3(value,value,value));
		}

		Entity* SetScale(const Pivot* pivot)
		{
			return SetScale(pivot->GetScale());
		}

		Entity* SetOrientation(float x, float y, float z)
		{
			return SetOrientation(quat(vec3(x,y,z)));
		}

		Entity* SetOrientation(const Pivot* pivot)
		{
			return SetOrientation(pivot->GetOrientation());
		}

		Entity* Move(float x, float y, float z)
		{
			return Move(vec3(x,y,z));
		}

		Entity* Turn(float x, float y, float z)
		{
			return Turn(quat(vec3(x,y,z)));
		}

		Entity* PointOn(const Pivot* pivot)
		{
			return PointOn(pivot->GetPosition());
		}
		Entity* PointOn(float x, float y, float z)
		{
			return PointOn(vec3(x,y,z));
		}

		virtual Entity* SetPickMode(PickMode pm) =0;
		virtual bool Save(File file) const = 0;

		virtual Entity* SetDepthWriteMode(DepthWriteMode dwm) = 0;
		virtual Entity* SetDepthTestMode(DepthTestMode dwm) = 0;

		virtual Entity* SetVisible(bool visible) = 0;

		int tag;
	};
}
#endif
