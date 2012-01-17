#include "StreamFile.h"

unsigned __int64 StreamFile::Size()
{
	return 0;
}

unsigned __int64 StreamFile::Seek(__int64 delta, axelynx::File::SeekType ST)
{
	return 0;
}

unsigned __int64 StreamFile::Pos()
{
	return 0;
}

bool StreamFile::Write(const void *data, unsigned __int64 size)
{
	stream.write(static_cast<const char*>(data),size);
	return true;
}

bool StreamFile::Read(void *data, unsigned __int64 size)
{
	stream.read(static_cast<char*>(data),size);
	return true;
}

bool StreamFile::Close()
{
	stream.close();
	return true;
}

bool StreamFile::Open()
{
	return stream.is_open();
}


std::string StreamFile::ReadString()
{
	std::string temp;
	stream>>temp;
	return temp;
}

std::wstring StreamFile::ReadWideString()
{
	std::wstring temp=L"";
//	stream>>temp;
	return temp;
}

bool StreamFile::Eof()
{
	return stream.eof();
}