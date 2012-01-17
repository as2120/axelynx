#pragma once

#include "axelynx/Scene.h"
#include <list>
#include "CEntity.h"


class CScene : public axelynx::Scene
{
	axelynx::Entity* entities_;
	int cnt_ent_;
	int max_ent_;

	axelynx::SceneGraph *scenegraph_;
	std::list<axelynx::Camera *> camlist;
	std::list<axelynx::Light *> lightlist;

	std::list<axelynx::Entity *> pickedlist;

	static int frame_;
	bool zpassed;
	
	axelynx::Shader *defShader_;
public:

	CScene(axelynx::SceneGraph *scenegrah);

	virtual bool ZPassRender();

	virtual bool Render();
	virtual bool Update(float twin=1.0f);

	virtual axelynx::Pivot* AddPivot();
	virtual axelynx::Camera* AddCamera();
	virtual axelynx::Light* CreateLight(axelynx::Light::Mode mode = axelynx::Light::LM_SPOT);
	virtual axelynx::Entity* Add(const axelynx::Geometry *geometry);
	virtual axelynx::AnimEntity* Add(const axelynx::AnimGeometry *animgeometry);
	virtual bool Remove(axelynx::Pivot *pivot);

	virtual axelynx::SceneGraph * GetSceneGraph() const
	{
		return scenegraph_;
	}

	static int GetFrame()
	{
		return frame_;
	}

	virtual axelynx::Scene* Shared(axelynx::Camera* shared);
	virtual axelynx::Scene* Shared(axelynx::Entity* shared);
	virtual axelynx::Scene* Shared(axelynx::Light* shared);

	virtual axelynx::Entity* LoadEntity(axelynx::File file);
	virtual axelynx::Entity* RestoreEntity(axelynx::File file);

	bool EntityPickMode(CEntity* ent, axelynx::PickMode pm);
	axelynx::PickResult Pick(axelynx::vec3 start, axelynx::vec3 end);

	virtual bool SetShadowShader(axelynx::Shader *shader);
	virtual bool RenderShadow();

	virtual unsigned int CountEntities() const;
	virtual unsigned int CountCameres() const;
	virtual unsigned int CountLights() const;

	virtual ~CScene(){};
	

};