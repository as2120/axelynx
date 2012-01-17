#pragma once

#include "axlib.h"
#include "Canvas.h"
#include "File.h"

#include <string>

namespace axelynx
{

	class Font
	{
	public:
		struct Desc
		{
			File file;
			int size;
			bool bold;
			bool italic;

			Desc()
			{
				file = L"arial.ttf";
				init();
			}

			Desc(const wchar_t *filename_)
			{
				file = filename_;
				init();
			}

			Desc(File file_)
			{
				file = file_;
				init();
			}
		private:
			void init()
			{
				size = 16;
				bold = false;
				italic = false;
			}
		};

		virtual bool Bind() const =0;
		virtual bool UnBind() const = 0;

		virtual void FAST_CALL Draw(const Canvas *c,const char *ansi_string, bool center = false) const =0;
		virtual void FAST_CALL Draw(const Canvas *c,const wchar_t *wide_string, bool center = false) const =0;

		virtual int GetHeight() const = 0;
		virtual int FAST_CALL GetWidth(const char *ansi_string) const = 0;
		virtual int FAST_CALL GetWidth(const wchar_t *wide_string) const = 0;

		virtual void StartDraw(const Canvas *c) const = 0;
		virtual void EndDraw() const = 0;

		virtual ~Font(){};
	};
}