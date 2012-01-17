#include "PakFile.h"

PakFile::PakFile(CPak *pak, const wchar_t *filename)
{
}

PakFile::PakFile(CPak *pak, const char *filename)
{
}

bool PakFile::Open()
{
	return true;
}
unsigned __int64 PakFile::Size()
{
	return 0;
}
unsigned __int64 PakFile::Seek(__int64 delta, axelynx::File::SeekType ST)
{
	return 0;
}
unsigned __int64 PakFile::Pos()
{
	return 0;
}

bool PakFile::Write(const void *data, unsigned __int64 size)
{
	return true;
}
bool PakFile::Read(void *data, unsigned __int64 size)
{
	return true;
}
bool PakFile::Close()
{
	return true;
}

PakFile::~PakFile()
{
}