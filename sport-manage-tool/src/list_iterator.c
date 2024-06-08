#include "../inc/pch.h"
#include "../inc/list_iterator.h"

LIST_ITERATOR *make_list_iterator(LIST *pList, LIST_NODE *pNode)
{
	LIST_ITERATOR *pIter = (LIST_ITERATOR *) malloc(sizeof(LIST_ITERATOR));

	if(pIter)
	{
		pIter->pList = pList;
		pIter->pNode = pNode;
	}

	return pIter;
}

void free_list_iterator(LIST_ITERATOR *pIter)
{
	if(pIter)
	{
		free(pIter);
	}
}

bool find_list_iterator(LIST *pList, const uint32_t iKey, LIST_ITERATOR **ppIter)
{
	if (NULL == pList)
	{
		abort();
	}

	LIST_NODE *pIterNode = pList->pVirtualHeadNode;
	if (NULL == pIterNode)
	{
		abort();
	}

	for(LIST_NODE *pNode = pList->pVirtualHeadNode->pNext; pNode != NULL && pNode->key < iKey; pNode = pNode->pNext)
	{
		pIterNode = pNode;
	}

	if (ppIter)
	{
		*ppIter = make_list_iterator(pList, pIterNode);
		if (NULL == *ppIter)
		{
			abort();
		}
	}

	return (pIterNode->pNext && iKey == pIterNode->pNext->key)?(true):(false);
}

bool insert_list_iterator(LIST_ITERATOR *pIter, const uint32_t iKey, const void *pValue, const size_t valueSize)
{
	if (NULL == pIter)
	{
		abort();
	}

	//                 <--
	//   pIter->pNode       pNewNode
	//                 -->

	LIST_NODE *pNewNode = make_list_node(iKey, pValue, valueSize);
	if (NULL == pNewNode)
	{
		abort();
	}

	pNewNode->pNext = pIter->pNode->pNext;
	pNewNode->pPrev = pIter->pNode;

	if (pNewNode->pNext)
	{
		pNewNode->pNext->pPrev = pNewNode;
	}
	else
	{
		if (NULL == pIter->pList || NULL == pIter->pList->pVirtualEndNode)
		{
			abort();
		}

		pIter->pList->pVirtualEndNode->pPrev = pNewNode;
	}

	pIter->pNode->pNext = pNewNode;
	return true;
}

const uint32_t get_list_iterator_key(const LIST_ITERATOR *pIter)
{
	if (NULL == pIter)
	{
		abort();
	}

	LIST_NODE *pGottenNode = pIter->pNode->pNext;
	if (NULL == pGottenNode)
	{
		abort();
	}

	return pGottenNode->key;
}

const void *get_list_iterator_value(const LIST_ITERATOR *pIter)
{
	if (NULL == pIter || NULL == pIter->pNode)
	{
		abort();
	}


	//                 <--
	//   pIter->pNode       pGottenNode
	//                 -->

	LIST_NODE *pGottenNode = pIter->pNode->pNext;
	if (NULL == pGottenNode)
	{
		abort();
	}

	return pGottenNode->pValue;
}

bool set_list_iterator_value(LIST_ITERATOR *pIter, const void *pValue, const size_t valueSize)
{
	if(NULL == pIter || NULL == pIter->pNode)
	{
		return false;
	}

	//                 <--
	//   pIter->pNode       pSetNode
	//                 -->

	LIST_NODE *pSetNode = pIter->pNode->pNext;
	if(NULL == pSetNode)
	{
		return false;
	}

	return (NULL == memcpy(pSetNode->pValue, pValue, valueSize))?(false):(true);
}

bool erase_list_iterator(LIST_ITERATOR *pIter)
{
	if (NULL == pIter || NULL == pIter->pNode)
	{
		abort();
	}

	//               <--            <--
	//	pIter->pNode      pKillNode       pKillNode->pNext
	//               -->            -->


	LIST_NODE *pKillNode = pIter->pNode->pNext;
	if (NULL == pKillNode)
	{
		abort();
	}

	pIter->pNode->pNext = pKillNode->pNext;

	if(pKillNode->pNext)
	{
		pKillNode->pNext->pPrev = pKillNode->pPrev;
	}
	else
	{
		if (NULL == pIter->pList || NULL == pIter->pList->pVirtualEndNode->pPrev)
		{
			abort();
		}

		pIter->pList->pVirtualEndNode->pPrev = pKillNode->pPrev;
	}

	release_list_node(pKillNode);

	return true;
}

LIST_ITERATOR *get_list_begin(LIST *pList)
{
	return make_list_iterator(pList, pList->pVirtualHeadNode);
}

bool forward_list_iterator(LIST_ITERATOR *pIter)
{
	pIter->pNode = pIter->pNode->pNext;

	return (NULL == pIter->pNode->pNext)?(false):(true);
}

LIST_ITERATOR *get_list_last(LIST *pList)
{
	if(pList->pVirtualEndNode->pPrev)
	{
		return make_list_iterator(pList, pList->pVirtualEndNode->pPrev);
	}
	else
	{
		return make_list_iterator(pList, pList->pVirtualHeadNode);
	}
}

bool is_list_iterator_equal(const LIST_ITERATOR *pL, const LIST_ITERATOR *pR)
{
	return pL->pNode == pR->pNode;
}