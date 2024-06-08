#include "../inc/pch.h"
#include "../inc/file.h"

FILE *_fopen_auto_create(const char *_FileName, const char *_Mode)
{
	FILE *pFile = fopen(_FileName, "rb");
	if(NULL == pFile)
	{
		pFile = fopen(_FileName, "wb");
		if (pFile)
		{
			fclose(pFile);
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		fclose(pFile);
	}

	return fopen(_FileName, _Mode);
}

bool _is_file_exist(const char *_FileName)
{
	FILE *pFile = fopen(_FileName, "rb");
	if (NULL == pFile)
	{
		return false;
	}
	fclose(pFile);
	return true;
}