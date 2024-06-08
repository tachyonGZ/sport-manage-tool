#include "../inc/pch.h"
#include "../inc/process_password.h"

#define PASSWORD_MAX_LENGTH 31

const char *g_szFileName = ".\\password";
char g_szPassword[PASSWORD_MAX_LENGTH + 1] = "00000000";

bool _store_password()
{
	FILE *pFile = _fopen_auto_create(g_szFileName, "wb");
	if (NULL == pFile)
	{
		abort();
	}
	fwrite(g_szPassword, sizeof(const char), 32, pFile);
	fclose(pFile);
	return true;
}

bool load_password()
{
	if (!_is_file_exist(g_szFileName))
	{
		return _store_password();
	}

	FILE *pFile = _fopen_auto_create(g_szFileName, "rb");
	if (NULL == pFile)
	{
		abort();
	}

	fread(g_szPassword, sizeof(const char), 32, pFile);
	fclose(pFile);
	return true;
}



const char *get_password()
{
	return g_szPassword;
}

bool is_password_right(const char *szPassword)
{
	if (strlen(szPassword) > PASSWORD_MAX_LENGTH)
	{
		return false;
	}
	return 0 == strcmp(szPassword, g_szPassword);
}

bool reset_password(const char *szPassword)
{
	if (strlen(szPassword) > PASSWORD_MAX_LENGTH)
	{
		return false;
	}

	strcpy(g_szPassword, szPassword);
	return _store_password();
}

const size_t get_password_max_length()
{
	return PASSWORD_MAX_LENGTH;
}