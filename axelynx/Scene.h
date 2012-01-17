#pragma once


#include "SceneGraph.h"
#include "Light.h"

namespace axelynx
{
	class Scene
	{
	public:
		virtual bool Render()=0;
		virtual bool ZPassRender()=0;
		virtual bool RenderShadow()=0;

		virtual bool Update(float twin=1.0f)=0;

		virtual Pivot* AddPivot()=0;
		virtual Camera* AddCamera()=0;
		virtual Light* CreateLight(Light::Mode mode = Light::LM_SPOT) = 0;
		virtual Entity* Add(const Geometry *geometry)=0;
		virtual AnimEntity* Add(const AnimGeometry *geometry)=0;

		virtual bool Remove(axelynx::Pivot *pivot)=0;

		virtual SceneGraph * GetSceneGraph() const =0;
		virtual ~Scene(){};

		virtual Scene* Shared(Camera* shared)=0;
		virtual Scene* Shared(Entity* shared)=0;
		virtual Scene* Shared(Light* shared)=0;

		virtual Entity* LoadEntity(File file)=0;

		virtual Entity* RestoreEntity(File file)=0;

		virtual bool SetShadowShader(Shader *shader)=0;

		virtual unsigned int CountEntities() const = 0;
		virtual unsigned int CountCameres() const = 0;
		virtual unsigned int CountLights()const  = 0;
	};
}