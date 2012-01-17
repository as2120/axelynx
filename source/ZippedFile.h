#pragma once
#include "axelynx/File.h"
#include <cstdlib>
#include "libs/unzip/unzip.h"
#include "libs/zlib/zlib.h"

class CZipArchive;

class ZippedFile : public axelynx::File::FileImpl
{
	unz_file_info file_info_;
	unzFile zip_file_;
	std::string filename_;
public:
	ZippedFile(CZipArchive* archive, const wchar_t *filename);
	ZippedFile(CZipArchive* archive, const char *filename);

	virtual bool Open();
	virtual bool Create()
	{
		return Open();
	}
	virtual unsigned __int64 Size();
	virtual unsigned __int64 Seek(__int64 delta, axelynx::File::SeekType ST = axelynx::File::ST_CUR);
	virtual unsigned __int64 Pos();	

	virtual bool Write(const void *data, unsigned __int64 size);
	virtual bool Read(void *data, unsigned __int64 size);
	virtual bool Close();

	virtual void* GetPointer()
	{
		return 0;
	}

	virtual ~ZippedFile();

	virtual std::wstring GetFileName()
	{
		return L"";
	}

	virtual std::string ReadString()
	{
		return "";
	}
	virtual std::wstring ReadWideString()
	{
		return L"";
	}

	virtual bool Eof()
	{
		return false;
	}

};