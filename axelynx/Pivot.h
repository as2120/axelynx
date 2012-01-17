#pragma once

#include "axelynx/math/vectormath.h"
#include "axelynx/File.h"

#include <list>

namespace axelynx
{
	class Pivot
	{
		class CPivot;
		CPivot *pimpl;
	public:
		AXELYNX_API Pivot();

		virtual Pivot* GetParent() const;

		virtual int AddChild(Pivot * child);
		virtual std::list<Pivot*>* ChildsList();
		virtual std::list<const Pivot*>* ChildsList() const;

		//Primary Setters
		virtual Pivot* SetParent(Pivot * parent);
		virtual Pivot* SetPosition(const vec3& position);
		virtual Pivot* SetScale(const vec3& scale);
		virtual Pivot* SetOrientation(const quat& orient);
		virtual Pivot* SetRotate(const vec3& angles);
		virtual Pivot* Translate(const vec3& translate);
		virtual Pivot* Move(const vec3& translate);
		virtual Pivot* Move(float value);
		virtual Pivot* MoveX(float value);
		virtual Pivot* MoveY(float value);
		virtual Pivot* MoveZ(float value);
		virtual Pivot* Turn(const vec3& angles);
		virtual Pivot* Turn(const quat& orient);
		virtual Pivot* PointOn(const vec3& point);

		//Extension Setters
		Pivot* SetPosition(const Pivot* pivot)
		{
			return SetPosition(pivot->GetPosition());
		}

		Pivot* SetPosition(float x, float y, float z)
		{
			return SetPosition(vec3(x,y,z));
		}

		Pivot* SetScale(float x, float y, float z)
		{
			return SetScale(vec3(x,y,z));
		}
		Pivot* SetScale(float value)
		{
			return SetScale(vec3(value,value,value));
		}

		Pivot* SetScale(const Pivot* pivot)
		{
			return SetScale(pivot->GetScale());
		}

		Pivot* SetOrientation(float x, float y, float z)
		{
			return SetOrientation(quat(vec3(x,y,z)));
		}

		Pivot* SetOrientation(const Pivot* pivot)
		{
			return SetOrientation(pivot->GetOrientation());
		}

		Pivot* Move(float x, float y, float z)
		{
			return Move(vec3(x,y,z));
		}

		Pivot* Turn(float x, float y, float z)
		{
			return Turn(quat(vec3(x,y,z)));
		}

		Pivot* PointOn(const Pivot* pivot)
		{
			return PointOn(pivot->GetPosition());
		}
		Pivot* PointOn(float x, float y, float z)
		{
			return PointOn(vec3(x,y,z));
		}

		virtual const vec3& GetPosition(bool global = true,bool immediate = true) const;
		virtual const vec3& GetScale(bool global = true) const;
		virtual const quat& GetOrientation(bool global = true) const;

		virtual const mat3& GetOrientationMatrix() const;
		virtual const mat4& GetTransformMatrix(bool immediate = true) const;
		virtual const mat4& GetLocalMatrix() const;

		virtual int GetBinarySize() const;
		virtual int WriteBinary(File file) const;

		virtual bool isMustRecalcTM() const;
		virtual ~Pivot();
	};
}
