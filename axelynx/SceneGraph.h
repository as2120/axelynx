#pragma once

#include "Pivot.h"
#include "Camera.h"
#include "Geometry.h"
#include "Entity.h"
#include "AnimEntity.h"

namespace axelynx
{
	class SceneGraph
	{
	public:
		virtual bool AddEntity(Entity *entity)=0;
		virtual bool AddStaticEntity(Entity *entity)=0;
		virtual bool Remove(Entity *entity)=0;

		virtual void Render(const Camera *cam)=0;
		virtual void Update(float twin = 1.0f)=0;
		virtual unsigned int CountEntities() const=0;
		virtual ~SceneGraph(){}
	};
}