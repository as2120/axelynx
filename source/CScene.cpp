

#include "CScene.h"
#include "CCamera.h"
#include "CEntity.h"
#include "CPivot.h"
#include "CShader.h"
#include "CTexture.h"
#include "gl/axgl.h"
#include "CMaterial.h"
#include "CLight.h"
#include "CEarlyZ.h"
#include "libs/tinyxml/tinyxml.h"
#include "axelynx/utils/axstring.h"
#include "CSurface.h"
#include "axelynx/StandartSurfaces.h"
#include "CEntity.h"
#include <iostream>
#include <vector>
#include "axelynx/math/axmath.h"
#include "CShadowPass.h"
#include "CAnimEntity.h"

int CScene::frame_ = 0;
CScene::CScene(axelynx::SceneGraph *scenegraph)
{
	scenegraph_ = scenegraph;
	zpassed = false;
}

bool CScene::Render()
{
	std::list<axelynx::Camera *>::const_iterator ci = camlist.begin();
	std::list<axelynx::Camera *>::const_iterator ei = camlist.end();

	OPENGL_CHECK_FOR_ERRORS();
	for(;ci!=ei;++ci)
	{
		if((*ci)->isEnabled())
		{
			frame_++;
			(*ci)->Bind(zpassed?-1:0);
			OPENGL_CHECK_FOR_ERRORS();
			CMaterial::Free(); //материал зависит от камеры
			OPENGL_CHECK_FOR_ERRORS();
			if(zpassed)
			{
				CEarlyZ::StartRenderPass();
			}
			else
			{
				glDepthFunc(GL_LESS);
				glDepthMask(GL_TRUE);
			}
			OPENGL_CHECK_FOR_ERRORS();
			scenegraph_->Render(*ci);
			(*ci)->UnBind();
			CMaterial::Free(); //материал зависит от камеры
		}
	}

	OPENGL_CHECK_FOR_ERRORS();
	CShader *cs = CShader::Current();

	if(cs)
		cs->UnBind();

	for(int i=0;i<16;++i)
	{
		CTexture *ct = CTexture::Current(i);
		if(ct)
			ct->UnBind();
	}

	OPENGL_CHECK_FOR_ERRORS();
	if(zpassed)
		CEarlyZ::EndRenderPass();
	OPENGL_CHECK_FOR_ERRORS();
	zpassed = false;
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}

bool CScene::ZPassRender()
{
	std::list<axelynx::Camera *>::const_iterator ci = camlist.begin();
	std::list<axelynx::Camera *>::const_iterator ei = camlist.end();

	CEarlyZ::StartEarlyZPass();
	for(;ci!=ei;++ci)
	{
		if((*ci)->isEnabled())
		{
			frame_++;
			(*ci)->Bind();
			CMaterial::Free(); //материал зависит от камеры
			OPENGL_CHECK_FOR_ERRORS();
			scenegraph_->Render(*ci);
						(*ci)->UnBind();
			CMaterial::Free(); //материал зависит от камеры
		}
	}

	CShader *cs = CShader::Current();

	if(cs)
		cs->UnBind();

	for(int i=0;i<16;++i)
	{
		CTexture *ct = CTexture::Current(i);
		if(ct)
			ct->UnBind();
	}
	CEarlyZ::EndEarlyZPass();

	zpassed = true;
	return true;
}

bool CScene::Update(float twin)
{
	scenegraph_->Update(twin);
//	scenegraph_->Update();
	return true;
}

axelynx::Pivot* CScene::AddPivot()
{
	return new axelynx::Pivot();
}

axelynx::Camera* CScene::AddCamera()
{
	CCamera *cam = new CCamera();
	cam->SetScene(this);
	//cam->Ortho(-100,100,-100,100,-100,100);
	bool main_camera = camlist.empty();

	cam->SetEnable(main_camera);

	camlist.push_back(cam);
	return cam;
}

axelynx::Entity* CScene::Add(const axelynx::Geometry *geometry)
{
	CEntity *ent = new CEntity(this, geometry);
	scenegraph_ -> AddEntity(ent);
	return ent;
}

axelynx::AnimEntity* CScene::Add(const axelynx::AnimGeometry *geometry)
{
	CAnimEntity *ent = new CAnimEntity(this, geometry);
	scenegraph_ -> AddEntity(ent);
	return ent;
}

bool CScene::Remove(axelynx::Pivot *pivot)
{
	axelynx::Entity *ent = dynamic_cast<axelynx::Entity*>(pivot);

	if(ent)
	{
		scenegraph_ -> Remove(ent);
	}
	else
	{
		axelynx::Camera *cam = dynamic_cast<axelynx::Camera*>(pivot);

		if(cam)
		{
			camlist.remove(cam);
		}

		axelynx::Light *light = dynamic_cast<axelynx::Light*>(pivot);
		if(light)
		{
		}
	}

	return true;
}

axelynx::Light* CScene::CreateLight(axelynx::Light::Mode mode)
{
	return 0;//new CLight(mode);
}

axelynx::Scene* CScene::Shared(axelynx::Camera* shared)
{
	camlist.push_back(shared);
	return this;
}

axelynx::Scene* CScene::Shared(axelynx::Entity* shared)
{
	scenegraph_->AddEntity(shared);
	return this;
}

axelynx::Scene* CScene::Shared(axelynx::Light* shared)
{
	lightlist.push_back(shared);
	return this;
}

struct SurfaceLib
{
public:
    std::string name;
    CSurface *surface;
};

axelynx::Entity* CScene::LoadEntity(axelynx::File file)
{
	axelynx::Entity* root_node = new CEntity(this,0);

	class VertexAttribute
	{
	public:
		std::string name;
		float* values;
		int size;
		int components;
		std::string semantic;
		int layer;
		int offset;

		VertexAttribute()
			:semantic("unknow"),name(""),layer(0),offset(0),values(0),size(0),components(0)
		{
		}
	};

	char filename[260];
	axelynx::utils::WideCharToMultiByte(file.GetFilename().c_str(),filename);
	std::cout<<"filename:"<<filename<<std::endl;
	TiXmlDocument doc(filename);

	bool ok = doc.LoadFile();
	std::cout<<"ok:"<<ok<<std::endl;
	//doc.ToDeclaration();
	TiXmlElement *root = doc.FirstChildElement("COLLADA");

	std::cout<<root->FirstAttribute()->Value()<<std::endl;

	TiXmlElement *geometries = root->FirstChildElement("library_geometries");
	TiXmlElement *geometry = geometries->FirstChildElement("geometry");

	std::vector<SurfaceLib> surfaces;

	VertexAttribute va[16];

	while(geometry)
	{
		int cur_va = 0;
		std::cout<<"id:"<<geometry->Attribute("id")<<" name:"<<geometry->Attribute("name")<<std::endl;

		TiXmlElement *mesh = geometry->FirstChildElement("mesh");
		TiXmlElement *source = mesh->FirstChildElement("source");

		SurfaceLib slib;
		slib.name = geometry->Attribute("id");

		while(source)
		{
			TiXmlElement *float_array = source->FirstChildElement("float_array");
			TiXmlElement *accessor = source->FirstChildElement("technique_common")->FirstChildElement("accessor");

			int count;
			float_array->Attribute("count",&count);

			std::cout<<" source id:"<<source->Attribute("id")<<"array size:"<<count<<std::endl;
			va[cur_va].size = count;
			va[cur_va].name = source->Attribute("id");
			va[cur_va].values = new float[count];

			accessor->QueryIntAttribute("stride",&va[cur_va].components);
			const char *txt = float_array->GetText();
			int txt_len = strlen(txt);
			setlocale(LC_NUMERIC, "C");
			sscanf(txt,"%f",&va[cur_va].values[0]);

			int curelement = 0;

			for(int i=0;i<txt_len;++i)
			{
				if(txt[i] == ' ')
				{
					curelement++;
					sscanf(txt+i+1,"%f",&va[cur_va].values[curelement]);
				}
			}

			++cur_va;
			source = source->NextSiblingElement("source");
		}

		TiXmlElement *vertices_link = mesh->FirstChildElement("vertices");
		std::string vertices_id = "#" + (std::string)vertices_link->Attribute("id");
		std::string vertices_pos = vertices_link->FirstChildElement("input")->Attribute("source");

		TiXmlElement *polygons = mesh->FirstChildElement("polygons");
		int count;
		polygons->Attribute("count",&count);


		TiXmlElement *input = polygons->FirstChildElement("input");
		int count_semantics = 0;

		while(input)
		{

			for(int i=0;i<16;++i)
			{
				if(std::string("#")+va[i].name == std::string(input->Attribute("source"))
					|| (std::string("#")+va[i].name == vertices_pos && vertices_id == std::string(input->Attribute("source"))))
				{
					va[i].semantic = input->Attribute("semantic");
					input->QueryIntAttribute("offset",&va[i].offset);

					if(va[i].semantic == "TEXCOORD")
					{
						input->QueryIntAttribute("set",&va[i].layer);
					}
				}
			}

			input = input->NextSiblingElement("input");

			count_semantics++;
		}

		slib.surface = new CSurface(count*3,count*3);

		TiXmlElement *tris = polygons->FirstChildElement("p");
		slib.surface->Lock();

		std::vector<int> indices;
		indices.resize(count * count_semantics * 3);

		int triindex=0;
		int curelement = 0;

		while(tris)
		{
			const char *txt = tris->GetText();

			int txt_len = strlen(txt);
			//setlocale(LC_NUMERIC, "C");

			sscanf(txt,"%d",&indices.at(curelement));

			curelement++;
			for(int i=0;i<txt_len;++i)
			{
				if(txt[i] == ' ')
				{

					sscanf(txt+i+1,"%d",&indices.at(curelement));
					curelement++;
				}
			}

			slib.surface->SetTriangle(triindex,triindex*3,triindex*3+1,triindex*3+2);
			triindex++;

			tris = tris->NextSiblingElement("p");
		}

		int allstride = count_semantics * 3;

		for(int i=0;i<count_semantics;++i)
		{
			if(va[i].semantic == "VERTEX")
			{
				for(int j=0;j<count*3;++j)
				{
					int vindex = indices[j * count_semantics + va[i].offset];

					slib.surface->SetVertexPosition(j
						,va[i].values[vindex * 3 +0]
						,va[i].values[vindex * 3 +1]
						,va[i].values[vindex * 3 +2]);
				}
			}

			if(va[i].semantic == "NORMAL")
			{
				for(int j=0;j<count*3;++j)
				{
					int vindex = indices[j * count_semantics + va[i].offset];

					slib.surface->SetVertexNormal(j
						,va[i].values[vindex * 3 +0]
						,va[i].values[vindex * 3 +1]
						,va[i].values[vindex * 3 +2]);
				}
			}

			if(va[i].semantic == "TEXCOORD")
			{
				for(int j=0;j<count*3;++j)
				{
					int vindex = indices[j * count_semantics + va[i].offset];

					slib.surface->SetVertexTexCoord(j
						,va[i].values[vindex * 2 +0]
						,va[i].values[vindex * 2 +1]
						,va[i].layer);
				}
			}
		}

		//delete[] indices;
		slib.surface->UnLock();
		//root_node->AddChild(node);
		geometry = geometry->NextSiblingElement();

		surfaces.push_back(slib);
	}

	TiXmlElement *entities = root->FirstChildElement("library_visual_scenes")->FirstChildElement("visual_scene")->FirstChildElement("node");

	axelynx::Entity* parent_node = root_node;

	class ProcessNodeEntity
	{
	public:
		static void Process(TiXmlElement *entities, axelynx::Entity *parent_node,CScene* scene, std::vector<SurfaceLib>& surfaces)
		{
			std::string id = entities->Attribute("id");

			wchar_t wbuff[255];
			axelynx::utils::MultiByteToWideChar(entities->Attribute("name"),wbuff);
			std::wstring name = wbuff;

			CSurface *geom = 0;
			for(unsigned int i=0;i<surfaces.size();++i)
			{
				TiXmlElement *geometry = (entities->FirstChildElement("instance_geometry"));

				if(geometry && std::string("#")+surfaces[i].name == std::string(geometry->Attribute("url")))
				{
					geom = surfaces[i].surface;
				}
			}

			axelynx::Entity* node = 0;

			if(geom)
			{
				node = scene->Add(geom);
			}
			else
			{
				node = new CEntity(scene,0);	 //pivot
			}


			TiXmlElement *scale = entities->FirstChildElement("scale");
			TiXmlElement *translate = entities->FirstChildElement("translate");

			float x=0,y=0,z=0;

			if(scale)
			{
				sscanf(scale->GetText(),"%f %f %f",&x,&y,&z);
				std::cout<<"scale: "<<x<<" - "<<y<<" - "<<z<<std::endl;
				node->SetScale(z,y,z);
			}

			if(translate)
			{
				sscanf(translate->GetText(),"%f %f %f",&x,&y,&z);
				std::cout<<"translate: "<<x<<" - "<<y<<" - "<<z<<std::endl;
				node->SetPosition(x,y,z);
			}

			std::wcout<<"node name:"<<name<<std::endl;
			node->SetName(name);
			parent_node->AddChild(node);
			TiXmlElement *node_element = entities->FirstChildElement("node");
			if(node_element)
				Process(node_element,node,scene,surfaces);
		}
	};

	while(entities)
	{
		ProcessNodeEntity::Process(entities,root_node,this,surfaces);

		entities = entities->NextSiblingElement("node");
	}

	//root_node = this->Add(surfaces[0].surface);



	axelynx::Surface *s =axelynx::StandartSurfaces::Cube();

	//axelynx::Entity *fake_cube = this->Add(s);
	//root_node->AddChild(fake_cube);

	return root_node;
}


axelynx::Entity* CScene::RestoreEntity(axelynx::File file)
{
	file.Open();

	file.Close();
	return 0;
}

bool CScene::EntityPickMode(CEntity* ent, axelynx::PickMode pm)
{
	if(pm == axelynx::PM_NONE)
	{
		pickedlist.remove(ent);
	}
	else
	{
		pickedlist.push_back(ent);
	}

	return true;
}

axelynx::PickResult CScene::Pick(axelynx::vec3 start, axelynx::vec3 end)
{
	axelynx::PickResult result;
	result.ent = 0;

	auto ci = pickedlist.begin();
	auto ei = pickedlist.end();

	for(;ci!=ei;++ci)
	{
		float r = (*ci)->GetGeometry()->GetRadius();

		if(r>=0)
		{
			axelynx::vec4 radius = axelynx::vec4((*ci)->GetPosition(),r);
			if(axelynx::LineSphereIntersect(start,end-start,radius))
			{
				result.ent = (*ci);
				return result;
			}
		}
	}

	return result;
}

bool CScene::SetShadowShader(axelynx::Shader *shader)
{
	defShader_ = shader;
	return true;
}

bool CScene::RenderShadow()
{
	std::list<axelynx::Camera *>::const_iterator ci = camlist.begin();
	std::list<axelynx::Camera *>::const_iterator ei = camlist.end();

	CShadowPass::SetShadowShader(defShader_);
	CShadowPass::StartPass();

	for(;ci!=ei;++ci)
	{
		if((*ci)->isEnabled())
		{
			frame_++;
			(*ci)->Bind(zpassed?-1:0);
			CMaterial::Free(); //материал зависит от камеры
			if(zpassed)
			{
				CEarlyZ::StartRenderPass();
			}
			else
			{
				glDepthFunc(GL_LESS);
				glDepthMask(GL_TRUE);
			}
			scenegraph_->Render(*ci);
						(*ci)->UnBind();
			CMaterial::Free(); //материал зависит от камеры
		}
	}

	CShader *cs = CShader::Current();

	if(cs)
		cs->UnBind();

	for(int i=0;i<16;++i)
	{
		CTexture *ct = CTexture::Current(i);
		if(ct)
			ct->UnBind();
	}

	if(zpassed)
		CEarlyZ::EndRenderPass();

	zpassed = false;
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);

	CShadowPass::EndPass();

	return true;
}

unsigned int CScene::CountEntities() const
{
	return scenegraph_->CountEntities();
}

unsigned int CScene::CountCameres() const
{
	return camlist.size();
}

unsigned int CScene::CountLights() const
{
	return lightlist.size();
}
