#pragma once

#include "axelynx/Surface.h"
#include "gl/axgl.h"
//#include <exception>

class CSurface : public axelynx::Surface
{
	friend class CInstanceGroup;

	GLuint vao_;
	GLuint vbo_;
	GLuint uabo_;
	GLuint ibo_;

	int count_vertices_;
	int count_indices_;

	GLenum draw_mode_;

	axelynx::vec3 *positions;
	axelynx::vec3 *normals;
	axelynx::vec3 *tangents;
	axelynx::vec2 *uv0;
	axelynx::vec2 *uv1;
	axelynx::vec4 *colors;

	void *indices_;

	bool MakeVBO();
	bool RemakeVBO;



	mutable float radius_;

	bool use_lightmap_;

	int count_tris_;
	axelynx::Surface::VertexPackMode packmode_;

	unsigned __int8 index_size_;

	inline int _getIndex(int i)
	{
		if(index_size_ == 1)
			return static_cast<unsigned char*>(indices_)[i];
		if(index_size_ == 2)
			return static_cast<unsigned short*>(indices_)[i];
		if(index_size_ == 4)
			return static_cast<unsigned int*>(indices_)[i];

		return 0;
	}

public:
	unsigned __int8 GetIndexSize() const
	{
			return index_size_;
	}

	virtual bool _recalcTangents();
	bool _recalcTangents8();
	bool _recalcTangents16();
	bool _recalcTangents32();

	CSurface(int count_vertices, int count_indices);
	CSurface(); //using for restore from binary only!
	virtual bool Draw() const;

	virtual int CountTriangles()const;
	virtual int CountVertices() const;

	virtual void Translate(const axelynx::vec3& translate);
	virtual void Rotate(const axelynx::quat& rotate);
	virtual void Scale(const axelynx::vec3& scale);

	virtual void UserVertexAttribs(const char* name,signed char components = 3,axelynx::VertexType vt = axelynx::VT_FLOAT, bool normalized = true);

	virtual int GetUserAttributeLocation(const char* name) const;
	virtual int UserAttribute(int location, void *data, int count = 1);

	virtual int SetVertexPosition(int index, const axelynx::vec3& position);
	virtual int SetVertexNormal(int index, const axelynx::vec3& normal);
	virtual int SetVertexTangent(int index, const axelynx::vec3& tangent);
	virtual int SetVertexTexCoord(int index, const axelynx::vec2& uv, int layer = 0);
	virtual int SetVertexColor(int index, const axelynx::vec4& color);

	virtual int SetVertexPosition(int index, float x, float y, float z)
	{
	    return SetVertexPosition(index,axelynx::vec3(x,y,z));
	}
	virtual int SetVertexNormal(int index, float x, float y, float z)
	{
	    return SetVertexNormal(index,axelynx::vec3(x,y,z));
	}
	virtual int SetVertexTangent(int index, float x, float y, float z)
	{
	    return SetVertexTangent(index,axelynx::vec3(x,y,z));
	}
	virtual int SetVertexTexCoord(int index, float u, float v, int layer = 0)
	{
	    return SetVertexTexCoord(index,axelynx::vec2(u,v));
	}
	virtual int SetVertexColor(int index, float r, float g, float b, float a = 1.0f)
	{
	    return SetVertexColor(index,axelynx::vec4(r,g,b,a));
	}

	virtual int SetTriangle(int index, int index0,int index1, int index2);
	virtual bool Lock();
	virtual bool UnLock();

	virtual ~CSurface();

	static CSurface* LoadSBS(axelynx::File file);
	static CSurface* LoadAXS(axelynx::File file);
	virtual float GetRadius() const;

	virtual bool SetVertexPackMode(const axelynx::Surface::VertexPackMode &pm);

	virtual int GetBinarySize() const;
	virtual int WriteBinary(axelynx::File file) const;
	virtual int RestoreFromBinary(axelynx::File file);

	virtual void UseLightmap()
	{
		use_lightmap_ = true;
	}

	virtual void UseColoredVertices()
	{
		use_lightmap_ = false;
	}

	virtual std::wstring GetClass() const
	{
		return L"surface";
	}

	virtual bool AutoCenter();
	virtual bool RecalcTangents();
};
