#pragma once

#include "Instance.h"
#include "Geometry.h"
#include "Entity.h"

namespace axelynx
{
	class InstanceGroup : public Geometry
	{
	public:
		virtual const Geometry* GetGeometry() const=0;
		virtual void SetGeometry(const Geometry*) = 0;
		virtual Instance* AddInstance() = 0;
		virtual bool RemoveInstance(Instance *instance) = 0;


	};
}