#include "../inc/pch.h"

#include "../inc/input.h"

char *g_szInput = NULL;
size_t g_inputMaxCount = 0;

void release_user_input()
{
	if (g_szInput)
	{
		free(g_szInput);
	}
}

bool init_user_input(const size_t maxBufferSize)
{
	release_user_input();
	
	g_szInput = calloc(maxBufferSize, sizeof(char));
	if (NULL == g_szInput)
	{
		//ÄÚ´æ·ÖÅäÊ§°Ü
		return false;
	}

	g_inputMaxCount = maxBufferSize;
	
	return true;
}

const char *get_user_input_by_str()
{
	if (NULL == g_szInput)
	{
		return false;
	}

	fputs("©¦>", stdout);
	fgets(g_szInput, (int) g_inputMaxCount, stdin);
	g_szInput[strlen(g_szInput) - 1] = '\0';
	return g_szInput;
}

int _print_split()
{
	return fputs("©°©¤©¤©¤\n", stdout);
}