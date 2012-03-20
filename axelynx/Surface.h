#pragma once

#include "Geometry.h"
#include "math\vectormath.h"
#include "File.h"

namespace axelynx
{
	enum VertexType
	{
		VT_UNSIGNED_BYTE
		,VT_SIGNED_BYTE
		,VT_UNSIGNED_SHORT
		,VT_SIGNED_SHORT
		,VT_UNSIGNE_INT
		,VT_INT
		,VT_HALF_FLOAT
		,VT_FLOAT
	};

	class Surface : public Geometry
	{
	public:
		enum RecalcNormalsMode
		{
			RNM_GEOMETRY
			,RNM_SPHERE
			,RNM_CUBE
		};

		struct VertexInfo
		{
			vec3 position;
			vec2 uv[8];
			vec4 color;
			vec3 normal;
			vec3 tangent;
		};

		enum VertexPackMode
		{
			VP_AUTO
			,VP_PACK
			,VP_UPACK
		};

		/// Render surface with current pipeline settings
		/// i.e. texture and shader (in shader also need setup world, view and projection matrix)
		virtual bool Draw() const=0;

		virtual int CountTriangles()const =0;
		virtual int CountVertices() const =0;

		virtual void Translate(const vec3& translate)=0;
		virtual void Rotate(const quat& rotate)=0;
		virtual void Scale(const vec3& scale)=0;
		virtual void Scale(float scale)=0;

		virtual void Attach(Surface *other)=0;

		virtual void FitBox(vec3 size, vec3 position = vec3(0))=0;
		virtual void FitSphere(float radius, vec3 position = vec3(0))=0;

		/// Binding user vertex attributes to surface
		/// example:	somesurface->UserVertexAttribs("wing_intensity",1)
		///				binded new attribute to each vertex in surface
		///				as float value (f.e. for tree branches)
		virtual void UserVertexAttribs(const char* name,signed char components = 3,VertexType vt = VT_FLOAT, bool normalized = true)=0;

		/// Get location user vertex attributes
		///	is needed for UserAttribute func
		/// example:	int loc = somesurface->GetUserAttributeLocation("wing_intensity");
		virtual int GetUserAttributeLocation(const char* name) const =0;

		/// Get location user vertex attributes
		///	is needed for UserAttribute func
		/// example:	
		///				somesurface->Lock();
		///				int loc = somesurface->GetUserAttributeLocation("wing_intensity");
		///				for(int i=0;i<somesurface->CountVertices();++i)
		///				{
		///					float myfloat = CalcWingIntensity(somesurface->Vertex(i).position);
		///					UserAttribute(loc, &myfloat);
		///				}
		///				somesurface->UnLock();
		virtual int UserAttribute(int location, void *data, int count = 1)=0;

		virtual int SetVertexPosition(int index, const vec3& position) =0;
		virtual int SetVertexNormal(int index, const vec3& normal) =0;
		virtual int SetVertexTangent(int index, const vec3& tangent) =0;
		virtual int SetVertexTexCoord(int index, const vec2& uv, int layer = 0) =0;
		virtual int SetVertexColor(int index, const vec4& color) =0;

		virtual int SetVertexPosition(int index, float x, float y, float z) =0;
		virtual int SetVertexNormal(int index, float x, float y, float z) =0;
		virtual int SetVertexTangent(int index, float x, float y, float z) =0;
		virtual int SetVertexTexCoord(int index, float u ,float v, int layer = 0) =0;
		virtual int SetVertexColor(int index, float r, float g, float b, float a=1.0f) =0;

		virtual int SetTriangle(int index, int index0,int index1, int index2) =0;

		virtual int GetVertexPosition(int index, vec3& position) =0;
		virtual int GetVertexNormal(int index, vec3& normal) =0;
		virtual int GetVertexTangent(int index, vec3& tangent) =0;
		virtual int GetVertexTexCoord(int index, vec2& uv, int layer = 0) =0;
		virtual int GetVertexColor(int index, vec4& color) =0;

		virtual int GetTriangle(int index, int& index0,int& index1, int& index2) =0;

		virtual bool Lock()=0;
		virtual bool UnLock()=0;

		virtual bool SetVertexPackMode(const VertexPackMode &pm) = 0;

		virtual void UseLightmap()=0;
		virtual void UseColoredVertices()=0;
		virtual ~Surface(){};

		virtual bool AutoCenter() = 0;

		virtual bool RecalcTangents()=0;

		virtual bool RecalcNormals(RecalcNormalsMode rnm = RNM_GEOMETRY)=0;
	};
}