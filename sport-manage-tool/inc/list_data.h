#pragma once

#include "../inc/pch.h"
#include "file.h"
#include "../inc/list_iterator.h"

typedef enum {
	DATA_RES_SUCCESS = 0,
	DATA_RES_FAIL = 1,
	DATA_RES_INVALID_KEY = 2,
	DATA_RES_KEY_CONFLICT,
	DATA_RES_STORE_FAIL,
	DATA_RES_UNLOADED,
	DATA_RES_MAX
} DATA_RES_t;


typedef struct {
	LIST *pList;
	const char *szFileName;
	const char *szTempFileName;
} LIST_DATA;

void clear_list_data(LIST_DATA *pListData);
LIST_DATA *load_list_data(const size_t valueSize, const char *szFileName, const char *szTempFileName);
bool store_list_data(LIST_DATA *pListData);

DATA_RES_t list_data_add(LIST_DATA *pListData, const uint32_t iKey, const void *pValue);
DATA_RES_t list_data_remove(LIST_DATA *pListData, const uint32_t iKey);
DATA_RES_t list_data_rewrite(LIST_DATA *pListData, const uint32_t iKey, const void *pValue);
DATA_RES_t list_data_query(const LIST_DATA *pListData, const uint32_t iKey, const void **pValue);

bool list_data_is_exist(const LIST_DATA *pListData, const uint32_t iKey);

typedef void (*LIST_DATA_ENUM_PROC)(const void *pValue, void *pParam);
DATA_RES_t list_data_enum(LIST_DATA *pListData, LIST_DATA_ENUM_PROC fnEnumProc, void *pParam);

LIST *list_data_get_internal(LIST_DATA *pListData);