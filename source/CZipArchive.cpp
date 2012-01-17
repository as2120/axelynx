

#include "CZipArchive.h"
#include "axelynx/utils/axstring.h"

CZipArchive::CZipArchive(const wchar_t *filename)
{
	char buff[260];
	axelynx::utils::WideCharToMultiByte(filename,buff);

	file_ = unzOpen(buff);
}


axelynx::File::FileImpl* CZipArchive::_getFile(const char *filename)
{
	wchar_t buff[260];
	axelynx::utils::MultiByteToWideChar(filename,buff);
	return new ZippedFile(this,buff);
}

axelynx::File::FileImpl* CZipArchive::_getFile(const wchar_t *filename)
{
	return new ZippedFile(this,filename);
}

CZipArchive::~CZipArchive()
{
	unzCloseCurrentFile(file_);
}