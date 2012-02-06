#pragma once
#ifndef AXELYNX_LOGGER_H
#define AXELYNX_LOGGER_H

#include "axlib.h"

#include <cstdio>
#include <string>
#include "math/vectormath.h"

#ifdef LOG_ON_COUT
#include <iostream>
#endif

namespace axelynx
{
	class Logger
	{
		FILE *file_;
		static Logger * cur_logger;
		std::wstring filename_;
		Logger(const char*		filename);

		
	public:
		bool FailOnEngineError();

		AXELYNX_API bool Init(const wchar_t *application);
		AXELYNX_API bool Free();

		static AXELYNX_API Logger* Instance();

		bool Write(const wchar_t * message,...);
		bool Error(const wchar_t * message,...);
		bool Warning(const wchar_t * message,...);
		bool Hint(const wchar_t * message,...);

		bool SourceError(const char *file, int line, const wchar_t *error, unsigned int errcode);
		bool Write(const std::wstring &message);
		bool Error(const std::wstring &message);
		bool Warning(const std::wstring &message);
		bool Hint(const std::wstring &message);

		std::wstring GetLogFileName();
		bool Show(const mat3x2& value);
	};
}



#ifdef LOG_ON_COUT

#define LOG_WRITE(a) {std::wcout<<a<<std::endl;}
#define LOG_ERROR(a) {std::wcout<<a<<std::endl;}
#define LOG_WARNING(a) {std::wcout<<a<<std::endl;}
#define LOG_HINT(a) {std::wcout<<a<<std::endl;}

#else

#define LOG_WRITE(a) {axelynx::Logger::Instance()->Write(a);}
#define LOG_ERROR(a) {axelynx::Logger::Instance()->Error(a);{if(axelynx::Logger::Instance()->FailOnEngineError())assert(!"Engine Error!");}}
#define LOG_WARNING(a) {axelynx::Logger::Instance()->Warning(a);}
#define LOG_HINT(a) {axelynx::Logger::Instance()->Hint(a);}

#define LOG_SOURCE_ERROR(a,errcode) axelynx::Logger::Instance()->SourceError(__FILE__,__LINE__,a,errcode);
#endif

#endif
