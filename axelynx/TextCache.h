#pragma once

#include "Font.h"


namespace axelynx
{
	class TextCache
	{
	public:
		virtual void Resize(int newsidth, int newheight) = 0;
		
		virtual void AddText(const char* ansi_string)=0;
		virtual void AddText(const wchar_t* unicode_string)=0;
		virtual void RemoveText(const char* ansi_string)=0;
		virtual void RemoveText(const wchar_t* unicode_string)=0;

		virtual void DrawText(const char* ansi_string)=0;
		virtual void DrawText(const wchar_t* unicode_string)=0;

		virtual void StartDraw(const Canvas *c)=0;
		virtual void EndDraw()=0;

		virtual ~TextCache(){};
	};
}