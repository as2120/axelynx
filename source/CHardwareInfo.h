#pragma once

#include "axelynx/HardwareInfo.h"


class CHardwareInfo : public axelynx::HardwareInfo
{
	static CHardwareInfo* instance_;

public:
	virtual int AvaibleVideoMemory();
	virtual int AvaibleSystemMemory();

	static CHardwareInfo* instance();

	CHardwareInfo();
	~CHardwareInfo();
};