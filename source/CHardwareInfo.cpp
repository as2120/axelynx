
#include "CHardwareInfo.h"
#include "gl\axgl.h"

CHardwareInfo* CHardwareInfo::instance_ = 0;

CHardwareInfo::CHardwareInfo()
{
}

CHardwareInfo::~CHardwareInfo()
{
}

CHardwareInfo* CHardwareInfo::instance()
{
	if(!instance_)
	{
		instance_ = new CHardwareInfo();
	}

	return instance_;
}

int CHardwareInfo::AvaibleVideoMemory()
{
	GLint mem[4] = {0};
	glGetIntegerv(GL_RENDERBUFFER_FREE_MEMORY_ATI, mem);

	return mem[0] + mem[2];
}

int CHardwareInfo::AvaibleSystemMemory()
{
	return 0;
}