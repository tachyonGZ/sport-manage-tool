#pragma once

#include "pch.h"

typedef enum{
	LIST_KEY_UINT32,
	LIST_KEY_UINT64,
} listKeyType_t;

typedef struct sLIST_NODE{
	struct sLIST_NODE *pNext;
	struct sLIST_NODE *pPrev;
	uint32_t key;
	void *pValue;
} LIST_NODE;

typedef struct {
	//size_t keySize;
	listKeyType_t keyType;
	size_t valueSize;
	LIST_NODE *pVirtualHeadNode;
	LIST_NODE *pVirtualEndNode;
	//LIST_NODE *pBeginNode;
	//LIST_NODE *pEndNode;
} LIST;



void release_list_node(void *pNode);
LIST_NODE *make_list_node(const uint32_t iKey, const void *pValue, const size_t valueSize);

void release_list(LIST *pList);
LIST *make_list(const listKeyType_t keyType, const size_t valueSize);

/*
bool find_list_node_uint32(const LIST *pList, const uint32_t key, LIST_NODE **ppPrevNode)
{
	if (NULL == ppPrevNode)
	{
		return false;
	}

	*ppPrevNode = pList->pVirtualHeadNode;

	for(LIST_NODE *pNode = pList->pVirtualHeadNode->pNext; pNode != NULL && pNode->key < key; pNode = pNode->pNext)
	{
		*ppPrevNode = pNode;
	}

	if (key == (*ppPrevNode)->key)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool insert_list_node_uint32_empty(LIST *pList, LIST_NODE *pPrevNode)
{
	LIST_NODE *pNode = make_list_node_uint32();
	if (NULL == pNode)
	{
		return false;
	}

	pNode->pNext = pPrevNode->pNext;
	pPrevNode->pNext = pNode;

	pNode->pPrev = pPrevNode;
	if (pPrevNode->pNext)
	{
		pPrevNode->pNext->pPrev = pNode;
	}
	return true;
}

bool set_list_node_uint32_value(LIST_NODE *pPrevNode, const void *pValue, const size_t valueSize)
{
	if (NULL == pPrevNode)
	{
		return false;
	}

	return NULL != memcpy(pPrevNode->pNext->pValue, pValue, valueSize);
}

const void *get_list_node_uint32_value(LIST_NODE *pPrevNode)
{
	if (NULL == pPrevNode)
	{
		return NULL;
	}

	return pPrevNode->pNext->pValue;
}

bool remove_list_node_uint32(LIST_NODE *pPrevNode)
{
	if (NULL == pPrevNode || NULL == pPrevNode->pNext)
	{
		return false;
	}

	LIST_NODE *killNode = pPrevNode;
	
	pPrevNode->pNext = killNode->pNext;

	if(pPrevNode->pNext)
	{
		pPrevNode->pNext->pPrev = pPrevNode;
	}

	release_list_node(killNode);
	return true;
}

bool list_put(LIST *pList, const void *pKey, const void *pValue)
{
	if (NULL == pKey)
	{
		return false;
	}	

	LIST_NODE *pPrevNode = NULL;
	if (!find_list_node_uint32(pList, *((const uint32_t *) pKey), &pPrevNode))
	{
		if (insert_list_node_uint32_empty(pList, pPrevNode))
		{
			return false;
		}
	}

	return set_list_node_uint32_value(pPrevNode, pValue, pList->valueSize);
}

const void *list_get(LIST *pList, const void *pKey)
{
	if (NULL == pKey)
	{
		return NULL;
	}	

	LIST_NODE *pPrevNode = NULL;
	if (!find_list_node_uint32(pList, *((const uint32_t *) pKey), &pPrevNode))
	{
		return NULL;
	}

	return get_list_node_uint32_value(pPrevNode);
}


bool list_delete(LIST *pList, const void *pKey)
{
	if (NULL == pKey)
	{
		return false;
	}	

	LIST_NODE *pPrevNode = NULL;
	if (!find_list_node_uint32(pList, *((const uint32_t *) pKey), &pPrevNode))
	{
		return false;
	}

	return remove_list_node_uint32(pPrevNode);
}
*/