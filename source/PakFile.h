#pragma once
#include "axelynx/File.h"
#include <cstdlib>
class CPak;

class PakFile : public axelynx::File::FileImpl
{
	
public:
	PakFile(CPak *pak, const wchar_t *filename);
	PakFile(CPak *pak, const char *filename);

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

	virtual ~PakFile();

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