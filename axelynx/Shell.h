#pragma once

#include "axlib.h"

namespace axelynx
{
	namespace shell
	{
		AXELYNX_API void ShowMessage(const char * message);
		AXELYNX_API void ShowMessage(const wchar_t * message);
		AXELYNX_API void ShowError(const char * message);
		AXELYNX_API void ShowError(const wchar_t * message);
		AXELYNX_API void Execute(const char * message);
		AXELYNX_API void Execute(const wchar_t * message);
	}
}