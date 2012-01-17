

#include "DiskFile.h"
#include "CFile.h"
#include "axelynx/utils/axstring.h"

DiskFile::DiskFile(const char *filename)
{
	wchar_t buff[280];
	axelynx::utils::MultiByteToWideChar(filename,buff);
	filename_ = buff;
	file_ = 0;
	size_ = 0;
}

DiskFile::DiskFile(const wchar_t *filename)
{
	filename_ = filename;
	file_ = 0;
	size_ = 0;
}

bool DiskFile::Open()
{
	if(axFileType(filename_.c_str()))
	{
		file_ = axReadFile(filename_.c_str());
	}
	else
	{
		file_ = axWriteFile(filename_.c_str());
	}
	return true;
}

DiskFile::DiskFile(FILE *file)
{
	file_ = file;
	size_ = 0;
	filename_ = L"";
}

unsigned __int64 DiskFile::Size()
{
	if(!file_)
		return 0;

	if(!size_)
	{
		long cur = ftell(file_);
		fseek(file_,0,SEEK_END);
		size_ = ftell(file_);
		fseek(file_,cur,SEEK_SET);
	}
	return size_;
}

unsigned __int64 DiskFile::Seek(__int64 delta, axelynx::File::SeekType ST)
{
	if(!file_)
		return 0;

	int orign = SEEK_SET;
	if(ST == axelynx::File::ST_CUR)
		orign = SEEK_CUR;
	if(ST == axelynx::File::ST_END)
		orign = SEEK_END;
	
	fseek(file_,delta,orign);
	return ftell(file_);
}

unsigned __int64 DiskFile::Pos()
{
	if(file_)
	return ftell(file_);
	else
		return 0;
}

bool DiskFile::Write(const void *data, unsigned __int64 size)
{
	if(file_)
		fwrite(data,1,size,file_);

	return true;
}

bool DiskFile::Read(void *data, unsigned __int64 size)
{
	if(file_)
		fread(data,1,size,file_);

	return true;
}

bool DiskFile::Close()
{
	if(file_)
		{
			auto result = fclose(file_)!=0;
			file_ = 0;
			return result;
	}
	else
		return false;
}

DiskFile::~DiskFile()
{
	if(file_)
	{
		Close();
	}
}


std::string DiskFile::ReadString()
{
	char c;
	fread(&c,1,1,file_);
	if(c==0)
	{
		return "";
	}

	std::string temp = "";
	while(c!=0)
	{
		temp += c;
		fread(&c,1,1,file_);
	}

	return temp;
}

std::wstring DiskFile::ReadWideString()
{
	wchar_t c;
	fread(&c,1,1,file_);
	if(c==0)
	{
		return L"";
	}

	std::wstring temp = L"";
	while(c!=0)
	{
		temp += c;
		fread(&c,sizeof(wchar_t),1,file_);
	}

	return temp;
}

bool DiskFile::Eof()
{
	return feof(file_)!=0;
}

bool DiskFile::Create()
{
	file_ = axWriteFile(filename_.c_str());
	return true;
}