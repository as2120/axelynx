#include "MemoryFile.h"
#include <cstring>

unsigned __int64 MemoryFile::Size()
{
	return filesize_;
}

bool MemoryFile::Open()
{
	return true;
}

unsigned __int64 MemoryFile::Seek(__int64 delta, axelynx::File::SeekType ST)
{
	if(ST == axelynx::File::ST_SET)
	{
		cur_ptr_ = static_cast<char*>(file_ptr_) +delta;
	}

	if(ST == axelynx::File::ST_CUR)
	{
		cur_ptr_ = static_cast<char*>(cur_ptr_) +delta;
	}

	if(ST == axelynx::File::ST_END)
	{
		cur_ptr_ = static_cast<char*>(file_ptr_) + (filesize_ - delta);
	}

	return Pos();
}

unsigned __int64 MemoryFile::Pos()
{
	return static_cast<char*>(cur_ptr_) - static_cast<char*>(file_ptr_);
}

bool MemoryFile::Write(const void *data, unsigned __int64 size)
{
	if(!cur_ptr_)
		return false;

	memcpy(cur_ptr_,data,size);
	cur_ptr_ = static_cast<char*>(cur_ptr_) +size;
	return true;
}

bool MemoryFile::Read(void *data, unsigned __int64 size)
{
	if(!cur_ptr_)
		return false;

	memcpy(data,cur_ptr_,size);
	cur_ptr_ = static_cast<char*>(cur_ptr_) +size;
	return true;
}

bool MemoryFile::Close()
{
	file_ptr_ = 0;
	cur_ptr_ = 0;

	filesize_ = 0;
	return true;
}

MemoryFile::~MemoryFile()
{
}

MemoryFile::MemoryFile(void *ptr, size_t size)
{
	file_ptr_ = ptr;
	cur_ptr_ = ptr;

	filesize_ = size;
}

void* MemoryFile::GetPointer()
{
	return cur_ptr_;
}



std::string MemoryFile::ReadString()
{
	std::string temp = (const char*)cur_ptr_;
	Seek(temp.length() + 1);
	return temp;
}

std::wstring MemoryFile::ReadWideString()
{
	std::wstring temp = (const wchar_t*)cur_ptr_;
	Seek((temp.length() + 1)*2);
	return temp;
}

bool MemoryFile::Eof()
{
	auto diff = ((char*)cur_ptr_ - (char*)file_ptr_);

	if(diff>=0)
	{
		return  static_cast<decltype(filesize_)>(diff)>= filesize_;
	}

	return true;
}
