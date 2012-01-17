

#include "CSpriteSystem.h"
#include "CStatistics.h"
#include "CPivot.h"
#include <cassert>
#include "saSpec.h"
#include <iostream>
#include "CCamera.h"
#include "CShader.h"
#include <process.h>

CSpriteSystem::params CSpriteSystem::p;

CSpriteSystem::CSpriteSystem(int capacity_)
{
	assert(capacity_>0);

	//instances_.reserve(capacity_);
	//positions_.reserve(capacity_);

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	// создадим и используем Vertex Buffer Object (VBO)
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	glVertexAttribPointer(VA_POSITION, 3, GL_FLOAT, GL_FALSE,	sizeof(Sprite), reinterpret_cast<const GLvoid*>(0));
	glEnableVertexAttribArray(VA_POSITION);

	glVertexAttribPointer(VA_POINT_SIZE, 1, GL_FLOAT, GL_FALSE,sizeof(Sprite), reinterpret_cast<const GLvoid*>(12));
	glEnableVertexAttribArray(VA_POINT_SIZE);

	glVertexAttribPointer(VA_COLOR, 4, GL_FLOAT, GL_FALSE,sizeof(Sprite), reinterpret_cast<const GLvoid*>(16));
	glEnableVertexAttribArray(VA_COLOR);

	recalcmode_ = axelynx::SpriteSystem::RM_ONDRAW;

	old_size_  = -1;
}

void CSpriteSystem::MTRecalc(void *param)
{
	const Pivots &instances = *p.instances;
	Positions &positions = *p.positions;

	for(int i = p.start; i<p.end; ++i)
	{
		

		Sprite sprite;
		const axelynx::vec3& pos = instances[i]->GetPosition(true);

		
		sprite.x = pos.x;
		sprite.y = pos.y;
		sprite.z = pos.z;

		sprite.size = instances[i]->size;
		sprite.r = instances[i]->r;
		sprite.g = instances[i]->g;
		sprite.b = instances[i]->b;
		sprite.a = instances[i]->a;

		positions[i] =sprite;
	}

	p.start = 0;
}

bool CSpriteSystem::Recalc() const
{
	Pivots::const_iterator pi = instances_.begin();
	Pivots::const_iterator epi = instances_.end();
	
	positions_.clear();

	int sz = instances_.size();
	p.start = 0;
	positions_.resize(sz);

	//std::cout<<sz<<std::endl;

	if(sz>1024)
	{

		//use MT

		int osz = sz;
		sz /= 2;



		p.instances = &instances_;
		p.positions = &positions_;
		p.start = sz;
		p.end = osz;

		_beginthread(CSpriteSystem::MTRecalc,0,0);
	}

	for(int i = 0; i<sz; ++i)
	{
		Sprite sprite;
		const axelynx::vec3& pos = instances_[i]->GetPosition(true);

		
		sprite.x = pos.x;
		sprite.y = pos.y;
		sprite.z = pos.z;


		sprite.size = instances_[i]->size;
		sprite.r = instances_[i]->r;
		sprite.g = instances_[i]->g;
		sprite.b = instances_[i]->b;
		sprite.a = instances_[i]->a;

		//std::cout<<"x:"<<sprite.size<<" y:"<<sprite.r<<" z:"<<sprite.a<<std::endl;

		positions_[i] = sprite;
	}

	while(p.start)
	{
		Sleep(1);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	int c_size = positions_.size() * sizeof(Sprite);

	if(c_size>old_size_)
	{
		glBufferData(GL_ARRAY_BUFFER,positions_.size() * sizeof(Sprite),positions_.data(),GL_DYNAMIC_DRAW);
		old_size_ = c_size;
	}
	else
	{
		glBufferSubData(GL_ARRAY_BUFFER,0,positions_.size() * sizeof(Sprite),positions_.data());
	}

	CStatistics::dips++;
	CStatistics::tris_rendered += CountTriangles();
	CStatistics::vertices_rendered += positions_.size();

	return true;
}

bool CSpriteSystem::Draw() const
{
	OPENGL_CHECK_FOR_ERRORS();
	const CCamera *cam = CCamera::Current();

	axelynx::mat3 orient = cam->GetOrientationMatrix();
	axelynx::vec3 viewup = axelynx::vec3(0,1,0);
	axelynx::vec3 viewright = axelynx::vec3(600.0f / 800.0f,0,0);
	axelynx::Shader *sh = CShader::Current();

	if(sh)
	{
		sh->SetUniform("viewup",viewup);
		sh->SetUniform("viewright",viewright);
	}


	glBindVertexArray(vao_);

	if(recalcmode_ == axelynx::SpriteSystem::RM_ONDRAW)
	{
		CSpriteSystem::Recalc();
	}
	else
	{
			glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	}

	//std::cout<<positions_.size()<<std::endl;
	OPENGL_CHECK_FOR_ERRORS();
	//glEnable(GL_POINT_SPRITE);
	OPENGL_CHECK_FOR_ERRORS();
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	OPENGL_CHECK_FOR_ERRORS();
	glDrawArrays(GL_POINTS,0,positions_.size());
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}

int CSpriteSystem::CountTriangles() const
{
	return positions_.size() * 2;
}

int CSpriteSystem::CountVertices() const
{
	return positions_.size();
}

axelynx::SpriteSystem::Sprite* CSpriteSystem::AddInstance()
{
	CSprite *piv = new CSprite;
	piv->SetColor(1.0f,1.0f,1.0f,1.0f);
	piv->SetSize(1.0f);
	instances_.push_back(piv);
	
	return piv;
}

bool CSpriteSystem::RemoveInstance(axelynx::SpriteSystem::Sprite* pivot)
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

int CSpriteSystem::SetDrawMode(DrawMode)
{
	return 0;
}

CSpriteSystem::~CSpriteSystem()
{
}

int CSpriteSystem::SetRecalcMode(axelynx::SpriteSystem::RecalcMode rm)
{
	recalcmode_ = rm;
	return 0;
}

int CSpriteSystem::GetBinarySize() const
{
	return 0;
}

int CSpriteSystem::WriteBinary(axelynx::File file) const
{
	return 0;
}

int CSpriteSystem::RestoreFromBinary(axelynx::File file)
{
	return 0;
}