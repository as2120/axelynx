#pragma once
#include "axelynx/RaySystem.h"
#include <vector>
#include "gl/axgl.h"
#include <iostream>

class CRaySystem : public axelynx::RaySystem
{
public:

	class CLightRay : public axelynx::RaySystem::LightRay
	{
	public:
		float width_;
		float lenght_;
		axelynx::vec4 color_;
		axelynx::vec3 start_;
		axelynx::vec3 end_;

		CLightRay()
		{
			width_ = 1.0f;
			lenght_ = 1.0f;
			color_ = axelynx::vec4(1.0f);
		}

		virtual void SetWidth(float width)
		{
			//std::cout<<"windth:"<<width<<std::endl;
			width_ = width;
		}
		virtual void SetLenght(float lenght)
		{
			lenght_ = lenght;
		}

		virtual void Place(axelynx::vec3 from, axelynx::vec3 to)
		{
			start_ = from;
			end_ = to;
			//axelynx::vec3 dir = to - from;
			//SetLenght(dir.length());
		}

		virtual void SetColor(float r, float g, float b, float a = 1)
		{
			color_.x = r;
			color_.y = g;
			color_.z = b;
			color_.w = a;
		}

		virtual const axelynx::vec3& StartPoint() const
		{
			return start_;
		}

		virtual const axelynx::vec3& EndPoint() const
		{
			return end_;
		}

		virtual float GetLenght() const
		{
			return lenght_;
		}

		virtual const axelynx::vec4& GetColor() const
		{
			return color_;
		}
		virtual float GetWidth() const
		{
			return width_;
		}
	};

	class CRay : public axelynx::RaySystem::Ray
	{
	public:
		float width_;
		float lenght_;
		axelynx::vec4 color_;
		axelynx::RaySystem::Ray::Start orign_;

		mutable axelynx::vec3 start_;
		mutable axelynx::vec3 end_;

		CRay()
		{
			width_ = 1.0f;
			lenght_ = 1.0f;
			color_ = axelynx::vec4(1.0f);
			orign_ = axelynx::RaySystem::Ray::S_START;
		}

		virtual void SetWidth(float width)
		{
			//std::cout<<"windth:"<<width<<std::endl;
			width_ = width;
		}
		virtual void SetLenght(float lenght)
		{
			lenght_ = lenght;
		}

		virtual void Place(axelynx::vec3 from, axelynx::vec3 to)
		{
			SetPosition(from);
			PointOn(to);
			axelynx::vec3 dir = to - from;
			SetLenght(dir.length());
		}

		virtual void SetColor(float r, float g, float b, float a = 1)
		{
			color_.x = r;
			color_.y = g;
			color_.z = b;
			color_.w = a;
		}

		virtual void Orign(axelynx::RaySystem::Ray::Start sp)
		{
			orign_ = sp;
		}

		virtual const axelynx::vec3& StartPoint() const
		{
			return GetPosition();
		}

		virtual const axelynx::vec3& EndPoint() const
		{
			axelynx::vec3 dir = GetOrientation().ztransform(lenght_);
			static axelynx::vec3 sresult;

			sresult = GetPosition() + dir;
			return sresult;
		}

		virtual float GetLenght() const
		{
			return lenght_;
		}

		virtual const axelynx::vec4& GetColor() const
		{
			return color_;
		}
		virtual float GetWidth() const
		{
			return width_;
		}
	};

	CRaySystem(int capaticiry_);

	virtual Ray* AddInstance();
	virtual LightRay* AddLightRay();
	virtual bool RemoveInstance(LightRay * pivot);

	virtual int SetRecalcMode(axelynx::RaySystem::RecalcMode);
	virtual int SetRenderType(axelynx::RaySystem::RenderType);
	virtual void SetDefaultOrign(axelynx::RaySystem::Ray::Start mode)
	{
	}
	virtual bool Recalc()const;
	virtual ~CRaySystem();

	//geometry funcs
	virtual bool Draw() const;
	virtual int CountTriangles() const;
	virtual int CountVertices() const;
	virtual float GetRadius() const
	{
		return -1;
	}
private:
	typedef std::vector<axelynx::RaySystem::LightRay*> Pivots; //user friendly data
	Pivots instances_;

	GLuint vao_;
	GLuint vbo_;
	GLuint ibo_;

	GLuint vao_volumetric_;
	GLuint vbo_volumetric_;
	GLuint ibo_volumetric_;

	struct Ray
	{
		axelynx::vec3 v0;
		float s0;
		axelynx::vec4 color0;

		axelynx::vec3 v1;
		float s1;
		axelynx::vec4 color1;
	};

	struct VolumetricRay
	{
		struct Vertex
		{
			axelynx::vec3 pos;
			axelynx::vec4 color;
			axelynx::vec3 flow;
			axelynx::vec2 size;
			axelynx::vec3 endpos;
		};

		Vertex v[4];
	};

	typedef std::vector<Ray> Positions; //gpu friendly data
	mutable Positions positions_;

	typedef std::vector<VolumetricRay> VolumetricPositions; //gpu friendly data
	mutable VolumetricPositions volumetric_positions_;

	axelynx::RaySystem::RecalcMode recalcmode_;
	axelynx::RaySystem::RenderType rendertype_;

	mutable int old_size_;
	static void MTRecalc(void *ps);

	virtual int GetBinarySize() const;
	virtual int WriteBinary(axelynx::File file) const;	
	virtual int RestoreFromBinary(axelynx::File file);

	virtual std::wstring GetClass()
	{
		return L"raysystem";
	}
};