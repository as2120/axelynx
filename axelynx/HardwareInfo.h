#pragma once

namespace axelynx
{
	class HardwareInfo
	{
	public:
		virtual int AvaibleVideoMemory() = 0;
		virtual int AvaibleSystemMemory() = 0;
	};
}