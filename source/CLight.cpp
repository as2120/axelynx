

#include "CLight.h"

CLight::CLight(axelynx::Light::Mode mode)
	:distance_(1000.0f),angle_(120.0f),color_(1.0f),mode_(mode)
{
}

void CLight::Color(float r, float g, float b)
{
}

void CLight::SetMode(const Mode &m)
{
	mode_ = m;
}

void CLight::SetDistance(float distance)
{
	distance_ = distance;
}

void CLight::SetAngle(float angle)
{
	angle_ = angle;
}

CLight::~CLight()
{
}



