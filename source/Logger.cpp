#include <axelynx\Logger.h>
#include <string>
#include <ctime>
#include <cstdlib>
#include <wchar.h>
#include "axelynx/utils/axstring.h"
#include <iostream>
#include <windows.h>
#include <axelynx\Engine.h>

extern bool AxelynxEngineSettingsFailOnError;

#if defined(_MSC_VER) && _MSC_VER > 1310

#define UTF8_LOCALE "" // default
const char * fromutf8(const wchar_t * str)
{
	const int MAX_TOUTF8_BUFFERS = 16;
	const int TOUTF8_BUFFER_SIZE = 1024;

        static int bufno = 0;
        static char buf[MAX_TOUTF8_BUFFERS][TOUTF8_BUFFER_SIZE] = {0};

        if(bufno > MAX_TOUTF8_BUFFERS)
                bufno = 0;

        WideCharToMultiByte(CP_UTF8, 0, str, -1, buf[bufno], TOUTF8_BUFFER_SIZE, NULL, NULL);

        bufno++;

        return buf[bufno - 1];
}
#define UTEXT(text) fromutf8(text)
#else
#define UTF8_LOCALE ".UTF-8"
#define UTEXT(text) text
#define toutf8(text) text
#endif

axelynx::Logger* axelynx::Logger::cur_logger = 0;

std::wstring axelynx::Logger::GetLogFileName()
{
	return filename_;
}

axelynx::Logger::Logger(const char*		filename)
{

	setlocale(LC_ALL, UTF8_LOCALE);
	freopen(filename, "w", stderr);

	wchar_t wbuff1[255];
	GetCurrentDirectoryW(225,wbuff1);

	wchar_t wbuff[260];
	axelynx::utils::MultiByteToWideChar(filename,wbuff);
	filename_ = wbuff1;
	filename_ += L"/";
	filename_ += (std::wstring)wbuff;

	std::wcout<<L"FILENAME FILENAME FILENAME:"<<filename_<<std::endl;
}

void AtExitShutdownLogger()
{
	axelynx::Logger::Instance()->Free();
}

AXELYNX_API bool axelynx::Logger::Init(const wchar_t *application)
{
	try
	{
		axelynx::Logger *l = axelynx::Logger::Instance();

		std::wstring header = L"<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><title>" + (std::wstring)application;
		header += L"</title></head><body>";


		l->Write(header.c_str());
		l->Write(L"<h1>axelynx log file</h1><hr>");

//		tm time;
		//_getsystime(&time);

		std::wstring wsd = L"<h4>start date: ";

		wchar_t datetime[80];

		//_swprintf_c(datetime,80,L"%d.%d.%d - %d:%d:%d",time.tm_mday,time.tm_mon+1,time.tm_year+1900,time.tm_hour,time.tm_min,time.tm_sec);

		wsd += datetime;
		wsd += L"</h4><hr>";

		l->Write(wsd.c_str());
	}
	catch(...)
	{
		return false;
	}

	return true;
}

AXELYNX_API bool axelynx::Logger::Free()
{
	if(file_)
	{
//		tm time;
		//_getsystime(&time);

		std::wstring wsd = L"<hr><h4>end date: ";

		wchar_t datetime[80];

		//_swprintf_c(datetime,80,L"%d.%d.%d - %d:%d:%d",time.tm_mday,time.tm_mon+1,time.tm_year+1900,time.tm_hour,time.tm_min,time.tm_sec);

		wsd += datetime;
		wsd += L"</h4><hr>";

		Write(wsd.c_str());

		std::wstring footer = L"</body></html>";

		Write(footer.c_str());
		//fclose(file_);
	}

	file_=0;

	return true;
}

AXELYNX_API axelynx::Logger* axelynx::Logger::Instance()
{
	if(cur_logger)
		return cur_logger;
	else
	{
		if(!cur_logger) //MT safe
		{
			cur_logger = new axelynx::Logger("axelynx_log.htm");

			//atexit(AtExitShutdownLogger);
		}

		return cur_logger;
	}
}

bool axelynx::Logger::Show(const mat3x2& value)
{
	if(file_)
	{
		Write(L"<table border=1>");
		char buff[80];
		for(int i=0;i<3;++i)
		{
			Write(L"<tr>");
			for(int j=0;j<2;++j)
			{
			    #ifdef _MSVCRT_
				Write(L"<td>");
				gcvt(value(i,j),8,buff);
				wchar_t *result = utils::MultiByteToWideChar(buff);
				Write(result);
				Write(L"</td>");
				#endif
			}
			Write(L"</tr>");
		}
		Write(L"</table>");
	}

	return true;
}
bool axelynx::Logger::Write(const wchar_t * message,...)
{
	if(file_)
	{
		fwprintf(stderr,message);
		fflush(stderr);
	}
	return file_ != 0;
}

bool axelynx::Logger::Error(const wchar_t * message,...)
{
	Write(L"<font color = red>");
	bool result = Write(message);
	Write(L"\n");
	Write(L"</font><br>");

	return result;
}

bool axelynx::Logger::Warning(const wchar_t * message,...)
{
	Write(L"<font color = yellow>");
	bool result = Write(message);
	Write(L"\n");
	Write(L"</font><br>");
	return result;
}

bool axelynx::Logger::Hint(const wchar_t * message,...)
{
	Write(L"<font color = green>");
	bool result = Write(message);
	Write(L"\n");
	Write(L"</font><br>");
	return result;
}

bool axelynx::Logger::SourceError(const char *file, int line, const wchar_t *error, unsigned int errcode)
{
	char buff[1024];
	sprintf(buff,"<table border=1><tr><td bgcolor=silver>%s</td><td bgcolor=silver>%d</td><td><font color=red>",file,line);
	wchar_t *nbuff = utils::MultiByteToWideChar(buff);
	Write(nbuff);
	delete[] nbuff;

	Write(error);


	sprintf(buff,":0x%X</font></td></tr></table>",errcode);
	nbuff = utils::MultiByteToWideChar(buff);
	Write(nbuff);
	delete[] nbuff;

	return true;
}

bool axelynx::Logger::Write(const std::wstring &message)
{
	return axelynx::Logger::Write(message.c_str());
}

bool axelynx::Logger::Error(const std::wstring &message)
{
	return axelynx::Logger::Error(message.c_str());
}

bool axelynx::Logger::Warning(const std::wstring &message)
{
	return axelynx::Logger::Warning(message.c_str());
}

bool axelynx::Logger::Hint(const std::wstring &message)
{
	return axelynx::Logger::Hint(message.c_str());
}

bool axelynx::Logger::FailOnEngineError()
{
	return axelynx::Engine::Instance()->Settings().Debug.FailOnEngineError;
}