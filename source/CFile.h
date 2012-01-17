#pragma once

#include <cstdlib>
#include <cstdio>


FILE* axReadFile(const wchar_t *filename);
FILE* axWriteFile(const wchar_t *filename);
FILE* axOpenFile(const wchar_t *filename);
bool axFileType(const wchar_t *filename);
