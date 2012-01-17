#pragma once

#include "axelynx/Light.h"
#include "axelynx/math/vec3.h"

class CLight : public axelynx::Light
{
	float distance_;
	float angle_;
	axelynx::vec3 color_;
	axelynx::Light::Mode mode_;
public:

	CLight(axelynx::Light::Mode mode);
	virtual void Color(float r, float g, float b);
	virtual void SetMode(const Mode &m);
	virtual void SetDistance(float distance);
	virtual void SetAngle(float angle);
	~CLight();
};