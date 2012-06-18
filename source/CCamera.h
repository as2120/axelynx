#pragma once

#include "axelynx/Camera.h"
#include "axelynx/Scene.h"

#include "CPivot.h"

class CCamera : public axelynx::Camera
{
	mutable axelynx::mat4 view_;
	mutable axelynx::mat4 projection_;
	mutable axelynx::mat4 viewprojection_;

	mutable axelynx::frustum frustum_;

	float znear_, zfar_;
	bool ccm_; // Cls mode (true|flase)
	bool cdm_; // Clear depth mode (true|flase)
	float ccr_,ccg_,ccb_,cca_; //clear color (red, green...)
	int x1_,y1_,x2_,y2_;
	float fov_;
	mutable bool need_recalc_;

	static const CCamera * current_;

	axelynx::Scene *my_scene_;
	bool enabled_;

	axelynx::Camera::DrawMode drawmode_;

	float aspect_ratio_;
public:
	bool SetScene(axelynx::Scene *scene)
	{
		my_scene_ = scene;
		return true;
	}

	virtual float GetAspectRatio() const
	{
		return aspect_ratio_;
	}

	CCamera();

	virtual const axelynx::mat4& GetViewMatrix() const;
	virtual const axelynx::mat4& GetProjectionMatrix() const;
	virtual const axelynx::mat4& GetViewProjectionMatrix() const;

	virtual axelynx::frustum GetFrustum() const;

	virtual void Viewport(int x1, int y1, int x2, int y2);

	virtual void Projection(float fov, float aspect, float znear, float zfar);
	virtual void Ortho(float xnear,float xfar, float ynear, float yfar, float znear, float zfar);

	virtual void Range(float znear, float zfar);
	virtual void ClearColorMode(bool mode);
	virtual void ClearDepthMode(bool mode);
	virtual void ClearColor(float r, float g, float b, float a);
	virtual void FOV(float fov);
	virtual bool Bind(int force_clear_depth) const;
	virtual bool UnBind() const;

	virtual axelynx::vec3 Project(axelynx::vec3 point) const;
	virtual axelynx::vec3 UnProject(axelynx::vec3 coords) const;
	virtual axelynx::PickResult Pick(axelynx::vec2 coords) const;
	static const CCamera * Current(){return current_;}

	virtual	bool SetEnable(bool value);
	virtual bool isEnabled() const;

	virtual void SetDrawMode(axelynx::Camera::DrawMode mode)
	{
		drawmode_ = mode;
	}
virtual ~CCamera();
};