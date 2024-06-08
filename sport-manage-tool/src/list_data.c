#include "../inc/pch.h"
#include "../inc/list_data.h"

inline void _err_open_file_fail(const char *szFileName, const char *szMode)
{
	fprintf(stderr, "[����]���ļ�ʧ�ܣ��ļ�����%s��ģʽ��%s", szFileName, szMode);
}

inline void _err_rename_file_fail(const char *_OldFileName, const char *_NewFIleName)
{
	fprintf(stderr, "[����]�������ļ�ʧ�ܣ����ļ�����%s�����ļ�����%s", _OldFileName, _NewFIleName);
}

void clear_list_data(LIST_DATA *pListData)
{
	if (pListData)
	{
		if (pListData->pList)
		{
			release_list(pListData->pList);
		}

		free(pListData);
	}
}

LIST_DATA *_make_list_data(const char *szFileName, const char *szTempFileName)
{
	LIST_DATA *pListData = malloc(sizeof(LIST_DATA));
	if(pListData)
	{
		//pListData->bLoaded = false;
		pListData->pList = NULL;
		pListData->szFileName = szFileName;
		pListData->szTempFileName = szTempFileName;
	}
	return pListData;
}

bool store_list_data(LIST_DATA *pListData)
{
	/*
	if (NULL == pListData)
	{
		abort();
	}
	*/

	// ��athlete.tmp
	FILE* pTempData = _fopen_auto_create(pListData->szTempFileName, "wb");
	{
		if (NULL == pTempData)
		{
			_err_open_file_fail(pListData->szTempFileName, "wb");
			abort();
		}


		// ��athlete.tmp�б��渱��
		LIST_ITERATOR *pIter = get_list_begin(pListData->pList);
		LIST_ITERATOR *pLast = get_list_last(pListData->pList);
		{
			
			if (NULL == pIter || NULL == pLast)
			{
				//fputs("����LIST_ITERATORʧ��"
				abort();
			}

			for(; !is_list_iterator_equal(pIter, pLast); forward_list_iterator(pIter))
			{
				const uint32_t iKey = get_list_iterator_key(pIter);
				fwrite(&iKey, sizeof(iKey), 1, pTempData);
				fwrite(get_list_iterator_value(pIter), pListData->pList->valueSize, 1, pTempData);
			}
		}
		free_list_iterator(pLast);
		free_list_iterator(pIter);
	}
	fclose(pTempData);

	// ����athlete.dat
	if (rename(pListData->szFileName, ".\\temp"))
	{
		_err_rename_file_fail(pListData->szFileName, ".\\temp");
		abort();
	}

	if (rename(pListData->szTempFileName, pListData->szFileName))
	{
		_err_open_file_fail(pListData->szTempFileName, pListData->szFileName);
		abort();
	}

	if (remove(".\\temp"))
	{
		fprintf(stderr, "[����]��ʱ�ļ�ɾ��ʧ�ܣ�%s\n", ".\\temp");
		abort();
	}

	return true;
}

LIST_DATA *load_list_data(const size_t valueSize, const char *szFileName, const char *szTempFileName)
{
	LIST_DATA *pListData = _make_list_data(szFileName, szTempFileName);
	if (NULL == pListData)
	{
		fputs("[����]����LIST_DATAʧ��", stderr);
		abort();
	}

	pListData->pList = make_list(LIST_KEY_UINT32, valueSize);
	if (NULL == pListData->pList)
	{
		fputs("[����]����LISTʧ��", stderr);
		abort();
	}

	FILE *pFile = _fopen_auto_create(pListData->szFileName, "rb");
	if (NULL == pFile)
	{
		_err_open_file_fail(pListData->szFileName, "rb");
		abort();
	}

	uint32_t iLoadTempKey = 0;
	void *pLoadTemp = malloc(valueSize);
	if (NULL == pLoadTemp)
	{
		fputs("[����]��ʱ���洴��ʧ��", stderr);
	}

	while (0 < fread(&iLoadTempKey, sizeof(iLoadTempKey), 1, pFile))
	{
		if (0 >= fread(pLoadTemp, valueSize, 1, pFile))
		{
			fprintf(stderr, "[����]�ļ��е����������𻵣��ļ���:%s", pListData->szFileName);
			abort();
		}

		LIST_ITERATOR *pIter = NULL;
		
		if (find_list_iterator(pListData->pList, iLoadTempKey, &pIter))
		{
			fprintf(stderr, "[����]�ļ��е��������ݳ�ͻ���ļ���:%s", pListData->szFileName);
			abort();
		}
		
		if (!insert_list_iterator(pIter, iLoadTempKey, pLoadTemp, valueSize))
		{
			fprintf(stderr, "[����]����Ԫ��ʧ��");
			abort();
		}

		free_list_iterator(pIter);
	}

	fclose(pFile);

	//pListData->bLoaded = true;

	return pListData;
}

DATA_RES_t list_data_add(LIST_DATA *pListData, const uint32_t iKey, const void *pValue)
{
	/*
	if (NULL == pListData)
	{
		abort();
		//return DATA_RES_UNLOADED;
	}
	*/

	LIST_ITERATOR *pIter = NULL;

	if (find_list_iterator(pListData->pList, iKey, &pIter))
	{
		return DATA_RES_KEY_CONFLICT;
	}

	if(!insert_list_iterator(pIter, iKey, pValue, pListData->pList->valueSize))
	{
		return DATA_RES_FAIL;
	}

	free_list_iterator(pIter);

	return (store_list_data(pListData))?(DATA_RES_SUCCESS):(DATA_RES_STORE_FAIL);
}

DATA_RES_t list_data_remove(LIST_DATA *pListData, const uint32_t iKey)
{
	if (NULL == pListData)
	{
		abort();
	}

	LIST_ITERATOR *pIter = NULL;
	if (!find_list_iterator(pListData->pList, iKey, &pIter))
	{
		return DATA_RES_INVALID_KEY;
	}

	if (!erase_list_iterator(pIter))
	{
		return DATA_RES_FAIL;
	}

	free_list_iterator(pIter);

	return (store_list_data(pListData))?(DATA_RES_SUCCESS):(DATA_RES_STORE_FAIL);
}


DATA_RES_t list_data_rewrite(LIST_DATA *pListData, const uint32_t iKey, const void *pValue)
{
	if (NULL == pListData)
	{
		abort();
	}

	LIST_ITERATOR *pIter = NULL;
	if (!find_list_iterator(pListData->pList, iKey, &pIter))
	{
		return DATA_RES_INVALID_KEY;
	}


	if (!set_list_iterator_value(pIter, pValue, pListData->pList->valueSize))
	{
		return DATA_RES_FAIL;
	}

	free_list_iterator(pIter);

	return (store_list_data(pListData))?(DATA_RES_SUCCESS):(DATA_RES_STORE_FAIL);
}

DATA_RES_t list_data_query(const LIST_DATA *pListData, const uint32_t iKey, const void **ppValue)
{
	if (NULL == pListData)
	{
		abort();
	}

	LIST_ITERATOR *pIter = NULL;
	if (!find_list_iterator(pListData->pList, iKey, &pIter))
	{
		return DATA_RES_INVALID_KEY;
	}

	*ppValue = get_list_iterator_value(pIter);

	free_list_iterator(pIter);

	return (*ppValue)?(DATA_RES_SUCCESS):(DATA_RES_FAIL);
}

bool list_data_is_exist(const LIST_DATA *pListData, const uint32_t iKey)
{
	LIST_ITERATOR *pIter = NULL;
	return find_list_iterator(pListData->pList, iKey, &pIter);
}

DATA_RES_t list_data_enum(LIST_DATA *pListData, LIST_DATA_ENUM_PROC fnEnumProc, void *pParam)
{
	LIST_ITERATOR *pIter = get_list_begin(pListData->pList);
	LIST_ITERATOR *pLast = get_list_last(pListData->pList);

	for(; !is_list_iterator_equal(pIter, pLast); forward_list_iterator(pIter))
	{
		if (NULL == pIter || NULL == pLast)
		{
			abort();
		}

		fnEnumProc(pIter->pNode->pNext->pValue, pParam);
	}
	free_list_iterator(pLast);
	free_list_iterator(pIter);

	return DATA_RES_SUCCESS;
}

LIST *list_data_get_internal(LIST_DATA *pListData)
{
	return pListData->pList;
}