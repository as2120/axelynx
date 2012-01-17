#pragma once

#include "axelynx/Entity.h"
#include "axelynx/Scene.h"
#include "axelynx/math/frustum.h"

class CEntity : public virtual axelynx::Entity
{
	axelynx::Scene *scene_;//
		
	axelynx::Texture *textures_[8];//
	int num_textures_;//

	axelynx::Shader *shader_;//
	std::wstring name_;//

	const axelynx::Geometry *geometry_;//

	axelynx::BlendMode bm_;//
	const axelynx::Material *material_;//

	float radius_;//
	bool show_;//

	bool recive_shadows_;//
	bool cast_shadows_;//

	Action *on_update_event_;//

	axelynx::PickMode current_pickmode_;//
	axelynx::DepthWriteMode dw_mode_;//
	axelynx::DepthTestMode dt_mode_;//

	bool is_visible_;

	axelynx::vec4 entitycolor_;//


public:
	CEntity(axelynx::Scene *scene_, const axelynx::Geometry * geom);
	virtual axelynx::Entity* SetName(std::wstring name);
	virtual std::wstring GetName() const;

	virtual axelynx::Entity* SetTexture(axelynx::Texture* texture, int layer=0, bool by_hierarhy = true);
	virtual axelynx::Entity* SetShader(axelynx::Shader* shader, bool by_hierarhy = true);
	virtual axelynx::Entity* SetBlend(axelynx::BlendMode bm, bool by_hierarhy = true);
	virtual axelynx::Entity* FindByName(const std::wstring &name, bool by_hierarhy = true);
	virtual axelynx::Entity* SetDepthWriteMode(axelynx::DepthWriteMode dwm);
	virtual axelynx::Entity* SetDepthTestMode(axelynx::DepthTestMode dtm);
	virtual axelynx::Entity* SetColor(const axelynx::vec4& color);

	virtual const axelynx::Geometry *GetGeometry() const;

	virtual bool Draw(const axelynx::Camera *camera);
	virtual bool Update(float twin = 1.0f);

	virtual axelynx::Entity* SetMaterial(const axelynx::Material * material, bool by_hierarhy = true);

	virtual bool inFrustum(const axelynx::frustum &f);

	virtual axelynx::Entity* Show(bool value = true);
	virtual bool PreparePipeline(const axelynx::Camera *camera);

	virtual Entity* Copy() const;
	virtual axelynx::Entity* ReciveShadows(bool value, bool by_hierarhy = true);
	virtual axelynx::Entity* CastShadows(bool value, bool by_hierarhy = true);

	virtual axelynx::Entity* OnUpdate(Action *action);

	virtual bool Save(axelynx::File file) const;

	virtual ~CEntity();

	virtual axelynx::Entity* SetPickMode(axelynx::PickMode pm);
	virtual axelynx::Entity* SetVisible(bool visible);

};