#pragma once

#include "axelynx/MorfedMesh.h"
#include "gl/axgl.h"


class CMorfedMesh : public axelynx::MorfedMesh
{
	Face *faces_;
	int count_faces_;

	GLuint vao_;
	GLuint vbo_;
	GLuint uabo_;
	GLuint ibo_;

	
	int count_frames_;
	int count_vertices_;
	bool locked_;

	bool MakeVBO();
public:
	class CMorfedFrame : public axelynx::MorfedMesh::Frame
	{
		int count_vertices_;
		Vertex *vertices_;


	public:
		CMorfedFrame()
			:count_vertices_(0),vertices_(0)
		{
		}

		CMorfedFrame(int count_vertices, int count_faces)
			:count_vertices_(count_vertices)
			,vertices_(new Vertex[count_vertices])
		{
		}
		virtual Vertex& GetVertex(int index)
		{
			return vertices_[index];
		}
		virtual const Vertex& GetVertex(int index) const
		{
			return vertices_[index];
		}

		virtual int CountVertices() const
		{
			return count_vertices_;
		}
		virtual bool Resize(int count_vertices, int count_faces)
		{
			count_vertices_ = count_vertices;

			delete[] vertices_;

			vertices_ =new Vertex[count_vertices];

			return true;
		}

		const float * GetVertexPointer() const
		{
			return reinterpret_cast<float*>(vertices_);
		}


		~CMorfedFrame()
		{
			delete[] vertices_;
		}
	};

	const unsigned __int16 * GetIndexPointer() const
	{
		return reinterpret_cast<unsigned __int16*>(faces_);
	}

	virtual Face& GetFace(int index)
	{
		return faces_[index];
	}
	virtual const Face& GetFace(int index) const
	{
		return faces_[index];
	}
	virtual int CountFaces() const
	{
		return count_faces_;
	}
	private:
	CMorfedFrame *frames_;
public:
	CMorfedMesh(int count_frames, int count_vertices, int count_faces);

	virtual int CountFrames() const;
	virtual CMorfedFrame& GetFrame(int index);
	virtual const CMorfedFrame& GetFrame(int index) const;
	virtual bool Resize(int frames_count);

	virtual bool Lock();
	virtual bool UnLock();

	virtual bool Draw(float frame) const;
	virtual bool Draw() const
	{
		return Draw(0);
	}

	virtual int CountTriangles() const;
	virtual int CountVertices() const;
	virtual float GetRadius() const;

	~CMorfedMesh();
};

