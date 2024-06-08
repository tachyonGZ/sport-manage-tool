#pragma once

#include "pch.h"
#include "list_iterator.h"

typedef struct{
	LIST **a_pList;
	uint32_t iDimension;
} CARTESIAN_PRODUCT;

void release_cartesian_product(CARTESIAN_PRODUCT *pCp);
CARTESIAN_PRODUCT *make_cartesian_product_2d(LIST *pList1, LIST *pList2);

CARTESIAN_PRODUCT *make_cartesian_product_3d(LIST *pList1, LIST *pLsit2, LIST *pLsit3);

typedef void (*CARTESIAN_PRODUCT_2D_ENUM_PROC)(const void *pValue1, const void *pValue2, void *pParam);
bool enum_cartesian_product_2d(CARTESIAN_PRODUCT *pCp,  CARTESIAN_PRODUCT_2D_ENUM_PROC fnEnumProc, void *pParam);

typedef void (*CARTESIAN_PRODUCT_3D_ENUM_PROC)(const void *pValue1, const void *pValue2, const void *pValue3, void *pParam);
bool enum_cartesian_product_3d(CARTESIAN_PRODUCT *pCp, CARTESIAN_PRODUCT_3D_ENUM_PROC fnEnumProc, void *pParam);