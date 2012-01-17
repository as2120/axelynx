#pragma once

#include "Pivot.h"
#include "math/vectormath.h"
#include "math/frustum.h"
//#include "Entity.h"

namespace axelynx
{
	class Entity;

	struct PickResult
	{
		Entity *ent;
		vec3 position;
		vec3 normal;

		int trisindex;
		vec2 uv_coords;

		PickResult()
			:ent(0)
		{
		}

	};

	class Camera : public Pivot
	{
	public:
		enum DrawMode
		{
			DM_SOLID
			,DM_WIREFRAME
			,DM_POINTS
		};

		virtual const mat4& GetViewMatrix() const =0;
		virtual const mat4& GetProjectionMatrix() const =0;
		virtual const mat4& GetViewProjectionMatrix() const=0;

		virtual frustum GetFrustum() const =0;

		virtual void Viewport(int x1, int y1, int x2, int y2)=0;

		virtual void Projection(float fov, float aspect, float znear, float zfar)=0;
		virtual void Ortho(float xnear,float xfar, float ynear, float yfar, float znear, float zfar)=0;

		virtual void Range(float znear, float zfar)=0;
		virtual void ClearColorMode(bool mode) =0;
		virtual void ClearDepthMode(bool mode) =0;
		virtual void ClearColor(float r, float g, float b, float a = 1)=0;
		virtual void FOV(float fov)=0;

		virtual bool Bind(int force_clear_depth=0) const =0;
		virtual bool UnBind() const =0;
		
		virtual vec3 Project(vec3 point) const =0;
		virtual vec3 UnProject(vec3 coords) const =0;

		virtual PickResult Pick(vec2 coords) const = 0;			
		
		virtual	bool SetEnable(bool value) = 0;
		virtual bool isEnabled() const = 0;

		virtual void SetDrawMode(DrawMode mode) = 0;
virtual ~Camera(){};
	};
}