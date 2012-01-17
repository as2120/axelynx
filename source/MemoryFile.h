#pragma once

#include "axelynx/File.h"

class MemoryFile : public axelynx::File::FileImpl
{
	void *file_ptr_;
	void *cur_ptr_;

	size_t filesize_;
public:
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

	virtual void* GetPointer();

	virtual ~MemoryFile();

	MemoryFile(void *ptr, size_t size);

	virtual std::string ReadString();
	virtual std::wstring ReadWideString();


	virtual std::wstring GetFileName()
	{
		return L"";
	}

	virtual bool Eof();
};