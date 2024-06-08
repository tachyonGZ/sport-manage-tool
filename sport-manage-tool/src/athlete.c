#include "../inc/pch.h"
#include "../inc/athlete.h"

LIST_DATA *g_pAthletes = NULL;

static const char *g_szTempFileName = ".\\athlete.tmp";
static const char *g_szFileName = ".\\athlete.dat";

void clear_athletes()
{
	if(g_pAthletes)
	{
		clear_list_data(g_pAthletes);
	}
}

bool load_athletes()
{
	if (g_pAthletes)
	{
		abort();
	}

	g_pAthletes = load_list_data(sizeof(ATHLETE), g_szFileName, g_szTempFileName);

	return NULL != g_pAthletes;
}

bool store_athletes()
{
	if (NULL == g_pAthletes)
	{
		abort();
	}

	return store_list_data(g_pAthletes);
}

DATA_RES_t add_athlete(const ATHLETE *pAthlete)
{
	return list_data_add(g_pAthletes, pAthlete->id, pAthlete);
}


DATA_RES_t remove_athlete(const athleteKey_t iAthleteId)
{
	return list_data_remove(g_pAthletes, iAthleteId);
}

DATA_RES_t rewrite_athlete(const ATHLETE *pAthlete)
{
	return list_data_rewrite(g_pAthletes, pAthlete->id, pAthlete);
}

DATA_RES_t query_athlete(const athleteKey_t iAthleteId, const ATHLETE **ppAthlete)
{
	return list_data_query(g_pAthletes, iAthleteId, ppAthlete);
}

bool is_athlete_exist(const athleteId_t athleteId)
{
	return list_data_is_exist(g_pAthletes, athleteId);
}

DATA_RES_t enum_athlete(ATHLETE_ENUM_PROC fnEnumProc, void *pParam)
{
	return list_data_enum(g_pAthletes, fnEnumProc, pParam);
}

LIST *get_athletes_internal()
{
	return list_data_get_internal(g_pAthletes);
}

/*
bool store_athlete_data()
{
	// 打开athlete.tmp
	FILE* pTempData = _fopen_auto_create(g_szTempDataFileName, "wb");
	if (NULL == pTempData)
	{
		_err_open_file_fail(g_szTempDataFileName, "wb");
		return false;
	}


	// 向athlete.tmp中保存副本
	LIST_ITERATOR *pIter = get_list_begin(g_athleteData.pAthleteList);
	LIST_ITERATOR *pLast = get_list_last(g_athleteData.pAthleteList);
	{
		if (NULL == pIter || NULL == pLast)
		{
			abort();
		}

		for(; !is_list_iterator_equal(pIter, pLast); forward_list_iterator(pIter))
		{
			fwrite(get_list_iterator_value(pIter), sizeof(ATHLETE), 1, pTempData);
		}
	}
	free_list_iterator(pLast);
	free_list_iterator(pIter);
	fclose(pTempData);

	// 覆盖athlete.dat
	if (rename(g_szDataFileName, ".\\temp"))
	{
		_err_rename_file_fail(g_szTempDataFileName, ".\\temp");
		abort();
	}

	if (rename(g_szTempDataFileName, g_szDataFileName))
	{
		_err_open_file_fail(g_szTempDataFileName, g_szDataFileName);
		abort();
	}

	if (remove(".\\temp"))
	{
		fprintf(stderr, "[错误]临时文件删除失败：%s\n", ".\\temp");
		abort();
	}

	return true;
}

bool load_athlete_data()
{
	g_athleteData.pAthleteList = make_list(LIST_KEY_UINT32, sizeof(ATHLETE));
	if (NULL == g_athleteData.pAthleteList)
	{
		fputs("[错误]创建ATHLETE链表失败", stderr);
		abort();
	}

	FILE *pFile = _fopen_auto_create(g_szDataFileName, "rb");
	if (NULL == pFile)
	{
		_err_open_file_fail(g_szDataFileName, "rb");
		abort();
	}

	ATHLETE loadTemp;

	while (0 < fread(&loadTemp, sizeof(loadTemp), 1, pFile))
	{
		LIST_ITERATOR *pIter = NULL;
		if (find_list_iterator(g_athleteData.pAthleteList, loadTemp.id, &pIter))
		{
			fprintf(stderr, "[错误]文件中的数据内容冲突，文件名:%s", g_szDataFileName);
			abort();
		}

		if (!insert_list_iterator(pIter, loadTemp.id, &loadTemp, sizeof(loadTemp)))
		{
			fprintf(stderr, "[错误]插入元素失败");
			abort();
		}

		free_list_iterator(pIter);
	}

	fclose(pFile);

	g_athleteData.bLoaded = true;

	return true;
}


void clear_athlete_data()
{
if (g_athleteData.pAthleteList)
{
release_list(g_athleteData.pAthleteList);
}

g_athleteData.bLoaded = false;
}

DATA_RES_t add_athlete(const ATHLETE *pAthlete)
{
	if (!g_athleteData.bLoaded)
	{
		return DATA_RES_UNLOADED;
	}

	if (NULL == pAthlete)
	{
		return DATA_RES_FAIL;
	}

	LIST_ITERATOR *pIter = NULL;

	if (find_list_iterator(g_athleteData.pAthleteList, pAthlete->id, &pIter))
	{
		return DATA_RES_KEY_CONFLICT;
	}

	if(!insert_list_iterator(pIter, pAthlete->id, pAthlete, sizeof(ATHLETE)))
	{
		return DATA_RES_FAIL;
	}

	free_list_iterator(pIter);

	return (store_athlete_data())?(DATA_RES_SUCCESS):(DATA_RES_STORE_FAIL);
}

DATA_RES_t remove_athlete(const uint32_t iAthleteId)
{
	if (!g_athleteData.bLoaded)
	{
		return DATA_RES_UNLOADED;
	}

	LIST_ITERATOR *pIter = NULL;
	if (!find_list_iterator(g_athleteData.pAthleteList, iAthleteId, &pIter))
	{
		return DATA_RES_INVALID_KEY;
	}

	if (!erase_list_iterator(pIter))
	{
		return DATA_RES_FAIL;
	}

	free_list_iterator(pIter);

	return (store_athlete_data())?(DATA_RES_SUCCESS):(DATA_RES_STORE_FAIL);
}

DATA_RES_t query_athlete(const uint32_t iAthleteId, const ATHLETE **ppAthlete)
{
	if (!g_athleteData.bLoaded)
	{
		return DATA_RES_UNLOADED;
	}

	if (NULL == ppAthlete)
	{
		return DATA_RES_FAIL;
	}

	LIST_ITERATOR *pIter = NULL;
	if (!find_list_iterator(g_athleteData.pAthleteList, iAthleteId, &pIter))
	{
		return DATA_RES_INVALID_KEY;
	}

	*ppAthlete = (const ATHLETE *) get_list_iterator_value(pIter);

	free_list_iterator(pIter);

	return (*ppAthlete)?(DATA_RES_SUCCESS):(DATA_RES_FAIL);
}


DATA_RES_t rewrite_athlete(const ATHLETE *pAthlete)
{
	if (!g_athleteData.bLoaded)
	{
		return DATA_RES_UNLOADED;
	}

	if (NULL == pAthlete)
	{
		return DATA_RES_FAIL; 
	}

	LIST_ITERATOR *pIter = NULL;
	if (!find_list_iterator(g_athleteData.pAthleteList, pAthlete->id, &pIter))
	{
		return DATA_RES_INVALID_KEY;
	}


	if (!set_list_iterator_value(pIter, pAthlete, sizeof(ATHLETE)))
	{
		return DATA_RES_FAIL;
	}

	free_list_iterator(pIter);

	return (store_athlete_data())?(DATA_RES_SUCCESS):(DATA_RES_STORE_FAIL);
}

*/