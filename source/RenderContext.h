#pragma once

#include "RenderContextInfo.h"

namespace RCInfo //for info of current renderccontext
{
	void setShaderSupport(bool value);
	void setRenderTargetSupport(bool value);
	void setDevice(bool value);
};