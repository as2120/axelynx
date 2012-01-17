
#include "CInstanceGroup.h"
#include "CShader.h"
#include "CStatistics.h"
#include <iostream>
CInstanceGroup::CInstanceGroup()
{
	instances_.reserve(64);

	tbo_ = 0;
	tex_ = 0;
}

const axelynx::Geometry* CInstanceGroup::GetGeometry() const
{
	return surface_;
}

void CInstanceGroup::SetGeometry(const axelynx::Geometry* geom)
{
	surface_ = dynamic_cast<const CSurface*>(geom);
}

axelynx::Instance* CInstanceGroup::AddInstance()
{
	CInstance * inst = new CInstance();
	
	instances_.push_back(inst);
	return inst;
}

bool CInstanceGroup::RemoveInstance(axelynx::Instance *instance)
{

	return true;
}

bool CInstanceGroup::Draw() const
{
	if(!tbo_)
	{
		glGenBuffers ( 1, &tbo_ );
		glBindBuffer ( GL_TEXTURE_BUFFER, tbo_ );


		float *data = new float[instances_.size() * 4];
		int index=0;

		auto ci=instances_.begin(),ei=instances_.end();
		for(;ci!=ei;++ci)
		{
			const axelynx::vec3 &pos = (*ci)->GetPosition(true);

			data[index*4 +0] = pos.x;
			data[index*4 +1] = pos.y;
			data[index*4 +2] = pos.z;
			data[index*4 +3] = 0;

			index++;
		}

		glBufferData ( GL_TEXTURE_BUFFER, instances_.size() * 4 * sizeof(float), data, GL_STATIC_DRAW_ARB );
		delete[] data;

		glGenTextures   ( 1, &tex_ );
		glBindTexture   ( GL_TEXTURE_BUFFER, tex_ );
		glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );

		glTexBuffer  (GL_TEXTURE_BUFFER_EXT, GL_RGBA32F, tbo_ );
		//
		glBindTexture   ( GL_TEXTURE_BUFFER, 0 );
		glBindBuffer ( GL_TEXTURE_BUFFER, 0 );

	}



	CShader *shader = CShader::Current();
	
	shader->SetUniform(shader->GetUniformLocation("positions"),7);

	//glBindBuffer(GL_TEXTURE_BUFFER_EXT, tbo_);

	//axelynx::vec4 positions[20000];
	//auto ci=instances_.begin(),ei=instances_.end();

	//int index=0;
	//for(;ci!=ei;++ci)
	//{
	//	positions[index++] = axelynx::vec4((*ci)->GetPosition(false),0);
	//}

	//glBufferSubData( GL_TEXTURE_BUFFER_EXT, 0, instances_.size(), positions);

	glActiveTexture(GL_TEXTURE0 + 7);
	glBindTexture(GL_TEXTURE_BUFFER,tex_);

	glBindVertexArray(surface_->vao_);

	CStatistics::dips++;
	CStatistics::tris_rendered += (instances_.size()) * surface_->count_tris_;
	CStatistics::vertices_rendered += (instances_.size()) * surface_->count_vertices_;
	
	glDrawElementsInstanced(surface_->draw_mode_,surface_->count_indices_,GL_UNSIGNED_SHORT,0,instances_.size());
	glBindTexture(GL_TEXTURE_BUFFER,0);
	glActiveTexture(GL_TEXTURE0);
	OPENGL_CHECK_FOR_ERRORS();

	return true;
}

int CInstanceGroup::CountTriangles() const
{
	return instances_.size() * surface_->CountTriangles();
}

int CInstanceGroup::CountVertices() const
{
	return instances_.size() * surface_->CountVertices();
}

float CInstanceGroup::GetRadius() const
{
	return -1;
}


int CInstanceGroup::GetBinarySize() const
{
	return 0;
}

int CInstanceGroup::WriteBinary(axelynx::File file) const
{
	return 0;
}

int CInstanceGroup::RestoreFromBinary(axelynx::File file)
{
	return 0;
}