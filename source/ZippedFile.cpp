#include "ZippedFile.h"

#include "CZipArchive.h"
#include "axelynx/utils/axstring.h"

ZippedFile::ZippedFile(CZipArchive* archive, const wchar_t *filename)
{
	zip_file_ = archive->file_;

	char buff[260];
	axelynx::utils::WideCharToMultiByte(filename,buff);

	filename_ = buff;
}

ZippedFile::ZippedFile(CZipArchive* archive, const char *filename)
{
	zip_file_ = archive->file_;
	filename_ = filename;

}

bool ZippedFile::Open()
{
	unzLocateFile(zip_file_,filename_.c_str(),0);
	unzGetCurrentFileInfo(zip_file_,&file_info_,0,0,0,0,0,0);
	unzOpenCurrentFile(zip_file_);

	return true;
}

unsigned __int64 ZippedFile::Size()
{
	return file_info_.uncompressed_size;
}

unsigned __int64 ZippedFile::Seek(__int64 delta, axelynx::File::SeekType ST)
{
	return 0;
}

unsigned __int64 ZippedFile::Pos()
{
	return 0;
}

bool ZippedFile::Write(const void *data, unsigned __int64 size)
{
	return 0;
}

bool ZippedFile::Read(void *data, unsigned __int64 size)
{
	unzReadCurrentFile(zip_file_,data,size);
	return true;
}

bool ZippedFile::Close()
{
	unzCloseCurrentFile(zip_file_);
	return true;
}

ZippedFile::~ZippedFile()
{
}
