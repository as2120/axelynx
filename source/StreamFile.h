#pragma once
#include <axelynx/File.h>
#include <fstream>

class StreamFile : public axelynx::File::FileImpl
{
	std::fstream &stream;
public:
	virtual bool Open();
	virtual bool Create()
	{
		return Open();
	}

	StreamFile(std::fstream &fs) : stream(fs)
	{
	}

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
	virtual ~StreamFile(){};

	virtual std::wstring GetFileName()
	{
		return L"";
	}

	virtual std::string ReadString();
	virtual std::wstring ReadWideString();

	virtual bool Eof();
};