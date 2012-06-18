#include <axelynx/utils/axstring.h>
#include <wchar.h>
#include <string.h>
#include <cstdlib>
#include <windows.h>
//#include <winnls.h>

AXELYNX_API wchar_t* axelynx::utils::MultiByteToWideChar(const char* s, wchar_t* into)
{
#ifdef WIN32
int i = 0;
i = (int)::MultiByteToWideChar(CP_ACP ,MB_PRECOMPOSED,s,-1,NULL,0);

wchar_t *d=into?into:((wchar_t *)malloc((i+1)*sizeof(wchar_t)));
i=(int)::MultiByteToWideChar(CP_ACP ,MB_PRECOMPOSED,s,-1,d,i);
d[i]=0;

return d;
#else
const char *ss=s;

int i=(int)mbsrtowcs(NULL,&ss,0,NULL);
if (i<0)
return NULL;
wchar_t *d=(wchar_t *)malloc((i+1)*sizeof(wchar_t));
mbsrtowcs(d,&s,i,NULL);
d[i]=0;

return d;
#endif
}

AXELYNX_API char* axelynx::utils::WideCharToMultiByte(const wchar_t* s, char* into)
{
#ifdef WIN32
UINT codePage=CP_ACP;

int i=(int)::WideCharToMultiByte(codePage, // code page
0, // performance and mapping flags
s, // wide-character string
-1, // number of chars in string
NULL, // buffer for new string
0, // size of buffer
NULL, // default for unmappable chars
NULL // set when default char used
);

if (i<0)
return NULL;


char *d= into?into:((char*)malloc(i+1));
::WideCharToMultiByte(codePage, // code page
0, // performance and mapping flags
s, // wide-character string
-1, // number of chars in string
d, // buffer for new string
i, // size of buffer
NULL, // default for unmappable chars
NULL // set when default char used
);
d[i]=0;

return d;
#else
const wchar_t *ss=s;

int i=(int)wcsrtombs(NULL,&ss,0,NULL);
if (i<0)
return NULL;
char *d=(char *)malloc(i+1);
wcsrtombs(d,&s,i,NULL);
d[i]=0;

return d;
#endif
}


AXELYNX_API wchar_t* axelynx::utils::AnsiToWideChar(const char* s, wchar_t* into)
{
    return 0;
}

AXELYNX_API char* axelynx::utils::WideCharToAnsi(const wchar_t* s, char* into)
{
    int len  =wcslen(s);

    char *buff = new char[len+1];
    char *src = WideCharToMultiByte(s);
    for(int i=0;i<len;++i)
    {
        buff[i] = src[i*2];
    }
    buff[len]=0;
    delete[] src;
    return buff;
}

AXELYNX_API std::wstring axelynx::utils::GetExtension(std::wstring filename)
{
	int l = filename.find_last_of(L'.');
	int size = filename.length() - l;

	std::wstring ext =  filename.substr(l+1,size);

	return ext;
}


AXELYNX_API void axelynx::utils::GetRelativePath(const char * absolute, const char * project, char *output)
{
	int l = strlen(absolute);

	int eq = -1;

	for(int i=0;i<l;++i)
	{
		if(absolute[i] != project[i])
			break;

		if(absolute[i] == '/')
			eq = i;
	}

	if(eq == -1)
	{
		strcpy(output,absolute);
		return;
	}

	int l2 = strlen(project);

	int folders = 0;

	for(int i = eq;i<l2;++i)
	{
		if(project[i] == '/')
			folders++;
	}

	int npos = 0;
	for(int i=0;i<folders;++i)
	{
		output[npos + 0] = '.';
		output[npos + 1] = '.';
		output[npos + 2] = '/';
		npos += 3;
	}

	strcpy(output + npos,absolute + eq +1);
}

AXELYNX_API std::wstring ToWide(std::string str)
{
	const wchar_t *rstr = axelynx::utils::MultiByteToWideChar(str.c_str());
	return rstr;
}

AXELYNX_API std::string ToMultibyte(std::wstring str)
{
	const char *rstr = axelynx::utils::WideCharToMultiByte(str.c_str());
	return rstr;
}