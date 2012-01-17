#pragma once

#include "../axlib.h"
#include <string>

namespace axelynx
{
    namespace utils
    {
        AXELYNX_API wchar_t* MultiByteToWideChar(const char* s,wchar_t* into=0);
        AXELYNX_API char* WideCharToMultiByte(const wchar_t* s,char* into=0);
        AXELYNX_API wchar_t* AnsiToWideChar(const char* s,wchar_t* into=0);
        AXELYNX_API char* WideCharToAnsi(const wchar_t* s,char* into=0);

		AXELYNX_API std::wstring GetExtension(std::wstring filename);
		AXELYNX_API void GetRelativePath(const char * absolute, const char * project, char *output);

		AXELYNX_API std::wstring FromMultibyte(const std::string& value);
		AXELYNX_API std::string FromWide(const std::wstring& value);
    }

	class str
	{
		std::wstring src;
	public:
		const std::wstring& wstring() const
		{
			return src;
		}
		std::string string() const
		{
			return utils::FromWide(src);
		}

		static AXELYNX_API std::wstring FromInteger(int value);
		static AXELYNX_API std::wstring FromDouble(double value);
	};
}
