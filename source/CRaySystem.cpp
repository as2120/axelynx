

#include "CRaySystem.h"
#include "CStatistics.h"
#include "CPivot.h"
#include <cassert>
#include "saSpec.h"
#include <iostream>
#include "CCamera.h"
#include "CShader.h"
#include <process.h>


CRaySystem::CRaySystem(int capacity_)
{
	assert(capacity_>0);

	//instances_.reserve(capacity_);
	//positions_.reserve(capacity_);

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	// создадим и используем Vertex Buffer Object (VBO)
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	glVertexAttribPointer(VA_POSITION, 3, GL_FLOAT, GL_FALSE,	sizeof(Ray), reinterpret_cast<const GLvoid*>(0));
	glEnableVertexAttribArray(VA_POSITION);

	glVertexAttribPointer(VA_POINT_SIZE, 1, GL_FLOAT, GL_FALSE,sizeof(Ray), reinterpret_cast<const GLvoid*>(12));
	glEnableVertexAttribArray(VA_POINT_SIZE);

	glVertexAttribPointer(VA_COLOR, 4, GL_FLOAT, GL_FALSE,sizeof(Ray), reinterpret_cast<const GLvoid*>(16));
	glEnableVertexAttribArray(VA_COLOR);

	glGenVertexArrays(1, &vao_volumetric_);
	glBindVertexArray(vao_volumetric_);

	// создадим и используем Vertex Buffer Object (VBO)
	glGenBuffers(1, &vbo_volumetric_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_volumetric_);

	glGenBuffers(1, &ibo_volumetric_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_volumetric_);

	int cnt_quads = 100000;
	int cnt_indices = cnt_quads * 6;
	 
	GLuint *indices = new GLuint[cnt_indices];

	for(int q = 0; q < cnt_quads; ++q)
	{
		int i = q * 6;
		int v = q * 4;
		//first tris
		indices[i+0] = v + 0;
		indices[i+1] = v + 1;
		indices[i+2] = v + 2;
		//second tris
		indices[i+3] = v + 0;
		indices[i+4] = v + 2;
		indices[i+5] = v + 3;
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLuint) * cnt_indices,indices,GL_STATIC_DRAW);
	delete[] indices;

	glVertexAttribPointer(VA_POSITION, 3, GL_FLOAT, GL_FALSE,	sizeof(VolumetricRay) / 4, reinterpret_cast<const GLvoid*>(0));
	glEnableVertexAttribArray(VA_POSITION);

	glVertexAttribPointer(VA_COLOR, 4, GL_FLOAT, GL_FALSE,sizeof(VolumetricRay) / 4, reinterpret_cast<const GLvoid*>(12));
	glEnableVertexAttribArray(VA_COLOR);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,sizeof(VolumetricRay) / 4, reinterpret_cast<const GLvoid*>(28));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,sizeof(VolumetricRay) / 4, reinterpret_cast<const GLvoid*>(40));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE,sizeof(VolumetricRay) / 4, reinterpret_cast<const GLvoid*>(48));
	glEnableVertexAttribArray(4);

	std::cout<<"size of sizeof(VolumetricRay):"<<sizeof(VolumetricRay)<<std::endl;

	recalcmode_ = axelynx::RaySystem::RM_ONDRAW;
	rendertype_ = axelynx::RaySystem::RT_FAKE;

	old_size_  = -1;
}

void CRaySystem::MTRecalc(void *param)
{

}

bool CRaySystem::Recalc() const
{
	if(rendertype_ == axelynx::RaySystem::RT_FAKE)
	{
		Pivots::const_iterator pi = instances_.begin();
		Pivots::const_iterator epi = instances_.end();
	
		positions_.clear();

		auto sz = instances_.size();
		if(positions_.size()<sz)
			positions_.resize(sz);



		for(unsigned int i = 0; i<sz; ++i)
		{
			Ray sprite;

			const axelynx::vec3& pos0 = instances_[i]->StartPoint();
			const axelynx::vec3& pos1 = instances_[i]->EndPoint();

		
			//std::cout<<"pos0 x:"<<pos0.x<<" y: "<<pos0.y<<" z: "<<pos0.z<<" - ";
			//std::cout<<"pos1 x:"<<pos1.x<<" y: "<<pos1.y<<" z: "<<pos1.z<<std::endl;
			sprite.v0 = pos0;
			sprite.s0 = instances_[i]->GetWidth();
			sprite.color0 = instances_[i]->GetColor();

			sprite.v1 = pos1;
			sprite.s1 = instances_[i]->GetWidth();
			sprite.color1 = instances_[i]->GetColor();

			positions_[i] = sprite;
		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo_);

		int c_size = positions_.size() * sizeof(Ray);

		if(c_size>old_size_)
		{
			glBufferData(GL_ARRAY_BUFFER,positions_.size() * sizeof(Ray),positions_.data(),GL_STATIC_DRAW);
			old_size_ = c_size;
		}
		else
		{
			glBufferSubData(GL_ARRAY_BUFFER,0,positions_.size() * sizeof(Ray),positions_.data());
		}

		CStatistics::dips++;
		CStatistics::tris_rendered += CountTriangles();
		CStatistics::vertices_rendered += positions_.size();
	}

	if(rendertype_ == axelynx::RaySystem::RT_VOLUMETRIC)
	{
		
		int sz = instances_.size();
		if(volumetric_positions_.size()!=sz)
		{
			volumetric_positions_.clear();
			volumetric_positions_.resize(sz);
		}

		for(int i = 0; i<sz; ++i)
		{
			using namespace axelynx;

			VolumetricRay& sprite=volumetric_positions_[i];

			const axelynx::vec3& pos0 = instances_[i]->StartPoint();
			const axelynx::vec3& pos1 = instances_[i]->EndPoint();

		
			//std::cout<<"pos0 x:"<<pos0.x<<" y: "<<pos0.y<<" z: "<<pos0.z<<" - ";
			//std::cout<<"pos1 x:"<<pos1.x<<" y: "<<pos1.y<<" z: "<<pos1.z<<std::endl;

			float fVolumeWidth = instances_[i]->GetWidth(); 
			float fHalfVolumeWidth = fVolumeWidth * 0.5f;
			//std::cout<<"fwidth:"<<instances_[i]->color_.x<<":"<<instances_[i]->color_.y<<":"<<instances_[i]->color_.z<<":"<<instances_[i]->color_.w<<":"<<std::endl;

			vec4 color = instances_[i]->GetColor();
			sprite.v[0].pos = pos0;
			sprite.v[0].color = color;
			sprite.v[0].flow.x = 0;
			sprite.v[0].flow.y = 0;
			sprite.v[0].flow.z = fVolumeWidth;

			sprite.v[0].size.x = -fVolumeWidth;
			sprite.v[0].size.y = fHalfVolumeWidth;
			sprite.v[0].endpos = pos1;

			sprite.v[1].pos = pos1;
			sprite.v[1].color = color;
			sprite.v[1].flow.x = 0.25f;
			sprite.v[1].flow.y = 0.0f;
			sprite.v[1].flow.z = fVolumeWidth;
			sprite.v[1].size.x = fVolumeWidth;
			sprite.v[1].size.y =fHalfVolumeWidth;
			sprite.v[1].endpos = pos0;

			sprite.v[2].pos = pos1;
			sprite.v[2].color = color;
			sprite.v[2].flow.x = 0.25f;
			sprite.v[2].flow.y = 0.25f;
			sprite.v[2].flow.z = fVolumeWidth;
			sprite.v[2].size.x = -fVolumeWidth;
			sprite.v[2].size.y =fHalfVolumeWidth;
			sprite.v[2].endpos = pos0;

			sprite.v[3].pos = pos0;
			sprite.v[3].color = color;
			sprite.v[3].flow.x = 0.0f;
			sprite.v[3].flow.y = 0.25f;
			sprite.v[3].flow.z = fVolumeWidth;
			sprite.v[3].size.x = fVolumeWidth;
			sprite.v[3].size.x = fHalfVolumeWidth;
			sprite.v[3].endpos = pos1;
		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo_volumetric_);

		int c_size = volumetric_positions_.size() * sizeof(VolumetricRay);

		if(c_size>old_size_)
		{
			glBufferData(GL_ARRAY_BUFFER,volumetric_positions_.size() * sizeof(VolumetricRay),volumetric_positions_.data(),GL_STATIC_DRAW);
			old_size_ = c_size;
		}
		else
		{
			glBufferSubData(GL_ARRAY_BUFFER,0,volumetric_positions_.size() * sizeof(VolumetricRay),volumetric_positions_.data());
		}

	}
	return true;
}

bool CRaySystem::Draw() const
{
	if(recalcmode_ == axelynx::RaySystem::RM_ONDRAW)
	{
		CRaySystem::Recalc();
	}

	if(rendertype_ == axelynx::RaySystem::RT_FAKE)
	{
		glBindVertexArray(vao_);

		glDrawArrays(GL_LINES,0,positions_.size());
	}

	if(rendertype_ == axelynx::RaySystem::RT_VOLUMETRIC)
	{
		glDisable(GL_CULL_FACE);
		glBindVertexArray(vao_volumetric_);

		glEnable(GL_POINT_SPRITE);
		glPointSize(32);

		//std::cout<<volumetric_positions_.size()<<std::endl;
		//glDrawArrays(GL_QUADS,0,volumetric_positions_.size());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo_volumetric_);
		glBindBuffer(GL_ARRAY_BUFFER,vbo_volumetric_);

		int size = volumetric_positions_.size();

		glDrawRangeElements(GL_TRIANGLES,0,size*6 -1,size*6,GL_UNSIGNED_INT,0);

		CStatistics::dips++;
		CStatistics::tris_rendered += size *2;
		CStatistics::vertices_rendered += size *4;
	}

	return true;
}

int CRaySystem::CountTriangles() const
{
	return positions_.size() * 8;
}

int CRaySystem::CountVertices() const
{
	return positions_.size();
}

axelynx::RaySystem::Ray* CRaySystem::AddInstance()
{
	CRay *piv = new CRay;
	piv->SetColor(1.0f,1.0f,1.0f,1.0f);
	piv->SetWidth(1.0f);
	piv->SetLenght(128.0f);

	instances_.push_back(piv);
	
	return piv;
}

axelynx::RaySystem::LightRay* CRaySystem::AddLightRay()
{
	CLightRay *piv = new CLightRay;
	piv->SetColor(1.0f,1.0f,1.0f,1.0f);
	piv->SetWidth(1.0f);
	//piv->SetLenght(128.0f);

	instances_.push_back(piv);
	
	return piv;
}

bool CRaySystem::RemoveInstance(axelynx::RaySystem::LightRay* pivot)
{
	bool removed = false;
	//exchange find element with last
	Pivots::iterator ei = instances_.end();
	for(Pivots::iterator i = instances_.begin(); i != ei; ++i)
	{
		if(*i == pivot)
		{
			*i = *ei;
			removed = true;
			break;
		}
	}

	//remove last element
	instances_.erase(--instances_.end(),instances_.end());
	return removed;
}

CRaySystem::~CRaySystem()
{
}

int CRaySystem::SetRecalcMode(axelynx::RaySystem::RecalcMode rm)
{
	recalcmode_ = rm;
	return 0;
}

int CRaySystem::SetRenderType(axelynx::RaySystem::RenderType rt)
{
	rendertype_ = rt;
	return 0;
}

int CRaySystem::GetBinarySize() const
{
	return 0;
}

int CRaySystem::WriteBinary(axelynx::File file) const
{
	return 0;
}

int CRaySystem::RestoreFromBinary(axelynx::File file)
{
	return 0;
}