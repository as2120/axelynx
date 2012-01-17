//#ifdef WIN32

#include "axelynx/Shell.h"
#include <windows.h>
#include <ShellAPI.h>

AXELYNX_API void axelynx::shell::ShowMessage(const char * message)
{
	MessageBoxA(0,message,"axelynx",0);
}

AXELYNX_API void axelynx::shell::ShowMessage(const wchar_t * message)
{
	MessageBoxW(0,message,L"axelynx",0);
}

AXELYNX_API void axelynx::shell::ShowError(const char * message)
{
	MessageBoxA(0,message,"error",0);
}

AXELYNX_API void axelynx::shell::ShowError(const wchar_t * message)
{
	MessageBoxW(0,message,L"error",0);
}

AXELYNX_API void axelynx::shell::Execute(const char * message)
{
	ShellExecuteA(0,"open",message,0,0,SW_SHOWNORMAL);
}

AXELYNX_API void axelynx::shell::Execute(const wchar_t * message)
{
	ShellExecuteW(0,L"open",message,0,0,SW_SHOWNORMAL);
}

//#endif