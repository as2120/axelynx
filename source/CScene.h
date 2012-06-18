#pragma once

#include "axelynx/Scene.h"
#include <list>
#include "CEntity.h"
#include <btBulletDynamicsCommon.h>

class CScene : public axelynx::Scene
{
	axelynx::Entity* entities_;
	int cnt_ent_;
	int max_ent_;

	axelynx::SceneGraph *scenegraph_[256];
	bool visible_groups_[256];

	std::list<axelynx::Camera *> camlist;
	std::list<axelynx::Light *> lightlist;

	std::list<axelynx::Entity *> pickedlist;

	static int frame_;
	bool zpassed;
	
	axelynx::Shader *defShader_;

	btDiscreteDynamicsWorld *physics_world_;

public:

	CScene(axelynx::SceneGraph *scenegrah);

	virtual bool ZPassRender();

	virtual bool Render();
	virtual bool Update(float twin=1.0f);

	virtual axelynx::Pivot* AddPivot();
	virtual axelynx::Camera* AddCamera();
	virtual axelynx::Light* CreateLight(axelynx::Light::Mode mode = axelynx::Light::LM_SPOT);
	virtual axelynx::Entity* Add(const axelynx::Geometry *geometry, int entity_group_id=0);
	virtual axelynx::AnimEntity* Add(const axelynx::AnimGeometry *animgeometry, int entity_group_id=0);
	virtual bool Remove(axelynx::Pivot *pivot);

	virtual axelynx::SceneGraph * GetSceneGraph(int index) const
	{
		return scenegraph_[index];
	}

	static int GetFrame()
	{
		return frame_;
	}

	virtual axelynx::Scene* Shared(axelynx::Camera* shared);
	virtual axelynx::Scene* Shared(axelynx::Entity* shared, int group_id);
	virtual axelynx::Scene* Shared(axelynx::Light* shared);

	virtual axelynx::Entity* LoadEntity(axelynx::File file, int entity_group_id=0);
	virtual axelynx::Entity* RestoreEntity(axelynx::File file, int entity_group_id=0);

	bool EntityPickMode(CEntity* ent, axelynx::PickMode pm);
	axelynx::PickResult Pick(axelynx::vec3 start, axelynx::vec3 end);

	virtual bool SetShadowShader(axelynx::Shader *shader);
	virtual bool RenderShadow();

	virtual unsigned int CountEntities() const;
	virtual unsigned int CountCameres() const;
	virtual unsigned int CountLights() const;

	virtual bool GetVisibleGroup(int entity_group_id);
	virtual void SetVisibleGroup(int entity_group_id, bool visible);

	virtual bool SetSceneGraph(int entity_group_id, axelynx::SceneGraph *sg=0);
	virtual ~CScene(){};
	
	virtual bool InitializePhysics(axelynx::PhysicsContext *context);

	axelynx::Body* AddBody(axelynx::Shape *shape);
};