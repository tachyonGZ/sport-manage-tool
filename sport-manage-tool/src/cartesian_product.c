#include "../inc/pch.h"
#include "../inc/cartesian_product.h"

void release_cartesian_product(CARTESIAN_PRODUCT *pCp)
{
	if (pCp)
	{
		if (pCp->a_pList)
		{
			free(pCp->a_pList);
		}

		free(pCp);
	}
}

CARTESIAN_PRODUCT *_make_cartesian_product(LIST **a_pList, uint32_t iDimension)
{
	CARTESIAN_PRODUCT *pCp = malloc(sizeof(CARTESIAN_PRODUCT));
	if (NULL == pCp)
	{
		return NULL;
	}

	pCp->a_pList = a_pList;
	pCp->iDimension = iDimension;

	return pCp;
}

CARTESIAN_PRODUCT *make_cartesian_product_2d(LIST *pList1, LIST *pList2)
{
	LIST **a_pList = calloc(sizeof(LIST *), 2);
	if (NULL == a_pList)
	{
		return NULL;
	}

	a_pList[0] = pList1;
	a_pList[1] = pList2;

	return _make_cartesian_product(a_pList, 2);
}

CARTESIAN_PRODUCT *make_cartesian_product_3d(LIST *pList1, LIST *pList2, LIST *pLsit3)
{
	LIST **a_pList = calloc(sizeof(LIST *), 3);
	if (NULL == a_pList)
	{
		return NULL;
	}

	a_pList[0] = pList1;
	a_pList[1] = pList2;
	a_pList[2] = pLsit3;

	return _make_cartesian_product(a_pList, 3);
}

bool enum_cartesian_product_2d(CARTESIAN_PRODUCT *pCp,  CARTESIAN_PRODUCT_2D_ENUM_PROC fnEnumProc, void *pParam)
{
	LIST_ITERATOR *pIter1 = get_list_begin(pCp->a_pList[0]);
	LIST_ITERATOR *pIter2 = get_list_begin(pCp->a_pList[1]);

	LIST_ITERATOR *pLast1 = get_list_last(pCp->a_pList[0]);
	LIST_ITERATOR *pLast2 = get_list_last(pCp->a_pList[1]);

	while (!is_list_iterator_equal(pIter1, pLast1))
	{
		fnEnumProc(get_list_iterator_value(pIter1), get_list_iterator_value(pIter2), pParam);
		
		// 递增迭代器2
		forward_list_iterator(pIter2);
		
		// 如果迭代器2到达末尾
		if (is_list_iterator_equal(pIter2, pLast2))
		{
			// 递增迭代器1
			forward_list_iterator(pIter1);
			
			// 迭代器2回到链表2的初始位置
			free_list_iterator(pIter2);
			pIter2 = get_list_begin(pCp->a_pList[1]);
		}
		
	}

	return true;
}

bool enum_cartesian_product_3d(CARTESIAN_PRODUCT *pCp,  CARTESIAN_PRODUCT_3D_ENUM_PROC fnEnumProc, void *pParam)
{
	LIST_ITERATOR *pIter1 = get_list_begin(pCp->a_pList[0]);
	LIST_ITERATOR *pIter2 = get_list_begin(pCp->a_pList[1]);
	LIST_ITERATOR *pIter3 = get_list_begin(pCp->a_pList[2]);

	LIST_ITERATOR *pLast1 = get_list_last(pCp->a_pList[0]);
	LIST_ITERATOR *pLast2 = get_list_last(pCp->a_pList[1]);
	LIST_ITERATOR *pLast3 = get_list_last(pCp->a_pList[2]);

	while (!is_list_iterator_equal(pIter1, pLast1))
	{
		fnEnumProc(get_list_iterator_value(pIter1), get_list_iterator_value(pIter2), get_list_iterator_value(pIter3), pParam);

		// 递增迭代器3
		forward_list_iterator(pIter3);

		// 如果迭代器3到达末尾
		if (is_list_iterator_equal(pIter3, pLast3))
		{
			// 递增迭代器2
			forward_list_iterator(pIter2);

			// 迭代器3回到链表3的初始位置
			free_list_iterator(pIter3);
			pIter3 = get_list_begin(pCp->a_pList[2]);

			// 如果迭代器2到达末尾
			if (is_list_iterator_equal(pIter2, pLast2))
			{
				// 递增迭代器1
				forward_list_iterator(pIter1);

				// 迭代器2回到链表2的初始位置
				free_list_iterator(pIter2);
				pIter2 = get_list_begin(pCp->a_pList[1]);
			}	
		}
	}

	return true;
}