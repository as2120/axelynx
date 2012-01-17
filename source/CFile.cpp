
#include "axelynx/Shell.h"
#include "axelynx/Engine.h"
#include "axelynx/File.h"

#include "CFile.h"
#include "axelynx/utils/axstring.h"
// Подкручиваем логирование
#include "axelynx/Logger.h"

#include "DiskFile.h"
#include "MemoryFile.h"
#include "PakFile.h"
#include "StreamFile.h"

#include "axelynx/Pak.h"

FILE* axReadFile(const wchar_t *filename)
{
    FILE *f = 0;

    #ifdef _MSVCRT_
    f =  _wfopen (filename, L"rb");
    #else
    char *filename_ansi = axelynx::utils::WideCharToMultiByte(filename);
    f = fopen(filename_ansi,"rb");
       delete[] filename_ansi;
    #endif

    if (!f)
    {
		axelynx::Engine::Settings::FileSystemSettings::FileExistsLevel_Enum level = axelynx::Engine::Instance()->Settings().FileSystem.FileExistsLevel;

		std::wstring errinfo = L"File not found:";
		errinfo+= std::wstring(filename);

		if(level == axelynx::Engine::Settings::FileSystemSettings::FEL_LOG_WARNING ||
			level == axelynx::Engine::Settings::FileSystemSettings::FEL_FATAL)
		{
			LOG_ERROR(errinfo.c_str());
		}

		if(level == axelynx::Engine::Settings::FileSystemSettings::FEL_MESSAGE_WARNING ||
			level == axelynx::Engine::Settings::FileSystemSettings::FEL_FATAL)
		{
			axelynx::shell::ShowError(errinfo.c_str());
		}


		if(level == axelynx::Engine::Settings::FileSystemSettings::FEL_FATAL)
		{
			exit(0);
		}
      return 0;
    }

    return f;
}

FILE* axWriteFile(const wchar_t *filename)
{
    #ifdef _MSVCRT_
    return _wfopen (filename, L"wb");
    #else
    FILE *f = 0;
    char *filename_ansi = axelynx::utils::WideCharToMultiByte(filename);
    f = fopen(filename_ansi,"wb");
    delete[] filename_ansi;
    return f;
    #endif
}

FILE* axOpenFile(const wchar_t *filename)
{
    #ifdef _MSVCRT_
    return _wfopen (filename, L"rwb");
    #else
    FILE *f = 0;
    char *filename_ansi = axelynx::utils::WideCharToMultiByte(filename);
    f = fopen(filename_ansi,"rwb");
    delete[] filename_ansi;
    return f;
    #endif
}

bool axFileType(const wchar_t*filename)
{
	FILE *f;

    #ifdef _MSVCRT_
    f =  _wfopen (filename, L"rb");
    #else
    char *filename_ansi = axelynx::utils::WideCharToMultiByte(filename);
    f = fopen(filename_ansi,"rb");
       delete[] filename_ansi;
    #endif

	if(f)
	{
		fclose(f);
		return true;
	}

	return false;
}


AXELYNX_API axelynx::File::FileImpl * axelynx::File::GetFromFilename(const char * filename)
{
	return new DiskFile(filename);
}

AXELYNX_API axelynx::File::FileImpl * axelynx::File::GetFromFilename(const wchar_t * filename)
{
	return new DiskFile(filename);
}

AXELYNX_API axelynx::File::FileImpl * axelynx::File::GetFromFile(FILE *file)
{
	return new DiskFile(file);
}

AXELYNX_API axelynx::File::FileImpl * axelynx::File::GetFromStream(std::fstream &stream)
{
	return new StreamFile(stream);
}

AXELYNX_API axelynx::File::FileImpl * axelynx::File::GetFromMemory(void *data, long size)
{
	return new MemoryFile(data, size);
}

AXELYNX_API axelynx::File::FileImpl * axelynx::File::GetFromPak(axelynx::Pak *pak, const wchar_t * filename)
{
	return pak->_getFile(filename);
}

AXELYNX_API axelynx::File::FileImpl * axelynx::File::GetFromPak(axelynx::Pak *pak, const char * filename)
{
	return pak->_getFile(filename);
}
