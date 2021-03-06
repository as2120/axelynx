

#include "CMorfedMesh.h"
#include "CFile.h"
#include "CShader.h"
#include "saSpec.h"
#include "CStatistics.h"
#include <iostream>
#include <cmath>
#include <vector>
#include "saSpec.h"
#include <axelynx\math\axmath.h>




CMorfedMesh::CMorfedMesh(int count_frames, int count_vertices, int count_faces)
	:count_frames_(count_frames),count_vertices_(count_vertices),count_faces_(count_faces)
	,frames_(new CMorfedFrame[count_frames]),faces_(new Face[count_faces]),locked_(true)
	,vao_(0),vbo_(0),ibo_(0),uabo_(0)
{
	for(int i=0;i<count_frames;++i)
	{
		frames_[i].Resize(count_vertices_,count_faces_);
	}
}

int CMorfedMesh::CountFrames() const
{
	return count_frames_;
}

CMorfedMesh::CMorfedFrame& CMorfedMesh::GetFrame(int index)
{
	return frames_[index];
}

const CMorfedMesh::CMorfedFrame& CMorfedMesh::GetFrame(int index) const
{
	return frames_[index];
}

bool CMorfedMesh::Resize(int frames_count)
{
	delete[] frames_;
	frames_ = new CMorfedFrame[frames_count];
	count_frames_ = frames_count;

	for(int i=0;i<count_frames_;++i)
	{
		frames_[i].Resize(count_vertices_,count_faces_);
	}

	return true;
}

void CMorfedMesh::CMorfedFrame::RecalcTangents(int count_indices, unsigned short * indices)
{
	using namespace axelynx; //for vec3

	register int i, j;
	std::vector<vec3> ntangents;	// ����������� ������� � ������������
 
	for ( i = 0; i < count_indices; i+=3 )
	{
		int ind0 = indices[ i + 0];
		int ind1 = indices[i + 1];
		int ind2 = indices[i + 2];
 
		vec3 v1 = vertices_[ind0].position;
		vec3 v2 = vertices_[ind1].position;
		vec3 v3 = vertices_[ind2].position;

		float s1      = vertices_[ind0].texcoord.x;
		float t1      = vertices_[ind0].texcoord.y;
		float s2      = vertices_[ind1].texcoord.x;
		float t2      = vertices_[ind1].texcoord.y;
		float s3      = vertices_[ind2].texcoord.x;
		float t3      = vertices_[ind2].texcoord.y;
 
		vec3  t, b;
		CalcTriangleBasis( v1, v2, v3, s1, t1, s2, t2, s3, t3, t, b );

		ntangents.push_back( t );
	}
 	
	for ( i = 0; i < count_vertices_; i++ )
	{
		std::vector<vec3> rt;

		for ( j = 0; j < count_indices; j+=3 )
		{
			if ( indices[ j + 0 ] == i || indices[ j + 1] == i || indices[ j + 2 ] == i )
			{
				rt.push_back( ntangents[ j / 3 ] );
			}
		}
 
		vec3 tangentRes( 0, 0, 0 );
		for ( j = 0; j < static_cast<int>(rt.size()); j++ )
		{
			tangentRes += rt[ j ];
		}

		tangentRes *= (1.0f/float( rt.size() ));
		tangentRes = Ortogonalize( vertices_[ i ].normal, tangentRes );
		vertices_[i].tangent =  tangentRes;
	}	
}


void CMorfedMesh::RecalcTangents()
{
	Lock();

	for(int i=0;i<count_frames_;++i)
	{
		frames_[i].RecalcTangents(count_faces_*3,(unsigned short*)faces_);
	}

	MakeVBO();

	UnLock();
}

bool CMorfedMesh::MakeVBO()
{
	if(locked_)
	{

	OPENGL_CHECK_FOR_ERRORS();
	std::cout<<"MakeVBO"<<std::endl;

	if(vao_)
		glDeleteVertexArrays(1,&vao_);

	if(ibo_)
		glDeleteBuffers(1,&ibo_);

	if(vbo_)
		glDeleteBuffers(1,&vbo_);

	if(uabo_)
		glDeleteBuffers(1,&uabo_);
	OPENGL_CHECK_FOR_ERRORS();
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	// �������� � ���������� Vertex Buffer Object (VBO)
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	glGenBuffers(1, &ibo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_faces_ * sizeof(unsigned short) * 3, GetIndexPointer(), GL_STATIC_DRAW);
	OPENGL_CHECK_FOR_ERRORS();
	
		std::cout<<"unpack"<<std::endl;
		axelynx::MorfedMesh::Frame::Vertex *gpu = new axelynx::MorfedMesh::Frame::Vertex[count_vertices_ * count_frames_];
	
		
		float max_radius = 0;

		for(int i=0;i<count_frames_;++i)
		{
			void *dest = &gpu[i * count_vertices_];
			const void *src = GetFrame(i).GetVertexPointer();

			float len = GetFrame(i).GetRadius();
			if(len>max_radius)
				max_radius = len;

			int size = count_vertices_ * sizeof(axelynx::MorfedMesh::Frame::Vertex);

			memcpy(dest,src,size);
		}

		radius_ = max_radius;

		const int framesize = count_vertices_ * sizeof(axelynx::MorfedMesh::Frame::Vertex);

		glBufferData(GL_ARRAY_BUFFER, framesize * count_frames_, gpu, GL_STATIC_DRAW);

		glVertexAttribPointer(VA_POSITION, 3, GL_FLOAT, GL_FALSE,sizeof(axelynx::MorfedMesh::Frame::Vertex), reinterpret_cast<const GLvoid*>(0));
		glEnableVertexAttribArray(VA_POSITION);
		glVertexAttribPointer(VA_NORMAL, 3, GL_FLOAT, GL_FALSE,sizeof(axelynx::MorfedMesh::Frame::Vertex), reinterpret_cast<const GLvoid*>(12));
		glEnableVertexAttribArray(VA_NORMAL);
		glVertexAttribPointer(VA_TANGENT, 3, GL_FLOAT, GL_FALSE,sizeof(axelynx::MorfedMesh::Frame::Vertex), reinterpret_cast<const GLvoid*>(24));
		glEnableVertexAttribArray(VA_TANGENT);
		glVertexAttribPointer(VA_TEXCOORD0, 2, GL_FLOAT, GL_FALSE,sizeof(axelynx::MorfedMesh::Frame::Vertex), reinterpret_cast<const GLvoid*>(36));
		glEnableVertexAttribArray(VA_TEXCOORD0);

		glVertexAttribPointer(VA_NEXT_POSITION, 3, GL_FLOAT, GL_FALSE,sizeof(axelynx::MorfedMesh::Frame::Vertex), reinterpret_cast<const GLvoid*>(0+framesize));
		glEnableVertexAttribArray(VA_NEXT_POSITION);
		glVertexAttribPointer(VA_NEXT_NORMAL, 3, GL_FLOAT, GL_FALSE,sizeof(axelynx::MorfedMesh::Frame::Vertex), reinterpret_cast<const GLvoid*>(12+framesize));
		glEnableVertexAttribArray(VA_NEXT_NORMAL);
		glVertexAttribPointer(VA_NEXT_TANGENT, 3, GL_FLOAT, GL_FALSE,sizeof(axelynx::MorfedMesh::Frame::Vertex), reinterpret_cast<const GLvoid*>(24+framesize));
		glEnableVertexAttribArray(VA_NEXT_TANGENT);
		glVertexAttribPointer(VA_NEXT_TEXCOORD0, 2, GL_FLOAT, GL_FALSE,sizeof(axelynx::MorfedMesh::Frame::Vertex), reinterpret_cast<const GLvoid*>(36+framesize));
		glEnableVertexAttribArray(VA_NEXT_TEXCOORD0);

	OPENGL_CHECK_FOR_ERRORS();

		return true;
	}
	return false;
}

bool CMorfedMesh::Lock()
{
	locked_ = true;
	return locked_;

}

bool CMorfedMesh::UnLock()
{
	bool result = MakeVBO();

	locked_ = false;

	return result;
}

CMorfedMesh::~CMorfedMesh()
{
	delete[] frames_;
}

bool CMorfedMesh::Draw(float frame) const
{

	//glDisable(GL_CULL_FACE);
	OPENGL_CHECK_FOR_ERRORS();
	static GLuint current_vao_ = -1;
	//if(current_vao_ != vao_)
	{
		glBindVertexArray(vao_);
		current_vao_ = vao_;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);

	int iframe = (int)frame;
	float scalar = frame - iframe;

	if(axelynx::Shader::GetCurrent())
	{
		axelynx::Shader::GetCurrent()->SetUniform("scalar",scalar);
	}

	CStatistics::dips++;
	CStatistics::tris_rendered += count_faces_;
	CStatistics::vertices_rendered += count_vertices_;
	OPENGL_CHECK_FOR_ERRORS();

	int framesize = count_vertices_ * sizeof(CMorfedMesh::CMorfedFrame::Vertex);

	int firstframe = iframe;
	int lastframe = iframe+1;
	
	if(lastframe>=count_frames_)
		lastframe = 0;

	glVertexAttribPointer(VA_POSITION, 3, GL_FLOAT, GL_FALSE,sizeof(axelynx::MorfedMesh::Frame::Vertex), reinterpret_cast<const GLvoid*>(0+framesize * firstframe));
	glVertexAttribPointer(VA_NORMAL, 3, GL_FLOAT, GL_FALSE,sizeof(axelynx::MorfedMesh::Frame::Vertex), reinterpret_cast<const GLvoid*>(12+framesize * firstframe));
	glVertexAttribPointer(VA_TANGENT, 3, GL_FLOAT, GL_FALSE,sizeof(axelynx::MorfedMesh::Frame::Vertex), reinterpret_cast<const GLvoid*>(24+framesize * firstframe));
	glVertexAttribPointer(VA_TEXCOORD0, 2, GL_FLOAT, GL_FALSE,sizeof(axelynx::MorfedMesh::Frame::Vertex), reinterpret_cast<const GLvoid*>(36+framesize * firstframe));

	glVertexAttribPointer(VA_NEXT_POSITION, 3, GL_FLOAT, GL_FALSE,sizeof(axelynx::MorfedMesh::Frame::Vertex), reinterpret_cast<const GLvoid*>(0+framesize * lastframe));
	glVertexAttribPointer(VA_NEXT_NORMAL, 3, GL_FLOAT, GL_FALSE,sizeof(axelynx::MorfedMesh::Frame::Vertex), reinterpret_cast<const GLvoid*>(12+framesize * lastframe));
	glVertexAttribPointer(VA_NEXT_TANGENT, 3, GL_FLOAT, GL_FALSE,sizeof(axelynx::MorfedMesh::Frame::Vertex), reinterpret_cast<const GLvoid*>(24+framesize * lastframe));
	glVertexAttribPointer(VA_NEXT_TEXCOORD0, 2, GL_FLOAT, GL_FALSE,sizeof(axelynx::MorfedMesh::Frame::Vertex), reinterpret_cast<const GLvoid*>(36+framesize * lastframe));

	glDrawRangeElements(GL_TRIANGLES,0,count_faces_ * 3 -1,count_faces_ * 3,GL_UNSIGNED_SHORT,(void*)(0));
	OPENGL_CHECK_FOR_ERRORS();
	return true;

	return true;
}

int CMorfedMesh::CountTriangles() const
{
	return count_faces_;
}

int CMorfedMesh::CountVertices() const
{
	return count_vertices_;
}

float CMorfedMesh::GetRadius() const
{
	return radius_;
}