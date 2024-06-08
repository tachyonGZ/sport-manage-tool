#include "../inc/pch.h"
#include "../inc/list.h"

LIST_NODE *make_list_node(const uint32_t iKey, const void *pValue, const size_t valueSize)
{
	LIST_NODE *pNode = (LIST_NODE *) malloc(sizeof(LIST_NODE));
	if(NULL == pNode)
	{
		return NULL;
	}

	pNode->pValue = malloc(valueSize);
	if (NULL == pNode->pValue)
	{
		free(pNode);
		return NULL;
	}

	if(NULL == memcpy(pNode->pValue, pValue, valueSize))
	{
		free(pNode->pValue);
		free(pNode);
		return NULL;
	}
	
	pNode->key = iKey;
	pNode->pNext = NULL;
	pNode->pPrev = NULL;

	return pNode;
}

void release_list_node(void *pNode)
{
	if (pNode)
	{
		free(pNode);
	}
}

void release_list(LIST *pList)
{
	if (NULL == pList)
	{
		return;
	}

	for(LIST_NODE *pNode = pList->pVirtualHeadNode; pNode != NULL; pNode = pNode->pNext)
	{
		release_list_node(pNode);
	}

	release_list_node(pList->pVirtualEndNode);

	free(pList);
}

LIST *make_list(const listKeyType_t keyType,	const size_t valueSize)
{
	LIST *pList = (LIST *) malloc(sizeof(LIST));
	if (NULL == pList)
	{
		return false;
	}

	LIST_NODE *pVirtualHeadNode = make_list_node(0, NULL, 0);
	if (NULL == pVirtualHeadNode)
	{
		free(pList);
		return false;
	}

	LIST_NODE *pVirtualEndNode = make_list_node(0, NULL, 0);
	if (NULL == pVirtualEndNode)
	{
		free(pList);
		release_list_node(pVirtualHeadNode);
		return false;
	}

	(pList)->keyType = keyType;
	(pList)->valueSize = valueSize;
	(pList)->pVirtualHeadNode = pVirtualHeadNode;
	(pList)->pVirtualEndNode = pVirtualEndNode;

	return pList;	
}