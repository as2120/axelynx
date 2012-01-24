
#include "gl/axgl.h"
#include "CEarlyZ.h"
#include "CShadowPass.h"
#include "CEntity.h"
#include "CCanvas.h"
#include <map>
#include "axelynx/Shell.h"
#include <iostream>
#include "CScene.h"
#undef max

CEntity::CEntity(axelynx::Scene *scene, const axelynx::Geometry * geom)
{
	name_ = L"unnamed";

	scene_=scene;

	for(int i=0;i<8;++i)
	{
		textures_[i]=0;
	}

	num_textures_=0;

	shader_=0;

	geometry_ = geom;

	material_=0;

	bm_ = axelynx::BM_NONE;

	if(geom)
		radius_ = geom->GetRadius();
	else
		radius_ = -1;
	show_ = true;

	recive_shadows_ = true;
	cast_shadows_ = true;

	on_update_event_ = 0;

	current_pickmode_ = axelynx::PM_NONE;

	dw_mode_ = axelynx::DW_AUTO;
	dt_mode_ = axelynx::DT_AUTO;

	entitycolor_ = vec4(1,1,1,1);

	is_visible_ = true;
}

axelynx::Entity* CEntity::SetName(std::wstring name)
{
	name_ = name;
	return this;
}

std::wstring CEntity::GetName() const
{
	return name_;
}


axelynx::Entity* CEntity::SetTexture(axelynx::Texture* texture, int layer, bool by_hierarhy)
{
	if(by_hierarhy && ChildsList())
	{
		auto ci = ChildsList()->begin();
		auto ei = ChildsList()->end();

		for(;ci!=ei;++ci)
		{
			if(Entity* ent = dynamic_cast<CEntity*>(*ci))
			{
				ent->SetTexture(texture,layer,true);
			}
		}
	}

	textures_[layer] = texture;
	if(num_textures_<layer+1)
		num_textures_=layer+1;

	return this;
}

axelynx::Entity* CEntity::SetShader(axelynx::Shader* shader, bool by_hierarhy)
{
	if(by_hierarhy && ChildsList())
	{
		auto ci = ChildsList()->begin();
		auto ei = ChildsList()->end();

		for(;ci!=ei;++ci)
		{
			if(Entity* ent = dynamic_cast<CEntity*>(*ci))
			{
				ent->SetShader(shader, true);
			}
		}
	}

	shader_ = shader;

	return this;
}

const axelynx::Geometry* CEntity::GetGeometry() const
{
	return geometry_;
}

CEntity::~CEntity()
{
	if(on_update_event_)
		delete on_update_event_;
}

bool CEntity::PreparePipeline(const axelynx::Camera *camera)
{
	OPENGL_CHECK_FOR_ERRORS();

	if(!geometry_) //geometry nai
		return false;

	if(material_) //if material exisists use it, else - use standart scheme
	{
		if(CEarlyZ::InEarlyZPass() && material_->GetBlendMode() != axelynx::BM_NONE)
			return false;
			OPENGL_CHECK_FOR_ERRORS();
		material_->Bind(camera);
		if(material_->GetLocationModelViewProj()>=0)
			material_->GetShader()->SetUniform(material_->GetLocationModelViewProj(),this->GetTransformMatrix(false) * camera->GetViewProjectionMatrix());
		if(material_->GetLocationModelView()>=0)
			material_->GetShader()->SetUniform(material_->GetLocationModelView(),this->GetTransformMatrix(false) * camera->GetViewMatrix());
		if(material_->GetLocationModel()>=0)
			material_->GetShader()->SetUniform(material_->GetLocationModel(),this->GetTransformMatrix(false));
			OPENGL_CHECK_FOR_ERRORS();
		if(material_->GetLocationNormal()>=0)
		{
			axelynx::mat3 normalmatrix = this->GetTransformMatrix(false).ToMat3();
			//std::cout<<normalmatrix(0,0)<<" "<<normalmatrix(0,1)<<" "<<normalmatrix(0,2)<<std::endl;
			//std::cout<<normalmatrix(1,0)<<" "<<normalmatrix(1,1)<<" "<<normalmatrix(1,2)<<std::endl;
			//std::cout<<normalmatrix(2,0)<<" "<<normalmatrix(2,1)<<" "<<normalmatrix(2,2)<<std::endl;
			material_->GetShader()->SetUniform(material_->GetLocationNormal(),normalmatrix);
		}
			OPENGL_CHECK_FOR_ERRORS();
		if(material_->GetShader()->GetStandartUniformLocation(axelynx::Shader::SU_EYEPOS)>=0)
			material_->GetShader()->SetUniform(axelynx::Shader::SU_EYEPOS,camera->GetPosition());
			OPENGL_CHECK_FOR_ERRORS();
		material_->GetShader()->SetUniform("modelview",this->GetTransformMatrix(false) * camera->GetViewMatrix());
			OPENGL_CHECK_FOR_ERRORS();
		//using
		if(material_->isUsingEntityTextures())
		{
			for(int i=0;i<num_textures_;++i)
			{
				textures_[i]->Bind(i);
			}
		}
			OPENGL_CHECK_FOR_ERRORS();
	}
	else
	{
		if(CEarlyZ::InEarlyZPass() && bm_ != axelynx::BM_NONE)
			return false;

		if(!shader_) //shader nai
			return false;


		axelynx::Shader *shader = shader_;
	OPENGL_CHECK_FOR_ERRORS();
		if(CShadowPass::inShadowPass())
		{
			shader = CShadowPass::GetShader();
		}
	OPENGL_CHECK_FOR_ERRORS();
		shader->Bind();
		//shader_->SetUniform("model",this->GetTransformMatrix());
		//shader_->SetUniform("proj",camera->GetProjectionMatrix());
		//shader_->SetUniform("view",camera->GetViewMatrix());
			OPENGL_CHECK_FOR_ERRORS();
		int location = shader->GetStandartUniformLocation(axelynx::Shader::SU_MODELVIEWPROJ);
	OPENGL_CHECK_FOR_ERRORS();
		if(location>=0)
		{
			const axelynx::mat4 & modelviewproj = this->GetTransformMatrix() * camera->GetViewProjectionMatrix();



			shader->SetUniform(location,modelviewproj);
		}
			OPENGL_CHECK_FOR_ERRORS();
		if(shader->GetStandartUniformLocation(axelynx::Shader::SU_MODELVIEW)>=0)
			shader->SetUniform(axelynx::Shader::SU_MODELVIEW,this->GetTransformMatrix() * camera->GetViewMatrix());
	OPENGL_CHECK_FOR_ERRORS();
		if(shader->GetStandartUniformLocation(axelynx::Shader::SU_MODEL)>=0)
			shader->SetUniform(axelynx::Shader::SU_MODEL,this->GetTransformMatrix());
	OPENGL_CHECK_FOR_ERRORS();
		if(shader->GetStandartUniformLocation(axelynx::Shader::SU_ENTITYCOLOR)>=0)
			shader->SetUniform(axelynx::Shader::SU_ENTITYCOLOR,entitycolor_);
	OPENGL_CHECK_FOR_ERRORS();
		if(shader->GetStandartUniformLocation(axelynx::Shader::SU_EYEPOS)>=0)
			shader->SetUniform(axelynx::Shader::SU_EYEPOS,camera->GetPosition());
	OPENGL_CHECK_FOR_ERRORS();
		if(shader->GetStandartUniformLocation(axelynx::Shader::SU_NORMALMATRIX)>=0)
		{
			axelynx::mat3 normalmatrix = this->GetTransformMatrix(false).ToMat3();
			shader->SetUniform(axelynx::Shader::SU_NORMALMATRIX,normalmatrix);
		}
	OPENGL_CHECK_FOR_ERRORS();
		for(int i=0;i<num_textures_;++i)
		{
			textures_[i]->Bind(i);
		}
	OPENGL_CHECK_FOR_ERRORS();
		CCanvas::Instance()->SetBlendMode(bm_);
	OPENGL_CHECK_FOR_ERRORS();
		if(!CEarlyZ::OnEarlyZPass())
		{
			if(dw_mode_ == axelynx::DW_AUTO)
			{
				if(bm_ == axelynx::BM_NONE)
					glDepthMask(GL_TRUE);
				else
					glDepthMask(GL_FALSE);
			}
			else
			{
				if(dw_mode_ == axelynx::DW_WRITE)
					glDepthMask(GL_TRUE);
				else
					glDepthMask(GL_FALSE);
			}
		}
	}
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}

bool CEntity::Draw(const axelynx::Camera *camera)
{
	OPENGL_CHECK_FOR_ERRORS();
	PreparePipeline(camera);

	OPENGL_CHECK_FOR_ERRORS();
	geometry_->Draw();
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}

axelynx::Entity* CEntity::SetBlend(axelynx::BlendMode bm, bool by_hierarhy)
{
	if(by_hierarhy && ChildsList())
	{
		auto ci = ChildsList()->begin();
		auto ei = ChildsList()->end();

		for(;ci!=ei;++ci)
		{
			if(Entity* ent = dynamic_cast<CEntity*>(*ci))
			{
				ent->SetBlend(bm, true);
			}
		}
	}

	bm_ = bm;

	return this;
}

axelynx::Entity* CEntity::SetMaterial(const axelynx::Material * material, bool by_hierarhy)
{

	if(by_hierarhy && ChildsList())
	{
		auto ci = ChildsList()->begin();
		auto ei = ChildsList()->end();

		for(;ci!=ei;++ci)
		{
			if(Entity* ent = dynamic_cast<CEntity*>(*ci))
			{
				ent->SetMaterial(material, true);
			}
		}
	}

	material_ = material;

	return this;
}

Entity*  CEntity::SetColor(const axelynx::vec4& color)
{
	entitycolor_ = color;
	return this;
}

bool CEntity::inFrustum(const axelynx::frustum &f)
{
	if(!is_visible_)
		return false;

	if(radius_<0)
		return true;
	const axelynx::vec3 &scale = this->GetScale();

	return f.CheckSphere(this->GetPosition(true,false),radius_ * std::max(scale.x,std::max(scale.y,scale.z)));
}

axelynx::Entity* CEntity::Show(bool value)
{
	if(value)
	{
		if(!show_)
		{
			scene_->GetSceneGraph()->AddEntity(this);
		}
	}
	else
	{
		if(show_)
		{
			scene_->GetSceneGraph()->Remove(this);
		}
	}

	return this;
}

axelynx::Entity* CEntity::ReciveShadows(bool value, bool by_hierarhy)
{
	if(by_hierarhy && ChildsList())
	{
		auto ci = ChildsList()->begin();
		auto ei = ChildsList()->end();

		for(;ci!=ei;++ci)
		{
			if(Entity* ent = dynamic_cast<CEntity*>(*ci))
			{
				ent->ReciveShadows(value, true);
			}
		}
	}

	recive_shadows_ = true;
	return this;
}

axelynx::Entity* CEntity::CastShadows(bool value, bool by_hierarhy)
{
	cast_shadows_ = true;
	return this;
}

bool CEntity::Update(float twin)
{
	bool need_remove = false;

	if(on_update_event_)
		need_remove = on_update_event_->operator()(this,twin);

	return need_remove;
}

axelynx::Entity* CEntity::OnUpdate(Action *action)
{
	on_update_event_ = action;

	return this;
}

axelynx::Entity* CEntity::Copy() const
{
	CEntity *ent = dynamic_cast<CEntity*>(this->scene_->Add(this->geometry_));
	ent->bm_ = this->bm_;
	ent->cast_shadows_ = this->cast_shadows_;
	ent->material_ = this->material_;
	ent->name_ = L"copy of " + this->name_;
	ent->num_textures_ = this->num_textures_;
	ent->on_update_event_ = this->on_update_event_;
	ent->radius_ = this->radius_;
	ent->recive_shadows_ = this->recive_shadows_;
	ent->shader_ = this->shader_;
	ent->show_ = this->show_;

	for(int i=0;i<8;++i)
		ent->textures_[i] = this->textures_[i];

	//ent->SetPosition(this->GetPosition());
	//ent->SetOrientation(this->GetOrientation());
	ent->SetScale(this->GetScale());
	ent->SetParent(this->GetParent());

	ent->is_visible_ = this->is_visible_;
	ent->entitycolor_ = this->entitycolor_;

	ent->on_update_event_ = this->on_update_event_;

	ent->current_pickmode_=this->current_pickmode_;
	ent->dw_mode_=this->dw_mode_;
	ent->dt_mode_=this->dt_mode_;

	return ent;
}

axelynx::Entity* CEntity::FindByName(const std::wstring &name, bool by_hierarhy)
{
	if(name == name_)
		return this;

	if(!ChildsList())
		return 0;

	auto ci = ChildsList()->begin();
	auto ei = ChildsList()->end();

	for(;ci!=ei;++ci)
	{
		if(CEntity* ent = dynamic_cast<CEntity*>(*ci))
		{
			if(ent->GetName() == name)
				return ent;
			else
			{
			}

		}
	}

	if(by_hierarhy)
	{
		auto ci = ChildsList()->begin();
		auto ei = ChildsList()->end();

		for(;ci!=ei;++ci)
		{
			if(CEntity* ent = dynamic_cast<CEntity*>(*ci))
			{
				CEntity* finded = dynamic_cast<CEntity*>(ent->FindByName(name,true));
				if(finded)
					return finded;
			}
		}

	}
	return 0;
}


bool CEntity::Save(axelynx::File file) const
{
	/*typedef std::map<const axelynx::Texture*,int> Textures_id;
	typedef std::map<const axelynx::Geometry*,int> Geometries_id;

	Textures_id textures_id;
	Geometries_id geometries_id;

	int current_tex_id = 0;
	int current_geometry_id = 0;

	geometries_id[geometry_] = current_geometry_id++;
	for(int i=0;i<num_textures_;++i)
	{
		textures_id[textures_[i]] = current_tex_id++;
	}*/

	//axelynx::Texture *textures_[8];
	//int num_textures_;

	//axelynx::Shader *shader_;
	//std::wstring name_;

	//const axelynx::Geometry *geometry_;

	//axelynx::BlendMode bm_;
	//const axelynx::Material *material_;

	//float radius_;
	//bool show_;

	//bool recive_shadows_;
	//bool cast_shadows_;

	//Action *on_update_event_;

	file.Write(num_textures_);
	file.Write(show_);
	file.Write(radius_);
	file.Write(bm_);
	file.Write(name_);
	file.Write(GetPosition(false));
	file.Write(GetOrientation(false));
	file.Write(GetScale(false));
	if(geometry_)
	{
		file.WriteString(geometry_->GetClass());
		geometry_->WriteBinary(file);
	}
	else
	{
		file.WriteString(L"NOGEOM");
	}

	if(ChildsList())
	{
		file.Write((int)ChildsList()->size());

		auto ci = ChildsList()->begin();
		auto ei = ChildsList()->end();
		for(;ci!=ei;++ci)
		{
			const CEntity *cent = dynamic_cast<const CEntity*>(*ci);
			cent->Save(file);
		}
	}
	else
	{
		file.Write(0);
	}
	return true;
}

axelynx::Entity* CEntity::SetPickMode(axelynx::PickMode pm)
{
	if(current_pickmode_ != pm)
	{
		CScene *scene = dynamic_cast<CScene*>(scene_);
		scene->EntityPickMode(this,pm);
	}

	current_pickmode_ = pm;

	return this;
}

axelynx::Entity* CEntity::SetDepthWriteMode(axelynx::DepthWriteMode dwm)
{
	dw_mode_  =dwm;
	return this;
}

axelynx::Entity* CEntity::SetDepthTestMode(axelynx::DepthTestMode dtm)
{
	dt_mode_  =dtm;
	return this;
}

axelynx::Entity* CEntity::SetVisible(bool visible)
{
	is_visible_  =visible;
	return this;
}
