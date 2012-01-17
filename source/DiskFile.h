#pragma once

#include "axelynx/File.h"
#include <cstdio>

class DiskFile : public axelynx::File::FileImpl
{
	FILE *file_;
	long size_;
	std::wstring filename_;
public:

	DiskFile(const char *filename);
	DiskFile(const wchar_t *filename);
	DiskFile(FILE *file);
	virtual bool Open();
	virtual bool Create();
	virtual unsigned __int64 Size();
	virtual unsigned __int64 Seek(__int64 delta, axelynx::File::SeekType ST = axelynx::File::ST_CUR);
	virtual unsigned __int64 Pos();	

	virtual bool Write(const void *data, unsigned __int64 size);
	virtual bool Read(void *data, unsigned __int64 size);

	virtual void* GetPointer()
	{
		return 0;
	}

	virtual bool Close();
	virtual ~DiskFile();

	virtual std::string ReadString();
	virtual std::wstring ReadWideString();


	virtual std::wstring GetFileName()
	{
		return filename_;
	}

	virtual bool Eof();
};