

#include "CPak.h"


		
CPak::CPak(const wchar_t *filename)
{
}

axelynx::File::FileImpl* CPak::_getFile(const char *filename)
{
	return new PakFile(this,filename);
}

axelynx::File::FileImpl* CPak::_getFile(const wchar_t *filename)
{
	return new PakFile(this,filename);
}

CPak::~CPak()
{
	//return new PakFile(this,filename);
}