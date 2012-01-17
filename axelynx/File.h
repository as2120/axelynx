#pragma once

#include "axlib.h"

#include <string>
#include <fstream>

namespace axelynx
{
	class Pak;

	class File
	{
	public:
		enum SeekType
		{
			ST_SET
			,ST_CUR
			,ST_END
		};


		class FileImpl
		{
		public:
			virtual bool Open() = 0;
			virtual bool Create() = 0;
			virtual unsigned __int64 Size()=0;
			virtual unsigned __int64 Seek(__int64 delta, axelynx::File::SeekType ST = axelynx::File::ST_SET)=0;
			virtual unsigned __int64 Pos()=0;

			virtual bool Write(const void *data, unsigned __int64 size)=0;
			virtual bool Read(void *data, unsigned __int64 size)=0;

			virtual void* GetPointer()=0;
			virtual bool Close()=0;
			virtual ~FileImpl(){};

			
			virtual std::string ReadString()=0;
			virtual std::wstring ReadWideString()=0;
			virtual std::wstring GetFileName()=0;

			virtual bool Eof()=0;
		};

	private:
		FileImpl *impl_;

		static AXELYNX_API FileImpl * GetFromFilename(const char * filename);
		static AXELYNX_API FileImpl * GetFromFilename(const wchar_t * filename);
		static AXELYNX_API FileImpl * GetFromFile(FILE *f);
		static AXELYNX_API FileImpl * GetFromStream(std::fstream &stream);
		static AXELYNX_API FileImpl * GetFromMemory(void *data, long size);
		static AXELYNX_API FileImpl * GetFromPak(Pak *pak, const wchar_t * filename);
		static AXELYNX_API FileImpl * GetFromPak(Pak *pak, const char * filename);

	public:
		File(){impl_=0;}

		File(const File& file)
		{
			impl_ = file.impl_;
		}

		File(const char * filename)
			:impl_(GetFromFilename(filename)) {}
		File(const wchar_t *filename)
			:impl_(GetFromFilename(filename)) {}
		File(std::string filename)
			:impl_(GetFromFilename(filename.c_str())) {}
		File(std::wstring filename)
			:impl_(GetFromFilename(filename.c_str())) {}
		File(FILE *f)
			:impl_(GetFromFile(f)) {}
		File(std::fstream &stream)
			:impl_(GetFromStream(stream)) {}
		File(void *data, long size)
			:impl_(GetFromMemory(data,size)) {}
		File(Pak *pak, const wchar_t * filename)
			:impl_(GetFromPak(pak,filename)){}
		File(Pak *pak, const char * filename)
			:impl_(GetFromPak(pak,filename)){}

		bool Open()
		{
			return impl_->Open();
		}
		bool Create()
		{
			return impl_->Create();
		}

		unsigned __int64 Size()
		{
			return impl_->Size();
		}

		unsigned __int64 Seek(unsigned __int64 delta, SeekType ST = ST_CUR)
		{
			return impl_->Seek(delta,ST);
		}
		
		bool Write(const void *data, unsigned __int64 size)
		{
			return impl_->Write(data,size);
		}

		bool Read(void *data, unsigned __int64 size)
		{
			return impl_->Read(data,size);
		}

		template <typename T>
		bool Write(T &val)
		{
			return impl_->Write(&val,sizeof(val));
		}

		template <typename T>
		bool Read(T &val)
		{
			return impl_->Read(&val,sizeof(val));
		}

		bool Write(int value)
		{
			auto temp = value;
			return impl_->Write(&temp,sizeof(temp));
		}

		bool Write(float value)
		{
			auto temp = value;
			return impl_->Write(&temp,sizeof(temp));
		}

		bool WriteString(std::string value)
		{
			return impl_->Write(value.data(),value.size()+1);
		}

		bool WriteString(std::wstring value)
		{
			return impl_->Write(value.data(),(value.size()+1) * sizeof(wchar_t));
		}

		bool Close()
		{
			bool result = impl_->Close();
			delete impl_;
			impl_ = 0;
			return result;
		}

		void * GetPointer()
		{
			return impl_->GetPointer();
		}

		std::wstring GetFilename() const
		{
			return impl_->GetFileName();
		}

		std::string ReadString()
		{
			return impl_->ReadString();
		}

		std::wstring ReadWideString()
		{
			return impl_->ReadWideString();
		}

		bool Eof()
		{
			return impl_->Eof();
		}
		~File(){}
	};
}