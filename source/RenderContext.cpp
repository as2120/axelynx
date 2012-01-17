#include "RenderContext.h"

namespace RCInfo
{
	bool shaders_support=false;
	bool rendertarget_support=false;
	bool device=false;
};

//bool RCInfo::shaders_support = false;
//bool RCInfo::rendertarget_support = false;
//bool RCInfo::device = false;

void RCInfo::setShaderSupport(bool value)
{
	shaders_support = value;
}
void RCInfo::setRenderTargetSupport(bool value)
{
	rendertarget_support = value;
}
void RCInfo::setDevice(bool value)
{
	device = value;
}

bool RCInfo::isShaderSupport()
{
	return shaders_support;
}

bool RCInfo::isRenderTargetSupport()
{
	return rendertarget_support;
}

bool RCInfo::isDevice()
{
	return device;
}