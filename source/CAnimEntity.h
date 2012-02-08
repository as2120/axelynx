#pragma once

#include "axelynx/AnimEntity.h"
#include "CEntity.h"


class CAnimEntity : public virtual axelynx::AnimEntity, public CEntity
{
	float frame_;
public:
	CAnimEntity(axelynx::Scene *scene_, const axelynx::AnimGeometry * geom);
	virtual bool Draw(const axelynx::Camera *camera);

	virtual ~CAnimEntity();
	virtual void SetSpeedFactor(float speedfactor);
	virtual const axelynx::AnimGeometry *GetGeometry() const;
	virtual bool Update(float twin = 1.0f);
		//wraping to CEntity
		virtual std::wstring GetName() const
		{
			return CEntity::GetName();
		}
		virtual bool inFrustum(const axelynx::frustum &f)
		{
			return CEntity::inFrustum(f);
		}

		virtual axelynx::AnimEntity* Copy() const
		{
			return new CAnimEntity(*this);
		}


		virtual axelynx::Entity* Show(bool value = true)
		{
			return CEntity::Show(value);
		}

		virtual axelynx::Entity* SetMaterial(axelynx::Material * material, bool by_hierarhy = true)
		{
			return CEntity::SetMaterial(material, by_hierarhy);
		}
		virtual axelynx::Entity* SetTexture(axelynx::Texture* texture, int layer=0, bool by_hierarhy = true)
		{
			return CEntity::SetTexture(texture, layer, by_hierarhy);
		}

		virtual axelynx::Entity* SetShader(axelynx::Shader* shader, bool by_hierarhy = true)
		{
					return CEntity::SetShader(shader, by_hierarhy);
		}
		virtual axelynx::Entity* SetBlend(axelynx::BlendMode bm, bool by_hierarhy = true)
		{
					return CEntity::SetBlend(bm, by_hierarhy);
		}
		virtual axelynx::Entity* SetName(std::wstring name)
		{
			return CEntity::SetName(name);
		}
		virtual axelynx::Entity* ReciveShadows(bool value, bool by_hierarhy = true)
		{
		return CEntity::ReciveShadows(value, by_hierarhy);
		}
		virtual axelynx::Entity* CastShadows(bool value, bool by_hierarhy = true)
		{
		return CEntity::CastShadows(value, by_hierarhy);
		}
		virtual axelynx::Entity* OnUpdate(Action *action)
		{
			return CEntity::OnUpdate(action);
		}

		virtual axelynx::Entity* FindByName(const std::wstring &name, bool by_hierarhy = true)
		{
			return CEntity::FindByName(name,by_hierarhy);
		}
		axelynx::Entity* Hide() { return Show(false); }

		virtual axelynx::Entity* SetColor(const axelynx::vec4& color)
		{
			return CEntity::SetColor(color);
		}

	virtual bool Save(axelynx::File file) const
	{
		return CEntity::Save(file);
	}


	virtual axelynx::Entity* SetPickMode(axelynx::PickMode pm)
	{
		return CEntity::SetPickMode(pm);
	}
	virtual axelynx::Entity* SetVisible(bool visible)
	{
		return CEntity::SetVisible(visible);
	}

	virtual axelynx::Entity* SetDepthWriteMode(axelynx::DepthWriteMode dwm)
	{
		return CEntity::SetDepthWriteMode(dwm);
	}

	virtual axelynx::Entity* SetDepthTestMode(axelynx::DepthTestMode dtm)
	{
		return CEntity::SetDepthTestMode(dtm);
	}
};